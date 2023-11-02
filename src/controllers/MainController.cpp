#include "MainController.h"

MainController::MainController(QObject *parent)
    : QObject{parent}
{
	m_geometryModel = std::make_unique<PolydotGeometryModel>();
}

PolydotGeometryModel *MainController::geometryModel() const
{
	return m_geometryModel.get();
}
