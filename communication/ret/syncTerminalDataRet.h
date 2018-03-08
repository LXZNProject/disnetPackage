#ifndef ADDVIPCUSTOMERRESP_H
#define ADDVIPCUSTOMERRESP_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include "qobjecthelper.h"
#include <QDebug>


class QTerminalInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString terminalName         READ  terminalName          WRITE setTerminalName)
    Q_PROPERTY(QString networkCard          READ  networkCard           WRITE setNetworkCard)
    Q_PROPERTY(QString linkManName          READ  linkManName           WRITE setLinkManName)
    Q_PROPERTY(QString linkMobilePhone      READ  linkMobilePhone       WRITE setLinkMobilePhone)
    Q_PROPERTY(QString smsSendWay           READ  smsSendWay            WRITE setSmsSendWay)
    Q_PROPERTY(QString perSmslimit          READ  perSmslimit           WRITE setPerSmslimit)
    Q_PROPERTY(QString tariffType           READ  tariffType            WRITE setTariffType)
    Q_PROPERTY(QString userBasicTariff      READ  userBasicTariff       WRITE setUserBasicTariff)
    Q_PROPERTY(QString expBigTariff         READ  expBigTariff          WRITE setExpBigTariff)
    Q_PROPERTY(QString expMiddleTariff      READ  expMiddleTariff       WRITE setExpMiddleTariff)
    Q_PROPERTY(QString expSmallTariff       READ  expSmallTariff        WRITE setExpSmallTariff)
    Q_PROPERTY(QString expireHours          READ  expireHours           WRITE setExpireHours)
    Q_PROPERTY(QString expireStatus         READ  expireStatus          WRITE setExpireStatus)
    Q_PROPERTY(QString expireStartTariff    READ  expireStartTariff     WRITE setExpireStartTariff)
    Q_PROPERTY(QString expireStep           READ  expireStep            WRITE setExpireStep)
    Q_PROPERTY(QString expireStepMoney      READ  expireStepMoney       WRITE setExpireStepMoney)
    Q_PROPERTY(QString vipPercent           READ  vipPercent            WRITE setVipPercent)
    Q_PROPERTY(QString goodsCheck           READ  goodsCheck            WRITE setGoodsCheck)
    Q_PROPERTY(QString forceExpTakeHours    READ  forceExpTakeHours     WRITE setForceExpTakeHours)

    Q_PROPERTY(QString storageHours         READ  storageHours          WRITE setStorageHours)
    Q_PROPERTY(QString storageStartTariff   READ  storageStartTariff    WRITE setStorageStartTariff)
    Q_PROPERTY(QString storageStep          READ  storageStep           WRITE setStorageStep)
    Q_PROPERTY(QString storageStepMoney     READ  storageStepMoney      WRITE setStorageStepMoney)

    Q_PROPERTY(QString sendBaseTariff          READ  sendBaseTariff           WRITE setSendBaseTariff)
    Q_PROPERTY(QString expGetTariff            READ  expGetTariff             WRITE setExpGetTariff)
    Q_PROPERTY(QString acceptExpCompanyName    READ  acceptExpCompanyName     WRITE setAcceptExpCompanyName)
    Q_PROPERTY(QString lanJianName             READ  lanJianName              WRITE setLanJianName)
    Q_PROPERTY(QString lanJianPhone            READ  lanJianPhone             WRITE setLanJianPhone)

    Q_PROPERTY(QString washSwitch         READ  washSwitch          WRITE setWashSwitch)
    Q_PROPERTY(QString washSenderTariff   READ  washSenderTariff    WRITE setWashSenderTariff)
    Q_PROPERTY(QString washGeterTariff    READ  washGeterTariff     WRITE setWashGeterTariff)
    Q_PROPERTY(QString washerName         READ  washerName          WRITE setWasherName)
    Q_PROPERTY(QString washerPhone        READ  washerPhone         WRITE setWasherPhone)

    Q_PROPERTY(QString serverIp         READ  serverIp           WRITE setServerIp)
    Q_PROPERTY(QString serverPort       READ  serverPort         WRITE setServerPort)
    Q_PROPERTY(QString encryptCode      READ  encryptCode        WRITE setEncryptCode)
    Q_PROPERTY(QString syncModifyTime   READ  syncModifyTime     WRITE setSyncModifyTime)

