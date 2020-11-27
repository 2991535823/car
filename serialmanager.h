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
    Q_INVOKABLE bool sendMsg(QString msg);
    Q_PROPERTY(QStringList ports READ getports NOTIFY portsupdata)

private:
    bool carStart();
    bool carStop();
    bool carRestart();
    bool serialConnect();
    bool serialDisconnect();
    QStringList getports();
    void setports();
    QStringList _ports;
    QString serialMsg;
    QString _baudrate=NULL;
    QString _com=NULL;
    QSerialPort *port=new QSerialPort(this);
    QSerialPortInfo *info=new QSerialPortInfo();
    QByteArray dataRoom="";

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
