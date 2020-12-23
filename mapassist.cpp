#include "mapassist.h"

MapAssist::MapAssist(QObject *parent) : QObject(parent)
{
    //    "106.2459461","29.4899794"
    p_carpoint.insert("x","106.123");
    p_carpoint.insert("y","29.520");
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

FileManager *MapAssist::getFile()
{
    return p_file;
}

void MapAssist::setFile(FileManager *file)
{
    p_file=file;
    connect(p_file,&FileManager::editfileUpdata,this,&MapAssist::updataMap);
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
        point["r"]=array[5];
        this->setProperty("carpoint",point);
    }
}

void MapAssist::updataMap(QString filename)
{
    QString file=filename;
    if(file==""){
        //        QMessageBox::information(NULL,"提示","没有地图文件，请不要发送",QMessageBox::Yes);
    }else {
        QJsonObject temp=p_file->getmap(file);
        QJsonArray pointArrays=temp["data"].toArray();
        if(pointArrays.size()==0)
        {
            QMessageBox::information(NULL,"info","map no data!!!",QMessageBox::Yes);
        }else {
//            DebugManager::i(pointArrays);
            this->setProperty("mapdata",pointArrays);
        }
    }
}
