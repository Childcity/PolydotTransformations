#pragma once

#include <QQmlEngine>
#include <QVector3D>

#include <enums/MeshType.h>
#include <models/MeshListModel.h>

class MainController : public QObject
{
	Q_OBJECT
	QML_NAMED_ELEMENT(MainController)

	Q_PROPERTY(QVector3D globalScale READ globalScale CONSTANT FINAL)
	Q_PROPERTY(MeshListModel *meshListModel READ meshListModel NOTIFY meshListModelChanged FINAL)
	Q_PROPERTY(MeshType meshType READ meshType WRITE setMeshType NOTIFY meshTypeChanged FINAL)

public:
	explicit MainController(QObject *parent = nullptr);
	~MainController() override;

	MeshListModel *meshListModel() const;

	QVector3D globalScale() const;

	MeshType meshType() const;
	void setMeshType(MeshType meshType);

public slots:
	void loadMeshes();
	void unloadMeshes();

	void applyPolydotTransformations(QVariantList origBasises, QVariantList resBasises);

signals:
	void loadComplete();
	void meshListModelChanged();
	void meshTypeChanged();

private:
	QVector3D m_globalScale = {100, 100, 100};

	std::unique_ptr<MeshListModel> m_meshListModel;
	MeshList m_meshes;
	MeshType m_meshType = MeshType::ClosedMesh;
};
