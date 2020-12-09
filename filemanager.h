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
    Q_INVOKABLE void createmap(QString filename);
    //qml 属性
    Q_PROPERTY(QStringList maplist READ getmaplist NOTIFY maplistupdata)
    Q_PROPERTY(QString editfile WRITE seteditfile)
    Q_PROPERTY(SerialManager *serial WRITE setserial);
    Q_PROPERTY(int nodesize READ getNodeSize NOTIFY nodeSizeUpdata)
    Q_PROPERTY(QString mappath READ getMapPath WRITE setMapPath NOTIFY mapPathUpdata)
    //选中编辑的地图,暴露接口
    QString _editfile;
    QJsonObject getmap(QString filename);
private:
    bool startCollection();
    bool stopCollection();
    bool doneCollection();

    //文件的操作
    bool deleteFile();
    QJsonObject readFile(QString filename,QString suffix=".json");
    bool writefile(QString filename,QJsonObject obj,QString suffix=".json");
    bool createFile(QString filename,QString suffix=".json");
    //Q_PROPERTY
    QStringList getmaplist();
    int getNodeSize();
    void seteditfile(QString name);
    void setmaplist();
    void setserial(SerialManager *manager);
    void setNodeSize();
    bool clearMapData();
    QString getMapPath();
    bool setMapPath(QString MapPath);
    //地图存放位置及文件格式
    QString MapFolder="E:/Map/";
    QString Suffix=".json";
    //创建的地图名称
    QString _filename="default";
    //创建的地图

    QFile *_file=nullptr;
    QDir dir;
    //地图列表
    QStringList _maplist;

    QString createtime;

    const QString className="filemanager debug:";
    //gps数据
    QString gpsData;
    //串口对象
    SerialManager *_serial=nullptr;
    QJsonArray map;
    int _nodesize=0;
signals:
    void maplistupdata();
    void nodeSizeUpdata();
    void mapPathUpdata();
public slots:
    void readSerial(const QString msg);

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override;
};

#endif // FILEMANAGER_H
