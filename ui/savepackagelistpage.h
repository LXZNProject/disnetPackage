#ifndef SAVEPACKAGELISTPAGE_H
#define SAVEPACKAGELISTPAGE_H

#include <QWidget>

#include "t_basic_delivery.h"
#include "takeviewmodel.h"
#include <QList>

namespace Ui {
class savepackagelistpage;
}

class savepackagelistpage : public QWidget
{
    Q_OBJECT
    
public:
    explicit savepackagelistpage(QWidget *parent = 0);
    ~savepackagelistpage();

    void initListPage();
    void readListInfo(QByteArray);
    void setExceptionBoxInfo(QStringList card,QStringList open,QStringList check,QString close);    

signals:
    void timeStop();

public slots:
    void okBtn_slots();
    void nextBtn_slots();
private:
    Ui::savepackagelistpage *ui;

    takeViewModel *showModel;
    QList<QStringList> modelData;

    QStringList cardInfoList;           //卡信息列表
    QStringList notOpenBoxList;         //无法打开的箱门列表
    QStringList notCheckBoxList;        //物品检测损坏的的箱门列表
    QString     notCloseBox;

    QList<DelivertDesc_t> savePackInfoList;       //存件信息

    void setBtnEnable(bool);

    void commitSavePackageInfo();      //提交存件信息，并修改格门
    void commitExceptionInfo();        //提交存件异常信息，并修改格门
    void sendNote(DelivertDesc_t savePackInfo);
    void sendNetNote(DelivertDesc_t savePackInfo);
    void sendExceptionNote(QString cell);
    void sendNetExceptionNote(QString cell);

};

#endif // SAVEPACKAGELISTPAGE_H
