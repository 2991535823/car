#include "filemanager.h"

FileManager::FileManager(QObject *parent) : QObject(parent)
{
    Q_UNUSED(parent)
    if(createFolder(iniFolder))
    {
        setMapPath(MapFolder);
    }

    MapFolder=getMapPath();
    DebugManager::v(MapFolder+"是目前的地图文件夹");
    DebugManager::v(createFolder(MapFolder)?"地图文件夹创建":"地图文件夹存在");
    startTimer(500);

    setmaplist();
    seteditfile(_maplist.size()>0?_maplist.first():"no file.json");
    DebugManager::d("file manager create");
}

FileManager::~FileManager()
{
    //写入日志
    if(!createFile("carlog",".log",MapFolder))
    {
        QJsonObject temp;
        temp.insert("creat",QDateTime::currentDateTime().toString());
        temp.insert("log",logarray);
        writefile("carlog",temp,".log",MapFolder);
    }
    _file->close();
    delete _file;
    DebugManager::d("file manager destory");
}

bool FileManager::doCmd(FileManager::Cmd cmd)
{
    Q_UNUSED(cmd)
    bool code=false;
    switch (cmd) {
    case Start:
        code=startCollection();
        break;
    case Stop:
        code=stopCollection();
        break;
    case Done:
        code=doneCollection();
        break;
    case Delete:
        code=deleteFile(_editfile,MapFolder);
        break;
    default:
        break;
    }
    return code;
}

bool FileManager::setParms(QString filename)
{
    //创建一个地图
    _filename=filename;
    clearMapData();
    QJsonObject jsonObject;
    //    QJsonDocument jsonDoc;
    bool fileExit=_file->exists(MapFolder+_filename+Suffix);

    QString filestatus=fileExit?"Edit":"Create";

    QDateTime time=QDateTime::currentDateTime();
    if(fileExit){
        jsonObject = readFile(filename,Suffix,MapFolder);
    }else {
        createFile(filename,Suffix,MapFolder);
    }
    jsonObject.insert(filestatus,time.toString());
    writefile(filename,jsonObject,Suffix,MapFolder);
    return true;
}

void FileManager::createmap(QString filename)
{
    _filename=filename;
    createFile(filename,Suffix,MapFolder);
}
//开放给MapManager和Cmd的接口
QJsonObject FileManager::getmap(QString filename, bool orNotToMap)
{

    if(filename.indexOf(".json")!=-1)
    {
        filename.remove(".json");
    }
    QJsonObject temp=readFile(filename,Suffix,MapFolder);//下面的代码可以优化
    if(orNotToMap)//发送给地图
    {
        QJsonArray pointmsgs= temp["data"].toArray();
        QJsonArray returnPointMsgs;
        QJsonObject t;
        for(auto &&i:pointmsgs)
        {
            QStringList info=i.toString().split(',');
            t["x"]=info[10];
            t["y"]=info[9];
            returnPointMsgs.append(t);
        }
        temp["data"]=returnPointMsgs;
        return  temp;
    }else {//发送给cmd
        QJsonArray pointmsgs= temp["data"].toArray();
        QJsonArray returnPointMsgs;
        for(auto &&i:pointmsgs)
        {
            QStringList msgs=i.toString().split(',');
            returnPointMsgs.append(msgs[9]+','+msgs[10]);
        }
        temp.insert("data",returnPointMsgs);
        return temp;
    }

}
//设置编辑的地图
void FileManager::seteditfile(QString name)
{

    if(_editfile!=name)
    {
        _editfile=name;
    }
    DebugManager::i("_editfile:"+_editfile);
    emit editfileUpdata(_editfile);
}

QString FileManager::geteditfile()
{
    return _editfile;
}


//采集一个地图点数据
bool FileManager::startCollection()
{
    if(DataCheck::checkFormat(gpsData))//正则校验
    {
        if(map.last()!=gpsData&&DataCheck::checkEffect(gpsData))//有效性校验
        {
            map.append(gpsData);
            return true;
        }else {
            DebugManager::v("小车位置没有变动,或校验位出错,采集停止");
        }
    }else {
        QMessageBox::information(NULL, "提示信息", "地图数据不符合要求,请检查串口！",
                                 QMessageBox::Yes);
    }
    return false;
}

bool FileManager::stopCollection()
{
    if(map.size()>0)
    {
        map.pop_back();
    }

    return true;
}
//完成采集。写入数据
bool FileManager::doneCollection()
{
    QJsonObject jsonobj=readFile(_filename,Suffix,MapFolder);
    jsonobj.insert("data",map);
    writefile(_filename,jsonobj,Suffix,MapFolder);
    clearMapData();
    return true;
}
//写日志
void FileManager::writeLog(QString data)
{
    logarray.append(data);
}
//删除文件
bool FileManager::deleteFile(QString filename, QString folder)
{
    QFile *deletefile=new QFile(folder+filename);
    return deletefile->remove();
}

