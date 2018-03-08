#ifndef NOTOPENEXCEPTION_H
#define NOTOPENEXCEPTION_H

#include <QWidget>
#include <QList>
#include "takeviewmodel.h"
#include "btndelegate.h"
#include "msgbox.h"

namespace Ui {
class notopenexception;
}

class notopenexception : public QWidget
{
    Q_OBJECT
    
public:
    explicit notopenexception(QWidget *parent = 0);
    ~notopenexception();

    void initNotOpenExceptionPage();

    void readNotOpenInfo();

public slots:
    void confirmRepair_slots(QModelIndex);
    void repairCell_slots();
    void quitBtn_slots();
private:
    Ui::notopenexception *ui;

    MsgBox myMsgBox;
    MsgBox confirmMsgBox;
    QModelIndex mIndex;
    QList<QStringList> modelData;
    takeViewModel *model;
    BtnDelegate *btnDelegate;

    void sendRepairNote(qint8 type,QString cell);
};

#endif // NOTOPENEXCEPTION_H
