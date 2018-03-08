#ifndef RECHARGE_H
#define RECHARGE_H

#include <QWidget>
#include "keyboard.h"
#include "msgbox.h"

namespace Ui {
class recharge;
}

class recharge : public QWidget
{
    Q_OBJECT
    
public:
    explicit recharge(QWidget *parent = 0);
    ~recharge();

    void initPage();
    void readInfor(QByteArray param);

    void timeStart();
    void timeStop();

signals:
    void resetTime();
public slots:
    void okBtn_slots();
    void quitBtn_slots();

    void readCard_slots();

    void myok();

private:
        bool checkInfo();
        void clearInfo();
        void dealReadCardInfo(QStringList myReadDataList);

        void btnEnable(bool value);
    
private:
        Ui::recharge *ui;

        MsgBox myMsgBox;
        MsgBox *isSureRecharge;
        QTimer          *openQueryTimer;
        QStringList    m_readDataList; // 读到卡的信息
        QStringList    cardInfoList;           //卡信息列表
        QStringList    rechargeCardInfoList;           //充值卡信息列表
        QStringList    InvalidCardInfor;           //充值卡信息列表
        keyboard *keypage;
        QTextCodec *tc;
        int                  rechargeType;
};

#endif // RECHARGE_H
