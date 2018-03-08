#ifndef RECHARGEDIALOG_H
#define RECHARGEDIALOG_H

#include <QDialog>
#include "msgbox.h"

namespace Ui {
class rechargeDialog;
}

class rechargeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit rechargeDialog(QWidget *parent = 0);
    ~rechargeDialog();
private:
    void initPage();
    bool readCard();
    void getReadCardInfo(QStringList dataList);
    bool checkCardInfo(QString companyNumber);
    void btnEnable(bool value);

public slots:
    void  localBtn_slots();
    void  cardBtn_slots();
    void  quitBtn_slots();
    
private:
    Ui::rechargeDialog *ui;
    MsgBox        myMsgBox;
};

#endif // RECHARGEDIALOG_H
