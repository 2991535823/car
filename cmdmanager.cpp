#include "cmdmanager.h"
#include "serialmanager.h"
#include "filemanager.h"
CmdManager::CmdManager()
{

}

void CmdManager::sendtostm32(SerialManager *manager,bool dataorcmd)
{
    QChar head=dataorcmd?parms[DataHead-0x11]:parms[CmdHead-0x11];
    waitSendMsg.append(head);
    if(dataorcmd)
    {
        waitSendMsg.append(parms[_orNotBack-0x11]);
        waitSendMsg.append(parms[_mode-0x11]);
        if(_map=="")
        {
            QMessageBox::information(NULL,"提示","地图数据为空！请检查",QMessageBox::Yes);
            return;
        }else {
          waitSendMsg.append(_map);
        }
    }else {
        waitSendMsg.append(parms[_cmd-0x11]);
    }
    waitSendMsg.append('*');
    waitSendMsg+=QString::number(DataCheck::calculate(waitSendMsg),16);
    send(manager,waitSendMsg);
}

bool CmdManager::send(SerialManager *manager, QString msg)
{
    DebugManager::v("发送消息给STM32");
    manager->sendMsg(msg);
    waitSendMsg.clear();
    return  true;
}
void CmdManager::setOrNotBack(bool value)
{
    _orNotBack=value?BackOn:BackOff;
    DebugManager::d("返航开关:" +QString::number(_orNotBack));
}

bool CmdManager::getOrNotBack()
{
    return _orNotBack;
}

void CmdManager::setMode(int mode)
{
    _mode=Mode(mode);
}

void CmdManager::mapServer(FileManager *manager)
{
    _mapServer=manager;
    connect(_mapServer,&FileManager::editfileUpdata,this,&CmdManager::setmapfile);
    DebugManager::d("得到地图提供方");
}

CmdManager::Mode CmdManager::getMode()
{
    return _mode;
}

void CmdManager::setmapfile(QString filename)
{
    QString file=filename;
    _map.clear();
    if(file==""){
//        QMessageBox::information(NULL,"提示","没有地图文件，请不要发送",QMessageBox::Yes);
    }else {
        QJsonObject temp=_mapServer->getmap(file,false);
        QJsonArray pointArrays=temp["data"].toArray();
        for(auto i:pointArrays)
        {
            _map.append(i.toString()+',');
        }
    }
    DebugManager::v("map name:"+file);
}

//CmdManager::Cmd CmdManager::getCmd()
//{
//    return _cmd;
//}

void CmdManager::setCmd(int cmd)
{

    _cmd=(Cmd)(cmd);
    qDebug()<<_cmd;
}
