#include "mainctrl.h"

#include "driver.h"
#include "homestatus.h"
#include "savepackagestatus.h"
#include "savepackagewaitstatus.h"
#include "savepackageliststatus.h"
#include "takepackagestatus.h"
#include "serverstatus.h"
#include "tempstoragestatus.h"
#include "tempstoragewaitstatus.h"
#include "registerstatus.h"
#include "rechargestatus.h"
#include "senderstatus.h"
#include "senderwaitstatus.h"
#include "balancestatus.h"
#include "rechagehistorystatus.h"
#include "managestatus.h"
#include "openemptycellstatus.h"
#include "waitconfirmstatus.h"
#include "housestatus.h"
#include "housewaitstatus.h"
#include "cabinetcellstatus.h"
#include "notesetstatus.h"
#include "basicsetstatus.h"
#include "networkconigstatus.h"
#include "checkdevicestatus.h"
#include "rechargedialogstatus.h"

#include "tariffstatus.h"
#include "assiststatus.h"
#include "modifyinfostatus.h"
#include "detainstatus.h"
#include "exptakestatus.h"
#include "devicereststatus.h"
#include "scancomm.h"
#include "t_basic_terminal.h"
#include "heartBeatReport.h"

CMainCtrl::CMainCtrl()
{
    isworking = false;
}

CMainCtrl::~CMainCtrl()
{

    QMap<STATUS, CStatus*>::iterator it;
    for (it = mStProcesser.begin(); it != mStProcesser.end(); ++it)
        delete it.value();

}

void CMainCtrl::init()
{
    mCurStatus = ST_NONE;


    //添加各个状态
    CHomeStatus*  homeStatus = new CHomeStatus;                            //主界面
    CServerStatus *serviceStatus = new CServerStatus;              //配置
    CtempStorageStatus *tempStorageStatus = new CtempStorageStatus;    //临时存储
    CtempStorageWaitStatus *tempStorageWaitStatus = new CtempStorageWaitStatus;//临时存储等待

//    CManageStatus  *managestatus = new CManageStatus;                      //管理员
//    CInfoinputStatus *infoputstatus = new CInfoinputStatus;                //信息录入
    CWaitconfirmStatus *waitconfirmstatus = new CWaitconfirmStatus;        //等待确认界面
//    CNoiclogStatus  *noiclogstatus = new CNoiclogStatus;                   //无卡登录
    CSavepackageStatus * savestatus = new CSavepackageStatus;              //存件界面
    CSavepackagewaitStatus *savewaitstatus = new CSavepackagewaitStatus;   //存件等待页面
    CSavepackagelistStatus *saveliststatus = new CSavepackagelistStatus;   //存件列表页面
    CTakePageStatus *takepagestatus = new CTakePageStatus;                 //取件
    CAssistStatus   *assiststatus = new CAssistStatus;                     //远程协助

    CRechargeStatus *rechargestatus = new CRechargeStatus;                 //充值
    CRechargedialogStatus *rechargedialogStatus = new CRechargedialogStatus;
    CSenderStatus *senderstatus     = new CSenderStatus;                   //寄件
    CSenderWaitStatus *senderwaitstatus     = new CSenderWaitStatus;       //寄件等待
    CBalanceStatus   *balanceStatus = new CBalanceStatus;
    CRechageHistoryStatus *rechageHistoryStatus = new CRechageHistoryStatus;//充值记录查询
    CManageStatus  *manageStatus = new CManageStatus; //维修
    COpenEmptyCellStatus  *openEmptyCellStatus = new COpenEmptyCellStatus;//打开空门

    CHouseStatus *housestatus = new CHouseStatus;
    CHouseWaitStatus *housewaitstatus = new CHouseWaitStatus;
    CabinetCellStatus *cabinetCellStatus = new CabinetCellStatus; //箱格设置
    CNoteSetStatus    *noteSetStatus = new CNoteSetStatus;//短信设置
    CBasicSetStatus   *basicSetStatus = new CBasicSetStatus;//基础信息设置
    CRegisterStatus *registerstatus = new CRegisterStatus;                 //
    CNetworkConigStatus *networkConfigstatus = new CNetworkConigStatus;                 //
    CCheckDeviceStatus *checkDevicestatus = new CCheckDeviceStatus;                 //
    CtariffStatus *tariffStatus = new CtariffStatus;
    CModifyInfoStatus *modifyInfoStatus = new CModifyInfoStatus;
    CDetainStatus *detainstatus = new CDetainStatus;
    CExpTakeStatus *exptakestatus = new CExpTakeStatus;
    CDeviceRestStatus *devicestatus = new CDeviceRestStatus;


//    CSetportstatus *portstatus = new CSetportstatus;                       //设置端口

//    CQueryBalanceStatus *balancestatus = new CQueryBalanceStatus;          //余额查询
//    CUseguideStatus *useguidestatus = new CUseguideStatus;                 //使用流程
//    CIntroductionLXYZStatus *introLxyzstatus = new CIntroductionLXYZStatus;  //易邮简介
//    pricecheckstatus      *priceCheckStatus = new pricecheckstatus;         //资费查询
//    CExceptionStatus *exceptionstatus = new CExceptionStatus;                           //异常处理
//    CExceptionOpenDoorStatus *exceptionopendoorstatus = new CExceptionOpenDoorStatus;   //异常打开箱门
//    CourierTakePackageStatus *courierpackagestatus = new CourierTakePackageStatus;      //快递员取滞留件
//    CDeviceRestStatus *devicereststatus = new CDeviceRestStatus;           //设备维护
//    CFinanceStatus *financestatus = new CFinanceStatus;                    //财务界面
//    CTakeListStatus *takeliststatus = new CTakeListStatus;                 //刷卡取件
//    CVipBindStatus *bindstatus = new CVipBindStatus;                       //自助绑定
//    CCardcheckStatus *cardstatus = new CCardcheckStatus;                   //刷卡显示
//    CExpHelpStatus *exphelpstatus = new CExpHelpStatus;                    //快递员取件



    //......
    addStatus(homeStatus);
    addStatus(serviceStatus);
    addStatus(tempStorageStatus);
    addStatus(tempStorageWaitStatus);
    addStatus(savestatus);
    addStatus(savewaitstatus);
    addStatus(saveliststatus);
    addStatus(takepagestatus);
    addStatus(assiststatus);
    addStatus(registerstatus);
    addStatus(rechargestatus);
    addStatus(rechargedialogStatus);
    addStatus(senderstatus);
    addStatus(senderwaitstatus);
    addStatus(balanceStatus);
    addStatus(rechageHistoryStatus);

    addStatus(manageStatus);
    addStatus(openEmptyCellStatus);
    addStatus(detainstatus);
    addStatus(exptakestatus);

    addStatus(waitconfirmstatus);
    addStatus(housestatus);
    addStatus(housewaitstatus);
    addStatus(cabinetCellStatus);
    addStatus(noteSetStatus);
    addStatus(basicSetStatus);
    addStatus(networkConfigstatus);
    addStatus(checkDevicestatus);

    addStatus(tariffStatus);
    addStatus(modifyInfoStatus);
    addStatus(devicestatus);

   // expiryCoin.insert(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),0);
    //读取背景图片位置，
    enterStatus(ST_HOME, "");

   // connect(this,SIGNAL(updateCell_CMainCtrl()),homeStatus,SLOT(updateCell()));

    //绑定驱动事件
   connect(SCDriver::instance(), SIGNAL(keyInput(QByteArray)), this, SLOT(keyInputProc(QByteArray)));
   connect(SCDriver::instance(), SIGNAL(cardInput(QByteArray)), this, SLOT(cardInputProc(QByteArray)));
   connect(SCDriver::instance(), SIGNAL(coinInput(QByteArray)), this, SLOT(coinInputProc(QByteArray)));
   connect(SCDriver::instance(), SIGNAL(upsInput(QByteArray)), this, SLOT(upsInputProc(QByteArray)));

   connect(SReartBeatReportThrd::instance(),SIGNAL(checkRunStatusSignal(qint8)),this,SLOT(checkRunStatusSlot(qint8)));
}

