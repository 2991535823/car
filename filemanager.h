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
#include <QMessageBox>
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
    //qml 接口
    Q_INVOKABLE bool doCmd(Cmd cmd);
    Q_INVOKABLE bool setParms(QString filename);
    //qml 属性
    Q_PROPERTY(QStringList maplist READ getmaplist NOTIFY maplistupdata)
    Q_PROPERTY(QString editfile WRITE seteditfile)
    Q_PROPERTY(SerialManager *serial WRITE setserial);
    Q_PROPERTY(int nodesize READ getNodeSize NOTIFY nodeSizeUpdata)
private:
    bool startCollection();
    bool stopCollection();
    bool doneCollection();

    //文件的操作
    bool deleteFile();
    QJsonObject readFile(QString filename);
    bool writefile(QString filename,QJsonObject obj);
    bool createFile(QString filename);
    //Q_PROPERTY
    QStringList getmaplist();
    int getNodeSize();
    void seteditfile(QString name);
    void setmaplist();
    void setserial(SerialManager *manager);
    void setNodeSize();
    bool clearMapData();

    //地图存放位置及文件格式
    QString MapFolder="D:/Map/";
    QString Suffix=".json";
    //创建的地图名称
    QString _filename="default";
    //创建的地图
    QFile *_file;
    QDir dir;
    //地图列表
    QStringList _maplist;

    //选中编辑的地图
    QString _editfile;

    QString createtime;

    const QString className="filemanager debug:";
    //gps数据
    QString gpsData;
    //串口对象
    SerialManager *_serial;
    QJsonArray map;
    int _nodesize=0;
signals:
    void maplistupdata();
    void nodeSizeUpdata();
public slots:
    void readSerial(const QString msg);

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override;
};

#endif // FILEMANAGER_H
