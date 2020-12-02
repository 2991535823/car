#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#include <QQuickPaintedItem>
#include "serialmanager.h"
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QPen>
#include <QPainter>
#include "filemanager.h"
#include <QPointF>
#include <QBrush>
class MapManager:public QQuickPaintedItem
{
    Q_OBJECT
public:
    MapManager(QQuickItem *parent=nullptr);

    Q_PROPERTY(SerialManager *serial WRITE setserial);
    Q_PROPERTY(FileManager *file WRITE setfile);
    Q_PROPERTY(bool viewcar WRITE setviewcar);
    Q_PROPERTY(bool viewmap WRITE setviewmap);
private:
    void setserial(SerialManager *manager);
    void setfile(FileManager *manager);
    void setviewcar(bool view);
    void setviewmap(bool view);
    bool _viewCar;
    bool _viewMap;

    QLineF line=QLineF(0,0,100,100);
    QPointF carPoint=QPointF(50,100);
    SerialManager *_serial;
    FileManager *_file;
    QString carLocation;
    QPen _pen=QPen(QBrush(Qt::SolidPattern),15);
public slots:
    void readSerial(const QString msg);
    // QQuickPaintedItem interface
public:
    virtual void paint(QPainter *painter) override;

    // QQuickItem interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // MAPMANAGER_H
