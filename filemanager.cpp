#include "filemanager.h"

FileManager::FileManager(QObject *parent) : QObject(parent)
{
    Q_UNUSED(parent)
    if(!dir.exists(MapFolder)){
        dir.mkdir(MapFolder);

    }
    startTimer(1000);
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
        code=deleteMap();
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

    QJsonObject jsonObject;
    QJsonDocument jsonDoc;

    bool fileExit=_file->exists(MapFolder+_filename+Suffix);
    QString filestatus=fileExit?"Edit":"Create";

    _file=new QFile(MapFolder+_filename+Suffix);
    QDateTime time=QDateTime::currentDateTime();
    if(fileExit){
        parseFile(filename);
        jsonObject.insert("Create",createtime);
    }
    jsonObject.insert(filestatus,time.toString());
    jsonDoc.setObject(jsonObject);
    if(_file->open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        qDebug()<<this->className<<"open success";
    }else {
        qDebug()<<this->className<<"open  failed";
    }

    _file->write(jsonDoc.toJson());
    _file->close();
    qDebug()<<this->className;
    return true;
}

void FileManager::seteditfile(QString name)
{
    qDebug()<<name;
    _editfile=name;
}



bool FileManager::startCollection()
{
    return true;
}

bool FileManager::stopCollection()
{
    return true;
}

bool FileManager::doneCollection()
{
    return true;
}

bool FileManager::deleteMap()
{
    QFile *deletefile=new QFile(MapFolder+_editfile);
    return deletefile->remove();

}

bool FileManager::parseFile(QString filename)
{
    _file->setFileName(MapFolder+filename+Suffix);
    QJsonParseError error;
    if(_file->exists()){
        if(_file->open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QJsonObject tempobj;
            QJsonDocument tempdoc;
            tempdoc=QJsonDocument::fromJson(_file->readAll(),&error);
            tempobj=tempdoc.object();

            createtime=tempobj["Create"].toString();
            qDebug()<<createtime;
            _file->close();
        }

    }
    return error.error==QJsonParseError::NoError?true:false;
}

QStringList FileManager::getmaplist()
{
    return _maplist;
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

void FileManager::readSerial(const QString msg)
{

    gpsData=msg;
    qDebug()<<gpsData;
}

void FileManager::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    setmaplist();
}
