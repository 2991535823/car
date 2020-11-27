#include "serialmanager.h"

SerialManager::SerialManager(QObject *parent) : QObject(parent)
{
    qDebug()<<"create SerialManager and start timer";
    startTimer(5000);
    setports();
}

SerialManager::~SerialManager()
{
    qDebug()<<"delete SerialManger";
}

bool SerialManager::doCmd(Cmd cmd)
{
    bool code=false;
    switch (cmd) {
    case CarStart:
        code=carStart();
        break;
    case CarStop:
        code=carStop();
        break;
    case CarResatrt:
        code=carRestart();
        break;
    case SerialConnect:
        code=serialConnect();
        break;
    case SerialDisconnect:
        code=serialDisconnect();
        break;
    }
    return code;
}



void SerialManager::setParms(QString baudrate, QString com)
{
    _baudrate=baudrate;
    _com=com;
    qDebug()<<_baudrate<<"|"<<_com;
}

bool SerialManager::sendMsg(QString msg)
{
    int code;
    if(msg.indexOf('\n')!=-1){
        msg+"\r\n";
    }
    code= port->write(msg.toLatin1());
    return code != -1?true:false;
}

bool SerialManager::carStart()
{
    return true;
}

bool SerialManager::carStop()
{
    return true;
}

bool SerialManager::carRestart()
{
    return true;
}

bool SerialManager::serialConnect()
{
    qDebug()<<"connect "+_com;
    port->setPortName(_com);
    if(port->open(QIODevice::ReadWrite))
    {
        //配置串口相应参数
        port->setBaudRate(_baudrate.toInt());
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setStopBits(QSerialPort::OneStop);
        port->setFlowControl(QSerialPort::NoFlowControl);
        connect(port,&QSerialPort::readyRead,this,&SerialManager::readSerial);
        return true;
    }else {
        port->close();
        return false;
    }
}

bool SerialManager::serialDisconnect()
{
    qDebug()<<"disconnect "+_com;
    port->close();
    return true;
}

QStringList SerialManager::getports()
{
        return _ports;
}

void SerialManager::setports()
{
    QStringList list;
    for (auto info : QSerialPortInfo::availablePorts()) {
        list.append(info.portName());
    }
    if(list!=_ports){
        _ports=list;
        emit portsupdata();
    }
}


void SerialManager::readSerial()
{
    static int elposition=0;
    dataRoom+=port->readLine();
    for(auto &&i:dataRoom){
        if(i!='\n'){
            elposition++;
        }else {
            serialMsg=dataRoom.left(elposition+1);
            dataRoom.remove(0,elposition+1);
            elposition=0;
            emit readDone(serialMsg);
        }
    }
}

void SerialManager::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    setports();
    qDebug()<<"timer event ok";
}

