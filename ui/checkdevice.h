#ifndef CHECKDEVICE_H
#define CHECKDEVICE_H

#include <QWidget>
#include "msgbox.h"
#include "lockdrv.h"

namespace Ui {
class checkDevice;
}

class checkDevice : public QWidget
{
    Q_OBJECT
    
public:
    explicit checkDevice(QWidget *parent = 0);
    ~checkDevice();
    void readInfo();

private:
    void initPage();
    void clearInfo();
    void scan();

    void setBtnEnable(bool value);
signals:
    void resetTime();
public slots:
    void cabinetBtn_slots();
    void cellBtn_slots();
    void networkBtn_slots();
    void timerBtn_slots();
    void quitBtn_slots();
    void readCardBtn_slots();
    void checkScan(QString param);

protected:
    bool event(QEvent *);
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::checkDevice *ui;
    MsgBox myMsgBox;
    CLockDrv lockoperation;
    QTextCodec *tc;
};

#endif // CHECKDEVICE_H
