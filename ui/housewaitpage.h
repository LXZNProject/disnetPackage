#ifndef HOUSEWAITPAGE_H
#define HOUSEWAITPAGE_H

#include <QWidget>

#include "lockdrv.h"
#include "t_basic_lan_jian.h"

namespace Ui {
class housewaitpage;
}

class housewaitpage : public QWidget
{
    Q_OBJECT
    
public:
    explicit housewaitpage(QWidget *parent = 0);
    ~housewaitpage();
    void initHouseWaitPage();
    void readWaitInfo(QByteArray param);

    void sendHouseNote(SenderDesc_t savePackInfo);

public slots:
    void overTime();
    void queryLockStatus();
private:
    Ui::housewaitpage *ui;

    qint8 waitSaveTime;
    QTimer isOverTime;
    QTimer queryLock;


    CLockDrv lockoperation;
    SenderDesc_t savePackInfo;       //存件信息

    QStringList notOpenBoxList;         //无法打开的箱门列表

    void commitExceptionInfo(QString notClose = "");
    void commitSavePackageInfo();      //提交存件信息，并修改格门
    void sendExceptionNote(QString cell);

};

#endif // HOUSEWAITPAGE_H
