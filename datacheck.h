#ifndef DATACHECK_H
#define DATACHECK_H
#include <QString>
#include "debugmanager.h"
#include <QRegExp>
class DataCheck
{
public:
    static bool checkEffect(QString data);
    static bool checkFormat(QString data);
};

#endif // DATACHECK_H
