#include "mapmanager.h"

MapManager::MapManager(QQuickItem *parent):QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);
}


void MapManager::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);

    if(_viewCar){
       painter->setPen(_pen);
      painter->drawPoint(carPoint);

    }else
    {
        qDebug()<<"clear car";
    }
    painter->setPen(Qt::black);
    painter->drawLine(line);
}

void MapManager::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        line=QLineF(0,0,200,400);
        break;
    case Qt::RightButton:
        line=QLineF(0,0,400,200);
        break;
    default:
        break;
    }
    update();
}
void MapManager::setserial(SerialManager *manager)
{
    _serial = manager;
    static int limit=0;
    qDebug()<< "setserial:"<<limit;
    if(limit==0)
    {
        connect(_serial,&SerialManager::readDone,this,&MapManager::readSerial);
    }
    limit++;
}

void MapManager::setfile(FileManager *manager)
{
    _file=manager;
    qDebug()<<"地图拥有了文件系统对象";
}

void MapManager::setviewcar(bool view)
{
    _viewCar=view;
    update();
}

void MapManager::setviewmap(bool view)
{
    _viewMap=view;
    update();
}



void MapManager::readSerial(const QString msg)
{
    carLocation=msg;
    qDebug()<<carLocation;
}
