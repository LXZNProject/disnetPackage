#ifndef SERVERCOMM_H
#define SERVERCOMM_H

#include <QString>
#include <QStringList>
#include "singleton.h"
#include "httpsync.h"
#include "respone.h"
#include "t_log_network.h"
#include "t_basic_delivery.h"
#include "heartBeatReportRet.h"
#include "queryStoreCardRet.h"
#include "passwordRechargeRet.h"
#include "syncRemoteCommandRet.h"
#include "t_basic_lan_jian.h"
#include "t_basic_temp_storage.h"
#include "t_temp_consume_record.h"
#include "loseBlackUserRet.h"
struct packInfo{
    QString packageId;
    QString mobilePhone;
    QString cellType;     // 1:表示大号箱  2：表示中号箱  3：表示小号箱
    QString cellId;
    QString isConfirm;
    QString isCheck;
};

/*
struct cellErrorInfo{
    QString scabId;
    QString scellId;
    QString errType;         // 1 打不开 2 未关闭
};


struct stockShowInfo{
    QString expressId;
    QString mobilePhone;
    QString boxId;
};
*/

typedef struct{
    QString mId;
    QString mCellId;
    QString mCmdType;
    QString mContent;
    QString mCmdStatus;
    QString mCmdRet;
}CmdDesc_t;

class CServerComm
{
public:
    void init();

    void checkVersion(checkVersionRet *respone);     //
    void syncTerminalData(syncTerminalDataRet *respone);
    void syncRemoteCommand(syncRemoteCommandRet *respone);
    void syncOrderCell(syncOrderCellRet *respone);
    void checkHomePage(checkHomePageRet *respone);

    NetworkDesc_t remoteCommandCallBack(QList<CmdDesc_t> infos);
    NetworkDesc_t saveDelivery(QList<DelivertDesc_t> infos,QStringList cardInfo);
    NetworkDesc_t confirmDelivery(DelivertDesc_t info,qint8 status);
    NetworkDesc_t takeDelivery(DelivertDesc_t info,qint8 status,QStringList cardInfo);

    NetworkDesc_t saveSendDelivery(QList<SenderDesc_t> infos,qint8 type);
    NetworkDesc_t takeSendDelivery(SenderDesc_t info,QStringList cardInfo);
    NetworkDesc_t saveTempDelivery(QList<BasicTempStorage> infos);
    NetworkDesc_t takeTempDelivery(BasicTempStorage info,QStringList cardInfo);


    NetworkDesc_t reportCellError(QList<NetCellExcept> exceptList);
    NetworkDesc_t repairCellError(QString cellId,qint8 type);
    NetworkDesc_t expTakeDelivery(QMap<QString,QString> sendInfo);

    NetworkDesc_t reportDebtRecord(TempRecordDesc_t tempRecord,QStringList info);
    NetworkDesc_t repairDebtRecord(QString cmmUuid);

    void queryStoreCard(QString cardNumStr,queryStoreCardRet *respone);//查询充值卡卡号
    void passwordRechargeSynchronous(QStringList info,passwordRechargeRet *respone);//密码卡用户确认充值同步接口
    void passwordRechargeAsynchronous(NetworkDesc_t &netWorkInfo,QStringList info);//密码卡用户确认充值异步接口

    void updateUserInfo(NetworkDesc_t &netWorkInfo,QStringList info);//修改用户据信息异步接口

    void syncIllData(QString loseUerTime,QString balckUerTime,QString forbidUerTime,loseBlackUserRet *respone);//挂失和黑名单用户

    void localRecharge(NetworkDesc_t &noteInfo,QStringList info);//本地充值
    void userRegist(NetworkDesc_t &noteInfo,QStringList info);//用户注册
    void heartBeatReport(QString  tempIP,heartBeatReportRet *respone);//心跳接口


    void uploadDebugFiles(QString fileName,heartBeatReportRet *respone);
    void uploadDbFiles(QString dbName,heartBeatReportRet *respone);
    bool download(QString url,QString filemd5,qint8 status); //status 1 :Qt.zip 系统 2:homepage.png 首页图片
    void sendSms(NetworkDesc_t &noteInfo);


    CServerComm();
    ~CServerComm();

    HttpSync* mHttpClient;
   // QString   mUrl;
   // QString   md5Code;
    QTextCodec *tc;

    DECLARE_SINGLETON_CLASS(CServerComm)
};


typedef Singleton<CServerComm> SCServerComm;


#endif // SERVERCOMM_H
