#include "notetemplate.h"
#include "ui_notetemplate.h"
#include "t_log_template.h"
#include "mainctrl.h"
#include <QBitmap>
notetemplate::notetemplate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::notetemplate)
{
    ui->setupUi(this);
    initPage();
}

notetemplate::~notetemplate()
{
    delete ui;
}

void notetemplate::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    QPixmap pixmap(":/note/images/note/background.png");
    this->setMask(pixmap.mask());
    this->resize(pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);



    QSize size = QPixmap(":/note/images/note/4.png").size();
    ui->tabWidget->setTabIcon(3,QIcon(":/note/images/note/4.png"));
    ui->tabWidget->setTabIcon(2,QIcon(":/note/images/note/3.png"));
    ui->tabWidget->setTabIcon(1,QIcon(":/note/images/note/2.png"));
    ui->tabWidget->setTabIcon(0,QIcon(":/note/images/note/1_1.png"));
    ui->tabWidget->setIconSize(QSize(size.width(),size.height()));

    ui->tabWidget->setFocusPolicy(Qt::NoFocus);
    QString style = "QTabBar::tab{border: 0px;border-top-left-radius: 0px;border-top-right-radius: 0px;min-width: 0ex;padding: 0px;min-width:100px;min-height:96px} \
             QTabWidget::pane{border-image: url(:/page/images/main/tab_bg.png)}";
    ui->tabWidget->setStyleSheet(style);

    //第一页
    ui->label->resize(QPixmap(":/note/images/note/first/lable1.png").size());
    ui->label->setPixmap(QPixmap(":/note/images/note/first/lable1.png"));
    ui->label_2->resize(QPixmap(":/note/images/note/first/lable2.png").size());
    ui->label_2->setPixmap(QPixmap(":/note/images/note/first/lable2.png"));
    ui->label_3->resize(QPixmap(":/note/images/note/first/lable3.png").size());
    ui->label_3->setPixmap(QPixmap(":/note/images/note/first/lable3.png"));
    ui->label_4->resize(QPixmap(":/note/images/note/first/lable4.png").size());
    ui->label_4->setPixmap(QPixmap(":/note/images/note/first/lable4.png"));

    ui->label_5->resize(QPixmap(":/note/images/note/second/lable6.png").size());
    ui->label_5->setPixmap(QPixmap(":/note/images/note/second/lable5.png"));
    ui->label_6->resize(QPixmap(":/note/images/note/second/lable6.png").size());
    ui->label_6->setPixmap(QPixmap(":/note/images/note/second/lable6.png"));
    ui->label_7->resize(QPixmap(":/note/images/note/second/lable6.png").size());
    ui->label_7->setPixmap(QPixmap(":/note/images/note/second/lable7.png"));
    ui->label_8->resize(QPixmap(":/note/images/note/second/lable6.png").size());
    ui->label_8->setPixmap(QPixmap(":/note/images/note/second/lable8.png"));

    ui->label_9->resize(QPixmap(":/note/images/note/three/lable9.png").size());
    ui->label_9->setPixmap(QPixmap(":/note/images/note/three/lable9.png"));
    ui->label_10->resize(QPixmap(":/note/images/note/three/lable10.png").size());
    ui->label_10->setPixmap(QPixmap(":/note/images/note/three/lable10.png"));
    ui->label_11->resize(QPixmap(":/note/images/note/three/lable11.png").size());
    ui->label_11->setPixmap(QPixmap(":/note/images/note/three/lable11.png"));

    ui->label_12->resize(QPixmap(":/note/images/note/fourth/lable12.png").size());
    ui->label_12->setPixmap(QPixmap(":/note/images/note/fourth/lable12.png"));
    ui->label_13->resize(QPixmap(":/note/images/note/fourth/lable13.png").size());
    ui->label_13->setPixmap(QPixmap(":/note/images/note/fourth/lable13.png"));
    ui->label_14->resize(QPixmap(":/note/images/note/fourth/lable14.png").size());
    ui->label_14->setPixmap(QPixmap(":/note/images/note/fourth/lable14.png"));
    ui->label_15->resize(QPixmap(":/note/images/note/fourth/lable15.png").size());
    ui->label_15->setPixmap(QPixmap(":/note/images/note/fourth/lable15.png"));



    ui->okBtn->setFlat(true);
    ui->okBtn->setFocusPolicy(Qt::NoFocus);
    ui->okBtn->setIcon(QIcon(":/public/images/public/okBtn_bg.png"));
    ui->okBtn->setMinimumSize(QPixmap(":/public/images/public/okBtn_bg.png").size());
    ui->okBtn->setIconSize(ui->okBtn->size());
    ui->okBtn->setStyleSheet("background-color:transparent");

    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");


    ui->tabWidget->setCurrentIndex(0);
    tc =  QTextCodec::codecForName("UTF-8");

    ui->pushButton->installEventFilter(this);
    ui->pushButton_2->installEventFilter(this);
    ui->pushButton_3->installEventFilter(this);
    ui->pushButton_4->installEventFilter(this);
    ui->pushButton_5->installEventFilter(this);
    ui->pushButton_6->installEventFilter(this);
    ui->pushButton_7->installEventFilter(this);
    ui->pushButton_8->installEventFilter(this);
    ui->pushButton_9->installEventFilter(this);
    ui->pushButton_10->installEventFilter(this);
    ui->pushButton_11->installEventFilter(this);
    ui->pushButton_12->installEventFilter(this);
    ui->pushButton_13->installEventFilter(this);
    ui->pushButton_14->installEventFilter(this);
    ui->pushButton_15->installEventFilter(this);

    analyNoteInfo(ui->textEdit,ui->pushButton,0);     //基本存取
    analyNoteInfo(ui->textEdit_2,ui->pushButton_2,1);
    analyNoteInfo(ui->textEdit_3,ui->pushButton_3,2);
    analyNoteInfo(ui->textEdit_4,ui->pushButton_4,4); //寄件
    analyNoteInfo(ui->textEdit_5,ui->pushButton_5,5);
    analyNoteInfo(ui->textEdit_6,ui->pushButton_6,6);
    analyNoteInfo(ui->textEdit_7,ui->pushButton_7,7); //临时寄存
    analyNoteInfo(ui->textEdit_8,ui->pushButton_8,11); //异常
    analyNoteInfo(ui->textEdit_9,ui->pushButton_9,12);
    analyNoteInfo(ui->textEdit_10,ui->pushButton_10,13);
    analyNoteInfo(ui->textEdit_11,ui->pushButton_11,14);
    analyNoteInfo(ui->textEdit_12,ui->pushButton_12,8); //干洗
    analyNoteInfo(ui->textEdit_13,ui->pushButton_13,9);
    analyNoteInfo(ui->textEdit_14,ui->pushButton_14,10);
    analyNoteInfo(ui->textEdit_15,ui->pushButton_15,3); //待确认

    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabPageChange_slots(int)));
}


