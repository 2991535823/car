#include "mapmanager.h"

MapManager::MapManager(QQuickItem *parent):QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);
}

void MapManager::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
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
