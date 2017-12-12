#include <QGuiApplication>
#include <QQmlApplicationEngine>


int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	qmlRegisterType<OSGItem>("OSGQuick.OSGItem", 1, 0, "OSGItem");

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty())
	{
		return -1;
	}
	return app.exec();
}
