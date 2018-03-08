#ifndef ADDCABINETWIDGET_H
#define ADDCABINETWIDGET_H

#include <QWidget>
#include "msgbox.h"
#include "keyboard.h"
namespace Ui {
class addCabinetWidget;
}

class addCabinetWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit addCabinetWidget(QWidget *parent = 0);
    ~addCabinetWidget();

private:
    bool checkCabinetNumber();

signals:
    void addSignal();

private slots:

    void okBtnClicked();
    void cancelBtnClicked();
    
private:
    Ui::addCabinetWidget *ui;
    QString mycabinetIDStr;

    MsgBox myMsgBox;
};

#endif // ADDCABINETWIDGET_H
