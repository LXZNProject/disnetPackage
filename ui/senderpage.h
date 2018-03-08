#ifndef SENDERPAGE_H
#define SENDERPAGE_H

#include <QWidget>
#include "lockdrv.h"

#include "t_basic_lan_jian.h"
#include "keyboard.h"
#include "msgbox.h"

namespace Ui {
class senderpage;

}
class QLabel;

class senderpage : public QWidget
{
    Q_OBJECT
    
public:
    explicit senderpage(QWidget *parent = 0);
    ~senderpage();

    void initSenderPage();
    void readShowInfo(QByteArray param);
signals:
    void resetTime();
public slots:
    void okBtn_slots();
    void quitBtn_slots();

    void buttonclick_slots(int);
    
protected:
    bool event(QEvent *event);
private:
    Ui::senderpage *ui;

    MsgBox myMsgBox;
    keyboard *keypage;
    QLabel *smallBoxLab;
    QLabel *smallTextLab;
    QLabel *smallCheckedLab;

    QLabel *middleBoxLab;
    QLabel *middleTextLab;
    QLabel *middleCheckedLab;

    QLabel *bigBoxLab;
    QLabel *bigTextLab;
    QLabel *bigCheckedLab;

    int mPayCoin;

    BTN_STATUS Cur_Status;             //当前选中状态
    CLockDrv lockoperation;

    SenderDesc_t savePackInfo;                  //寄件信息

    QStringList bigCellList;                    //大号箱体信息
    QStringList middleCellList;                 //中号箱体信息
    QStringList smallCellList;                  //小号箱体信息

    QStringList notOpenBoxList;                 //无法打开的箱门列表

    void setEableEdit(bool status);

    void updateBox();
    void checkBoxStatus(BTN_STATUS);
    bool inputCheck();                         //检测信息是否正确  0：失败 1：成功
    bool openSelectLocks(QStringList*);        //打开箱门，重新选择打开箱门
    void packageData(QByteArray &data);        //打包数据
    void packageConfirmInfo();                 //录入箱体按钮确认信息


};

#endif // SENDERPAGE_H
