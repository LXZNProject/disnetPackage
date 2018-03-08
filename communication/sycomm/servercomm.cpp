#include <QDebug>
#include "httpparam.h"
#include "servercomm.h"
#include "qobjecthelper.h"
#include "parser.h"
#include "t_basic_terminal.h"
#include "t_log_network.h"
#include "t_image_time.h"
#include "t_basic_cell_order.h"
#include "shaEncy.h"
#include "httpupdate.h"
#include "fileencode.h"
#include <QUuid>
#include <QTextCodec>


using namespace QJson;

CServerComm::CServerComm()
{
    mHttpClient = new HttpSync;
}

CServerComm::~CServerComm()
{
    delete mHttpClient;
}

void CServerComm::init()
{
     tc =  QTextCodec::codecForName("UTF-8");
    //mUrl = "http://" + SCTerminal::instance()->terminalInfo.serverIp +":" + SCTerminal::instance()->terminalInfo.serverPort;
    //mUrl = "http://192.168.1.76:8092";
   // QString md5Id = SCTerminal::instance()->terminalInfo.encryptCode;
    //md5Code = md5Id;//QString(SHA1(md5Id.toStdString()).c_str());
}

void CServerComm::checkVersion(checkVersionRet *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap  version;
    version.insert("appType","1");
    version.insert("numVersion",SCTerminal::instance()->mVersion_Num);

    paramMap.insert("checkVersionReq",version);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "checkVersion send:" << sparam;

    strRespone = mHttpClient->post(SCTerminal::instance()->mUrl + "/lxyz02/soapManager_checkVersion.do?",  sparam);

    strRespone = QString::fromUtf8(strRespone.toUtf8());
    qDebug() << "info -- checkVersion, recv : " << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

}

void CServerComm::syncTerminalData(syncTerminalDataRet *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();
    paramMap.insert("terminalId", SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);
//    paramMap.insert("terminalId", "010025000001");
//    paramMap.insert("companyId","00000001");
//    paramMap.insert("encryptCode","1");


    QVariantMap time;
    time.insert("syncModifyTime",SCTerminal::instance()->terminalInfo.syncModifyTime.toString("yyyy-MM-dd hh:mm:ss"));
    //time.insert("syncModifyTime","2015-1-1 00:00:00");
    paramMap.insert("syncTerminalDataReq",time);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "syncTerminalData send:" << sparam;

    strRespone = mHttpClient->post(SCTerminal::instance()->mUrl + "/lxyz02/soapManager_syncTerminalData.do?", sparam);

    strRespone = QString::fromUtf8(strRespone.toUtf8());
    qDebug() << "syncTerminalData recv: " << strRespone;

    //QVariantMap retMap = parser.parse(strRespone.toAscii()).toMap();
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

}

