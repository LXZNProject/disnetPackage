#include "rechagehistorypage.h"
#include "ui_rechagehistorypage.h"
#include <QMessageBox>
#include "mainctrl.h"
rechageHistoryPage::rechageHistoryPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rechageHistoryPage)
{
    ui->setupUi(this);
    initPage();
}

rechageHistoryPage::~rechageHistoryPage()
{
    delete ui;
}

void rechageHistoryPage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/rechageHistory/images/rechageHistory/background.png")));
    this->setPalette(palette);


    ui->label->resize(QPixmap(":/rechageHistory/images/rechageHistory/phone.png").size());
    ui->label->setPixmap(QPixmap(":/rechageHistory/images/rechageHistory/phone.png"));

    ui->queryBtn->setFlat(true);
    ui->queryBtn->setFocusPolicy(Qt::NoFocus);
    ui->queryBtn->setIcon(QIcon(":/rechageHistory/images/rechageHistory/query.png"));
    ui->queryBtn->setMinimumSize(QPixmap(":/rechageHistory/images/rechageHistory/query.png").size());
    ui->queryBtn->setIconSize(ui->queryBtn->size());
    ui->queryBtn->setStyleSheet("background-color:transparent");

    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");


    myhistoryList.clear();
    m_model = new takeViewModel(this);
    QStringList headers;
    headers << "充值手机号码" << "充值时间" << "充值金额";
    m_model->setHorizontalHeaderList(headers);

    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableView->setFocusPolicy(Qt::NoFocus);
    ui->tableView->setMinimumWidth(500);

    connect(ui->queryBtn,SIGNAL(clicked()),this,SLOT(queryBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(cancelBtn_slots()));


}

void rechageHistoryPage::queryBtn_slots()
{
    myhistoryList.clear();
    QRegExp rxcellphone("^1\\d{10}");
    QString info;
    QString phoneStr = ui->phoneLineEdit->text().trimmed();

    if(!rxcellphone.exactMatch(phoneStr))
    {
        info = "手机号码输入有误！请输入正确号码。";
        myMsgbox.showMsgBox(info);
        return;
    }
    SBasicStoredRecord::instance()->getRecordHistory(phoneStr,myhistoryList);


    QList<QStringList> data;
    for(int i = 0;i < myhistoryList.size();i++)
    {
        QStringList templist;
        templist << myhistoryList.at(i).relPhone << myhistoryList.at(i).recordtime.toString("yyyy-MM-dd HH:mm:ss") << myhistoryList.at(i).price;
        data.append(templist);
    }

    m_model->setModelData(data);
    m_model->refrushModel();
    ui->tableView->setModel(m_model);
}

void rechageHistoryPage::cancelBtn_slots()
{
    myhistoryList.clear();

    QList<QStringList> data;
    for(int i = 0;i < myhistoryList.size();i++)
    {
        QStringList templist;
        templist << myhistoryList.at(i).relPhone << myhistoryList.at(i).recordtime.toString("yyyy-MM-dd HH:mm:ss") << myhistoryList.at(i).price;
        data.append(templist);
    }

    m_model->setModelData(data);
    m_model->refrushModel();
    ui->tableView->setModel(m_model);

    ui->phoneLineEdit->clear();
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
