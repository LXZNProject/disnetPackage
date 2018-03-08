#ifndef MSGBOX_H
#define MSGBOX_H

#include <QDialog>
#include <QTimer>
#include <QTextCodec>

namespace Ui {
class msgbox;
}

class MsgBox : public QDialog
{
    Q_OBJECT
    
public:
    explicit MsgBox(QWidget *parent = 0);
    ~MsgBox();

    void setCheckTimer(qint8 time);
    void showMsgBox(QString text,bool code = true,qint8 btnNum = 1,Qt::WindowModality model = Qt::WindowModal);

signals:
    void msgBox_Ok();
    void msgBox_Cancel();
    void msgBox_Close();

public slots:
    void stopTimer();
private:
    Ui::msgbox *ui;

    QTimer timer;
    QTextCodec *tc;

    void initMsgBoxPage();
};

#endif // MSGBOX_H
