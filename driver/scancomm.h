#ifndef SCANCOMM_H
#define SCANCOMM_H

#include <QObject>
#include "qextserialport.h"
#include "singleton.h"
#include <QTimer>

class scancomm : public QObject
{
    Q_OBJECT
public:
    explicit scancomm(QObject *parent = 0);
    void init();
    void openMyCom();
    void readMyScanCom();
    
signals:
    void scanSignal(QString);
    
public slots:
    void closeReadCom();
    void readMyCom();
private:
    QString comName;
    QextSerialPort *myCom;
    QTimer *timer;

    DECLARE_SINGLETON_CLASS(scancomm)
    
};

typedef Singleton<scancomm> SCScanComm;

#endif // SCANCOMM_H
