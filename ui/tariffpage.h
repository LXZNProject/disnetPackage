#ifndef TARIFFPAGE_H
#define TARIFFPAGE_H

#include <QWidget>

namespace Ui {
class tariffPage;
}

class tariffPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit tariffPage(QWidget *parent = 0);
    ~tariffPage();
    void initPage();
    void readInfo();
signals:
    void resetTime();
public slots:
    void quitBtn_slots();
    void tabPageChange_slots(int);
    QString getNameFrom();
    
private:
    Ui::tariffPage *ui;
    QTextCodec *tc;
};

#endif // TARIFFPAGE_H
