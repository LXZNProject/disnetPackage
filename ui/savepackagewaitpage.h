#ifndef SAVEPACKAGEWAITPAGE_H
#define SAVEPACKAGEWAITPAGE_H

#include <QWidget>
#include <QTimer>
#include "t_basic_delivery.h"
#include "lockdrv.h"
#include "msgbox.h"

namespace Ui {
class savepackagewaitpage;
}

class savepackagewaitpage : public QWidget
{
    Q_OBJECT
    
public:
    explicit savepackagewaitpage(QWidget *parent = 0);
    ~savepackagewaitpage();

    void initWaitpage();
    void readWaitInfo(QByteArray param);

public slots:
    void okBtn_slots();
    void cancelBtn_slots();

    void overTime();
    void queryLockStatus();
    
private:
    Ui::savepackagewaitpage *ui;

    MsgBox myMsgBox;
    bool isOpenBox;
    qint8 waitSaveTime;
    QTimer isOverTime;
    QTimer queryLock;
    QString notCloseBox;


    CLockDrv lockoperation;
    DelivertDesc_t savePackInfo;       //存件信息

    QStringList cardInfoList;           //卡信息列表
    QStringList notOpenBoxList;         //无法打开的箱门列表
    QStringList notCheckBoxList;        //物品检测损坏的的箱门列表

    void setBtnEnable(bool status, qint8 btn = 3); // btn = 1 代表确认按钮 btn = 2 代表取消按钮
};

#endif // SAVEPACKAGEWAITPAGE_H
