#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QDir>
#include "serialmanager.h"
#include <QDateTime>
class FileManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(Cmd)
public:
    explicit FileManager(QObject *parent = nullptr);
    ~FileManager();
    enum Cmd{
        Start,
        Stop,
        Done,
        Delete,
        Edit
    };
    Q_INVOKABLE bool doCmd(Cmd cmd);
    Q_INVOKABLE bool setParms(QString filename);
private:
    bool startCollection();
    bool stopCollection();
    bool doneCollection();
    bool parseFile(QString filename);
    const QString MapFolder="D:/Map/";
    QString _filename="default";
    const QString Suffix=".json";


    QFile *_file;
    QDir dir;


    SerialManager *serial=new SerialManager;
    QString createtime;
    QString className="filemanager debug:";
    QString gpsData;
signals:

public slots:
    void readSerial(const QString msg);
};

#endif // FILEMANAGER_H