public:

    explicit QTerminalInfo(QObject *parent = 0): QObject(parent){}

    QString terminalName() const{return mTerminalName;}
    QString networkCard() const{return mNetworkCard;}
    QString linkManName() const{return mLinkManName;}
    QString linkMobilePhone() const{return mLinkMobilePhone;}
    QString smsSendWay() const{return mSmsSendWay;}
    QString perSmslimit() const{return mPerSmslimit;}
    QString tariffType() const{return mTariffType;}
    QString userBasicTariff() const{return mUserBasicTariff;}
    QString expBigTariff() const{return mExpBigTariff;}
    QString expMiddleTariff() const{return mExpMiddleTariff;}
    QString expSmallTariff() const{return mExpSmallTariff;}
    QString expireHours() const{return mExpireHours;}
    QString expireStatus() const{return mExpireStatus;}
    QString expireStartTariff() const{return mExpireStartTariff;}
    QString expireStep() const{return mExpireStep;}
    QString expireStepMoney() const{return mExpireStepMoney;}
    QString vipPercent() const{return mVipPercent;}
    QString goodsCheck() const{return mGoodsCheck;}
    QString forceExpTakeHours() const{return mForceExpTakeHours;}

    QString storageHours() const{return mStorageHours;}
    QString storageStartTariff() const{return mStorageStartTariff;}
    QString storageStep() const{return mStorageStep;}
    QString storageStepMoney() const{return mStorageStepMoney;}

    QString sendBaseTariff() const{return mSendBaseTariff;}
    QString expGetTariff() const{return mExpGetTariff;}
    QString acceptExpCompanyName() const{return mAcceptExpCompanyName;}
    QString lanJianName() const{return mLanJianName;}
    QString lanJianPhone() const{return mLanJianPhone;}

    QString washSwitch() const{return mWashSwitch;}
    QString washSenderTariff() const{return mWashSenderTariff;}
    QString washGeterTariff() const{return mWashGeterTariff;}
    QString washerName() const{return mWasherName;}
    QString washerPhone() const{return mWasherPhone;}

    QString serverIp() const{return mServerIp;}
    QString serverPort() const{return mServerPort;}
    QString encryptCode() const{return mEncryptCode;}
    QString syncModifyTime() const{return mSyncModifyTime;}


    void setTerminalName(QString terminalName)
    {
        mTerminalName = terminalName;
    }

    void setNetworkCard(QString networkCard)
    {
        mNetworkCard = networkCard;
    }

    void setLinkManName(QString linkManName)
    {
        mLinkManName = linkManName;
    }

    void setLinkMobilePhone(QString linkMobilePhone)
    {
        mLinkMobilePhone = linkMobilePhone;
    }

    void setSmsSendWay(QString smsSendWay)
    {
        mSmsSendWay = smsSendWay;
    }

    void setPerSmslimit(QString perSmslimit)
    {
        mPerSmslimit = perSmslimit;
    }

    void setTariffType(QString tariffType)
    {
        mTariffType = tariffType;
    }

    void setUserBasicTariff(QString userBasicTariff)
    {
        mUserBasicTariff = userBasicTariff;
    }

    void setExpBigTariff(QString expBigTariff)
    {
        mExpBigTariff = expBigTariff;
    }

    void setExpMiddleTariff(QString expMiddleTariff)
    {
        mExpMiddleTariff = expMiddleTariff;
    }

    void setExpSmallTariff(QString expSmallTariff)
    {
        mExpSmallTariff = expSmallTariff;
    }

    void setExpireHours(QString expireHours)
    {
        mExpireHours = expireHours;
    }

    void setExpireStatus(QString expireStatus)
    {
        mExpireStatus = expireStatus;
    }

    void setExpireStartTariff(QString expireStartTariff)
    {
        mExpireStartTariff = expireStartTariff;
    }


    void setExpireStep(QString expireStep)
    {
        mExpireStep = expireStep;
    }

    void setExpireStepMoney(QString expireStepMoney)
    {
        mExpireStepMoney = expireStepMoney;
    }

    void setVipPercent(QString vipPercent)
    {
        mVipPercent = vipPercent;
    }

    void setGoodsCheck(QString goodsCheck)
    {
        mGoodsCheck = goodsCheck;
    }

    void setForceExpTakeHours(QString forceExpTakeHours)
    {
        mForceExpTakeHours = forceExpTakeHours;
    }

    void setStorageHours(QString storageHours)
    {
        mStorageHours = storageHours;
    }

    void setStorageStartTariff(QString storageStartTariff)
    {
        mStorageStartTariff = storageStartTariff;
    }

    void setStorageStep(QString storageStep)
    {
        mStorageStep = storageStep;
    }

    void setStorageStepMoney(QString storageStepMoney)
    {
        mStorageStepMoney = storageStepMoney;
    }

    void setSendBaseTariff(QString sendBaseTariff)
    {
        mSendBaseTariff = sendBaseTariff;
    }


    void setExpGetTariff(QString expGetTariff)
    {
        mExpGetTariff = expGetTariff;
    }

    void setAcceptExpCompanyName(QString acceptExpCompanyName)
    {
        mAcceptExpCompanyName = acceptExpCompanyName;
    }

    void setLanJianName(QString lanJianName)
    {
        mLanJianName = lanJianName;
    }

    void setLanJianPhone(QString lanJianPhone)
    {
        mLanJianPhone = lanJianPhone;
    }

    void setWashSwitch(QString washSwitch)
    {
        mWashSwitch = washSwitch;
    }

    void setWashSenderTariff(QString washSenderTariff)
    {
        mWashSenderTariff = washSenderTariff;
    }


    void setWashGeterTariff(QString washGeterTariff)
    {
        mWashGeterTariff = washGeterTariff;
    }

    void setWasherName(QString washerName)
    {
        mWasherName = washerName;
    }

    void setWasherPhone(QString washerPhone)
    {
        mWasherPhone = washerPhone;
    }

    void setServerIp(QString serverIp)
    {
        mServerIp = serverIp;
    }

    void setServerPort(QString serverPort)
    {
        mServerPort = serverPort;
    }

    void setEncryptCode(QString encryptCode)
    {
        mEncryptCode = encryptCode;
    }

    void setSyncModifyTime(QString syncModifyTime)
    {
        mSyncModifyTime = syncModifyTime;
    }