void notetemplate::tabPageChange_slots(int id)
{
    ui->tabWidget->currentWidget()->setFocus();
    if(3 == id)
    {
        ui->tabWidget->setTabIcon(3,QIcon(":/note/images/note/4_4.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/note/images/note/3.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/note/images/note/2.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/note/images/note/1.png"));
    }
    else if(2 == id)
    {
        ui->tabWidget->setTabIcon(3,QIcon(":/note/images/note/4.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/note/images/note/3_3.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/note/images/note/2.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/note/images/note/1.png"));
    }
    else if(1 == id)
    {
        ui->tabWidget->setTabIcon(3,QIcon(":/note/images/note/4.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/note/images/note/3.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/note/images/note/2_2.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/note/images/note/1.png"));
    }
    else if(0 == id)
    {
        ui->tabWidget->setTabIcon(3,QIcon(":/note/images/note/4.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/note/images/note/3.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/note/images/note/2.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/note/images/note/1_1.png"));
    }
}

void notetemplate::readPageInfo()
{
    ui->tabWidget->setCurrentIndex(0);

    analyNoteInfo(ui->textEdit,ui->pushButton,0);     //基本存取
    analyNoteInfo(ui->textEdit_2,ui->pushButton_2,1);
    analyNoteInfo(ui->textEdit_3,ui->pushButton_3,2);
    analyNoteInfo(ui->textEdit_4,ui->pushButton_4,4); //寄件
    analyNoteInfo(ui->textEdit_5,ui->pushButton_5,5);
    analyNoteInfo(ui->textEdit_6,ui->pushButton_6,6);
    analyNoteInfo(ui->textEdit_7,ui->pushButton_7,7); //临时寄存
    analyNoteInfo(ui->textEdit_8,ui->pushButton_8,11); //异常
    analyNoteInfo(ui->textEdit_9,ui->pushButton_9,12);
    analyNoteInfo(ui->textEdit_10,ui->pushButton_10,13);
    analyNoteInfo(ui->textEdit_11,ui->pushButton_11,14);
    analyNoteInfo(ui->textEdit_12,ui->pushButton_12,8); //干洗
    analyNoteInfo(ui->textEdit_13,ui->pushButton_13,9);
    analyNoteInfo(ui->textEdit_14,ui->pushButton_14,10);
    analyNoteInfo(ui->textEdit_15,ui->pushButton_15,3); //待确认

    this->show();

}

