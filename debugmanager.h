#ifndef DEBUGMANAGER_H
#define DEBUGMANAGER_H
#include <QDebug>

class DebugManager:public QDebug
{

public:
    static void v(QString msg);
    static void d(QString msg);
    static void i(QString msg);
    static void w(QString msg);
    static void e(QString msg);
private:
    enum Level{
        NONE=0,
        VERBOSE,
        DEBUG,
        INFO,
        WARN,
        ERROR
    };
    static const int level=NONE;
};

#endif // DEBUGMANAGER_H
