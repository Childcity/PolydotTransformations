#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <controllers/MainController.h>
#include <models/GeometryPrimitives.h>
#include <utils/MathUtils.h>

#define REG_SINGLETON(type)                                                                        \
	qmlRegisterSingletonType<type>(#type, 1, 0, #type, &type::CreateInstance)
#define REG_TYPE(type) qmlRegisterType<type *>(#type, 1, 0, #type)

int main(int argc, char *argv[])
{
	{
		REG_SINGLETON(MathUtils);
		REG_TYPE(PointGeometry);
		REG_TYPE(LineGeometry);
		REG_TYPE(MainController);
	}

	QGuiApplication app(argc, argv);
	app.setOrganizationName("Childcity");
	app.setOrganizationDomain("childcity.com");
	app.setApplicationName("Polydot Transformations");

	QQmlApplicationEngine engine;
	QObject::connect(
	    &engine,
	    &QQmlApplicationEngine::objectCreationFailed,
	    &app,
	    []() { QCoreApplication::exit(-1); },
	    Qt::QueuedConnection);

	engine.loadFromModule("PolydotTransformationUi", "Main");

	return app.exec();
}
