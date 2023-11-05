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

	//	std::jthread([this,
	//	              oldMeshes = std::move(oldMeshes),
	//	              origBasises = std::move(origBasises),
	//	              resBasises = std::move(resBasises)] {
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
	qDebug() << "";

	for (auto mesh : m_meshes) {
		Mesh tMesh;

		// assert(mesh.size() % 2 == 0);
		if (mesh.size() % 2 != 0) {
			auto lastLine = mesh.back();
			lastLine.p1 += {0.001, 0.001, 0};
			mesh.emplace_back(lastLine);
		}

		// to find last intersection point, we copy first line to the end
		mesh.emplace_back(mesh.front());

		std::vector<QVector3D> transformedPoints;
		auto lastTransformedLine = MathUtils::getPolydotTransformedLine(
		    mesh.front(), origBasises, resBasises);

		for (const auto &line : mesh | std::views::drop(1)) {
			auto streightLine = MathUtils::getPolydotTransformedLine(line, origBasises, resBasises);
			QVector3D intersection;
			try {
				intersection = lastTransformedLine.intersect(streightLine);
			} catch (...) {
				try {
					streightLine.A += 0.000000001;
					streightLine.B += 0.000000001;
					streightLine.C += 0.000000001;
					intersection = lastTransformedLine.intersect(streightLine);
				} catch (...) {
				}
			}
			transformedPoints.emplace_back(intersection);
			lastTransformedLine = streightLine;
		}

		QVector3D lastPoint = transformedPoints.back();
		for (auto p : transformedPoints | std::views::take(transformedPoints.size())) {
			tMesh.emplace_back(lastPoint, p);
			lastPoint = p;
		}

		tMeshes.emplace_back(std::move(tMesh));
	}

	m_meshListModel = std::make_unique<MeshListModel>(std::move(tMeshes));
	emit meshListModelChanged();
	//}).detach();
}
