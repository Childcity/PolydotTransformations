#pragma once

#include <QQmlEngine>
#include <QVector3D>

#include <models/MeshListModel.h>

class MainController : public QObject
{
	Q_OBJECT
	QML_NAMED_ELEMENT(MainController)

	Q_PROPERTY(QVector3D globalScale READ globalScale CONSTANT FINAL)
	Q_PROPERTY(MeshListModel *meshListModel READ meshListModel NOTIFY meshListModelChanged FINAL)

public:
	explicit MainController(QObject *parent = nullptr);
	~MainController() override = default;

	MeshListModel *meshListModel() const;

	QVector3D globalScale() const;

public slots:
	void unloadMeshes();
	void loadMeshes();

	void applyPolydotTransformations(QVariantList origBasises, QVariantList resBasises);

signals:
	void meshListModelChanged();

private:
	QVector3D m_globalScale = {100, 100, 100};

	std::unique_ptr<MeshListModel> m_meshListModel;
	MeshList m_meshes;
};
