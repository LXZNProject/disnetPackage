#ifndef RECHAGEHISTORYPAGE_H
#define RECHAGEHISTORYPAGE_H

#include <QWidget>
#include "takeviewmodel.h"
#include "t_basic_stored_record.h"
#include "msgbox.h"
#include "keyboard.h"
namespace Ui {
class rechageHistoryPage;
}

class rechageHistoryPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit rechageHistoryPage(QWidget *parent = 0);
    ~rechageHistoryPage();

 private :
    void initPage();

private slots:
    void queryBtn_slots();
    void cancelBtn_slots();

private:
    Ui::rechageHistoryPage *ui;
    takeViewModel        * m_model;
    QList<BasicStoredRecord> myhistoryList;
    MsgBox       myMsgbox;
};

#endif // RECHAGEHISTORYPAGE_H
