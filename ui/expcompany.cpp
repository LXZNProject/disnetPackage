#include "expcompany.h"
#include "ui_expcompany.h"

#include <QButtonGroup>
#include "t_basic_terminal.h"
#include <QStyleFactory>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QBitmap>
#include <QGridLayout>
#include <QCheckBox>
#include <QTextCodec>

QStringList myCompanyNameList;

expcompany::expcompany(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::expcompany)
{
    ui->setupUi(this);

    initExpCompanyPage();
}

expcompany::~expcompany()
{
    delete ui;
}

void expcompany::initExpCompanyPage()
{
    QTextCodec *tc =  QTextCodec::codecForName("UTF-8");
    this->setWindowFlags(Qt::FramelessWindowHint);
    if(!SCTerminal::instance()->terminalInfo.acceptExpCompanyName.isEmpty())
    {
        expInfoList = SCTerminal::instance()->terminalInfo.acceptExpCompanyName.split(',');
    }

    myCompanyNameList << tc->toUnicode("中通") << tc->toUnicode("圆通") << tc->toUnicode("汇通") \
                      << tc->toUnicode("申通") << tc->toUnicode("顺丰") << tc->toUnicode("韵达") \
                      << tc->toUnicode("天天") << tc->toUnicode("邮政") << tc->toUnicode("全峰") \
                      << tc->toUnicode("全通") << tc->toUnicode("国通") << tc->toUnicode("联邦") \
                      << tc->toUnicode("DHL") << tc->toUnicode("天猫") << tc->toUnicode("京东")\
                      << tc->toUnicode("苏宁") << tc->toUnicode("一号店") << tc->toUnicode("亚马逊") \
                      << tc->toUnicode("宅急送") << tc->toUnicode("如风达") << tc->toUnicode("其他");

    QPixmap pixmap(":/terminal/images/terminal/userConfig/background.png");
    this->setMask(pixmap.mask());
    this->resize(pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);


    ui->okBtn->setFlat(true);
    ui->okBtn->setFocusPolicy(Qt::NoFocus);
    ui->okBtn->setIcon(QIcon(":/public/images/public/okBtn_bg.png"));
    ui->okBtn->setMinimumSize(QPixmap(":/public/images/public/okBtn_bg.png").size());
    ui->okBtn->setIconSize(ui->okBtn->size());
    ui->okBtn->setStyleSheet("background-color:transparent");


    QGridLayout * mainLayout= new QGridLayout(ui->groupBox);
    checkGroup = new QButtonGroup(this);
    QStyle *style = QStyleFactory::create("windows");
    QCheckBox *checkBox;

    for(int column  = 0; column < 4; column++)
    {
        for(int row = 0; row < 6; row++)
        {
            qint8 length = column*6 + row;
            if(length + 1 > myCompanyNameList.size())
            {
                break;
            }
            checkBox = new QCheckBox(myCompanyNameList.at(length),ui->groupBox);
            checkBox->setStyle(style);
            checkBox->setFocusPolicy(Qt::NoFocus);
            mainLayout->addWidget(checkBox, row, column);
            checkGroup->addButton(checkBox,column*6 + row);  //顺丰

        }
    }

    checkGroup->setExclusive(false);  //互斥

    ui->groupBox->setLayout(mainLayout);

    this->setFocusPolicy(Qt::NoFocus);

    connect(checkGroup,SIGNAL(buttonClicked(QAbstractButton *)),this,SLOT(buttonclick_slots(QAbstractButton *)));
    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
}

void expcompany::buttonclick_slots(QAbstractButton *tempCheckBox)
{
    QString text = tempCheckBox->text().trimmed();


    QCheckBox *checkBox = static_cast<QCheckBox *>(tempCheckBox);

    if(checkBox->isChecked())
    {
        if(!expInfoList.contains(text))
        {
            expInfoList.append(text);
            checkBox->setCheckState(Qt::Checked);
        }
    }
    else
    {
        expInfoList.removeOne(text);
        checkBox->setCheckState(Qt::Unchecked);
    }
}

void expcompany::okBtn_slots()
{

    emit expInfo_signal(expInfoList);
    this->close();
}

void expcompany::readExpInfo()
{

    expInfoList.clear();
    if(!SCTerminal::instance()->terminalInfo.acceptExpCompanyName.isEmpty())
    {
        expInfoList = SCTerminal::instance()->terminalInfo.acceptExpCompanyName.split(',');
    }

    for(int i = 0; i < myCompanyNameList.size(); i++)
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(i));
        checkBox->setCheckState(Qt::Unchecked);

    }

    if(expInfoList.contains(QString::fromUtf8("顺丰")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("顺丰"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("韵达")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("韵达"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("申通")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("申通"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("圆通")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("圆通"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("中通")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("中通"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("京东")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("京东"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("邮政")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("邮政"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("天猫")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("天猫"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("苏宁")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("苏宁"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("全通")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("全通"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("天天")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("天天"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("国通")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("国通"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("汇通")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("汇通"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("一号店")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("一号店"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("亚马逊")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("亚马逊"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("宅急送")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("宅急送"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("联邦")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("联邦"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("DHL")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("DHL"))));
        checkBox->setCheckState(Qt::Checked);
    }
    if(expInfoList.contains(QString::fromUtf8("如风达")))
    {
        QCheckBox *checkBox = static_cast<QCheckBox *>(checkGroup->button(myCompanyNameList.indexOf(QString::fromUtf8("如风达"))));
        checkBox->setCheckState(Qt::Checked);
    }

    this->move((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);
    this->exec();
}
