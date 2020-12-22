#ifndef MAPASSIST_H
#define MAPASSIST_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <debugmanager.h>
#include "datacheck.h"
#include <serialmanager.h>
class MapAssist : public QObject
{
    Q_OBJECT
public:
    explicit MapAssist(QObject *parent = nullptr);
    Q_PROPERTY(QJsonObject carpoint MEMBER p_carpoint NOTIFY onCarPointChanged);
    Q_PROPERTY(SerialManager *serial READ getSerial WRITE setSerial NOTIFY onSerialChanged);
    Q_PROPERTY(QJsonObject mapdata MEMBER p_map NOTIFY onMapReload);
private:
    SerialManager* getSerial();
    void setSerial(SerialManager *serial);
public slots:
    //    void debug(const QJsonObject &point);
    void updataPoint(const QString msg);
signals:
    void onCarPointChanged(const QJsonObject &point);//HTML需要的是信号
    void onSerialChanged(SerialManager *serial);
    void onMapReload(const QJsonObject &data);
private:
    QJsonObject p_map;
    QJsonObject p_carpoint;
    SerialManager *p_serial=nullptr;
};

#endif // MAPASSIST_H
