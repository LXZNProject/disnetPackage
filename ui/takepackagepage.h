#ifndef TAKEPACKAGEPAGE_H
#define TAKEPACKAGEPAGE_H

#include <QWidget>
#include <QStackedLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "t_basic_delivery.h"
#include "t_basic_lan_jian.h"
#include "t_basic_temp_storage.h"
#include "lockdrv.h"
#include "t_basic_temp_storage.h"
#include "keyboard.h"

#include "debtpage.h"
#include "msgbox.h"
#include "takeopencell.h"
#include "advertpage.h"
#include "opencellpage.h"

namespace Ui {
class takepackagepage;
}

class takepackagepage : public QWidget
{
    Q_OBJECT
public:
    explicit takepackagepage(QWidget *parent = 0);
    ~takepackagepage();

    void initTakePackPage();

    void readTakePackInfo();

    void setBtnEnable(bool);

signals:
    void resetTime();
    void savecard_signal();
    void sendercard_signal();

public slots:
    void okBtn_takePack_slots();
    void againBtn_slots();
    void quitBtn_slots();

    void okBtn_getCode_slots();
    void cancelBtn_slots();

    void queryCardInfo_slots();

    void showAdvertPage();
    void readyOpenBoxLock();
    void openBoxLock();
    void openSenderBoxLock();
    void openStorageBoxLock();

    void skipCurPage();

protected:
    bool event(QEvent *);
private:
    Ui::takepackagepage *ui;

    MsgBox myMsgBox;
    keyboard *keypage;
    TAKE_STATUS Cur_Status;
    double mDebtPay;
    CLockDrv lockoperation;
    DelivertDesc_t deliveryInfo;
    SenderDesc_t senderInfo;
    BasicTempStorage storageInfo;


    debtpage debtInfoPage;
    takeopencell openCellPage;
    advertpage advertInfoPage;
    opencellpage senderOpenPage;

    bool isWorking;
    bool mCardWay;                         //刷卡支付方式
    QStringList cardInfoList;              //卡信息列表
    bool checkGodeCode();
    double expandDouble(double);

    bool getSavePack();
    bool getSenderPack();

    bool getStoragePack();

    bool checkPhoneStyle(QString &);

    void clearInfo();

    void sendNote(DelivertDesc_t savePackInfo);
    void sendNote(SenderDesc_t savePackInfo);
    void sendNote(QString code,int noteNum);

    void sendExceptionNote(DelivertDesc_t savePackInfo);
    void sendExceptionNote(SenderDesc_t savePackInfo);
    void sendExceptionNote(BasicTempStorage savePackInfo);



    
};

#endif // TAKEPACKAGEPAGE_H
