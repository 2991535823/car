#include "debugmanager.h"

void DebugManager::v(QString msg)
{
    if(level>=VERBOSE)
    {
        qDebug()<<msg;
    }
}

void DebugManager::d(QString msg)
{
    if(level>=DEBUG)
    {
        qDebug()<<msg;
    }
}

void DebugManager::i(QString msg)
{
    if(level>=INFO)
    {
        qDebug()<<msg;
    }
}

void DebugManager::w(QString msg)
{
    if(level>=WARN)
    {
        qDebug()<<msg;
    }
}

void DebugManager::e(QString msg)
{
    if(level>=ERROR)
    {
        qDebug()<<msg;
    }
}
