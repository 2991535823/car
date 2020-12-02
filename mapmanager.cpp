#include "mapmanager.h"

MapManager::MapManager(QQuickItem *parent):QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);
    startTimer(1000);
}


void MapManager::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setWindow(0, height(),  width(), -1 * height());//坐标系变换
    if(_viewCar){
        painter->setPen(_pen);
        painter->drawPoint(carPoint);

    }else
    {
        qDebug()<<"clear car";
    }
    if(_viewMap)
    {
        painter->setPen(Qt::black);
        map=file2map();
        for(int i=0;i<map.size();i++)
        {
            QLineF tem=transLine(map[i]);
            painter->save();

            if(i==map.size()-1){
                painter->setWindow(-1*tem.p2().x(),-height()+tem.p2().y(),width(),height());
                painter->drawText(QPointF(0,0),QString::number(i+1));
            }else {
                painter->setWindow(-1*tem.p1().x(),-height()+tem.p1().y(),width(),height());
                painter->drawText(QPointF(0,0),QString::number(i));
            }
            painter->restore();
            painter->drawLine(tem);
        }

    }else {

    }

}

void MapManager::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        //        line=QLineF(0,0,200,400);
        break;
    case Qt::RightButton:
        //        line=QLineF(0,0,400,200);
        break;
    default:
        break;
    }
    update();
}

void MapManager::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    _width=width();
    _height=height();
    //    qDebug()<<"width"<<_width<<"height"<<_height;
    //    if(carLocation.indexOf("$GPHCD"))
    //    {
    //        carPoint=QPointF(carLocation[12].toInt(),carLocation[13].toInt());
    //    }
    //    update();
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

QLineF MapManager::transLine(QLineF line)
{
    QPointF start=line.p1();
    QPointF end=line.p2();
    float mapsizex=maplimit[0]-maplimit[2];
    float mapsizey=maplimit[1]-maplimit[3];
    int scalex=width()/(mapsizex+1);
    qDebug()<<width();
    int scaley=height()/(mapsizey+1);
    qreal x=start.x()-maplimit[2];
    qreal y=start.y()-maplimit[3];
    start =QPointF(x*scalex,y*scaley);
    x=end.x()-maplimit[2];
    y=end.y()-maplimit[3];
    end =QPointF(x*scalex,y*scaley);
    return QLineF(start,end);
}



QVector<QLineF> MapManager::file2map()
{
    QJsonObject _map=_file->getmap();
    QJsonArray _mapnode=_map["data"].toArray();
    qDebug()<<_mapnode;
    QVector<QLineF> lineGroup;
    float x0,y0,x1,y1;
    QStringList start,end;

    for (int i=0;i<_mapnode.size()-1;i++) {
        start=_mapnode[i].toString().split(',');
        x0=start[12].toFloat();
        y0=start[13].toFloat();
        end=_mapnode[i+1].toString().split(',');
        x1=end[12].toFloat();
        y1=end[13].toFloat();
        //有问题
        if(i==0)
        {
            maplimit[0]=x0;
            maplimit[1]=y0;
            maplimit[2]=x0;
            maplimit[3]=y0;
        }
        maplimit[0]=qMax(maplimit[0],x1);
        maplimit[1]=qMax(maplimit[1],y1);
        maplimit[2]=qMin(maplimit[2],x1);
        maplimit[3]=qMin(maplimit[3],y1);
        lineGroup.insert(i,QLineF(x0,y0,x1,y1));
    }
    return lineGroup;
}



void MapManager::readSerial(const QString msg)
{
    carLocation=msg.split(',');
    qDebug()<<carLocation;
}