void CMainCtrl::addStatus(CStatus *status)
{
     mStProcesser.insert(status->status(), status);

}
int CMainCtrl::curStatus()
{
    return mCurStatus;
}

void CMainCtrl::enterStatus(STATUS status, QByteArray param)
{
    qDebug() << "Current Status : " << mCurStatus << "Enter Status :" << status;

    if(SCTerminal::instance()->terminalInfo.runStatus == 3 && mCurStatus == ST_NONE)
    {
        status = ST_DEVICE_SERVICE;
    }
    //释放当前的资源
    if(!isworking)
    {
        isworking = true;
        if (mStProcesser[mCurStatus])
            mStProcesser[mCurStatus]->leave(param);

        //进入新的状态
        if (mStProcesser[status])
            mStProcesser[status]->enter(param);

        mCurStatus = status;
        isworking = false;
    }
}

void CMainCtrl::keyInputProc(QByteArray keyVal)
{

    if (mStProcesser[mCurStatus])
        mStProcesser[mCurStatus]->keyInputProc(keyVal);
}

void CMainCtrl::cardInputProc(QByteArray cardNo)
{
    if (mStProcesser[mCurStatus])
        mStProcesser[mCurStatus]->cardInputProc(cardNo.toHex().toUpper()); //.toHex().toUpper()
}

void CMainCtrl::coinInputProc(QByteArray coinVal)
{
    if (mStProcesser[mCurStatus])
        mStProcesser[mCurStatus]->coinInputProc(coinVal);
}

void CMainCtrl::upsInputProc(QByteArray upsVal)
{
    qDebug() << " ups : " << upsVal.toHex();

    if (mStProcesser[mCurStatus])
        mStProcesser[mCurStatus]->upsInputProc(upsVal.toHex());

}

void CMainCtrl::checkRunStatusSlot(qint8 value)
{
    qDebug() << " checkRunStatusSlot : " << value;

    if(value < 1 || value > 3)
    {
        return ;
    }

    if(SCTerminal::instance()->terminalInfo.runStatus == 3)
    {
        if(ST_DEVICE_SERVICE == mCurStatus)
        {
            enterStatus(ST_HOME, "");
        }
    }
    else if(SCTerminal::instance()->terminalInfo.runStatus == 2)
    {
        if(value == 3)
        {
            if(ST_HOME == mCurStatus)
            {
                enterStatus(ST_DEVICE_SERVICE,"");
            }
        }
    }
    else if(SCTerminal::instance()->terminalInfo.runStatus == 1)
    {
        if(value == 3)
        {
            if(ST_HOME == mCurStatus)
            {
                enterStatus(ST_DEVICE_SERVICE,"");
            }
        }
    }
    else
    {
        return;
    }

    SCTerminal::instance()->terminalInfo.runStatus = value;
    SCTerminal::instance()->setTerminalInfo();
}

