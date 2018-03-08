#ifndef EXPTAKEPAGE_H
#define EXPTAKEPAGE_H

#include <QWidget>
#include "takeviewmodel.h"
#include "btndelegate.h"
#include "msgbox.h"
#include "lockdrv.h"
#include "t_log_network.h"

namespace Ui {
class exptakepage;
}

class exptakepage : public QWidget
{
    Q_OBJECT
    
public:
    explicit exptakepage(QWidget *parent = 0);
    ~exptakepage();

    void initPage();
    void readExpTakePageInfo(QByteArray param);

    void sendNote(QStringList list);
signals:
    void resetTime();
public slots:
    void quitBtn_slots();
    void readlyOpen_slots(QModelIndex index);
    void openCell_slots();
    
private:
    Ui::exptakepage *ui;

    QModelIndex mIndex;
    CLockDrv lockoperation ;
    QList<NetCellExcept> exceInfoList;
    QMap<QString,QString> sendInfo;
    QList<QStringList> modelData;
    takeViewModel *model;
    BtnDelegate *btnDelegate;
    MsgBox myMsgBox;
    MsgBox confirmMsgBox;
};

#endif // EXPTAKEPAGE_H
