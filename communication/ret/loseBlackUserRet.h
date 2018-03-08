#ifndef LOSEBLACKUSERRET_H
#define LOSEBLACKUSERRET_H
#include <QObject>
#include <QStringList>
#include <QVariantMap>
class loseBlackUserRet: public QObject
{
        Q_OBJECT

        Q_PROPERTY(QString lossUserResultCode       READ lossUserResultCode     WRITE setLossUserResultCode)
        Q_PROPERTY(QString lossUsers                READ lossUsers              WRITE setLossUsers)
        Q_PROPERTY(QString lossUserSyncTime         READ lossUserSyncTime       WRITE setLossUserSyncTime)
        Q_PROPERTY(QString blackUserResultCode      READ blackUserResultCode    WRITE setBlackUserResultCode)
        Q_PROPERTY(QString blackUsers               READ blackUsers             WRITE setBlackUsers)
        Q_PROPERTY(QString blackUserSyncTime        READ blackUserSyncTime      WRITE setBlackUserSyncTime)
        Q_PROPERTY(QString cellResultCode           READ cellResultCode         WRITE setCellResultCode)
        Q_PROPERTY(QString notAvaliCells            READ notAvaliCells          WRITE setNotAvaliCells)
        Q_PROPERTY(QString notAvaliCellSyncTime     READ notAvaliCellSyncTime   WRITE setNotAvaliCellSyncTime)
        Q_PROPERTY(QString resultMessage            READ resultMessage          WRITE setResultMessage)

public:

    explicit loseBlackUserRet(QObject *parent = 0): QObject(parent){}

    QString lossUserResultCode() const{return mLossUserResultCode;}
    QString lossUsers() const{return mLossUsers;}
    QString lossUserSyncTime() const{return mLossUserSyncTime;}
    QString blackUserResultCode() const{return mBlackUserResultCode;}
    QString blackUsers() const{return mBlackUsers;}
    QString blackUserSyncTime() const{return mBlackUserSyncTime;} 
    QString cellResultCode() const{return mCellResultCode;}
    QString notAvaliCells() const{return mNotAvaliCells;}
    QString notAvaliCellSyncTime() const{return mNotAvaliCellSyncTime;}
    QString resultMessage() const{return mResultMessage;}

    void setLossUserResultCode(QString value)
    {
        mLossUserResultCode = value;
    }
    void setLossUsers(QString value)
    {
        mLossUsers = value;
    }
    void setLossUserSyncTime(QString value)
    {
        mLossUserSyncTime = value;
    }
    void setBlackUserResultCode(QString value)
    {
        mBlackUserResultCode = value;
    }
    void setBlackUsers(QString value)
    {
        mBlackUsers = value;
    }
    void setBlackUserSyncTime(QString value)
    {
        mBlackUserSyncTime = value;
    }

    void setCellResultCode(QString value)
    {
        mCellResultCode = value;
    }
    void setNotAvaliCells(QString value)
    {
        mNotAvaliCells = value;
    }
    void setNotAvaliCellSyncTime(QString value)
    {
        mNotAvaliCellSyncTime = value;
    }
    void setResultMessage(QString value)
    {
        mResultMessage = value;
    }

private:
    QString mLossUserResultCode ;
    QString mLossUsers ;
    QString mLossUserSyncTime ;
    QString mBlackUserResultCode ;
    QString mBlackUsers ;
    QString mBlackUserSyncTime ;
    QString mCellResultCode ;
    QString mNotAvaliCells ;
    QString mNotAvaliCellSyncTime ;
    QString mResultMessage ;
};
#endif // LOSEBLACKUSERRET_H
