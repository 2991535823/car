#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "serialmanager.h"
#include "filemanager.h"
#include <QApplication>
//#include "mapmanager.h"
#include "cmdmanager.h"
#include "mapassist.h"
#include <QtWebEngine>

int main(int argc, char *argv[])
{
    //test
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication app(argc, argv);
    QtWebEngine::initialize();
    qmlRegisterType<SerialManager>("SerialManager",1,0,"Serial");
    qmlRegisterType<FileManager>("FileManager",1,0,"File");
//    qmlRegisterType<MapManager> ("MapManager",1,0,"Map");
    qmlRegisterType<MapAssist>("MapAssist",1,0,"MapAssist");
    qmlRegisterType<CmdManager>("CmdManager",1,0,"Cmd");
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
