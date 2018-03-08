#ifndef T_BALCKLIST_H
#define T_BALCKLIST_H

#include "singleton.h"
#include <QDateTime>
#include <QStringList>

//1会员卡挂失禁用、 2收件人黑名单、 3不可用格子

typedef struct{
    int          syncType;
    QString      syncValue;
    QString      lastSyncTime ;
}syncIllData;
class t_balckList
{
public:
    t_balckList();
    void init();
    void clearInfo(int type);

    void AnalysisInfo(QString timeStr,QString userInfo,int type); //type 1、挂失 2、黑名单 3、箱门不可用

    QString getBalckUserTime();
    QString getLossUserTime();
    QString getForbidUserTime();

    QStringList    lossUserInfo;
    QStringList    balckUserInfo;
    QStringList    forbidUserInfo;

private:
    void getBalckUserInfo();
    void getLossUserInfo();
    void getForbidUserInfo();

    void insertBalckUserInfo(QList<syncIllData> deliveryInfo);
    void insertLossUserInfo(QList<syncIllData> deliveryInfo);
    void insertForbidUserInfo(QList<syncIllData> deliveryInfo);

private:
    QString    lossUserTimeStr;
    QString    balckUserTimeStr;
    QString    forbidUserTimeStr;

public:
    DECLARE_SINGLETON_CLASS(t_balckList)
};

typedef Singleton<t_balckList> SBalckList;

#endif // T_BALCKLIST_H
