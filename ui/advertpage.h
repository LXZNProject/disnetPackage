#ifndef ADVERTPAGE_H
#define ADVERTPAGE_H

#include <QWidget>
#include <QTimer>
namespace Ui {
class advertpage;
}

class advertpage : public QWidget
{
    Q_OBJECT
    
public:
    explicit advertpage(QWidget *parent = 0);
    ~advertpage();

    void initPage();
    void advPageShow();
signals:
    void advPageClose();
private:
    Ui::advertpage *ui;
    QTimer timer;
};

#endif // ADVERTPAGE_H
