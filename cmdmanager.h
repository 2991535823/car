#ifndef CMDMANAGER_H
#define CMDMANAGER_H



#include <QObject>
class SerialManager;
class CmdManager:public QObject
{
    Q_OBJECT
public:
    CmdManager();
//    Q_PROPERTY();

    Q_INVOKABLE void sendtostm32(SerialManager *manager);
    bool send(SerialManager *manager,QString msg);
    Q_PROPERTY(int orNotBack READ getOrNotBack WRITE setOrNotBack);
    Q_PROPERTY(Mode mode READ getMode WRITE setMode);
private:
    enum Mode{
        S2E=0X04,
        E2S=0X05,
        C2E=0X06,
        EC2=0X07
    };
    enum Packet{
        DataHead=0x01,
        BackOn=0x02,
        BackOff=0x03,
        End=0x09,
        CmdHead=0x0A
    };


    void setOrNotBack(int value);
    bool getOrNotBack();
    void setMode(Mode mode);
    Mode getMode();
    Mode _mode=S2E;
    int _orNotBack=BackOff;
    int checkBits=0;
    QString waitSendMsg="";
};

#endif // CMDMANAGER_H
