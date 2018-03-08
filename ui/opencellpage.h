#ifndef OPENCELLPAGE_H
#define OPENCELLPAGE_H

#include <QDialog>
#include <QTimer>
#include "t_basic_lan_jian.h"

namespace Ui {
class opencellpage;
}

class opencellpage : public QDialog
{
    Q_OBJECT
    
public:
    explicit opencellpage(QWidget *parent = 0);
    ~opencellpage();
    void initPage();
    void showOpneCell(SenderDesc_t senderInfo);
signals:
    void disOpenCell();
public slots:
    void timerOver();
    void okBtn_slots();
private:
    Ui::opencellpage *ui;
    QTimer timer;
};

#endif // OPENCELLPAGE_H
