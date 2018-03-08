#ifndef MODIFYINFOPAGE_H
#define MODIFYINFOPAGE_H

#include <QWidget>
#include "msgbox.h"
#include "keyboard.h"
namespace Ui {
class modifyINfoPage;
}

class modifyINfoPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit modifyINfoPage(QWidget *parent = 0);
    ~modifyINfoPage();

    void timeStart();
    
private:
    void initPage();

    void dealReadCardInfo(QStringList valueList);
    void clearInfo();
    void setBtnEnable(bool status);

    QString getStringFromIndex(QString indexStr);

    bool checkInfo();
signals:
    void resetTime();
public slots:
    void okBtn_slots();
    void quitBtn_slots();

    void readCard_slots();

private:
    Ui::modifyINfoPage *ui;
    MsgBox myMsgBox;
    QTimer          *openQueryTimer;
    QStringList    mReadDataList;
    QTextCodec *tc;
    int      userType;
};

#endif // MODIFYINFOPAGE_H
