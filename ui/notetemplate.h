#ifndef NOTETEMPLATE_H
#define NOTETEMPLATE_H

#include <QWidget>
#include "msgbox.h"

class QTextEdit;
class QPushButton;

namespace Ui {
class notetemplate;
}

class notetemplate : public QWidget
{
    Q_OBJECT
    
public:
    explicit notetemplate(QWidget *parent = 0);
    ~notetemplate();

    void initPage();
    void readPageInfo();
    void analyNoteInfo(QTextEdit*,QPushButton*,int i);
    void groupNoteInfo(QTextEdit*,QPushButton*,int i);
    void setBtnEnable(bool);

public slots:
    void okBtn_slots();
    void quitBtn_slots();
    void tabPageChange_slots(int );

protected:
    bool eventFilter(QObject *, QEvent *);
private:
    Ui::notetemplate *ui;
    QTextCodec *tc;
    MsgBox myMsgBox;
};

#endif // NOTETEMPLATE_H
