#pragma once

#include <PolydotGeometryModel.h>

class MainController : public QObject
{
	Q_OBJECT
	QML_NAMED_ELEMENT(MainController)

	Q_PROPERTY(PolydotGeometryModel *geometryModel READ geometryModel CONSTANT FINAL)

public:
	explicit MainController(QObject *parent = nullptr);

	PolydotGeometryModel *geometryModel() const;

private:
	std::unique_ptr<PolydotGeometryModel> m_geometryModel;
};
