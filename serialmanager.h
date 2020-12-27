#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QQmlEngine>
#include <QJSEngine>
#include "debugmanager.h"
#include "datacheck.h"
#include "cmdmanager.h"
#include <QMessageBox>

class SerialManager : public QObject
{
    Q_OBJECT

public:

    explicit SerialManager(QObject *parent = nullptr);
    ~SerialManager();
    enum Cmd{
        CarStart=0,
        CarStop=1,
        CarResatrt=2,
        SerialConnect=3,
        SerialDisconnect=4
    };
//    friend bool CmdManager::send(SerialManager *manager,QString msg);
//    friend QJsonObject CmdManager::getMinDistancePoint(SerialManager *manager);
    Q_INVOKABLE bool doCmd(int cmd);
    Q_INVOKABLE void setParms(QString baudrate,QString com);
    Q_INVOKABLE bool sendMsg(QString msg,bool orNot16=false);
    Q_PROPERTY(QStringList ports READ getports NOTIFY portsupdata)
    inline QString getSerialMsg(){return serialMsg;}
private:
    bool carStart();
    bool carStop();
    bool carRestart();
    bool serialConnect();
    bool serialDisconnect();

    QByteArray String2Hex(QString &str);
    char ConvertHexChar(char ch);
    QStringList getports();
    void setports();

    QStringList _ports;
    QString serialMsg;
    QString _baudrate=NULL;
    QString _com=NULL;
    QSerialPort *port=new QSerialPort(this);
    QSerialPortInfo *info=new QSerialPortInfo();
    QByteArray dataRoom="";
    //存放16进制数据
    QByteArray _msg;
    bool _orNotBack=false;

signals:
    void readDone(const QString msg);
    void portsupdata();
private slots:
    void readSerial();


    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override;
};

#endif // SERIALMANAGER_H
