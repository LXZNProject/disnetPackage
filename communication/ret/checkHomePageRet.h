#ifndef CHECKHOMEPAGERET_H
#define CHECKHOMEPAGERET_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>

class checkHomePageRet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString resultCode               READ resultCode        WRITE setResultCode)
    Q_PROPERTY(QString resultMessage            READ resultMessage     WRITE setResultMessage)
    Q_PROPERTY(QString downloadURL              READ downloadURL       WRITE setDownloadURL)
    Q_PROPERTY(QString md5Code                  READ md5Code           WRITE setMd5Code)
    Q_PROPERTY(QString modifyTime               READ modifyTime        WRITE setModifyTime)

public:

    explicit checkHomePageRet(QObject *parent = 0): QObject(parent){}

    QString resultCode() const{return mResultCode;}
    QString resultMessage() const{return mResultMessage;}
    QString downloadURL() const{return mDownloadURL;}
    QString md5Code() const{return mMD5;}
    QString modifyTime() const{return mModifyTime;}

    void setResultCode(QString errCode)
    {
        mResultCode = errCode;
    }

    void setResultMessage(QString message)
    {
        mResultMessage = message;
    }

    void setDownloadURL(QString downloadURL)
    {
        mDownloadURL = downloadURL;
    }

    void setMd5Code(QString MD5)
    {
        mMD5 = MD5;
    }

    void setModifyTime(QString time)
    {
        mModifyTime = time;
    }


private:
    QString mResultCode ;
    QString mResultMessage ;
    QString mDownloadURL;
    QString mMD5;
    QString mModifyTime;
};
#endif // CHECKHOMEPAGERET_H
