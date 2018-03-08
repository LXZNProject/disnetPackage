#ifndef DETAINPAGE_H
#define DETAINPAGE_H

#include <QWidget>
#include "takeviewmodel.h"
#include "lockdrv.h"
#include "msgbox.h"

namespace Ui {
class detainpage;
}

class detainpage : public QWidget
{
    Q_OBJECT
    
public:
    explicit detainpage(QWidget *parent = 0);
    ~detainpage();
    void initPage();
    void readDetainInfo(QByteArray param);
    void sendNote(QStringList list);

signals:
    void resetTime();
public slots:
    void openBtn_slots();
    void quitBtn_slots();
    
private:
    Ui::detainpage *ui;

    MsgBox myMsgBox;
    QMap<QString,QString> sendInfo;
    CLockDrv lockoperation ;
    QList<QStringList> modelData;
    takeViewModel *model;
};

#endif // DETAINPAGE_H
