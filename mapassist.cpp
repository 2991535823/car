#include "mapassist.h"

MapAssist::MapAssist(QObject *parent) : QObject(parent)
{
//    "106.2459461","29.4899794"
    p_carpoint.insert("x","106.123");
    p_carpoint.insert("y","29.520");
    p_map.insert("x","106.123");
    p_map.insert("y","29.520");
//    connect(this,&MapAssist::onCarPointChanged,this,&MapAssist::debug);
}

SerialManager *MapAssist::getSerial()
{
    return p_serial;
}

void MapAssist::setSerial(SerialManager *serial)
{
    p_serial=serial;
    connect(p_serial,&SerialManager::readDone,this,&MapAssist::updataPoint);
    emit onSerialChanged(p_serial);
}


//void MapAssist::debug(const QJsonObject &point)
//{
//    DebugManager::i(point["x"].toString());
//}

void MapAssist::updataPoint(const QString msg)
{
    DebugManager::i(msg);
    if(DataCheck::checkFormat(msg)&&DataCheck::checkEffect(msg))
    {
        QStringList array=msg.split(',');
        QJsonObject point;
        point["x"]=array[10];
        point["y"]=array[9];
        this->setProperty("carpoint",point);
    }
}
