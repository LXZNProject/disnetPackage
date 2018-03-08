#ifndef DEBTPAGE_H
#define DEBTPAGE_H

#include <QWidget>
#include <QTimer>

#include "t_basic_delivery.h"
#include "t_basic_lan_jian.h"
#include "t_basic_temp_storage.h"
#include "msgbox.h"

typedef enum{
    NOT_ST = 0x00,
    SAVE_ST,
    SENDER_ST,
    STORE_ST
}TAKE_STATUS;


namespace Ui {
class debtpage;
}

class debtpage : public QWidget
{
    Q_OBJECT
    
public:
    explicit debtpage(QWidget *parent = 0);
    ~debtpage();

    void initDebtPage();
    void readDebtInfo(TAKE_STATUS,double,double pay = 0,double debt = 0);

    void startPayCoin(DelivertDesc_t &);
    void startPayCoin(SenderDesc_t &);
    void startPayCoin(BasicTempStorage &);

    bool startPayCard(DelivertDesc_t & ,QStringList);
    bool startPayCard(SenderDesc_t &,QStringList);
signals:
    void savecoin_signal();
    void sendercoin_signal();
    void storagecoin_signal();


public slots:
    void timeOver();
private:
    Ui::debtpage *ui;

    bool isUpdate;
    MsgBox myMsgBox;
    QTimer timer;

    TAKE_STATUS mStatus;
    DelivertDesc_t mDeliveryInfo;
    SenderDesc_t mSenderInfo;
    BasicTempStorage mStorageInfo;
};

#endif // DEBTPAGE_H
