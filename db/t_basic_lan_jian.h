#ifndef T_BASIC_LAN_JIAN_H
#define T_BASIC_LAN_JIAN_H


#include "singleton.h"
#include <QDateTime>

typedef struct{
    QString lanJianId;
    qint8 deliveryType;
    QString cellId;
    qint8 cellType;
    QString cabinetId;
    QString mobilephone;
    QString companyName;
    QString lanJianName;
    QString lanJianPhone;
    QDateTime createtime;
    qint8 status;
    qint8 messageStatus;
    QDateTime messageSentTime;
    QString getCode;
    QDateTime takeTime;
    double hasPayMoney;
    double ownerBasePay;
    double expressPay;
    qint8 payway;
    double cashMoney;
    qint8 goodsCheckStatus;
    qint8 switchCharge;
    QString senderPhotoPath;
    QString getterPhotoPath;
}SenderDesc_t;

class CSender
{
public:
    CSender();

    void init();
    bool insertSenderInfo(SenderDesc_t senderInfo);
    bool updateSenderInfo(SenderDesc_t senderInfo);

    bool selectTakeDeliveryInfo(QString code, SenderDesc_t &senderInfo);
    void clearDeliveryInfo(SenderDesc_t &senderInfo);

    bool selectTakeCodeForPhone(QString phone,QString &code);

    bool selectExceptionCodeForCell(QString cell,SenderDesc_t &senderInfo);
    QString createRandomCode();

    void updateSenderNoteSendTimeInfo(QString uuid ,QDateTime time);

    qint8 clearTariffInfo(QString uuid);

    qint8 closeDeliveryInfo(QString uuid,QString &cell);

    DECLARE_SINGLETON_CLASS(CSender)
};

typedef Singleton<CSender> SCSender;


#endif // T_BASIC_LAN_JIAN_H
