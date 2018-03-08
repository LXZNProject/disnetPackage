#ifndef TEMPSTORAGEWAIT_H
#define TEMPSTORAGEWAIT_H

#include <QWidget>
#include "lockdrv.h"
#include "t_basic_temp_storage.h"
namespace Ui {
class tempStorageWait;
}

class tempStorageWait : public QWidget
{
    Q_OBJECT
    
public:
    explicit tempStorageWait(QWidget *parent = 0);
    ~tempStorageWait();

    void initStorageWaitPage();
    void readWaitInfo(QByteArray param);

    void sendStorageNote(BasicTempStorage savePackInfo);

public slots:
    void overTime();
    void queryLockStatus();

private:
    Ui::tempStorageWait *ui;

    qint8 waitSaveTime;
    QTimer isOverTime;
    QTimer queryLock;


    CLockDrv lockoperation;
    BasicTempStorage savePackInfo;       //存件信息

    QStringList notOpenBoxList;         //无法打开的箱门列表
    QStringList notCheckBoxList;        //物品检测损坏的的箱门列表

    void commitExceptionInfo(QString notClose = "");
    void commitSavePackageInfo();      //提交存件信息，并修改格门
    void sendExceptionNote(QString cell);

};

#endif // TEMPSTORAGEWAIT_H
