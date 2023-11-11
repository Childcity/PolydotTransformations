#include "MainController.h"

#include <utils/import/ColladaFormatImporter.h>
#include <utils/MathUtils.h>

#include <ranges>

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

		m_meshes = importer.getGeometries();

		m_meshListModel = std::make_unique<MeshListModel>(m_meshes);
		emit meshListModelChanged();
	}).detach();
}

void MainController::applyPolydotTransformations(QVariantList origBasises, QVariantList resBasises)
{
	if (!m_meshListModel || !m_meshListModel->rowCount({})) {
		return;
	}

	unloadMeshes();

	MeshList tMeshes;

	//	for (const auto &mesh : m_meshes) {
	//		Mesh tMesh;
	//		for (const auto &line : mesh) {
	//			auto streightLine = MathUtils::getPolydotTransformedLine(line, origBasises, resBasises);

	//			auto tLine = Line::FromStreightLine(streightLine);

	//			if (!tLine.isNull()) {
	//				tMesh.emplace_back(tLine);
	//			}
	//		}
	//		tMeshes.emplace_back(std::move(tMesh));
	//	}

	for (Mesh mesh : m_meshes) {
		if (mesh.size() % 2 != 0) {
			auto lastLine = mesh.back();
			lastLine.p1 += {0.001, 0.001, 0};
			// mesh.emplace_back(lastLine);
		}

		mesh = MathUtils::getPolydotTransformedMesh(std::move(mesh), origBasises, resBasises);
		tMeshes.emplace_back(std::move(mesh));
	}

	m_meshListModel = std::make_unique<MeshListModel>(std::move(tMeshes));
	emit meshListModelChanged();
}
