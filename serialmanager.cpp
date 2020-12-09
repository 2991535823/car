#include "serialmanager.h"

SerialManager::SerialManager(QObject *parent) : QObject(parent)
{
    DebugManager::d("Serial Manager create and start serialport scanning");
    startTimer(5000);
    setports();
}

SerialManager::~SerialManager()
{
    delete port;
    delete info;
    DebugManager::d("Serial Manger destory");
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
    DebugManager::d(_baudrate+"|"+_com);
}

bool SerialManager::sendMsg(QString msg,bool orNot16)
{
    int code;
    if(orNot16){
        _msg=String2Hex(msg);
    }else {
        _msg=msg.toLatin1();
    }
    if(_msg.indexOf('\n')==-1){
        _msg.append('\r');
        _msg.append('\n');
    }
    qDebug()<<_msg;
    code=port->write(_msg);
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
    DebugManager::d("connect "+_com);
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
    DebugManager::d("disconnect "+_com);
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

void SerialManager::setOrNotBack(bool value)
{
    _orNotBack=value;
    DebugManager::d("返航开关:" +QString::number(_orNotBack));
}


void SerialManager::readSerial()
{
    static int elposition=0;
    dataRoom+=port->readLine();
    DebugManager::w(dataRoom);
    for(auto &&i:dataRoom){
        if(i!='\n'){
            elposition++;
        }else {
            serialMsg=dataRoom.left(elposition+1);
            DebugManager::i(serialMsg);
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
}
//16进制发送
QByteArray SerialManager::String2Hex(QString &str)
{
    QByteArray senddata;
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return  senddata;
}
char SerialManager::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

