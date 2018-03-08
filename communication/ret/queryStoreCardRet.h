#ifndef QUERYSTORECARDRET_H
#define QUERYSTORECARDRET_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>

class queryStoreCardRet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString resultCode                  READ resultCode           WRITE setResultCode)
    Q_PROPERTY(QString resultMessage            READ resultMessage     WRITE setResultMessage)
    Q_PROPERTY(QString cardNo                        READ cardNo                  WRITE setCardNo)
    Q_PROPERTY(QString cardPasswd                READ cardPasswd           WRITE setCardPasswd)
    Q_PROPERTY(QString cardType                     READ cardType                WRITE setCardType)
    Q_PROPERTY(QString cardValue                   READ cardValue               WRITE setCardValue)

public:

    explicit queryStoreCardRet(QObject *parent = 0): QObject(parent){}

    QString resultCode() const{return mResultCode;}
    QString resultMessage() const{return mResultMessage;}
    QString cardNo() const{return mCardNo;}
    QString cardPasswd() const{return mCardPasswd;}
    QString cardType() const{return mCardType;}
    QString cardValue() const{return mCardValue;}

    void setResultCode(QString errCode)
    {
        mResultCode = errCode;
    }
    void setResultMessage(QString message)
    {
        mResultMessage = message;
    }
    void setCardNo(QString number)
    {
        mCardNo = number;
    }
    void setCardPasswd(QString passwd)
    {
        mCardPasswd = passwd;
    }
    void setCardType(QString type)
    {
        mCardType = type;
    }
    void setCardValue(QString value)
    {
        mCardValue = value;
    }

private:
    QString mResultCode ;
    QString mResultMessage ;
    QString mCardNo ;
    QString mCardPasswd ;
    QString mCardType ;
    QString mCardValue ;
};



#endif // QUERYSTORECARDRET_H
