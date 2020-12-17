#include "datacheck.h"
bool DataCheck::checkEffect(QString data)
{
    int check=calculate(data);
    int num=data.right(4).toInt(nullptr,16);
    DebugManager::e(QString::number(num)+"=="+QString::number(check));
    return num==check;
}

bool DataCheck::checkFormat(QString data)
{
    QRegExp regular("^(\\$GPHCD)\\S+(\\r\\n)$");
    return regular.indexIn(data)==0;
}

int DataCheck::calculate(QString msg)
{
    QByteArray processData=msg.toLatin1();
    int check=processData[1];
    for (int i = 2; processData[i]!='*'; i++)
    {
        check=check^processData[i];
    }
    return check;
}
QByteArray DataCheck::Hex2Str(QString &str)
{
    QByteArray senddata;
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return  senddata;
}
char DataCheck::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}
