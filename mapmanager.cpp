#include "mapmanager.h"

MapManager::MapManager(QQuickItem *parent):QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);
    startTimer(200);
//    carimg.load(QStringLiteral(":/img/car.jpg"));
    carimg.load(QStringLiteral("E:/Desktop/car/car.jpg"));
}

//绘图
void MapManager::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setWindow(0, height(),  width(), -1 * height());//坐标系变换
    if(_viewCar){
        painter->save();
        QPointF tem=transPoint(carPoint);
        painter->setWindow(-1*tem.x(),height()-tem.y(),width(), -1 * height());//小车车坐标系变换
        painter->rotate(cardirection);
        DebugManager::d(QString::number(cardirection)+"|"+QString::number(carPoint.x())+","+QString::number(carPoint.y())+"|"+QString::number(carimg.width()));
        painter->drawImage(QPointF(-1*carimg.width()/2,-1*carimg.height()/2),carimg);
        painter->restore();

    }else
    {
        DebugManager::d("clear car");
    }
    if(_viewMap)
    {


        map=file2map();
        for(int i=0;i<map.size();i++)
        {

            QLineF tem=transLine(map[i]);

            painter->drawLine(tem);

            painter->save();

            if(i==map.size()-1){
                painter->setPen(_pen);
                painter->setWindow(-1*tem.p2().x(),-height()+tem.p2().y(),width(),height());
                painter->drawPoint(QPointF(0,0));
                painter->setPen(Qt::SolidLine);
                painter->drawText(QPointF(10,10),QString::number(i+2));

            }
            painter->setPen(_pen);
            painter->setWindow(-1*tem.p1().x(),-height()+tem.p1().y(),width(),height());
            painter->drawPoint(QPointF(0,0));
            painter->setPen(Qt::SolidLine);
            painter->drawText(QPointF(10,10),QString::number(i+1));
            painter->restore();
        }

    }else {

    }

}
//鼠标左右键处理
void MapManager::mousePressEvent(QMouseEvent *event)
{
//    QPoint point=event->pos();
    switch (event->button()) {
    case Qt::LeftButton:
    {

    }
        break;
    case Qt::RightButton:
    {

    }
        break;
    default:
        break;
    }
//    update();
}

void MapManager::mouseReleaseEvent(QMouseEvent *event)
{
//    QPoint point=event->pos();
    switch (event->button()) {
    case Qt::LeftButton:
    {

    }
        break;
    case Qt::RightButton:
    {

    }
        break;
    default:
        break;
    }
}
//定时器
void MapManager::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    //    _width=width();
    //    _height=height();

    if(_viewCar)
    {
        if(!carLocation.isEmpty())
        {
            carPoint=QPointF(carLocation[12].toInt(),carLocation[13].toInt());
            cardirection=carLocation[5].toFloat();
            //            qDebug()<<carLocation[5];
        }else {
            carPoint=QPointF(0,0);
            cardirection=0;
        }

        update();
    }

}
void MapManager::setserial(SerialManager *manager)
{
    _serial = manager;
    static int limit=0;
    DebugManager::d( "set map serial :"+QString::number(limit));
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
    return QLineF(transPoint(start),transPoint(end));
}

QPointF &MapManager::transPoint(QPointF &point)
{
    float mapsizex=maplimit[0]-maplimit[2];
    float mapsizey=maplimit[1]-maplimit[3];
    int scalex=0.8*width()/(mapsizex);
    int scaley=0.8*height()/(mapsizey);
    point.setX((point.x()-maplimit[2])*scalex+0.1*width());
    point.setY((point.y()-maplimit[3])*scaley+0.1*height());
    return point;
}

QVector<QLineF> MapManager::file2map()
{
    QJsonObject _map=_file->getmap(_file->_editfile);//getmap接口应当重写
    QJsonArray _mapnode=_map["data"].toArray();

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

int MapManager::calculationdistance(QPointF p1, QPointF p2)
{
    Q_UNUSED(p1)
    Q_UNUSED(p2)
    int distance=0;
    return distance;
}

void MapManager::readSerial(const QString msg)
{
    carLocation=msg.split(',');
}
