#ifndef T_BASIC_TEMP_STORAGE_H
#define T_BASIC_TEMP_STORAGE_H
#include "singleton.h"
#include <QDateTime>

typedef struct{
    QString storageId;
    QString  cellId;
    int          cellType;
    QString  cabinetId;
    QString  mobilephone;
    QDateTime  createtime;
    int          status;
    int          messageStatus;
    QDateTime  messageSentTime;
    QString  getCode;
    QDateTime  takeTime;
    double   hasPayMoney;
    int          payway;
    double  saveBasePay;
    double  debtPay;
    double  cashMoney;
    int         goodsCheckStatus;
    int         switchCharge;
    QString senderPhotoPath;
    QString getterPhotoPath;

}BasicTempStorage;

class t_basic_temp_storage
{
public:
    t_basic_temp_storage();

    void init();
    bool insertStorage(BasicTempStorage date);
    bool updateStorageInfo(BasicTempStorage storageInfo);


    bool selectTakeDeliveryInfo(QString code, BasicTempStorage &storageInfo);

    void clearDeliveryInfo(BasicTempStorage &deliveryInfo);
    bool selectTakeCodeForPhone(QString phone,QString &code);
    bool selectExceptionCodeForCell(QString cell,BasicTempStorage &storageInfo);
    QString createRandomCode();

    void updateStorageNoteSendTimeInfo(QString uuid ,QDateTime time);

    qint8 clearTariffInfo(QString uuid);

    qint8 closeDeliveryInfo(QString uuid,QString &cell);

    DECLARE_SINGLETON_CLASS(t_basic_temp_storage)
};
typedef Singleton<t_basic_temp_storage> SBasicTempStorage;

#endif // T_BASIC_TEMP_STORAGE_H