void CServerComm::syncRemoteCommand(syncRemoteCommandRet *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);



    QString sparam = serializer.serialize(paramMap);

    qDebug() << "syncRemoteCommand send:" << sparam;

    strRespone = mHttpClient->post(SCTerminal::instance()->mUrl + "/lxyz02/soapManager_syncRemoteCommand.do?",  sparam);

    strRespone = QString::fromUtf8(strRespone.toUtf8());
    qDebug() << "syncRemoteCommand, recv : " << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::syncOrderCell(syncOrderCellRet *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap  orderInfo;
    orderInfo.insert("orderCellSyncTime",STCellOrder::instance()->orderCellTime);

    paramMap.insert("syncOrderCellReq",orderInfo);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "syncOrderCell send:" << sparam;

    strRespone = mHttpClient->post(SCTerminal::instance()->mUrl + "/lxyz02/soapManager_syncOrderCell.do?",  sparam);

    strRespone = QString::fromUtf8(strRespone.toUtf8());
    qDebug() << "syncOrderCell recv : " << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::checkHomePage(checkHomePageRet *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap  version;
    version.insert("appType","1");
    version.insert("modifyTime",SCImageTime::instance()->mHomeTime);

    paramMap.insert("checkHomePageReq",version);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "checkHomePage send:" << sparam;

    strRespone = mHttpClient->post(SCTerminal::instance()->mUrl + "/lxyz02/soapManager_checkHomePage.do?",  sparam);

    strRespone = QString::fromUtf8(strRespone.toUtf8());
    qDebug() << "info -- checkHomePage, recv : " << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

NetworkDesc_t CServerComm::remoteCommandCallBack(QList<CmdDesc_t> infos)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    QVariantList list;

    paramMap.insert("terminalId", SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap retCmdInfo;
    for (int i = 0; i < infos.size(); i++)
    {
        QVariantMap  cmdInfo;
        cmdInfo.insert("id", infos[i].mId);
        cmdInfo.insert("commandType", infos[i].mCmdType);
        cmdInfo.insert("cellId",infos[i].mCellId);
        cmdInfo.insert("commandContent", infos[i].mContent);
        cmdInfo.insert("commandResult",infos[i].mCmdStatus);
        cmdInfo.insert("remark",infos[i].mCmdRet);
        list << cmdInfo;
    }

    retCmdInfo.insert("resultList", list);

    paramMap.insert("commandCallBackReq",retCmdInfo);

    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_remoteCommandCallBack.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "remoteCommandCallBack send : " << netWorkInfo.mContent;

    return netWorkInfo;


}

NetworkDesc_t CServerComm::saveDelivery(QList<DelivertDesc_t> infos, QStringList cardInfo)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    QVariantList list;

    paramMap.insert("terminalId", SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap basicInfo;
    basicInfo.insert("terminalSmsAddr",SCTerminal::instance()->terminalInfo.terminalName);
    basicInfo.insert("expMobilePhone",cardInfo.at(3));
    basicInfo.insert("expCompanyName",cardInfo.at(4));
    basicInfo.insert("expM1CardId",cardInfo.at(6));
    basicInfo.insert("expUuid",cardInfo.at(7));

    for (int i = 0; i < infos.size(); i++)
    {
        QVariantMap  packeInfo;
        packeInfo.insert("localPriKey", infos[i].deliveryId);
        packeInfo.insert("cellId", infos[i].cellId);
        packeInfo.insert("cellType",QString::number(infos[i].cellType));
        packeInfo.insert("cabinetId", infos[i].cellId.left(2) );
        packeInfo.insert("mobilePhone",infos[i].mobilephone);
        packeInfo.insert("packageId", infos[i].packageId);
        packeInfo.insert("status",QString::number(infos[i].status));
        packeInfo.insert("getCode", infos[i].getCode);
        packeInfo.insert("hasPayMoney", QString::number(infos[i].hasPayMoney));
        packeInfo.insert("expressPay",QString::number(infos[i].expressPay));
        packeInfo.insert("ownerBasePay",QString::number(infos[i].ownerBasePay));
        packeInfo.insert("tariffType", QString::number(infos[i].tariffType));
        packeInfo.insert("expireStatus",QString::number(infos[i].expireStatus));
        packeInfo.insert("expireHours",QString::number(infos[i].expireHours));
        packeInfo.insert("expireStartTariff",QString::number(infos[i].expireStartTariff));
        packeInfo.insert("expireStep",QString::number(infos[i].expireStep));
        packeInfo.insert("expireStepMoney",QString::number(infos[i].expireStepMoney));
        packeInfo.insert("goodsCheckStatus",QString::number(infos[i].goodsCheckStatus));
        list << packeInfo;
    }
    basicInfo.insert("deliveryArr", list);

    paramMap.insert("saveDeliveryReq",basicInfo);

    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_saveDelivery.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "saveDelivery send : " << netWorkInfo.mContent;

    return netWorkInfo;
}

NetworkDesc_t CServerComm::confirmDelivery(DelivertDesc_t info,qint8 status)
{
    QVariantMap paramMap;
    HttpParam   serializer;

    paramMap.insert("terminalId", SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap confirmInfo;
    confirmInfo.insert("localPriKey",info.deliveryId);
    confirmInfo.insert("status",QString::number(status));

    paramMap.insert("confirmDeliveryReq",confirmInfo);
    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_confirmDelivery.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "confirmDelivery send : " << netWorkInfo.mContent;

    return netWorkInfo;
}

NetworkDesc_t CServerComm::takeDelivery(DelivertDesc_t info, qint8 status, QStringList cardInfo)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    qint8       type;
    QString     cardUuid;

    paramMap.insert("terminalId", SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap packInfo;
    packInfo.insert("localPriKey",info.deliveryId);
    packInfo.insert("hasPayMoney",QString::number(info.hasPayMoney));
    packInfo.insert("expressPay",QString::number(info.expressPay));
    packInfo.insert("ownerBasePay",QString::number(info.ownerBasePay));
    packInfo.insert("payway",QString::number(info.payway));
    packInfo.insert("debtPay",QString::number(info.debtPay));
    packInfo.insert("cashMoney",QString::number(info.cashMoney));
    packInfo.insert("takeWay",QString::number(status));

    if(info.messageSentTime.secsTo(info.takeTime) >= info.expireHours*60*60)
    {
        type = 2;
    }
    else
    {
        type = 1;
    }
    packInfo.insert("debtTakeOut",QString::number(type));
    packInfo.insert("yearCardTake","1");

    if(!cardInfo.isEmpty())
    {
        cardUuid = cardInfo.at(7);
    }
    packInfo.insert("payerUuid",cardUuid);

    paramMap.insert("takeDeliveryReq",packInfo);
    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_takeDelivery.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "takeDelivery send : " << netWorkInfo.mContent;

    return netWorkInfo;

}

NetworkDesc_t CServerComm::saveSendDelivery(QList<SenderDesc_t> infos, qint8 type)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    QVariantList list;

    paramMap.insert("terminalId", SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap basicInfo;
    basicInfo.insert("terminalSmsAddr",SCTerminal::instance()->terminalInfo.terminalName);
    basicInfo.insert("lanJianName",infos[0].lanJianName);
    basicInfo.insert("lanJianPhone",infos[0].lanJianPhone);


    for (int i = 0; i < infos.size(); i++)
    {
        QVariantMap  packeInfo;
        packeInfo.insert("localPriKey", infos[i].lanJianId);
        packeInfo.insert("cellId", infos[i].cellId);
        packeInfo.insert("cellType",QString::number(infos[i].cellType));
        packeInfo.insert("cabinetId", infos[i].cellId.left(2) );
        packeInfo.insert("deliveryType",QString::number(type));
        packeInfo.insert("sendMobilePhone", infos[i].mobilephone);
        packeInfo.insert("sendSelCompany",infos[i].companyName);
        packeInfo.insert("getCode", infos[i].getCode);
        packeInfo.insert("expressPay",QString::number(infos[i].expressPay));
        packeInfo.insert("ownerBasePay",QString::number(infos[i].ownerBasePay));
        packeInfo.insert("goodsCheckStatus",QString::number(infos[i].goodsCheckStatus));
        list << packeInfo;
    }
    basicInfo.insert("deliveryArr", list);

    paramMap.insert("saveSendDeliveryReq",basicInfo);

    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_saveSendDelivery.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "saveSendDelivery send : " << netWorkInfo.mContent;

    return netWorkInfo;
}

NetworkDesc_t CServerComm::takeSendDelivery(SenderDesc_t info, QStringList cardInfo)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    QString     cardUuid;

    paramMap.insert("terminalId", SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap packInfo;
    packInfo.insert("localPriKey",info.lanJianId);
    packInfo.insert("hasPayMoney",QString::number(info.hasPayMoney));
    packInfo.insert("expressPay",QString::number(info.expressPay));
    packInfo.insert("ownerBasePay",QString::number(info.ownerBasePay));
    packInfo.insert("payway",QString::number(info.payway));
    packInfo.insert("cashMoney",QString::number(info.cashMoney));

    if(!cardInfo.isEmpty())
    {
        cardUuid = cardInfo.at(7);
    }
    packInfo.insert("payerUuid",cardUuid);

    paramMap.insert("takeSendDeliveryReq",packInfo);
    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_takeSendDelivery.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "takeSendDelivery send : " << netWorkInfo.mContent;

    return netWorkInfo;
}

NetworkDesc_t CServerComm::saveTempDelivery(QList<BasicTempStorage> infos)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    QVariantList list;

    paramMap.insert("terminalId", SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap basicInfo;
    basicInfo.insert("terminalSmsAddr",SCTerminal::instance()->terminalInfo.terminalName);

    for (int i = 0; i < infos.size(); i++)
    {
        QVariantMap  packeInfo;
        packeInfo.insert("localPriKey", infos[i].storageId);
        packeInfo.insert("cellId", infos[i].cellId);
        packeInfo.insert("cellType",QString::number(infos[i].cellType));
        packeInfo.insert("cabinetId", infos[i].cellId.left(2) );
        packeInfo.insert("sendMobilePhone", infos[i].mobilephone);
        packeInfo.insert("getCode", infos[i].getCode);
        packeInfo.insert("hasPayMoney", QString::number(infos[i].hasPayMoney));
        packeInfo.insert("goodsCheckStatus",QString::number(infos[i].goodsCheckStatus));
        list << packeInfo;
    }
    basicInfo.insert("deliveryArr", list);

    paramMap.insert("saveTempDeliveryReq",basicInfo);

    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_saveTempDelivery.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "saveTempDelivery send : " << netWorkInfo.mContent;

    return netWorkInfo;
}

NetworkDesc_t CServerComm::takeTempDelivery(BasicTempStorage info, QStringList cardInfo)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    QString     cardUuid;

    paramMap.insert("terminalId", SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap packInfo;
    packInfo.insert("localPriKey",info.storageId);
    packInfo.insert("hasPayMoney",QString::number(info.hasPayMoney));
    packInfo.insert("debtPay",QString::number(info.debtPay));
    packInfo.insert("payway",QString::number(info.payway));
    packInfo.insert("cashMoney",QString::number(info.cashMoney));

    if(!cardInfo.isEmpty())
    {
        cardUuid = cardInfo.at(7);
    }
    packInfo.insert("payerUuid",cardUuid);

    paramMap.insert("takeTempDeliveryReq",packInfo);
    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_takeTempDelivery.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "takeTempDelivery send : " << netWorkInfo.mContent;

    return netWorkInfo;
}

NetworkDesc_t CServerComm::reportCellError(QList<NetCellExcept> exceptList)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    QVariantList list;

    paramMap.insert("terminalId", SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap basicInfo;
    for (int i = 0; i < exceptList.size(); i++)
    {
        QVariantMap  exceptInfo;
        exceptInfo.insert("cellId", exceptList[i].cellId);
        exceptInfo.insert("cabinetId", exceptList[i].cellId.left(2));
        exceptInfo.insert("flowType",QString::number(exceptList[i].flowType));
        exceptInfo.insert("errorType",QString::number(exceptList[i].errorType));
        exceptInfo.insert("delLocalPriKey",exceptList[i].Relkey);

        list << exceptInfo;
    }

    basicInfo.insert("errorArr", list);

    paramMap.insert("reportCellErrorReq",basicInfo);

    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_reportCellError.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "reportCellError send : " << netWorkInfo.mContent;

    return netWorkInfo;

}

NetworkDesc_t CServerComm::repairCellError(QString cellId, qint8 type)
{
    QVariantMap paramMap;
    HttpParam   serializer;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap basicInfo;
    basicInfo.insert("cellId",cellId);
    basicInfo.insert("errorType",QString::number(type));

    paramMap.insert("repairCellErrorReq",basicInfo);

    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_repairCellError.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "repairCellError send : " << netWorkInfo.mContent;

    return netWorkInfo;
}

NetworkDesc_t CServerComm::expTakeDelivery(QMap<QString, QString> sendInfo)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    QVariantList list;
    qint8 type;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap basicInfo;
    for(QMap<QString,QString>::iterator it = sendInfo.begin(); it != sendInfo.end(); it++)
    {
        QVariantMap  expTakeInfo;
        expTakeInfo.insert("uuid",it.key());

        if(QDateTime::fromString(it.value(),Qt::ISODate).secsTo(QDateTime::currentDateTime()) >= SCTerminal::instance()->terminalInfo.expireHours*60*60)
        {
            type = 2;
        }
        else
        {
            type = 1;
        }
        expTakeInfo.insert("debtTakeOut",QString::number(type));

        list << expTakeInfo;
    }
    basicInfo.insert("delUuidList", list);

    paramMap.insert("expTakeDeliveryReq",basicInfo);

    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_expTakeDelivery.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "expTakeDelivery send : " << netWorkInfo.mContent;

    return netWorkInfo;

}

NetworkDesc_t CServerComm::reportDebtRecord(TempRecordDesc_t tempRecord, QStringList info)
{
    QVariantMap paramMap;
    HttpParam   serializer;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap basicInfo;
    basicInfo.insert("m1CardId", tempRecord.m1Card);
    basicInfo.insert("mobilePhone",info.at(3));
    basicInfo.insert("userUuid",tempRecord.usrUuid);
    basicInfo.insert("debtCoin",QString::number(tempRecord.curCash));
    basicInfo.insert("localPrimary",tempRecord.cmmUuid);

    paramMap.insert("reportDebtRecordReq",basicInfo);

    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_reportDebtRecord.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "reportDebtRecord send : " << netWorkInfo.mContent;

    return netWorkInfo;

}

NetworkDesc_t CServerComm::repairDebtRecord(QString cmmUuid)
{
    QVariantMap paramMap;
    HttpParam   serializer;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap basicInfo;
    basicInfo.insert("localPrimary", cmmUuid);

    paramMap.insert("repairDebtRecordReq",basicInfo);

    NetworkDesc_t netWorkInfo;
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_repairDebtRecord.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSendTime = QDateTime::currentDateTime();
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "repairDebtRecord send : " << netWorkInfo.mContent;

    return netWorkInfo;
}

void CServerComm::queryStoreCard(QString cardNumStr,queryStoreCardRet *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap  cardNum;
    cardNum.insert("password",cardNumStr);

    paramMap.insert("queryStoreCardReq",cardNum);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "queryStoreCard send:" << sparam;

    strRespone = mHttpClient->post(SCTerminal::instance()->mUrl + "/lxyz02/soapManager_queryStoreCard.do?",  sparam);

    qDebug() << "info -- queryStoreCard, recv : " << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::passwordRechargeSynchronous(QStringList info, passwordRechargeRet *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap  cardNum;
    cardNum.insert("cardNo",info.at(0));
    cardNum.insert("cardPasswd",info.at(1));
    cardNum.insert("relUuid",info.at(2));
    cardNum.insert("relM1Card",info.at(3));
    cardNum.insert("relPhone",info.at(4));
    cardNum.insert("cardType",info.at(5));
    cardNum.insert("cardValue",info.at(6));
    cardNum.insert("balance",info.at(7));
    cardNum.insert("vvipStartTime",info.at(8));
    cardNum.insert("vvipEndTime",info.at(9));
    cardNum.insert("localPriKey",info.at(10));

    paramMap.insert("passwordRechargeReq",cardNum);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "passwordRechargeSynchronous send:" << sparam;

    strRespone = mHttpClient->post(SCTerminal::instance()->mUrl + "/lxyz02/soapManager_passwordRecharge.do?",  sparam);

    qDebug() << "info -- passwordRechargeSynchronous, recv : " << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}
void CServerComm::passwordRechargeAsynchronous(NetworkDesc_t &netWorkInfo,QStringList info)
{
    QVariantMap paramMap;
    HttpParam   serializer;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap  cardNum;
    cardNum.insert("cardNo",info.at(0));
    cardNum.insert("cardPasswd",info.at(1));
    cardNum.insert("relUuid",info.at(2));
    cardNum.insert("relM1Card",info.at(3));
    cardNum.insert("relPhone",info.at(4));
    cardNum.insert("cardType",info.at(5));
    cardNum.insert("cardValue",info.at(6));
    cardNum.insert("balance",info.at(7));
    cardNum.insert("vvipStartTime",info.at(8));
    cardNum.insert("vvipEndTime",info.at(9));
    cardNum.insert("localPriKey",info.at(10));

    paramMap.insert("passwordRechargeReq",cardNum);

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_passwordRecharge.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "passwordRechargeAsynchronous send : " << netWorkInfo.mContent;
}


void CServerComm::updateUserInfo(NetworkDesc_t &netWorkInfo, QStringList info)
{
    QVariantMap paramMap;
    HttpParam   serializer;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap  myinfo;
    myinfo.insert("uuid",info.at(0));
    myinfo.insert("mobilePhone",info.at(1));
    myinfo.insert("expCompanyName",info.at(2));


    paramMap.insert("updateUserInfoReq",myinfo);

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    netWorkInfo.mNetworkId = strUuId;
    netWorkInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_updateUserInfo.do?";
    netWorkInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = netWorkInfo.mContent.toUtf8();
    netWorkInfo.mContent = QString(contentByte);
    netWorkInfo.mSmsType = 0;
    netWorkInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(netWorkInfo);

    qDebug() << "updateUserInfo send : " << netWorkInfo.mContent;
}

void CServerComm::syncIllData(QString loseUerTime, QString balckUerTime,QString forbidUerTime, loseBlackUserRet *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap  syncIllDataReq;
    syncIllDataReq.insert("lossUserSyncTime",loseUerTime);
    syncIllDataReq.insert("blackUserSyncTime",balckUerTime);
    syncIllDataReq.insert("notAvaliCellSyncTime",forbidUerTime);

    paramMap.insert("syncIllDataReq",syncIllDataReq);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "syncIllData send:" << sparam;

    strRespone = mHttpClient->post(SCTerminal::instance()->mUrl + "/lxyz02/soapManager_syncIllData.do?",  sparam);

    qDebug() << "info -- syncIllData, recv : " << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::localRecharge(NetworkDesc_t &noteInfo, QStringList info)
{
    QVariantMap paramMap;
    HttpParam   serializer;

    if(noteInfo.mNetworkId.isEmpty())
    {
        //保存发送给后台的信息
        QUuid uuId = QUuid::createUuid();
        QString strUuId = uuId.toString();
        strUuId = strUuId.remove(QRegExp("[{}-]"));
        strUuId = strUuId.toUpper();

        noteInfo.mNetworkId = strUuId;
    }

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap  user;
    user.insert("relPhone",info.at(0));
    user.insert("relM1Card",info.at(1));
    user.insert("relUuid",info.at(2));
    user.insert("optUuid",info.at(3));
    user.insert("cardType",info.at(4));
    user.insert("cardValue",info.at(5));
    user.insert("balance",info.at(6));
    user.insert("vvipStartTime",info.at(7));
    user.insert("vvipEndTime",info.at(8));
    user.insert("localPriKey",info.at(9));

    paramMap.insert("localRechargeReq",user);

    noteInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_localRecharge.do?";

    noteInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = noteInfo.mContent.toUtf8();
    noteInfo.mContent = QString(contentByte);
    noteInfo.mSendTime = QDateTime::currentDateTime();
    noteInfo.mSmsType = 0;
    noteInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(noteInfo);

    qDebug() << "info -- localRecharge, send : " << noteInfo.mContent;

}

void CServerComm::userRegist(NetworkDesc_t &noteInfo,QStringList info)
{
    QVariantMap paramMap;
    HttpParam   serializer;

    if(noteInfo.mNetworkId.isEmpty())
    {
        //保存发送给后台的信息
        QUuid uuId = QUuid::createUuid();
        QString strUuId = uuId.toString();
        strUuId = strUuId.remove(QRegExp("[{}-]"));
        strUuId = strUuId.toUpper();

        noteInfo.mNetworkId = strUuId;
    }


    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap  user;
    user.insert("userType",info.at(0));
    user.insert("mobilePhone",info.at(1));
    user.insert("expCompanyName",info.at(2));
    user.insert("m1CardId",info.at(3));
    user.insert("uuid",info.at(4));
    user.insert("optUuid",info.at(5));
    user.insert("oldUuid",info.at(6));


    paramMap.insert("userRegistReq",user);


    noteInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_userRegist.do?";

    noteInfo.mContent = serializer.serialize(paramMap);
    QByteArray contentByte = noteInfo.mContent.toUtf8();
    noteInfo.mContent = QString(contentByte);
    noteInfo.mSendTime = QDateTime::currentDateTime();
    noteInfo.mSmsType = 0;
    noteInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(noteInfo);

    qDebug() << "info -- userRegist, send : " << noteInfo.mContent;
}

void CServerComm::heartBeatReport(QString  tempIP,heartBeatReportRet *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap  version;
    version.insert("ipAddr",tempIP);
    version.insert("numVersion",SCTerminal::instance()->mVersion_Num);
    version.insert("displayVersion",SCTerminal::instance()->mVersion_Name);

    paramMap.insert("heartBeatReportReq",version);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "heartBeatReport send:" << sparam;

    strRespone = mHttpClient->post(SCTerminal::instance()->mUrl + "/lxyz02/soapManager_heartBeatReport.do?",  sparam);

    strRespone = QString::fromUtf8(strRespone.toUtf8());
    qDebug() << "info -- heartBeatReport recv : " << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}


void CServerComm::uploadDebugFiles(QString fileName,heartBeatReportRet *respone)
{
    Parser   parser;
    QString  strRespone;
    FileHttp fileHttp;

    QString url = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_uploadDebugFiles.do";

    strRespone = fileHttp.postFile(url,fileName);

    strRespone = QString::fromUtf8(strRespone.toUtf8());
    qDebug() << "uploadDebugFiles recv : " << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

}

void CServerComm::uploadDbFiles(QString dbName, heartBeatReportRet *respone)
{
    Parser      parser;
    QString  strRespone;
    FileHttp fileHttp;

    QString url = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_uploadDbFiles.do";

    strRespone = fileHttp.postFile(url,dbName);

    strRespone = QString::fromUtf8(strRespone.toUtf8());
    qDebug() << "uploadDbFiles recv : " << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

bool CServerComm::download(QString url, QString fileMd5, qint8 status)
{
    bool ret = false;
    httpUpdate https;

    qDebug() << "download send fileMd5:" << fileMd5;

    ret = https.getAppInfo(url,fileMd5,status);

    qDebug() << "download recv:" << ret;

    return ret;
}

void CServerComm::sendSms(NetworkDesc_t &noteInfo)
{
    QVariantMap paramMap;
    HttpParam   serializer;

    if(noteInfo.mNetworkId.isEmpty())
    {
        //保存发送给后台的信息
        QUuid uuId = QUuid::createUuid();
        QString strUuId = uuId.toString();
        strUuId = strUuId.remove(QRegExp("[{}-]"));
        strUuId = strUuId.toUpper();

        noteInfo.mNetworkId = strUuId;
    }


    paramMap.insert("terminalId",SCTerminal::instance()->terminalInfo.terminalId);
    paramMap.insert("companyId",SCTerminal::instance()->terminalInfo.companyId);
    paramMap.insert("encryptCode",SCTerminal::instance()->terminalInfo.encryptCode);

    QVariantMap  note;
    note.insert("localPriKey",noteInfo.mNetworkId);
    note.insert("mobilePhone",noteInfo.mPhone);
    note.insert("messContent",noteInfo.mContent);
    note.insert("messageType",QString::number(noteInfo.mSmsType));
    note.insert("delLocalPriKey",noteInfo.mRelPriKey);

    paramMap.insert("sendSmsReq",note);

    noteInfo.mUrl = SCTerminal::instance()->mUrl + "/lxyz02/soapManager_sendSms.do?";
    noteInfo.mContent = serializer.serialize(paramMap);

    QByteArray contentByte = noteInfo.mContent.toUtf8();
    noteInfo.mContent = QString(contentByte);
    noteInfo.mSendTime = QDateTime::currentDateTime();
    noteInfo.mStatus = 3;    //待发送

    SCNetwork::instance()->insertNetworkInfo(noteInfo);

    qDebug() << "sendSms send:" << noteInfo.mContent;
}

