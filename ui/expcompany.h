#ifndef EXPCOMPANY_H
#define EXPCOMPANY_H

#include <QDialog>

#include <QAbstractButton>



namespace Ui {
class expcompany;
}

class expcompany : public QDialog
{
    Q_OBJECT
    
public:
    explicit expcompany(QWidget *parent = 0);
    ~expcompany();

    void initExpCompanyPage();

signals:
    void expInfo_signal(QStringList);

public slots:
    void readExpInfo();
    void buttonclick_slots(QAbstractButton *);
    void okBtn_slots();
private:
    Ui::expcompany *ui;
    QStringList expInfoList;
    QButtonGroup *checkGroup;

};

#endif // EXPCOMPANY_H
