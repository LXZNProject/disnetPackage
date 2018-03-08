#ifndef MANAGEPAGE_H
#define MANAGEPAGE_H

#include <QWidget>
#include "lockdrv.h"
#include "msgbox.h"
class notopenexception;
namespace Ui {
class managePage;
}

class managePage : public QWidget
{
    Q_OBJECT
    
public:
    explicit managePage(QWidget *parent = 0);
    ~managePage();

    void initPage();
signals:
    void resetTime();
public slots:
    void openCellBtnClicked();
    void cancelBtnClicked();
    void notOpenBtn_slots();

private:
    Ui::managePage *ui;
    CLockDrv lockoperation;
    notopenexception *notopenpage;
    MsgBox   myMsgBox;
};

#endif // MANAGEPAGE_H
