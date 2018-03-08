#include "homestatus.h"
#include "mainctrl.h"
#include <QFile>
//#include "protcommu.h"
#include "portcomm.h"

CHomeStatus::CHomeStatus(STATUS status):
     CStatus(status)
{
    mPage = new disnetPackage();
    openQueryTimer = new QTimer(this);
    connect(openQueryTimer, SIGNAL(timeout()),mPage, SLOT(readCard_slots()));
}
void CHomeStatus::enter(QByteArray param)
{
    openQueryTimer->start(2000);
    mPage->startRetHomePage(true);
    mPage->show();
}
void CHomeStatus::leave(QByteArray param)
{
    mPage->startRetHomePage(false);
    openQueryTimer->stop();
    //qDebug() << " leave homestatus" << param;
    //mPage->hide();
}
void CHomeStatus::coinInputProc(QByteArray param)
{
}
void CHomeStatus::cardInputProc(QByteArray param)
{
}

/**********************************************************************
* 函数名称： specialcard
* 功能描述： 判断卡号是否是特别卡（可以设置端口号）
* 输入参数：
* 输出参数：
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2013/08/07	 V1.0
***********************************************************************/

bool CHomeStatus::specialcard(QByteArray param)
{
    QStringList list;
    bool ok = false;
    QFile f("IPSetCard.txt");
    if(f.open(QIODevice::ReadOnly))
    {
        QTextStream in(&f);
        while(!in.atEnd())
        {
            list.append(in.readLine());
        }
        for(int i = 0;i < list.size();i++)
        {
            //qDebug() << "special card:" << list.at(i);
            if(list.at(i) == QString(param).toUpper())
            {
                ok = true;
                break;
            }
        }
    }
    else
    {
        //qDebug() << "open error";
        ok = false;
    }

    f.close();
    return ok;
}

