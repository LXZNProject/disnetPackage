#ifndef T_BASIC_DELIVERY_H
#define T_BASIC_DELIVERY_H


#include "singleton.h"
#include <QDateTime>

typedef struct{
    QString  deliveryId;
    QString cellId;
    qint8 cellType;
    QString cabinetId;
    QString m1card;
    QString mobilephone;
    QString packageId;
    QString expName;
    QString expMobilePhone;
    QString expCompanyName;
    QDateTime createtime;
    qint8 status;
    qint8 messageStatus;
    QDateTime messageSentTime;
    QString getCode;
    QDateTime takeTime;
    double hasPayMoney;
    double expressPay;
    double ownerBasePay;
    qint8 tariffType;
    qint8 expireStatus;
    qint8 payway;
    double debtPay;
    double cashMoney;
    qint8 goodsCheckStatus;
    qint8 switchCharge;
    QString senderPhotoPath;
    QString getterPhotoPath;
    int expireHours;
    double expireStartTariff;
    int expireStep;
    double expireStepMoney;
}DelivertDesc_t;

class CDelivery
{
public:
    CDelivery();

    void init();
    bool insertDeliveryInfo(DelivertDesc_t deliveryInfo);
    bool updateDeliveryInfo(DelivertDesc_t deliveryInfo);

    void deleteOverTimeDeliveryInfo();

    qint8 selectSaveDeliveryInfo(QString m1Card,QList<DelivertDesc_t> &deliveryInfo); //0 无待确认和滞留 1 待确认 2 滞留 3 混合

    bool selectExpTakeDeliveryInfo(QString m1Card,QList<DelivertDesc_t> &deliveryInfo);

    bool selectTakeDeliveryInfo(QString code, DelivertDesc_t &deliveryInfo);

    bool selectTakeCodeForPhone(QString phone,QString &code);

    QString createRandomCode();

    bool selectExceptionCodeForCell(QString cell,DelivertDesc_t &deliveryInfo);

    void clearDeliveryInfo(DelivertDesc_t &deliveryInfo);

    void updateDeliveryNoteSendTimeInfo(QString uuid ,QDateTime time);
    void updateDeliveryExpTakeInfo(QString uuid);

    qint8 clearTariffInfo(QString uuid);

    qint8 closeDeliveryInfo(QString uuid,QString &cell);
    qint8 confirmDeliveryInfo(QString uuid,DelivertDesc_t &deliveryInfo);

    bool queryExpDeliveryInfo(QList<DelivertDesc_t> deliveryInfo,QList<QStringList> &modelData);
    bool queryExpTakeDeliveryInfo(QList<DelivertDesc_t> deliveryInfo,QList<QStringList> &modelData);

    DECLARE_SINGLETON_CLASS(CDelivery)
};

typedef Singleton<CDelivery> SCDelivery;

#endif // T_BASIC_DELIVERY_H
