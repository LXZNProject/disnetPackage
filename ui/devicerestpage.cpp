#include "devicerestpage.h"
#include "ui_devicerestpage.h"


#include "mainctrl.h"
#include <QBitmap>




devicerestpage::devicerestpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::devicerestpage)
{
    ui->setupUi(this);
    initPage();
}

devicerestpage::~devicerestpage()
{
    delete ui;
}


void devicerestpage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);
    this->resize(800,600);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPixmap pixmap(":/device/images/device/device.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);

}

