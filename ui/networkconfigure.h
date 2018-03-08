#ifndef NETWORKCONFIGURE_H
#define NETWORKCONFIGURE_H

#include <QWidget>
#include "msgbox.h"
#include "syncTerminalDataRet.h"
namespace Ui {
class networkConfigure;
}

class networkConfigure : public QWidget
{
    Q_OBJECT
    
public:
    explicit networkConfigure(QWidget *parent = 0);
    ~networkConfigure();

    void readInfo();
    void startUpdateTerminalInfo(syncTerminalDataRet &);

private:
    void initPage();
    void hidden(bool type);
    void writeResolvFile();
    bool checkInfo();
    void setBtnEnable(bool status);

public slots:
    void updateTerminalInfo();
    void checkVersionInfo();
private slots:
    void comboBoxChanged_Slots(int );
    void okBtn_slots();
    void quitBtn_slots();
    
private:
    Ui::networkConfigure *ui;
    MsgBox              myMsgBox;
    int                       myindex;
};

#endif // NETWORKCONFIGURE_H
