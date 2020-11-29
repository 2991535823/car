#include "filemanager.h"

FileManager::FileManager(QObject *parent) : QObject(parent)
{
    Q_UNUSED(parent)
    if(!dir.exists(MapFolder)){
        dir.mkdir(MapFolder);

    }
    startTimer(500);
    setmaplist();
    qDebug()<<"file manager create";
}

FileManager::~FileManager()
{
    _file->close();
    delete _file;
    qDebug()<<"file manager destory";
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
        jsonObject = readFile(filename);
    }else {
        createFile(filename);
    }
    jsonObject.insert(filestatus,time.toString());
    writefile(filename,jsonObject);
    return true;
}

void FileManager::seteditfile(QString name)
{
    qDebug()<<name;
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
    QJsonObject jsonobj=readFile(_filename);
    jsonobj.insert("data",map);
    writefile(_filename,jsonobj);
    clearMapData();
    return true;
}

bool FileManager::deleteFile()
{
    QFile *deletefile=new QFile(MapFolder+_editfile);
    return deletefile->remove();
}

QJsonObject FileManager::readFile(QString filename)
{
    _file=new QFile(MapFolder+filename+Suffix);
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

QStringList FileManager::getmaplist()
{
    return _maplist;
}

int FileManager::getNodeSize()
{
    return _nodesize;
}

bool FileManager::writefile(QString filename, QJsonObject obj)
{
    _file->setFileName(MapFolder+filename+Suffix);
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

bool FileManager::createFile(QString filename)
{
    bool code=false;
    _file=new QFile(MapFolder+filename+Suffix);
    if(!_file->exists())
    {
        code=_file->open(QIODevice::ReadWrite|QIODevice::Text);
        _file->close();
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
    qDebug()<< "setserial:"<<limit;
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
