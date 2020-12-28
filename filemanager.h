#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include <QObject>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QDir>
#include "serialmanager.h"
#include <QDateTime>
#include <QRegExp>
#include <QMessageBox>
#include "datacheck.h"
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
    Q_INVOKABLE bool doCmd(FileManager::Cmd cmd);
    Q_INVOKABLE bool setParms(QString filename);
    Q_INVOKABLE void createmap(QString filename);
    //qml 属性
    Q_PROPERTY(QStringList maplist READ getmaplist NOTIFY maplistupdata)
    Q_PROPERTY(QString editfile READ geteditfile WRITE seteditfile NOTIFY editfileUpdata)
    Q_PROPERTY(SerialManager *serial READ getSerial WRITE setserial NOTIFY onSerialChanged);
    Q_PROPERTY(int nodesize READ getNodeSize NOTIFY nodeSizeUpdata)
    Q_PROPERTY(QString mappath READ getMapPath WRITE setMapPath NOTIFY mapPathUpdata)
    //选中编辑的地图,暴露接口
    QString _editfile;
    QJsonObject getmap(QString filename,bool orNotToMap=true);
private:
    bool startCollection();
    bool stopCollection();
    bool doneCollection();
    void writeLog(QString data);
    //文件的操作
    bool deleteFile(QString filename,QString folder="C:/Map/");
    QJsonObject readFile(QString filename,QString suffix=".json",QString folder="C:/Map/");
    bool writefile(QString filename,QJsonObject obj,QString suffix=".json",QString folder="C:/Map/");
    bool createFile(QString filename,QString suffix=".json",QString folder="C:/Map/");
    bool createFolder(QString folder);
    //Q_PROPERTY
    QStringList getmaplist();
    int getNodeSize();
    void seteditfile(QString name);
    QString geteditfile();
    void setmaplist();
    void setserial(SerialManager *manager);
    SerialManager* getSerial();
    void setNodeSize();
    bool clearMapData();
    QString getMapPath();
    bool setMapPath(QString MapPath);

private:
    //地图存放位置及文件格式
    QString MapFolder="C:/Map/";
    QString iniFolder="C:/.map/";
    QString Suffix=".json";
    //创建的地图名称
    QString _filename="default";
    //创建的地图

    QFile *_file=nullptr;
    QDir dir;
    //地图列表
    QStringList _maplist;

    QString createtime;

    //gps数据
    QString gpsData;
    //串口对象
    SerialManager *_serial=nullptr;


    QJsonArray map;
    QJsonArray logarray;
    int _nodesize=0;
signals:
    void maplistupdata();
    void nodeSizeUpdata();
    void mapPathUpdata();
    void editfileUpdata(QString filename);
    void onSerialChanged(SerialManager *serial);
public slots:
    void readSerial(const QString msg);

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override;
};

#endif // FILEMANAGER_H
