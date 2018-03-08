#include "advertpage.h"
#include "ui_advertpage.h"
#include <QBitmap>

advertpage::advertpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::advertpage)
{
    ui->setupUi(this);

    initPage();
}

advertpage::~advertpage()
{
    delete ui;
}

void advertpage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);

    QPixmap pixmap("./images/advertiser.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);

    timer.setInterval(1*1000);
    timer.setSingleShot(true);


    connect(&timer,SIGNAL(timeout()),this,SIGNAL(advPageClose()));
}

void advertpage::advPageShow()
{
    timer.start();
    this->show();
}
