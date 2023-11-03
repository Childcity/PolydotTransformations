#include "MainController.h"

MainController::MainController(QObject *parent)
    : QObject{parent}
{
	m_geometryModel = std::make_unique<LinesMeshModel>();
}

LinesMeshModel *MainController::geometryModel() const
{
	return m_geometryModel.get();
}

QVector3D MainController::globalScale() const
{
	return m_globalScale;
}
