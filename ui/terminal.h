#ifndef TERMINAL_H
#define TERMINAL_H

#include <QWidget>
#include "expcompany.h"
#include "msgbox.h"
#include <QTextCodec>

namespace Ui {
class terminal;
}

class terminal : public QWidget
{
    Q_OBJECT
    
public:
    explicit terminal(QWidget *parent = 0);
    ~terminal();

    void initPage();
    void readTerminalInfo(QByteArray param);
    void setBtnEnable(bool);
    void rewrite();

public slots:
    void okBtn_slots();
    void quitBtn_slots();
    void goodsCheckBtn_slots();
    void washSwitchBtn_slots();
    void acceptExpCompanyNameBtn_slots(QStringList);
    void tabPageChange_slots(int);
    
private:
    Ui::terminal *ui;

    expcompany *exppage;
    MsgBox myMsgBox;
    QTextCodec *tc;
};

#endif // TERMINAL_H
