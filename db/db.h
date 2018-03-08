#ifndef _DB_H_
#define _DB_H_

#include "singleton.h"
#include "servercomm.h"
#include "lockdrv.h"
#include <QtSql/QSqlDatabase>

class CDatBase{

public:

    void init();
    //编辑配置信息

    QList<CmdDesc_t> assistTerminalInfo(QList<QRemoteCommandList*> cmd);

    void sendNote(DelivertDesc_t &deliveryInfo);
    void sendNetNote(DelivertDesc_t &deliveryInfo);

    void setWarn(QString status);  //on 开 off 关
    QString getWarn();

    void setServerInfo(QString ip,QString port);
    void getServerInfo(QString &ip,QString &port);

    void setRfidAddr(quint8 addr);
    quint8 getRfidAddr();

    void setComPort(QString com);
    QString getComPort();


    void setWarnAddr(quint8 addr);
    quint8 getWarnAddr();

    void setWarnComPort(QString com);
    QString getWarnComPort();

private:
    CDatBase() {}
    ~CDatBase() {}

    QSqlDatabase db;
    CLockDrv lockoperation;

    DECLARE_SINGLETON_CLASS(CDatBase)

};

typedef Singleton<CDatBase> SCDatBase;


























#endif
