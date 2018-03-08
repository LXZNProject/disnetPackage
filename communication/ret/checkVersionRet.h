#ifndef CHECKVERSIONRET_H
#define CHECKVERSIONRET_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>

class checkVersionRet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString resultCode               READ resultCode        WRITE setResultCode)
    Q_PROPERTY(QString resultMessage            READ resultMessage     WRITE setResultMessage)
    Q_PROPERTY(QString numVersion               READ numVersion        WRITE setNumVersion)
    Q_PROPERTY(QString displayVersion           READ displayVersion    WRITE setDisplayVersion)
    Q_PROPERTY(QString downloadURL              READ downloadURL       WRITE setDownloadURL)
    Q_PROPERTY(QString md5Code                  READ md5Code           WRITE setMd5Code)
    Q_PROPERTY(QString systemTime               READ systemTime        WRITE setSystemTime)

public:

    explicit checkVersionRet(QObject *parent = 0): QObject(parent){}

    QString resultCode() const{return mResultCode;}
    QString resultMessage() const{return mResultMessage;}
    QString numVersion() const{return mNumVersion;}
    QString displayVersion() const{return mDisplayVersion;}
    QString downloadURL() const{return mDownloadURL;}
    QString md5Code() const{return mMD5;}
    QString systemTime() const{return mSystemTime;}

    void setResultCode(QString errCode)
    {
        mResultCode = errCode;
    }

    void setResultMessage(QString message)
    {
        mResultMessage = message;
    }
    void setNumVersion(QString numVersion)
    {
        mNumVersion = numVersion;
    }

    void setDisplayVersion(QString displayVersion)
    {
        mDisplayVersion = displayVersion;
    }
    void setDownloadURL(QString downloadURL)
    {
        mDownloadURL = downloadURL;
    }

    void setMd5Code(QString MD5)
    {
        mMD5 = MD5;
    }

    void setSystemTime(QString time)
    {
        mSystemTime = time;
    }


private:
    QString mResultCode ;
    QString mResultMessage ;
    QString mNumVersion;
    QString mDisplayVersion;
    QString mDownloadURL;
    QString mMD5;
    QString mSystemTime;
};

#endif // CHECKVERSIONRET_H
