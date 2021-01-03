#define cout qDebug() << "[" <<__FILE__ <<":"<<__FUNCTION__<<":"<<__LINE__ <<"]"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "serialmanager.h"
#include "filemanager.h"
#include <QApplication>
//#include "mapmanager.h"
#include "cmdmanager.h"
#include "mapassist.h"
#include <QtWebEngine>
static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
int main(int argc, char *argv[])
{
    //test
    qInstallMessageHandler(myMessageOutput);
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
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type)
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    QString str_type = QString("类型:%1\n").arg(localMsg.constData());
    QString str_file_line_function = QString("位置:%1:%2,%3\n").arg(file).arg(context.line).arg(function);
    QString str_date_time = "时间" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd") + '\n';
    QString message = str_type + str_file_line_function + str_date_time;


    QString logPath = qApp->applicationDirPath() + "/log/"; /* log文件夹 */

    QDir dir(logPath);
    if(!dir.exists())
    {
        dir.mkpath(logPath);
    }
    /* log文件 */
    QString filePath = QString("%1%2.log").arg(logPath).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    QFile logFile(filePath); /* 创建一个log文件 */
    /* 以只写方式打开，追加数据 */
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    /* 文本数据流 */
    QTextStream text_stream(&logFile);
    /* 写入数据 */
    text_stream << message << "\r\n";
    logFile.flush();
    logFile.close();
}
