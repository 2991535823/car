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

    Q_PROPERTY(QStringList maplist READ getmaplist NOTIFY maplistupdata)
    Q_PROPERTY(QString editfile WRITE seteditfile)
private:
    bool startCollection();
    bool stopCollection();
    bool doneCollection();
    bool deleteMap();
    bool parseFile(QString filename);
    QStringList getmaplist();
    void seteditfile(QString name);
    void setmaplist();

    QString MapFolder="D:/Map/";
    QString _filename="default";
    QString Suffix=".json";


    QFile *_file;
    QDir dir;
    QStringList _maplist;
    QString _editfile;
    QString createtime;
    QString className="filemanager debug:";
    QString gpsData;

signals:
void maplistupdata();
public slots:
    void readSerial(const QString msg);

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override;
};

#endif // FILEMANAGER_H
