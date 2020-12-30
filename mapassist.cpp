#include "mapassist.h"

MapAssist::MapAssist(QObject *parent) : QObject(parent)
{

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
            QMessageBox::information(NULL,"info","地图里面没有数据!!!",QMessageBox::Yes);
        }else {
            qDebug()<<pointArrays;
            this->setProperty("mapdata",pointArrays);
        }
    }
}
