#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QQmlEngine>
#include <QJSEngine>
class SerialManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(Cmd)
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

    Q_INVOKABLE bool doCmd(Cmd cmd);
    Q_INVOKABLE void setParms(QString baudrate,QString com);
    Q_INVOKABLE bool sendMsg(QString msg,bool orNot16=false);
    Q_PROPERTY(QStringList ports READ getports NOTIFY portsupdata)

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