void notetemplate::analyNoteInfo(QTextEdit *text, QPushButton *btn, int i)
{
    NoteTemplateDesc_t note = SCNoteTemplate::instance()->noteTemplateInfo.at(i);
    text->setText(tc->toUnicode(note.templateContent.toUtf8()));

    btn->setFlat(true);
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setIcon(QIcon(":/note/images/note/start.png"));
    btn->setMinimumSize(QPixmap(":/note/images/note/start.png").size());
    btn->setIconSize(btn->size());
    btn->setStyleSheet("background-color:transparent");

    if(1 == note.canShutDown) //按钮不可操作
    {
        btn->setEnabled(false);
    }
    if(2 == note.canShutDown)
    {
        btn->setEnabled(true);
    }

    if(1 == note.status)
    {
        btn->setWindowIconText("已开启");
        if(btn->isEnabled())
        {
            btn->setIcon(QIcon(":/note/images/note/start.png"));
        }
        else
        {
            btn->setIcon(QIcon(":/note/images/note/start_1.png"));
        }
    }

    if(2 == note.status)
    {
        btn->setWindowIconText("已关闭");
        if(btn->isEnabled())
        {
            btn->setIcon(QIcon(":/note/images/note/close.png"));
        }
        else
        {
            btn->setIcon(QIcon(":/note/images/note/close_1.png"));
        }
    }
}

void notetemplate::groupNoteInfo(QTextEdit *text, QPushButton *btn, int i)
{
    NoteTemplateDesc_t note = SCNoteTemplate::instance()->noteTemplateInfo.at(i);

    if(btn->windowIconText() == "已开启")
    {
        note.status = 1;
    }

    if(btn->windowIconText() == "已关闭")
    {
        note.status = 2;
    }

    note.templateContent = text->toPlainText();

    SCNoteTemplate::instance()->noteTemplateInfo.replace(i,note);
}

void notetemplate::setBtnEnable(bool status)
{
    ui->okBtn->setEnabled(status);
    ui->quitBtn->setEnabled(status);
}

void notetemplate::okBtn_slots()
{
    setBtnEnable(false);

    groupNoteInfo(ui->textEdit,ui->pushButton,0);     //基本存取
    groupNoteInfo(ui->textEdit_2,ui->pushButton_2,1);
    groupNoteInfo(ui->textEdit_3,ui->pushButton_3,2);
    groupNoteInfo(ui->textEdit_4,ui->pushButton_4,4); //寄件
    groupNoteInfo(ui->textEdit_5,ui->pushButton_5,5);
    groupNoteInfo(ui->textEdit_6,ui->pushButton_6,6);
    groupNoteInfo(ui->textEdit_7,ui->pushButton_7,7); //临时寄存
    groupNoteInfo(ui->textEdit_8,ui->pushButton_8,11); //异常
    groupNoteInfo(ui->textEdit_9,ui->pushButton_9,12);
    groupNoteInfo(ui->textEdit_10,ui->pushButton_10,13);
    groupNoteInfo(ui->textEdit_11,ui->pushButton_11,14);
    groupNoteInfo(ui->textEdit_12,ui->pushButton_12,8); //干洗
    groupNoteInfo(ui->textEdit_13,ui->pushButton_13,9);
    groupNoteInfo(ui->textEdit_14,ui->pushButton_14,10);
    groupNoteInfo(ui->textEdit_15,ui->pushButton_15,3); //待确认

    SCNoteTemplate::instance()->setNoteTemplateInfo();

    myMsgBox.showMsgBox("信息配置成功");
    setBtnEnable(true);

}

void notetemplate::quitBtn_slots()
{
    //this->close();
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}


bool notetemplate::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QPushButton *myobj = static_cast<QPushButton *>(obj);

        if(myobj->isEnabled())
        {
            if(myobj->windowIconText() == "已开启")
            {
                myobj->setWindowIconText("已关闭");
                myobj->setIcon(QIcon(":/note/images/note/close.png"));
            }
            else if(myobj->windowIconText() == "已关闭")
            {
                myobj->setWindowIconText("已开启");
                myobj->setIcon(QIcon(":/note/images/note/start.png"));
            }
        }
    }
    return QWidget::eventFilter(obj,event);
}
