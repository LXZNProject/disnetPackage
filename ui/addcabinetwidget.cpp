#include "addcabinetwidget.h"
#include "ui_addcabinetwidget.h"
#include "t_cabinet_cell.h"
#include <QBitmap>
#include <QDesktopWidget>
addCabinetWidget::addCabinetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addCabinetWidget)
{
    ui->setupUi(this);
    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtnClicked()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtnClicked()));

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/cell/images/cell/background.png")));
    this->setPalette(palette);


    ui->label->resize(QPixmap(":/cell/images/cell/cabinetNum.png").size());
    ui->label->setPixmap(QPixmap(":/cell/images/cell/cabinetNum.png"));
    ui->label_2->resize(QPixmap(":/cell/images/cell/cabinetType.png").size());
    ui->label_2->setPixmap(QPixmap(":/cell/images/cell/cabinetType.png"));

    ui->okBtn->setFlat(true);
    ui->okBtn->setFocusPolicy(Qt::NoFocus);
    ui->okBtn->setIcon(QIcon(":/public/images/public/okBtn_bg.png"));
    ui->okBtn->setMinimumSize(QPixmap(":/public/images/public/okBtn_bg.png").size());
    ui->okBtn->setIconSize(ui->okBtn->size());
    ui->okBtn->setStyleSheet("background-color:transparent");


    ui->cancelBtn->setFlat(true);
    ui->cancelBtn->setFocusPolicy(Qt::NoFocus);
    ui->cancelBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->cancelBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->cancelBtn->setIconSize(ui->cancelBtn->size());
    ui->cancelBtn->setStyleSheet("background-color:transparent");

    this->move((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);

}

addCabinetWidget::~addCabinetWidget()
{
    delete ui;
}

bool  addCabinetWidget::checkCabinetNumber()
{
    QString cabinetIDStr = ui->lineEdit->text().trimmed();
    QRegExp rrx("\\d{2}");
    if(!rrx.exactMatch(cabinetIDStr) )
    {
        myMsgBox.showMsgBox("柜子号错误，请输入柜子号。");
        return false;
    }
    if(ST_cabinet_cell::instance()->checkCabinetIsExit(cabinetIDStr))
    {
        myMsgBox.showMsgBox("柜子已存在，请检查后重新输入。");
        return false;
    }
    mycabinetIDStr = cabinetIDStr;
    return true;
}

void addCabinetWidget::okBtnClicked()
{
    if(!checkCabinetNumber())
    {
        return;
    }

    int cabinetType = ui->comboBox->currentIndex() + 1;
    int cabinetNum = 0;
    if(cabinetType == 1)
    {
        cabinetNum = 20;
    }
    else
    {
        cabinetNum = 20;
    }

    ST_cabinet_cell::instance()->insertCabinet(mycabinetIDStr,cabinetType,cabinetNum);
    ST_cabinet_cell::instance()->initCell(mycabinetIDStr,cabinetType,cabinetNum);
    ui->lineEdit->clear();
    emit addSignal();
}

void addCabinetWidget::cancelBtnClicked()
{
    mycabinetIDStr.clear();
    ui->lineEdit->clear();
    hide();
}
