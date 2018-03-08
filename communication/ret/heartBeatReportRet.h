#ifndef HEARTBEATREPORTRET_H
#define HEARTBEATREPORTRET_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>

class heartBeatReportRet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString resultCode               READ resultCode        WRITE setResultCode)
    Q_PROPERTY(QString resultMessage            READ resultMessage     WRITE setResultMessage)
    Q_PROPERTY(QString terStatus                READ terStatus         WRITE setTerStatus)

public:

    explicit heartBeatReportRet(QObject *parent = 0): QObject(parent){}

    QString resultCode() const{return mResultCode;}
    QString resultMessage() const{return mResultMessage;}
    QString terStatus() const{return mTerStatus;}

    void setResultCode(QString errCode)
    {
        mResultCode = errCode;
    }

    void setResultMessage(QString message)
    {
        mResultMessage = message;
    }

    void setTerStatus(QString terStatus)
    {
        mTerStatus = terStatus;
    }

private:
    QString mResultCode ;
    QString mResultMessage ;
    QString mTerStatus;
};


#endif // HEARTBEATREPORTRET_H
