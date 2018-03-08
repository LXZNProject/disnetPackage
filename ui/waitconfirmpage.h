#ifndef WAITCONFIRMPAGE_H
#define WAITCONFIRMPAGE_H

#include <QDialog>
#include <QStringList>
#include "t_basic_delivery.h"
#include "msgbox.h"

namespace Ui {
    class waitconfirmpage;
}

class waitconfirmpage : public QDialog {
    Q_OBJECT
public:
    waitconfirmpage(QWidget *parent = 0);
    ~waitconfirmpage();

    void readwaitconfirminfo(QByteArray);
    void sendNote(DelivertDesc_t &deliveryInfo);
    void sendNetNote(DelivertDesc_t &deliveryInfo);

signals:
    void resetTime();

public slots:
    void okBtn_slot();
    void cancelBtn_slot();
    void confirmPack_slot();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::waitconfirmpage *ui;

    MsgBox myMsgBox;
    MsgBox confirmMsgBox;
    QStringList cardInfoList;
    DelivertDesc_t delivery;

    void initPage();


};

#endif // WAITCONFIRMPAGE_H
