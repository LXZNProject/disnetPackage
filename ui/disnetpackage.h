#ifndef DISNETPACKAGE_H
#define DISNETPACKAGE_H

#include <QWidget>
#include "mainctrl.h"

#include "msgbox.h"
#include "rechargedialog.h"
namespace Ui {
class disnetPackage;
}

class disnetPackage : public QWidget
{
    Q_OBJECT
    
public:
    explicit disnetPackage(QWidget *parent = 0);
    ~disnetPackage();
    void getReadCardInfo(QStringList dataList);
    void startRetHomePage(bool value);

private:
    void initHomePage();
    void btnEnable(bool type);
    bool checkCardInfo(QString companyNumber);

    bool checkTempRecordInfo(QStringList&);

    bool clickReadCard();
    
public slots:
    void tempStorageBtn_slots();
    void saveBtn_slots();
    void senderBtn_slots();
    void register_slots();
    void takeBtn_slots();
    void rechargeBtn_slots();
    void balanceBtn_slots();
    void balanceHistoryBtn_slots();
    void manageBtn_slots();
    void emptyCellBtn_slots();
    void cellSetBtn_slots();
    void noteSetBtn_slots();
    void basicSetBtn_slots();
    void houseBtn_slots();
    void assistBtn_slots();
    void networkConfigBtn_slots();
    void checkDeviceBtn_slots();
    void tariffBtn_slots();
    void modifyInfoBtn_slots();
    void exptakeBtn_slots();

    void retHomePage_slots();
    void tabPageChange_slots(int);

    bool readCard_slots();
    void detainPage_slots();
    void detainMsg_cancel_slots();

private:
    Ui::disnetPackage *ui;
    QStringList mCardInfoList;
    STATUS               myStatus;
    MsgBox myMsgBox;
    MsgBox cardMsgBox;
    MsgBox detainMsgBox;
    QTimer homeTimer;
    rechargeDialog  mRechargeDialog;
};

#endif // DISNETPACKAGE_H
