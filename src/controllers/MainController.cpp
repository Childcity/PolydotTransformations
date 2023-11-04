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

		m_meshes = importer.getGeometries();

		///////////////////////////////////////////////////
		//		auto letterP = *(m_meshes.begin() + 7);
		//		letterP = {letterP[2],letterP[3]};

		//		m_meshes = {letterP};
		///////////////////////////////////////////////////

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

	//	std::jthread([this,
	//	              oldMeshes = std::move(oldMeshes),
	//	              origBasises = std::move(origBasises),
	//	              resBasises = std::move(resBasises)] {
	MeshList tMeshes;

	for (const auto &mesh : m_meshes) {
		Mesh tMesh;
		for (const auto &line : mesh) {
			auto streightLine = MathUtils::getPolydotTransformedLine(line, origBasises, resBasises);

			auto tLine = Line::FromStreightLine(streightLine);
			if (!tLine.isNull()) {
				qDebug() << LineGeometry(tLine);
				tMesh.emplace_back(tLine);
			}
		}
		tMeshes.emplace_back(std::move(tMesh));
	}

	// tMeshes = Mes(tMeshes.begin()+3, tMeshes.end());
	tMeshes = {tMeshes.front()};

	m_meshListModel = std::make_unique<MeshListModel>(std::move(tMeshes));
	emit meshListModelChanged();
	//}).detach();
}
