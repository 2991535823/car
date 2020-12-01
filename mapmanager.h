#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#include <QQuickPaintedItem>

#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QPen>
#include<QPainter>
class MapManager:public QQuickPaintedItem
{
    Q_OBJECT
public:
    MapManager(QQuickItem *parent=nullptr);

private:
    QLineF line=QLineF(0,0,100,100);

    // QQuickPaintedItem interface
public:
    virtual void paint(QPainter *painter) override;

    // QQuickItem interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // MAPMANAGER_H
