#ifndef SENDERWAITPAGE_H
#define SENDERWAITPAGE_H

#include <QWidget>
#include "lockdrv.h"
#include "t_basic_lan_jian.h"

#include <QTimer>

namespace Ui {
class senderwaitpage;
}

class senderwaitpage : public QWidget
{
    Q_OBJECT
    
public:
    explicit senderwaitpage(QWidget *parent = 0);
    ~senderwaitpage();

    void initSenderWaitPage();
    void readWaitInfo(QByteArray param);

    void sendNote(SenderDesc_t savePackInfo);

public slots:
    void overTime();
    void queryLockStatus();
private:
    Ui::senderwaitpage *ui;

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

#endif // SENDERWAITPAGE_H
