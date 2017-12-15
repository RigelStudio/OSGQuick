#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "OSGView.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	qmlRegisterType<OSGView>("OSGQuick.OSGView", 1, 0, "OSGView");
	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	return app.exec();
}
