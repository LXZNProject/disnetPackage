#ifndef SERVERPAGE_H
#define SERVERPAGE_H

#include <QWidget>
#include "msgbox.h"

namespace Ui {
class serverpage;
}

class serverpage : public QWidget
{
    Q_OBJECT
    
public:
    explicit serverpage(QWidget *parent = 0);
    ~serverpage();

    void initServicePage();

public slots:
    void dryBtn_slots();
    void quitBtn_slots();
private:
    Ui::serverpage *ui;
    MsgBox myMsgBox;
};

#endif // SERVERPAGE_H
