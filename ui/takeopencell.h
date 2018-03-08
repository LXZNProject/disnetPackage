#ifndef TAKEOPENCELL_H
#define TAKEOPENCELL_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class takeopencell;
}

class takeopencell : public QDialog
{
    Q_OBJECT
    
public:
    explicit takeopencell(QWidget *parent = 0);
    ~takeopencell();

    void initOpenCellPage();
    void showOpneCell(QString);

signals:
    void disOpenCell();
public slots:
    void timerOver();

private:
    Ui::takeopencell *ui;
    QTimer timer;
};

#endif // TAKEOPENCELL_H
