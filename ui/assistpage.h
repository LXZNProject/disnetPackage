#ifndef ASSISTPAGE_H
#define ASSISTPAGE_H

#include <QWidget>
#include "msgbox.h"

#include <QProgressDialog>

namespace Ui {
class assistpage;
}

class assistpage : public QWidget
{
    Q_OBJECT
    
public:
    explicit assistpage(QWidget *parent = 0);
    ~assistpage();
    void initPage();

    void setBtnEnable(bool status);
signals:
    void resetTime();
public slots:
    void startBtn_slots();
    void quitBtn_slots();
    
private:
    Ui::assistpage *ui;

    MsgBox myMsgBox;
    QProgressDialog progress;
};

#endif // ASSISTPAGE_H
