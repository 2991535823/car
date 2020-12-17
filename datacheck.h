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
    static int calculate(QString msg);

    static QByteArray Hex2Str(QString &str);
private:
    static char ConvertHexChar(char ch);
};

#endif // DATACHECK_H