private:
    QString mTerminalName;
    QString mNetworkCard ;
    QString mLinkManName ;
    QString mLinkMobilePhone ;
    QString mSmsSendWay ;
    QString mPerSmslimit ;
    QString mTariffType ;
    QString mUserBasicTariff ;
    QString mExpBigTariff ;
    QString mExpMiddleTariff ;
    QString mExpSmallTariff ;
    QString mExpireHours ;
    QString mExpireStatus ;
    QString mExpireStartTariff ;
    QString mExpireStep ;
    QString mExpireStepMoney ;
    QString mVipPercent ;
    QString mGoodsCheck ;
    QString mForceExpTakeHours ;

    QString mStorageHours ;
    QString mStorageStartTariff ;
    QString mStorageStep ;
    QString mStorageStepMoney ;

    QString mSendBaseTariff ;
    QString mExpGetTariff ;
    QString mAcceptExpCompanyName ;
    QString mLanJianName ;
    QString mLanJianPhone;

    QString mWashSwitch ;
    QString mWashSenderTariff ;
    QString mWashGeterTariff ;
    QString mWasherName ;
    QString mWasherPhone ;

    QString mServerIp ;
    QString mServerPort ;
    QString mEncryptCode ;
    QString mSyncModifyTime;
};

class QNoteTemplateInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString templateType     READ  templateType     WRITE setTemplateType)
    Q_PROPERTY(QString canShutDown      READ  canShutDown      WRITE setCanShutDown)
    Q_PROPERTY(QString status           READ  status           WRITE setStatus)
    Q_PROPERTY(QString templateContent  READ  templateContent  WRITE setTemplateContent)

public:

    explicit QNoteTemplateInfo(QObject *parent = 0): QObject(parent){}

    QString templateType() const{return mTemplateType;}
    QString canShutDown() const{return mCanShutDown;}
    QString status() const{return mStatus;}
    QString templateContent() const{return mTemplateContent;}

    void setTemplateType(QString templateType)
    {
        mTemplateType = templateType;
    }

    void setCanShutDown(QString canShutDown)
    {
        mCanShutDown = canShutDown;
    }

    void setStatus(QString status)
    {
        mStatus = status;
    }

    void setTemplateContent(QString templateContent)
    {
        mTemplateContent = templateContent;
    }

