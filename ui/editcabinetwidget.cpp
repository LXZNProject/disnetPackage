#include "editcabinetwidget.h"
#include "ui_editcabinetwidget.h"
#include <QMessageBox>
#include <QDesktopWidget>
editCabinetWidget::editCabinetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editCabinetWidget)
{
    ui->setupUi(this);
    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtnClicked()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SLOT(cancelClicked()));

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/cell/images/cell/background.png")));
    this->setPalette(palette);


    this->move((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);

    ui->label->resize(QPixmap(":/cell/images/cell/cellNum.png").size());
    ui->label->setPixmap(QPixmap(":/cell/images/cell/cellNum.png"));

    ui->label_2->resize(QPixmap(":/cell/images/cell/cellType.png").size());
    ui->label_2->setPixmap(QPixmap(":/cell/images/cell/cellType.png"));

    ui->label_3->resize(QPixmap(":/cell/images/cell/cellStatus.png").size());
    ui->label_3->setPixmap(QPixmap(":/cell/images/cell/cellStatus.png"));

    ui->label_4->resize(QPixmap(":/cell/images/cell/isNull.png").size());
    ui->label_4->setPixmap(QPixmap(":/cell/images/cell/isNull.png"));

    ui->label_5->resize(QPixmap(":/cell/images/cell/goodcheck.png").size());
    ui->label_5->setPixmap(QPixmap(":/cell/images/cell/goodcheck.png"));

    ui->label_6->resize(QPixmap(":/cell/images/cell/gookcheckstatus.png").size());
    ui->label_6->setPixmap(QPixmap(":/cell/images/cell/gookcheckstatus.png"));

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
}

editCabinetWidget::~editCabinetWidget()
{
    delete ui;
}

void editCabinetWidget::readInfo(basicCell tempvalue)
{
    mybasiCell = tempvalue;
    ui->numberLineEdit->setText(tempvalue.cellId);
    ui->numberLineEdit->setEnabled(false);
    ui->typecomboBox->setCurrentIndex(tempvalue.cellType - 1);
    ui->statuscomboBox->setCurrentIndex(tempvalue.status - 1);
    ui->nullcomboBox->setCurrentIndex(tempvalue.hasGoods - 1);
    ui->thingscomboBox->setCurrentIndex(tempvalue.goodsCheck - 1);
    ui->thingsStatuscomboBox->setCurrentIndex(tempvalue.goodsCheckStatus - 1);
}

void editCabinetWidget::okBtnClicked()
{
    basicCell tempvalue;
    tempvalue.cabinetId = mybasiCell.cabinetId;
    tempvalue.cellId = mybasiCell.cellId;
    tempvalue.cellType = ui->typecomboBox->currentIndex() + 1;
    tempvalue.status = ui->statuscomboBox->currentIndex() + 1;
    tempvalue.hasGoods = ui->nullcomboBox->currentIndex() + 1;
    tempvalue.goodsCheck = ui->thingscomboBox->currentIndex() + 1;
    tempvalue.goodsCheckStatus = ui->thingsStatuscomboBox->currentIndex() + 1;

    if(ST_cabinet_cell::instance()->changeCell(tempvalue))
    {
        myMsgBox.showMsgBox("更改格子信息成功。");
        emit editOKSignal();
    }
    else
    {
        myMsgBox.showMsgBox("更改格子信息失败。");
    }
    mybasiCell.cellId = "";
    hide();
}

void editCabinetWidget::cancelClicked()
{
    hide();
    mybasiCell.cellId = "";
}
