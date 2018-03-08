#ifndef UPDATEFORBIDDEN_H
#define UPDATEFORBIDDEN_H
#include <QThread>
#include "singleton.h"
#include "mainctrl.h"
#include "servercomm.h"
#include "t_basic_sync_forbidden.h"
#include <QObject>
#include <QTime>
#include <QDebug>

#define  UPDATE_FORBIDDEN_TIME  30*60

class CUpdateForbidden : public QThread
{
    Q_OBJECT
public:
    void init()
    {
        start();
    }
private:
    CUpdateForbidden(){}
    ~CUpdateForbidden(){}

    void run()
    {

        qsrand(time(0));
        qrandTime = qrand()%600;
        while(1)
        {
            sleep(UPDATE_FORBIDDEN_TIME + qrandTime);

            qDebug() << "update forbidden program";

            updateForbidden();

        }

    }

    void updateForbidden()
    {
        //更新黑名单和挂失用名单
        loseUserTime = SBalckList::instance()->getLossUserTime();
        blackUserTime = SBalckList::instance()->getBalckUserTime();
        forbidUserTime = SBalckList::instance()->getForbidUserTime();

        loseBlackUserRet loseBlackRet;
        SCServerComm::instance()->syncIllData(loseUserTime,blackUserTime,forbidUserTime,&loseBlackRet);
        if(loseBlackRet.blackUserResultCode() == RET_TRUE)//黑名单
        {
            SBalckList::instance()->AnalysisInfo(loseBlackRet.blackUserSyncTime(),loseBlackRet.blackUsers(),2);

        }
        if(loseBlackRet.lossUserResultCode() == RET_TRUE)//挂失用户
        {
            SBalckList::instance()->AnalysisInfo(loseBlackRet.lossUserSyncTime(),loseBlackRet.lossUsers(),1);
        }
        if(loseBlackRet.cellResultCode() == RET_TRUE) //禁用格子
        {
            SBalckList::instance()->AnalysisInfo(loseBlackRet.notAvaliCellSyncTime(),loseBlackRet.notAvaliCells(),3);
        }
    }


    int qrandTime;

    QString loseUserTime;
    QString blackUserTime;
    QString forbidUserTime;


    DECLARE_SINGLETON_CLASS(CUpdateForbidden)
};

typedef Singleton<CUpdateForbidden> SCUpdateForbidden;
#endif // UPDATEFORBIDDEN_H
