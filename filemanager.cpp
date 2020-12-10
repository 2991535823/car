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
    DebugManager::d("file manager create");
}

FileManager::~FileManager()
{
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
        code=deleteFile();
        break;
    default:
        break;
    }
    return code;
}

bool FileManager::setParms(QString filename)
{
    //正则校验
    _filename=filename;
    clearMapData();
    QJsonObject jsonObject;
    QJsonDocument jsonDoc;
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

QJsonObject FileManager::getmap(QString filename)
{
    if(filename.indexOf(".json")!=-1)
    {
        filename.remove(".json");
    }
    QJsonObject temp=readFile(filename,Suffix,MapFolder);
    return  temp;
}

void FileManager::seteditfile(QString name)
{
    //    qDebug()<<name;
    _editfile=name;
}



bool FileManager::startCollection()
{

    map.append(gpsData);
    return true;
}

bool FileManager::stopCollection()
{
    return true;
}

bool FileManager::doneCollection()
{
    QJsonObject jsonobj=readFile(_filename,Suffix,MapFolder);
    jsonobj.insert("data",map);
    writefile(_filename,jsonobj,Suffix,MapFolder);
    clearMapData();
    return true;
}

bool FileManager::deleteFile(QString folder)
{
    QFile *deletefile=new QFile(folder+_editfile);
    return deletefile->remove();
}


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

QStringList FileManager::getmaplist()
{
    return _maplist;
}

int FileManager::getNodeSize()
{
    return _nodesize;
}

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


bool FileManager::createFolder(QString folder)
{
    bool code=false;
    if(!dir.exists(folder)){
        dir.mkdir(folder);
        code=true;
    }
    return code;
}

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

void FileManager::setNodeSize()
{
    if(_nodesize!=map.size())
    {
        _nodesize=map.size();
        emit nodeSizeUpdata();
    }

}

bool FileManager::clearMapData()
{
    int length=map.size();
    for(int i=0;i<length ;i++)
    {
        map.removeFirst();
    }
    return true;
}

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

bool FileManager::setMapPath(QString MapPath)
{
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
        }
    }
    return true;//不规范
}

void FileManager::readSerial(const QString msg)
{
    gpsData=msg;
}

void FileManager::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    setmaplist();
    setNodeSize();
}