//读文件内容
QJsonObject FileManager::readFile(QString filename, QString suffix, QString folder)
{
    _file=new QFile(folder+filename+suffix);
    QJsonParseError error;
    QJsonObject tempobj;
    QJsonDocument tempdoc;
    if(_file->exists()){
        if(_file->open(QIODevice::ReadOnly|QIODevice::Text))
        {

            tempdoc=QJsonDocument::fromJson(_file->readAll(),&error);
            tempobj=tempdoc.object();
            _file->close();
        }

    }
    return tempobj;
}

//写文件
bool FileManager::writefile(QString filename, QJsonObject obj, QString suffix, QString folder)
{
    _file->setFileName(folder+filename+suffix);
    bool code=false;
    if(_file->exists()){
        if(_file->open(QIODevice::ReadWrite|QIODevice::Truncate))
        {
            QJsonDocument _doc(obj);
            _file->write(_doc.toJson());
            _file->close();
            code=true;
        }else {
            code=false;
        }
    }
    return code;
}
//得到地图List
QStringList FileManager::getmaplist()
{
    return _maplist;
}
//得到采集的地图点
int FileManager::getNodeSize()
{
    return _nodesize;
}
//创建文件
bool FileManager::createFile(QString filename, QString suffix, QString folder)
{
    bool code=false;
    _file=new QFile(folder+filename+suffix);
    if(!_file->exists())
    {
        code=_file->open(QIODevice::ReadWrite|QIODevice::Text);
        _file->close();
    }
    return code;
}

//创建文件
bool FileManager::createFolder(QString folder)
{
    bool code=false;
    if(!dir.exists(folder)){
        dir.mkdir(folder);
        code=true;
    }
    return code;
}
//更新地图列表信息
void FileManager::setmaplist()
{
    QDir dir(MapFolder);
    QStringList list;
    QFileInfoList infolist=dir.entryInfoList();
    for(auto &&i:infolist){
        if(i.suffix()=="json")
        {
            list.append(i.fileName());
        }
    }
    if(_maplist!=list){
        _maplist=list;
        emit maplistupdata();
    }
}
//设置该类的Serial的对象
void FileManager::setserial(SerialManager *manager)
{
    _serial = manager;
    static int limit=0;
    //    qDebug()<< "setserial:"<<limit;
    if(limit==0)
    {
        connect(_serial,&SerialManager::readDone,this,&FileManager::readSerial);
    }
    limit++;
}

SerialManager *FileManager::getSerial()
{
    return _serial;
}
//设置采集数据的个数
void FileManager::setNodeSize()
{
    if(_nodesize!=map.size())
    {
        _nodesize=map.size();
        emit nodeSizeUpdata();
    }

}
//清楚地图数据
bool FileManager::clearMapData()
{
    int length=map.size();
    for(int i=0;i<length ;i++)
    {
        map.removeFirst();
    }
    return true;
}
//得到地图文件存放的路径
QString FileManager::getMapPath()
{
    if(createFile("settings",".ini",iniFolder))
    {
        DebugManager::d("配置文件夹不存在,开始进行设置......");
        setMapPath(MapFolder);
        return MapFolder;
    }else {
        QJsonObject temp=readFile("settings",".ini",iniFolder);
        QJsonValue path=temp["mapfolder"];
        return path.toString();
    }
}
//设置地图文件存放的路径
bool FileManager::setMapPath(QString MapPath)
{
    //这两行代码重要
    MapPath.remove("file:///");
    MapPath.append('/');
    if(createFile("settings",".ini",iniFolder))
    {
        //第一次创建
        QJsonObject temp;
        temp.insert("mapfolder",MapPath);
        writefile("settings",temp,".ini",iniFolder);
        emit mapPathUpdata();
    }else {
        QJsonObject temp=readFile("settings",".ini",iniFolder);
        QJsonValue path=temp["mapfolder"];
        if(path!=MapPath)
        {
            temp.insert("mapfolder",MapPath);
            writefile("settings",temp,".ini",iniFolder);
            emit mapPathUpdata();
            QMessageBox::information(NULL,"提示","修改了地图存放路径，必须重启软件加载设置",QMessageBox::Yes);
        }
    }
    return true;//不规范
}
//槽函数
void FileManager::readSerial(const QString msg)
{
    gpsData=msg;
    writeLog(msg);
}
//重写事件
void FileManager::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    setmaplist();
    setNodeSize();
}
