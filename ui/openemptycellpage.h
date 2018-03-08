#ifndef OPENEMPTYCELLPAGE_H
#define OPENEMPTYCELLPAGE_H

#include <QWidget>
#include "lockdrv.h"
#include "msgbox.h"
namespace Ui {
class openEmptyCellPage;
}

class openEmptyCellPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit openEmptyCellPage(QWidget *parent = 0);
    ~openEmptyCellPage();

    void initPage();

signals:
    void resetTime();
public slots:

    void openBtnClicked();
    void cancelBtnClicked();

 private:
    bool filterCell(QString openCell);
    
private:
    Ui::openEmptyCellPage *ui;
    CLockDrv lockoperation;
    MsgBox    myMsgBox;
};

#endif // OPENEMPTYCELLPAGE_H
