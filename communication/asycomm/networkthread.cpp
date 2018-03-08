#include "networkthread.h"
#include "httpparam.h"
#include "httpsync.h"
#include "parser.h"
#include <QApplication>
#include "t_basic_delivery.h"
#include "t_basic_lan_jian.h"
#include "t_basic_temp_storage.h"
#include <QUuid>
#include <QRegExp>
#include <QDebug>

using namespace QJson;

CNetworkThread::CNetworkThread(QObject *parent) :
    QObject(parent)
{
}

void CNetworkThread::init()
{

}

void CNetworkThread::sendNetWorkEvent(NetworkDesc_t networkInfo)
{
    CNetworkEvent* netEvt = new CNetworkEvent(NET_COMM_TYPE, networkInfo);
    QApplication::postEvent(this,netEvt);
}

bool CNetworkThread::sendNetworkProc(NetworkDesc_t networkInfo)
{
    bool ret = false;
    HttpSync client;
    Parser   parser;

    QString     strRespone = client.post(networkInfo.mUrl,networkInfo.mContent);

    qDebug() << "sendNetworkProc:" << strRespone;
    QVariantMap retMap = parser.parse(strRespone.toUtf8()).toMap();
    QVariant retValue = retMap.value("resultCode");
    QString retStr = retValue.toString();
    if(retStr.isEmpty() || retStr.contains("500") || retStr.contains("10000"))
    {
        ret = false;
    }
    else
    {
        ret = true;
    }

    return ret;
}

void CNetworkThread::customEvent(QEvent *event)
{
    if(event->type() == NET_COMM_TYPE)
    {
        QDateTime time = QDateTime::currentDateTime();
        CNetworkEvent *myEvent = static_cast<CNetworkEvent *>(event);

        if(sendNetworkProc(myEvent->mNetInfo))
        {
            myEvent->mNetInfo.mStatus = 1;
            if(1 == myEvent->mNetInfo.mSmsType)
            {
                SCDelivery::instance()->updateDeliveryNoteSendTimeInfo(myEvent->mNetInfo.mRelPriKey,time);
            }
            else if(12 == myEvent->mNetInfo.mSmsType || 32 == myEvent->mNetInfo.mSmsType) //寄件
            {
                SCSender::instance()->updateSenderNoteSendTimeInfo(myEvent->mNetInfo.mRelPriKey,time);
            }
            else if(21 == myEvent->mNetInfo.mSmsType) //临时存储
            {
                SBasicTempStorage::instance()->updateStorageNoteSendTimeInfo(myEvent->mNetInfo.mRelPriKey,time);
            }
        }
        else
        {
            myEvent->mNetInfo.mStatus = 2;
        }



        SCNetwork::instance()->insertNetworkInfo(myEvent->mNetInfo);
    }
}


