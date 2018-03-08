#ifndef T_BASIC_TERMINAL_H
#define T_BASIC_TERMINAL_H

#include "singleton.h"
#include <QDateTime>
typedef struct{
    QString companyId;
    QString terminalId;
    QString encryptCode;
    QString terminalName;
    QString networkCard;
    QString serviceName;
    QString servicePhone;
    qint8 smsSendWay;
    qint8 noteLimit;
    qint8 tariffType;
    double userBasicTariff;
    double bigTariff;
    double middleTariff;
    double smallTariff;
    int expireHours;
    int expireStatus;
    double expireStartTariff;
    int expireStep;
    double expireStepMoney;
    double vipPercent;
    qint8 goodsCheck;
    int forceExpTakeHours;

    //临时寄存
    int storageHours;
    double storageStartTariff;
    int storageStep;
    double storageStepMoney;

    //寄件
    double sendBaseTariff;
    double expGetTariff;
    QString acceptExpCompanyName;
    QString lanJianName;
    QString lanJianPhone;

    //干洗店代收
    qint8 washSwitch;
    qint8 washSenderTariff;
    qint8 washGeterTariff;
    QString washerName;
    QString washerPhone;


    QString serverIp;
    QString serverPort;
    QDateTime syncModifyTime ;

    qint8 runStatus;

}TerminalDesc_t;

class CTerminal
{
public:
    TerminalDesc_t terminalInfo;

    QString mUrl;
    QString mVersion_Num;
    QString mVersion_Name;
public:
    CTerminal();

    void init();

    void getTerminalInfo();

    bool setTerminalInfo();

    void initConfigure();
    void readSetting();

    bool setCompanyId(QString companyId);
    bool setTerminalName(QString terminalName);
    bool setNetworkCard(QString card);
    bool setPhone(QString phone);
    bool setPerSmslimit(QString noteNumber);
    bool setTariffType(QString tartype);
    bool setUserBasicTariff(QString userBasic);
    bool setExpTariff(QString big,QString middle,QString small);
    bool setExpireHours(QString expireHours);
    bool setExpireStatus(QString expireStatus);
    bool setExpireStartTariff(QString expireStartTariff);
    bool setExpireStep(QString expireStep);
    bool setExpireStepmoney(QString expireStepmoney);
    bool setVipPercent(QString vipPercent);
    bool setGoodsCheck(QString goodsCheck);
    bool setForceExpTakeHours(QString forceExpTakeHours);

    //临时寄存
    bool setTempStorage(QString hours,QString startTariff, QString step, QString stepMoney);

    //用户寄件配置
    bool setSender(QString BaseTariff,QString expGetTariff,QString ExpCompanyName,QString name,QString phone);

    DECLARE_SINGLETON_CLASS(CTerminal)

};

typedef Singleton<CTerminal> SCTerminal;


#endif // T_BASIC_TERMINAL_H
