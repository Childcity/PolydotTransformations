#include "MainController.h"

#include <utils/import/ColladaFormatImporter.h>
#include <utils/MathUtils.h>

MainController::MainController(QObject *parent)
    : QObject(parent)
{
}

MeshListModel *MainController::meshListModel() const
{
	return m_meshListModel.get();
}

QVector3D MainController::globalScale() const
{
	return m_globalScale;
}

void MainController::unloadMeshes()
{
	m_meshListModel.reset();
	emit meshListModelChanged();
}

void MainController::loadMeshes()
{
	unloadMeshes();

	std::jthread([this] {
		const auto path = "C:\\Users\\Ariel\\Documents\\AAScetch\\exp\\Untitled.dae";
		ColladaFormatImporter importer(path);
		importer.importGeometries();

		const auto geoms = importer.getGeometries();

		m_meshListModel = std::make_unique<MeshListModel>(geoms);
		emit meshListModelChanged();
	}).detach();
}

void MainController::applyPolydotTransformations(QVariantList origBasises, QVariantList resBasises)
{
	if (!m_meshListModel || !m_meshListModel->rowCount({})) {
		return;
	}

	auto oldMeshes = m_meshListModel->rawData();
	unloadMeshes();

	std::jthread([this,
	              oldMeshes = std::move(oldMeshes),
	              origBasises = std::move(origBasises),
	              resBasises = std::move(resBasises)] {
		MeshList tMeshes;

		for (const auto &mesh : oldMeshes) {
			Mesh tMesh;
			for (const auto &line : mesh) {
				auto streightLine = MathUtils::getPolydotTransformedLine(
				    line, origBasises, resBasises);

				auto tLine = Line::FromStreightLine(streightLine);
				// tLine.p1 *= -100;
				// tLine.p2 *= 100;
				if (!tLine.isNull()) {
					tMesh.emplace_back(tLine);
				}
			}
			tMeshes.emplace_back(std::move(tMesh));
		}

		m_meshListModel = std::make_unique<MeshListModel>(std::move(tMeshes));
		emit meshListModelChanged();
	}).detach();
}
