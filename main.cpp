#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
	QString appDir = QGuiApplication::applicationDirPath();
    QQmlApplicationEngine engine;
	QString str = QStringLiteral("D:/WorkSpace/Rigel/OSGQuick/main.qml");
    engine.load(QUrl(str));
//      if (engine.rootObjects().isEmpty())
//          return -1;

    return app.exec();
}
