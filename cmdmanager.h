#ifndef CMDMANAGER_H
#define CMDMANAGER_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
class SerialManager;
class FileManager;
class CmdManager:public QObject
{
    Q_OBJECT
public:
    CmdManager();
    //    Q_PROPERTY();

    Q_INVOKABLE void sendtostm32(bool ornotdata=true);
    Q_INVOKABLE void setCmd(int cmd);
    Q_INVOKABLE void setMode(int mode);
    Q_INVOKABLE void mapServer(FileManager *manager);
    Q_INVOKABLE void serialServer(SerialManager *manager);
    bool send(SerialManager*, QString msg);
    QJsonObject getMinDistancePoint();
    Q_PROPERTY(int orNotBack READ getOrNotBack WRITE setOrNotBack);
    //    Q_PROPERTY(Mode mode READ getMode WRITE setMode);
    //    Q_PROPERTY(Cmd cmd READ getCmd WRITE setCmd);
private:
    enum  Mode{
        S2E=0X14,
        E2S=0X15,
        C2E=0X16,
        C2S=0X17
    };
    enum  Packet{
        DataHead=0x11,
        BackOn=0x12,
        BackOff=0x13,
        End=0x19,
        CmdHead=0x1A
    };
    enum  Cmd{
        Start=0x1B,
        Stop=0x1C,
        Continue=0x1D
    };
    const QString parms[14]={"d,","i,","o,","se,","es,","ne,","ns,","n,","f,","c,","t,","p,","e,"};
    FileManager *_mapServer=nullptr;
    Mode _mode=S2E;
    Packet _orNotBack=BackOn;
    QString _map="";
    Cmd _cmd=Start;
    int checkBits=0;
    QString waitSendMsg="";
    int p_mapsize=0;
    QJsonArray pointArrays;
    QVector<QJsonObject> p_maplist;
    SerialManager *p_SerialServer=nullptr;
    QJsonObject p_currentPoint;
private:
    void setOrNotBack(bool value);
    bool getOrNotBack();
    Mode getMode();
    double getDistance(QJsonObject p1,QJsonObject p2);
    //    Cmd getCmd();
    QString dealData(Mode mode=S2E,Packet back=BackOn);
    QString jobjToString(QVector<QJsonObject>);

public slots:
    void setmapfile(QString filename);
    void currenPoint(QString msg);
};

#endif // CMDMANAGER_H
