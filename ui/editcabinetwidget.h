#ifndef EDITCABINETWIDGET_H
#define EDITCABINETWIDGET_H

#include <QWidget>
#include "t_cabinet_cell.h"
#include "msgbox.h"
namespace Ui {
class editCabinetWidget;
}

class editCabinetWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit editCabinetWidget(QWidget *parent = 0);
    ~editCabinetWidget();

    void readInfo(basicCell tempvalue);

private slots:
    void okBtnClicked();
    void cancelClicked();

signals:
    void editOKSignal();

private:
    Ui::editCabinetWidget *ui;
    basicCell     mybasiCell;
    MsgBox   myMsgBox;
};

#endif // EDITCABINETWIDGET_H
