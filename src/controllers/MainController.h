#pragma once

#include <models/LinesMeshModel.h>

class MainController : public QObject
{
	Q_OBJECT
	QML_NAMED_ELEMENT(MainController)

	Q_PROPERTY(QVector3D globalScale READ globalScale CONSTANT FINAL)
	Q_PROPERTY(LinesMeshModel *geometryModel READ geometryModel CONSTANT FINAL)

public:
	explicit MainController(QObject *parent = nullptr);

	LinesMeshModel *geometryModel() const;

	QVector3D globalScale() const;

private:
	QVector3D m_globalScale = {100, 100, 100};

	std::unique_ptr<LinesMeshModel> m_geometryModel;
};
