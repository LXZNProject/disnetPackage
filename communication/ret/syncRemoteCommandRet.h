#ifndef SYNCREMOTECOMMANDRET_H
#define SYNCREMOTECOMMANDRET_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include "qobjecthelper.h"

class QRemoteCommandList : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id               READ id              WRITE setId)
    Q_PROPERTY(QString commandType      READ commandType     WRITE setCommandType)
    Q_PROPERTY(QString commandContent   READ commandContent  WRITE setCommandContent)

public:

    explicit QRemoteCommandList(QObject *parent = 0): QObject(parent){}

    QString id() const{return mId;}
    QString commandType() const{return mCmdType;}
    QString commandContent() const{return mCmdContent;}

    void setId(QString id)
    {
        mId = id;
    }

    void setCommandType(QString type)
    {
        mCmdType = type;
    }
    void setCommandContent(QString content)
    {
        mCmdContent = content;
    }

private:
    QString mId ;
    QString mCmdType ;
    QString mCmdContent ;
};

class syncRemoteCommandRet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString resultCode               READ resultCode        WRITE setResultCode)
    Q_PROPERTY(QString resultMessage            READ resultMessage     WRITE setResultMessage)
    Q_PROPERTY(QVariantList remoteCommandList   READ remoteCommandList WRITE setRemoteCommandList)

public:

    explicit syncRemoteCommandRet(QObject *parent = 0): QObject(parent){}

    QString resultCode() const{return mResultCode;}
    QString resultMessage() const{return mResultMessage;}
    QVariantList remoteCommandList() const
    {
        QVariantList list;
        return list;
    }

    QList<QRemoteCommandList*> getCmdInfo()
    {
        return mCmdList;
    }

    ~syncRemoteCommandRet()
    {
        for (int i = 0; i < mCmdList.size(); i++)
        {
            delete mCmdList[i];
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
    void setRemoteCommandList(QVariantList cmdList)
    {
        for (int i = 0; i < cmdList.size(); i++)
        {
            QRemoteCommandList* cmdInfo = new QRemoteCommandList;
            mObjectHelper.qvariant2qobject(cmdList[i].toMap(), cmdInfo);
            mCmdList.append(cmdInfo);
        }
    }

private:
    QString mResultCode ;
    QString mResultMessage ;
    QList<QRemoteCommandList*>mCmdList;
    QJson::QObjectHelper mObjectHelper;
};
#endif // SYNCREMOTECOMMANDRET_H
