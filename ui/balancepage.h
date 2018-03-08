#ifndef BALANCEPAGE_H
#define BALANCEPAGE_H

#include <QWidget>
#include "msgbox.h"
namespace Ui {
class balancePage;
}

class balancePage : public QWidget
{
    Q_OBJECT
    
public:
    explicit balancePage(QWidget *parent = 0);
    ~balancePage();

    void initPage();
    void readInfo();

    void timeStart();
    void timeStop();

public slots:
    void queryBtn_slots();
    void quitBtn_slots();

    void readCard_slots();

private:
        void clearInfo();
        void dealReadCardInfo(QStringList myReadDataList);
        void setBtnStatus(bool type);

        void hideLable(bool value);


    
private:
    Ui::balancePage *ui;
    QTimer          *openQueryTimer;
    QTextCodec *tc;
    MsgBox myMsgBox;
};

#endif // BALANCEPAGE_H
