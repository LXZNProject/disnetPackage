#ifndef STATUS_H
#define STATUS_H


#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QTimer>

typedef enum{
    ST_NONE = 0x00,
    ST_HOME,                 //1、主页面
    ST_MANAGE,               //2、系统维修页面
    ST_PAKEAGE_CONFIRM,      //3、存件确认页面
    ST_SAVE_DELIVERY_MENU,   //4、存件主页面
    ST_SAVE_DELIVERY_WAIT,   //5、存件等待页面
    ST_SAVE_DELIVERY_LIST,   //6、存件列表页面
    ST_GET_DELIVERY,         //7、取件页面
    ST_RECHARGE,             //8、充值
    ST_RECHARGE_QUERY,             //9、充值查询
    ST_RECHARGE_DIAG,             //10、充值查询中间界面
    ST_EXCEPTION_OPENDOOR,   //11、异常处理
    ST_OPENEMPTYDOOR,   //12、打开空箱门
    ST_ASSIST,               //13、远程协助
    ST_QUERY_BALANCE,        //14、余额查询
    ST_DETAIN_PACK,         //15、快递员取滞留件
    ST_COURIER_PACK,     //16、快递员自助取件
    ST_TEMP_STORAGE,       //17、临时存储
    ST_TEMP_STORAGE_WAIT,   //18、临时存储等待
    ST_REGISTER,                //19、注册
    ST_SENDER_PACK_MENU,        //20、寄件
    ST_SENDER_PACK_WAIT,
    ST_HOUSE_SERVICE,       //22、社区服务
    ST_HOUSE_PACK_MENU,    //23、干洗服务
    ST_HOUSE_PACK_WAIT,
    ST_CABINET_CELL_SET,     //25、箱格设置
    ST_NOTE_SET,     //26、短信设置
    ST_BASIC_SET,    //27、基础信息设置
    ST_NETWORK_SET,    //28、网络配置
    ST_TARIFF,    //29、资费查询
    ST_MODIFY_INFO,    //30、资费查询
    ST_CHECK_DEVICE,    //31、设备检测
    ST_DEVICE_SERVICE          //32、设备维护中

}STATUS;

#define DEFAULT_TIMER_INTERVAL 120*1000
class CStatus : public QObject
{
    Q_OBJECT

public:
    CStatus(STATUS status) : mStatus(status)
    {
        timer.setInterval(DEFAULT_TIMER_INTERVAL);
        timer.setSingleShot(true);

        QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    }

    virtual void enter(QByteArray param) = 0;
    virtual void leave(QByteArray param) = 0;

    virtual void keyInputProc(QByteArray param) {qDebug()<<param;}
    virtual void cardInputProc(QByteArray param) {qDebug()<<param;}
    virtual void coinInputProc(QByteArray param) {qDebug()<<param;}
    virtual void upsInputProc(QByteArray param) {qDebug()<<param;}


    STATUS status()
    {
        return mStatus;
    }

    void openTimer()
    {
        //qDebug() << "start timer";
        timer.start();
    }

    void clearTimer()
    {
        timer.setInterval(DEFAULT_TIMER_INTERVAL);
    }

    void stopTimer()
    {
        timer.stop();
    }
/*
    void setTimeOutStatus(STATUS staus, QString defaulParam="")
    {
        timoutStatus = staus;
        mDefaultParam = defaulParam;
    }
*/
signals:
    void jumpPage_signal();

public slots:
    void timeOut()
    {
        //qDebug() << "end timer";
        emit jumpPage_signal();
        //SCMainCtrl::instance()->enterStatus(timoutStatus,"");
    }

private:
    STATUS  mStatus;    //当前页面标识
    QTimer  timer;
    STATUS  timoutStatus;
    QString mDefaultParam;
};











#endif // PAGE_H