private:
    QString mTemplateType ;
    QString mCanShutDown ;
    QString mStatus;
    QString mTemplateContent;
};

class QCabinetInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString cabinetId        READ  cabinetId        WRITE setCabinetId)
    Q_PROPERTY(QString cabinetType      READ  cabinetType      WRITE setCabinetType)


public:

    explicit QCabinetInfo(QObject *parent = 0): QObject(parent){}

    QString cabinetId() const{return mCabinetId;}
    QString cabinetType() const{return mCabinetType;}


    void setCabinetId(QString cabinetId)
    {
        mCabinetId = cabinetId;
    }

    void setCabinetType(QString cabinetType)
    {
        mCabinetType = cabinetType;
    }


private:
    QString mCabinetId ;
    QString mCabinetType ;
};

class syncTerminalDataRet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString resultCode               READ resultCode        WRITE setResultCode)
    Q_PROPERTY(QString systemTime               READ systemTime        WRITE setSystemTime)
    Q_PROPERTY(QString resultMessage            READ resultMessage     WRITE setResultMessage)
    Q_PROPERTY(QVariantMap soapTerminal         READ soapTerminal      WRITE setSoapTerminal)
    Q_PROPERTY(QVariantList smsTemplateInfos    READ smsTemplateInfos  WRITE setSmsTemplateInfos)
    Q_PROPERTY(QVariantList cabinets            READ cabinets          WRITE setCabinets)

public:
    syncTerminalDataRet(QObject *parent = 0): QObject(parent){}

    QString resultCode() const{return mResultCode;}
    QString systemTime() const{return mSystemTime;}
    QString resultMessage() const{return mResultMessage;}
    QVariantMap soapTerminal() const
    {
        QVariantMap list;
        return list;
    }
    QVariantList smsTemplateInfos() const
    {
        QVariantList list;
        return list;
    }
    QVariantList cabinets() const
    {
        QVariantList list;
        return list;
    }

    QList<QTerminalInfo*> getTerminalInfo()
    {
        return mTerminaInfoList;
    }

    QList<QNoteTemplateInfo*> getNoteTemplateList()
    {
        return mNoteInfoList;
    }

    QList<QCabinetInfo*> getCabinetList()
    {
        return mCabinetInfoList;
    }

    ~syncTerminalDataRet()
    {
        for (int i = 0; i < mNoteInfoList.size(); i++)
        {
            delete mNoteInfoList[i];
        }

        for(int j = 0;j < mCabinetInfoList.size(); j++)
        {
            delete mCabinetInfoList[j];
        }

        for(int k = 0;k < mTerminaInfoList.size(); k++)
        {
            delete mTerminaInfoList[k];
        }
    }

    void setResultCode(QString errCode)
    {
        mResultCode = errCode;
    }

    void setSystemTime(QString time)
    {
        mSystemTime = time;
    }

    void setResultMessage(QString message)
    {
        mResultMessage = message;
    }

    void setSoapTerminal(QVariantMap terminalList)
    {
        QTerminalInfo* terminaInfo = new QTerminalInfo;
        mObjectHelper.qvariant2qobject(terminalList, terminaInfo);
        mTerminaInfoList.append(terminaInfo);
    }

    void setSmsTemplateInfos(QVariantList noteList)
    {
        for (int i = 0; i < noteList.size(); i++)
        {
            QNoteTemplateInfo* note = new QNoteTemplateInfo;
            mObjectHelper.qvariant2qobject(noteList[i].toMap(), note);
            mNoteInfoList.append(note);
        }
    }

    void setCabinets(QVariantList cabinetList)
    {
        for (int i = 0; i < cabinetList.size(); i++)
        {
            QCabinetInfo* cabinet = new QCabinetInfo;
            mObjectHelper.qvariant2qobject(cabinetList[i].toMap(), cabinet);
            mCabinetInfoList.append(cabinet);
        }
    }

private:
    QString mResultCode ;
    QString mSystemTime ;
    QString mResultMessage;
    QList<QTerminalInfo*>mTerminaInfoList;
    QList<QNoteTemplateInfo*> mNoteInfoList;
    QList<QCabinetInfo*> mCabinetInfoList;
    QJson::QObjectHelper mObjectHelper;
 };

#endif // ADDVIPCUSTOMERRESP_H
