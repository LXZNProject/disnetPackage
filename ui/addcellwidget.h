#ifndef ADDCELLWIDGET_H
#define ADDCELLWIDGET_H

#include <QWidget>
#include "msgbox.h"
#include "keyboard.h"
namespace Ui {
class addCellWidget;
}

class addCellWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit addCellWidget(QWidget *parent = 0);
    ~addCellWidget();
    void initPage();
    void readInfo(QString cabinetStr);
    
private slots:
    bool cellNumLineEditChange();
    void okBtnClick();
    void cancelBtnClick();

signals:
    void addCellOK();
private:
    Ui::addCellWidget *ui;

    MsgBox myMsgBox;
    QString  mycabinetStr;
    QString mycellId;
    bool  checkResult;
};

#endif // ADDCELLWIDGET_H
