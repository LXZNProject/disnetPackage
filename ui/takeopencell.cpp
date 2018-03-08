#include "takeopencell.h"
#include "ui_takeopencell.h"
#include <QDesktopWidget>

takeopencell::takeopencell(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::takeopencell)
{
    ui->setupUi(this);
    initOpenCellPage();
}

takeopencell::~takeopencell()
{
    delete ui;
}

void takeopencell::initOpenCellPage()
{
    timer.setInterval(5 * 1000);
    timer.setSingleShot(true);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(350,250);

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/msgbox/images/msgBox/msgbox_bg.png")));
    this->setPalette(palette);
    this->move((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);

    ui->door->resize(QPixmap(":/save/images/saveWait/door.png").size());
    ui->door->setPixmap(QPixmap(":/save/images/saveWait/door.png"));

    ui->doorId->setStyleSheet("font-family:wenquanyi; font-size:30px; color: rgb(0, 255, 64)");

    ui->doorCabinet->resize(QPixmap(":/save/images/saveWait/doorcabinet.png").size());
    ui->doorCabinet->setPixmap(QPixmap(":/save/images/saveWait/doorcabinet.png"));


    ui->doorCellId->setStyleSheet("font-family:wenquanyi; font-size:30px; color: rgb(0, 255, 64)");

    ui->doorCell->resize(QPixmap(":/save/images/saveWait/doorcell.png").size());
    ui->doorCell->setPixmap(QPixmap(":/save/images/saveWait/doorcell.png"));

    connect(&timer,SIGNAL(timeout()),this,SLOT(timerOver()));

}

void takeopencell::showOpneCell(QString data)
{

    ui->doorId->setText(data.left(2));
    ui->doorCellId->setText(data.right(2));
    timer.start();
    this->exec();
}

void takeopencell::timerOver()
{
    emit disOpenCell();
    this->close();
}
