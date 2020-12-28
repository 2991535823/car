#include "cmdmanager.h"
#include "serialmanager.h"
#include "filemanager.h"
CmdManager::CmdManager()
{

}

void CmdManager::sendtostm32(bool ornotdata)
{

    QString head=ornotdata?parms[DataHead-0x11]:parms[CmdHead-0x11];
    waitSendMsg.append(head);
    if(ornotdata)
    {
        waitSendMsg.append(parms[_orNotBack-0x11]);
        waitSendMsg.append(parms[_mode-0x11]);
        DebugManager::v("start debug cmd");
        waitSendMsg.append(dealData(_mode,_orNotBack));

        waitSendMsg.append(QString::number(p_mapsize));
    }else {
        waitSendMsg.append(parms[_cmd-0x11]);
    }
    waitSendMsg.append('*');
    waitSendMsg+=QString::number(DataCheck::calculate(waitSendMsg),16);
    send(p_SerialServer,waitSendMsg);
}

bool CmdManager::send(SerialManager *manager, QString msg)
{
//    DebugManager::v("发送消息给STM32");
    manager->sendMsg(msg);
    waitSendMsg.clear();
    return  true;
}

QJsonObject CmdManager::getMinDistancePoint()
{
    QJsonObject minpoint;
    currenPoint(p_SerialServer->getSerialMsg());
    minpoint["val"]=getDistance(p_currentPoint,p_maplist[0]);
    minpoint["position"]=0;
    for(int i=1;i<p_maplist.size();i++)
    {
        if(minpoint["val"].toDouble()>getDistance(p_currentPoint,p_maplist[i]))
        {
            minpoint["position"]=i;
            minpoint["val"]=getDistance(p_currentPoint,p_maplist[i]);
        }
    }
    minpoint["x"]=p_maplist[minpoint["position"].toInt()]["x"];
    minpoint["y"]=p_maplist[minpoint["position"].toInt()]["y"];
    return minpoint;
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

void CmdManager::serialServer(SerialManager *manager)
{
    p_SerialServer=manager;
//    connect(p_SerialServer,&SerialManager::readDone,this,&CmdManager::currenPoint);
}

CmdManager::Mode CmdManager::getMode()
{
    return _mode;
}

double CmdManager::getDistance(QJsonObject p1, QJsonObject p2)
{
    double p1x=p1["x"].toString().toDouble();
    double p1y=p1["y"].toString().toDouble();
    double p2x=p2["x"].toString().toDouble();
    double p2y=p2["y"].toString().toDouble();
    double disx2=pow((p1x-p2x)*1000000,2);
    double disy2=pow((p1y-p2y)*1000000,2);
    return  disx2+disy2;
}

QString CmdManager::dealData(Mode mode, Packet back)
{
    QVector<QJsonObject> copy;
    QString map;
    map.clear();
    p_maplist.clear();
    for(auto &&i:pointArrays)
    {
        p_maplist.append(i.toObject());
    }
    copy=p_maplist;
    switch (mode) {
    case CmdManager::S2E:
    {
        map=jobjToString(p_maplist);
        if(back==BackOn)
        {
            copy.pop_back();
            copy.reserve(copy.size());
            std::reverse(copy.begin(),copy.end());
            map+=jobjToString(copy);
        }
    }
        break;
    case CmdManager::E2S:
    {
        p_maplist.reserve(p_maplist.size());
        std::reverse(p_maplist.begin(),p_maplist.end());
        map=jobjToString(p_maplist);
        if(back==BackOn)
        {
            copy.pop_front();
            map+=jobjToString(copy);
        }
    }
        break;
    case CmdManager::C2E://当前到终点
    {
        QJsonObject t=getMinDistancePoint();
        qDebug()<<t["position"].toInt();
        if(t["position"].toInt()!=0)
        {
            if(t["position"].toInt()==(p_maplist.size()-1))
            {
                DebugManager::v("小车在最后一个位置了，注意");
            }
            p_maplist.remove(0,t["position"].toInt());//留下后面的数据
        }

        map=jobjToString(p_maplist);
        if(back==BackOn)
        {
            copy.pop_back();
            copy.reserve(copy.size());
            std::reverse(copy.begin(),copy.end());
            map+=jobjToString(copy);
        }

    }
        break;
    case CmdManager::C2S://当前到起点
    {
        QJsonObject t=getMinDistancePoint();
        //三目运算符替代if else
        (t["position"].toInt()!=(p_maplist.size()-1))?p_maplist.remove(t["position"].toInt()+1,p_maplist.size()-(t["position"].toInt()+1)):DebugManager::e("空语句");
//        if(t["position"].toInt()!=(p_maplist.size()-1))
//        {
//            p_maplist.remove(t["position"].toInt()+1,p_maplist.size()-(t["position"].toInt()+1));//留下前面的数据
//        }
        p_maplist.reserve(p_maplist.size());
        std::reverse(p_maplist.begin(),p_maplist.end());
        map=jobjToString(p_maplist);
        if(back==BackOn)
        {
            copy.pop_front();
            map+=jobjToString(copy);
        }
    }
        break;
    default:
    {
    }
        break;
    }
    p_mapsize=p_maplist.size()+(back==BackOn?copy.size():0);
    return map;
}

QString CmdManager::jobjToString(QVector<QJsonObject> vjobj)
{
    QString str;
    for(auto &&i:vjobj)
    {
        str.append(i["x"].toString()+','+i["y"].toString()+',');
    }
    return str;
}


void CmdManager::setmapfile(QString filename)
{
    QString file=filename;
    _map.clear();
    if(file==""){
        //        QMessageBox::information(NULL,"提示","没有地图文件，请不要发送",QMessageBox::Yes);
    }else {
        QJsonObject t=_mapServer->getmap(file);
        pointArrays=t["data"].toArray();
        p_mapsize=pointArrays.size();
        for(auto &&i:pointArrays)
        {
            _map.append(i.toObject()["x"].toString()+',');
            _map.append(i.toObject()["y"].toString()+',');
        }
    }
    DebugManager::v("map name:"+file);
}

void CmdManager::currenPoint(QString msg)
{
    QStringList msglist=msg.split(',');
    p_currentPoint["x"]=msglist[10];
    p_currentPoint["y"]=msglist[9];
}

void CmdManager::setCmd(int cmd)
{

    _cmd=(Cmd)(cmd);
    qDebug()<<_cmd;
}
