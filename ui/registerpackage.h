#ifndef REGISTERPACKAGE_H
#define REGISTERPACKAGE_H

#include <QWidget>
#include "msgbox.h"
#include "keyboard.h"

namespace Ui {
class registerPackage;
}

class registerPackage : public QWidget
{
    Q_OBJECT
    
public:
    explicit registerPackage(QWidget *parent = 0);
    ~registerPackage();
    void initPage();
    void readInfo(QByteArray param);

public slots:
    void okBtn_slots();
    void quitBtn_slots();

private:
        bool checkInfo();
        void clearInfo();
        int getIndexFromString();
        bool readCard();

signals:
    void resetTime();
private slots:
    void typeComboBoxChanged(QString);
    void myok();

private:
    Ui::registerPackage *ui;
    QTextCodec *tc;
    QString         typeControl;
    QString         myCardNumStr;
    MsgBox        *myMsgBox;
    MsgBox        *waringMsgBox;
    keyboard     *keypage;
    QStringList  cardInfoList;
    QStringList  readCardInfoList;
};

#endif // REGISTERPACKAGE_H
