#ifndef SAVEPACKAGE_H
#define SAVEPACKAGE_H

#include <QWidget>
#include "lockdrv.h"
#include "t_basic_delivery.h"
#include "t_basic_cell_order.h"
#include "keyboard.h"
#include "msgbox.h"

class QLabel;
namespace Ui {
class savepackage;
}

class savepackage : public QWidget
{
    Q_OBJECT
    
public:
    explicit savepackage(QWidget *parent = 0);
    ~savepackage();

    void initSavePage();
    void readShowInfo(QByteArray param);

    bool loadcodeScan();
signals:
    void resetTime();

public slots:
    void okBtn_slots();
    void quitBtn_slots();
    void buttonclick_slots(int);
    void codeSCan_slots();
    void startLoadScan(QString param);
protected:
    bool eventFilter(QObject *, QEvent *);
private:
    Ui::savepackage *ui;

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

    BTN_STATUS Cur_Status;             //当前选中状态
    CLockDrv lockoperation;
    DelivertDesc_t savePackInfo;       //存件信息
    QList<userOrder> cellInfos;        //用户承包的箱格信息

    QStringList bigCellList;                    //大号箱体信息
    QStringList middleCellList;                 //中号箱体信息
    QStringList smallCellList;                  //小号箱体信息

    QStringList expOrderBigCellList;               //快递员承包大号箱体信息
    QStringList expOrderMiddleCellList;            //快递员承包中号箱体信息
    QStringList expOrderSmallCellList;             //快递员承包小号箱体信息

    QStringList userOrderBigList;                  //用户承包大号箱体信息
    QStringList userOrderMiddleList;               //用户承包中号箱体信息
    QStringList userOrderSmallList;                //用户承包小号箱体信息

    QStringList tempBigCellList;                    //过滤后大号箱体信息
    QStringList tempMiddleCellList;                 //过滤后中号箱体信息
    QStringList tempSmallCellList;                  //过滤后小号箱体信息

    QStringList cardInfoList;           //卡信息列表
    QStringList notOpenBoxList;         //无法打开的箱门列表
    QStringList notCheckBoxList;        //物品检测损坏的的箱门列表

    //double mBalance;                     //卡的余额

    void initList(QByteArray param);
    void setEableEdit(bool);

    void updateBox();
    void checkBoxStatus(BTN_STATUS);
    bool inputCheck();                         //检测信息是否正确  0：失败 1：成功
    bool openSelectLocks(QStringList*,QStringList*);        //打开箱门，重新选择打开箱门,维护两张格子表
    void packageData(QByteArray &data);        //打包数据
    void packageConfirmInfo();                 //录入箱体按钮确认信息
    void checkIsUserOrder(QString phone);
};

#endif // SAVEPACKAGE_H
