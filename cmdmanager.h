#ifndef CMDMANAGER_H
#define CMDMANAGER_H

#include <QObject>

class SerialManager;
class FileManager;
class CmdManager:public QObject
{
    Q_OBJECT
public:
    CmdManager();
//    Q_PROPERTY();

    Q_INVOKABLE void sendtostm32(SerialManager *manager,bool dataorcmd=true);
    Q_INVOKABLE void setCmd(int cmd);
    Q_INVOKABLE void setMode(int mode);
    Q_INVOKABLE void mapServer(FileManager *manager);
    bool send(SerialManager *manager,QString msg);
    Q_PROPERTY(int orNotBack READ getOrNotBack WRITE setOrNotBack);
//    Q_PROPERTY(Mode mode READ getMode WRITE setMode);
//    Q_PROPERTY(Cmd cmd READ getCmd WRITE setCmd);
private:
    enum  Mode{
        S2E=0X14,
        E2S=0X15,
        C2E=0X16,
        E2C=0X17
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

    const QChar parms[14]={QChar(DataHead),QChar(BackOn),QChar(BackOff),QChar(S2E),QChar(E2S),QChar(C2E),QChar(E2C),QChar(0X18),QChar(End),QChar(CmdHead),QChar(Start),QChar(Stop),QChar(Continue)};
    void setOrNotBack(bool value);
    bool getOrNotBack();

    Mode getMode();
//    Cmd getCmd();
    FileManager *_mapServer=nullptr;
    Mode _mode=S2E;
    int _orNotBack=BackOff;
    QString _map="15.0,128.4";
    Cmd _cmd=Start;
    int checkBits=0;
    QString waitSendMsg="";
public slots:
    void setmapfile();

};

#endif // CMDMANAGER_H
