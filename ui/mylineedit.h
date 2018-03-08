#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include <QTimer>
#include "keyboard.h"

class CKeyBoardLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CKeyBoardLineEdit(QWidget * parent = 0);
    ~CKeyBoardLineEdit();

    void bindKeyPage(keyboard *page);

signals:
    void btnPress();
    void okBtnPress();
public slots:
    void btn0slot();
    void btn1slot();
    void btn2slot();
    void btn3slot();
    void btn4slot();
    void btn5slot();
    void btn6slot();
    void btn7slot();
    void btn8slot();
    void btn9slot();
    void btnXslot();
    void btnspaceslot();
    void btnwellslot();
    void backbtnslot();
    void okbtnslot();
    void cancelbtnslot();

protected:
    void keyPressEvent(QKeyEvent *);
    void focusInEvent(QFocusEvent *);
    void focusOutEvent(QFocusEvent *);


private:
    keyboard *mykeyboard;
    bool recv;
};

#endif // MYLINEEDIT_H
