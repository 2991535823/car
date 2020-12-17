#include "cmdmanager.h"
#include "serialmanager.h"

CmdManager::CmdManager()
{

}

void CmdManager::sendtostm32(SerialManager *manager)
{
    QChar head=DataHead;
    QChar parms[2]={QChar(_orNotBack),QChar(_mode)};
    waitSendMsg.append(head);

    waitSendMsg.append(parms,2);
    waitSendMsg.append('*');
    waitSendMsg+=QString::number(DataCheck::calculate(waitSendMsg),16);
    send(manager,waitSendMsg);
}

bool CmdManager::send(SerialManager *manager, QString msg)
{
    manager->sendMsg(msg);
    return  true;
}
void CmdManager::setOrNotBack(int value)
{
    _orNotBack=value;
    DebugManager::d("返航开关:" +QString::number(_orNotBack));
}

bool CmdManager::getOrNotBack()
{
    return _orNotBack;
}

void CmdManager::setMode(CmdManager::Mode mode)
{
    _mode=mode;
}

CmdManager::Mode CmdManager::getMode()
{
    return _mode;
}
