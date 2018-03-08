#ifndef SYNCORDERCELLRET_H
#define SYNCORDERCELLRET_H


#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include "qobjecthelper.h"

class expOrdersCellList : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString m1CardId             READ  m1CardId     WRITE setM1CardId)
    Q_PROPERTY(QString cellId               READ  cellId       WRITE setCellId)
    Q_PROPERTY(QString cellType             READ  cellType     WRITE setCellType)
    Q_PROPERTY(QString expireTime           READ  expireTime   WRITE setExpireTime)

public:

    explicit expOrdersCellList(QObject *parent = 0): QObject(parent){}

    QString m1CardId() const{return mM1CardId;}
    QString cellId() const{return mCellId;}
    QString cellType() const{return mCellType;}
    QString expireTime() const{return mExpireTime;}

    void setM1CardId(QString m1card)
    {
        mM1CardId = m1card;
    }

    void setCellId(QString cellId)
    {
        mCellId = cellId;
    }

    void setCellType(QString cellType)
    {
        mCellType = cellType;
    }

    void setExpireTime(QString expireTime)
    {
        mExpireTime = expireTime;
    }


private:
    QString mM1CardId ;
    QString mCellId ;
    QString mCellType;
    QString mExpireTime;
};


class userOrdersCellList : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString mobilePhone          READ  mobilePhone  WRITE setMobilePhone)
    Q_PROPERTY(QString cellId               READ  cellId       WRITE setCellId)
    Q_PROPERTY(QString cellType             READ  cellType     WRITE setCellType)
    Q_PROPERTY(QString expireTime           READ  expireTime   WRITE setExpireTime)

public:

    explicit userOrdersCellList(QObject *parent = 0): QObject(parent){}

    QString mobilePhone() const{return mMobilePhone;}
    QString cellId() const{return mCellId;}
    QString cellType() const{return mCellType;}
    QString expireTime() const{return mExpireTime;}

    void setMobilePhone(QString phone)
    {
        mMobilePhone = phone;
    }

    void setCellId(QString cellId)
    {
        mCellId = cellId;
    }

    void setCellType(QString cellType)
    {
        mCellType = cellType;
    }

    void setExpireTime(QString expireTime)
    {
        mExpireTime = expireTime;
    }


private:
    QString mMobilePhone ;
    QString mCellId ;
    QString mCellType;
    QString mExpireTime;
};

class syncOrderCellRet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString resultCode               READ resultCode        WRITE setResultCode)
    Q_PROPERTY(QString resultMessage            READ resultMessage     WRITE setResultMessage)
    Q_PROPERTY(QString orderCellSyncTime        READ orderCellSyncTime WRITE setOrderCellSyncTime)
    Q_PROPERTY(QVariantList expOrders           READ expOrders         WRITE setExpOrders)
    Q_PROPERTY(QVariantList userOrders          READ userOrders        WRITE setUserOrders)

public:

    explicit syncOrderCellRet(QObject *parent = 0): QObject(parent){}

    QString resultCode() const{return mResultCode;}
    QString resultMessage() const{return mResultMessage;}
    QString orderCellSyncTime() const{return mOrderCellSyncTime;}

    QVariantList expOrders() const
    {
        QVariantList list;
        return list;
    }
    QVariantList userOrders() const
    {
        QVariantList list;
        return list;
    }

    QList<expOrdersCellList*> getExpOrderList()
    {
        return mExpOrderList;
    }
    QList<userOrdersCellList*> getUserOrderList()
    {
        return mUserOrderList;
    }

    ~syncOrderCellRet()
    {
        for (int i = 0; i < mExpOrderList.size(); i++)
        {
            delete mExpOrderList[i];
        }

        for(int j = 0;j < mUserOrderList.size(); j++)
        {
            delete mUserOrderList[j];
        }
    }

    void setResultCode(QString errCode)
    {
        mResultCode = errCode;
    }

    void setResultMessage(QString message)
    {
        mResultMessage = message;
    }
    void setOrderCellSyncTime(QString orderCellSyncTime)
    {
        mOrderCellSyncTime = orderCellSyncTime;
    }

    void setExpOrders(QVariantList expList)
    {
        for (int i = 0; i < expList.size(); i++)
        {
            expOrdersCellList* exp = new expOrdersCellList;
            mObjectHelper.qvariant2qobject(expList[i].toMap(), exp);
            mExpOrderList.append(exp);
        }
    }

    void setUserOrders(QVariantList userList)
    {
        for (int i = 0; i < userList.size(); i++)
        {
            userOrdersCellList* user = new userOrdersCellList;
            mObjectHelper.qvariant2qobject(userList[i].toMap(), user);
            mUserOrderList.append(user);
        }
    }

private:
    QString mResultCode ;
    QString mResultMessage ;
    QString mOrderCellSyncTime;
    QList<expOrdersCellList*> mExpOrderList;
    QList<userOrdersCellList*> mUserOrderList;
    QJson::QObjectHelper mObjectHelper;
};

#endif // SYNCORDERCELLRET_H
