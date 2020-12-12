#include "datacheck.h"
bool DataCheck::checkEffect(QString data)
{
    QByteArray msg=data.toLatin1();
    int check=msg[1];
    for (int i = 2; msg[i]!='*'; i++)
    {
        check=check^msg[i];
    }
    int num=data.right(4).toInt(nullptr,16);
    DebugManager::e(QString::number(num)+"=="+QString::number(check));
    return num==check;

}

bool DataCheck::checkFormat(QString data)
{
    QRegExp regular("^(\\$GPHCD)\\S+(\\r\\n)$");
    return regular.indexIn(data)==0;
}
