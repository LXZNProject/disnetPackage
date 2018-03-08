#ifndef PASSWORDRECHARGERET_H
#define PASSWORDRECHARGERET_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>

class passwordRechargeRet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString resultCode               READ resultCode        WRITE setResultCode)
    Q_PROPERTY(QString resultMessage            READ resultMessage     WRITE setResultMessage)

public:

    explicit passwordRechargeRet(QObject *parent = 0): QObject(parent){}

    QString resultCode() const{return mResultCode;}
    QString resultMessage() const{return mResultMessage;}

    void setResultCode(QString errCode)
    {
        mResultCode = errCode;
    }

    void setResultMessage(QString message)
    {
        mResultMessage = message;
    }

private:
    QString mResultCode ;
    QString mResultMessage ;
};



#endif // PASSWORDRECHARGERET_H
