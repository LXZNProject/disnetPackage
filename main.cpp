#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainctrl.h"
#include "db.h"
#include "driver.h"
#include "clockthread.h"
#include "portcomm.h"
#include "exception.h"
#include "notethread.h"
#include "checknotethread.h"
#include "networkthread.h"
#include "checknetworkthread.h"
#include "servercomm.h"
#include "updatesystem.h"
#include "updateterminal.h"
//#include "syszuxpinyin.h"
//#include "syszuxim.h"
#include "networkinit.h"
#include "runscript.h"
#include "watchdog.h"
#include "heartBeatReport.h"
#include "updateforbidden.h"
#include "sendfile.h"
#include "exceptioncell.h"
#include "scancomm.h"
#include <QWSServer>


void customMessageHandler(QtMsgType type, const char *msg)
{
     QTime current_time = QTime::currentTime();
     QString txt;
     switch (type) {
        //调试信息提示
       case QtDebugMsg:
               txt = QString("Debug: %1").arg(msg);
              break;

       //一般的warning提示
       case QtWarningMsg:
               txt = QString("Warning: %1").arg(msg);
       break;
       //严重错误提示
       case QtCriticalMsg:
               txt = QString("Critical: %1").arg(msg);
       break;
       //致命错误提示
      case QtFatalMsg:
               txt = QString("Fatal: %1").arg(msg);
               abort();
       }

      txt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd") +QString(":%1").arg(current_time.msec()) + txt;
      QFile outFile("debuglog.txt");
      outFile.open(QIODevice::WriteOnly | QIODevice::Append);
      QTextStream ts(&outFile);
      ts << txt << endl;
       if(outFile.size() > 10*1024*1024)
       {
         outFile.resize(0);
       }

}

void checkTime()
{
    QTimer *timer = new QTimer;
    timer->setInterval(5*60*1000);
    timer->setSingleShot(true);
    timer->start();

    QObject::connect(timer,SIGNAL(timeout()),SCUpdateSystemThrd::instance(),SLOT(checkClockTime()));

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // QWSServer::setCursorVisible(false);
   // qInstallMsgHandler(customMessageHandler);

   // a.setOverrideCursor(QCursor(Qt::BlankCursor));
  //  a.setStyleSheet("QPushButton {border:1px groove gray;border-radius:1px;padding:2px 4px;}");
    a.setStyleSheet("QScrollBar:vertical {background:rgba(0,0,0,0%);width:30px;padding-top:25px;padding-bottom:25px;}\
     QScrollBar::handle:vertical{width:30px;background:rgba(0,0,0,50%);border-radius:4px;  min-height:20;}");//加宽下拉框按钮
   // QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
  //  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
   // QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));



    //喂狗线程
    Watchdog wtdg;
    wtdg.start();
   // QApplication::setStyle("windows");

    SCDatBase::instance()->init();

    //驱动初始化
    SCDriver::instance()->init();

    SCMainCtrl::instance()->init();

    //串口通讯
    SCPortComm::instance()->init();
    SCScanComm::instance()->init();
    //打开、查询箱门线程
    SCLockThread::instance()->init();
    QThread lockOperation;
    SCLockThread::instance()->moveToThread(&lockOperation);
    lockOperation.start();

    //短信线程
    SCNoteThread::instance()->init();
    QThread sendNoteThread;
    SCNoteThread::instance()->moveToThread(&sendNoteThread);
    sendNoteThread.start();

    //通讯
    SCServerComm::instance()->init();

    //网络线程
    SCNetworkThread::instance()->init();
    QThread sendNetWorkThread;
    SCNetworkThread::instance()->moveToThread(&sendNetWorkThread);
    sendNetWorkThread.start();

    //远程更新同步
    SCUpdateTerminal::instance()->init();
    SCUpdateSystemThrd::instance()->init();
    SCUpdateForbidden::instance()->init();

     //异常处理线程
    SExceptionThrd::instance()->init();
    SCExceptionCellThrd::instance()->init();
    SCheckNoteThread::instance()->init();
    SCheckNetWorkThread::instance()->init();

    //上传文件
    SCSendFileThrd::instance()->init();

    //联网
    SCNetworkInit::instance()->connetNetwork();

    //监测网络
    SRunScriptThrd::instance()->init();
    //心跳
    SReartBeatReportThrd::instance()->init();

    checkTime();

 //   system("syslogd -O /usr/lxyz/debuglog.txt -s 15000"); //Klogd的输出结果会传送给syslogd进行处理,syslogd会根据/etc/syslog.conf的配置把log信息输出到/var/log/下

 //   system("klogd -n /usr/lxyz/debuglog.txt &");

    //SyszuxIM::installInputMethod();

    return a.exec();
}
