#ifndef UPDATESYSTEM_H
#define UPDATESYSTEM_H

#include <QThread>
#include "singleton.h"
#include "mainctrl.h"
#include "servercomm.h"
#include "httpupdate.h"
#include "t_image_time.h"
#include <QObject>
#include <QTime>
#include <QDateTime>
#include <QDebug>
#include <QFile>

#define UPDATE_SLEEP_TIME  25*60

class CUpdateSystemThrd : public QThread
{
    Q_OBJECT
public:

    void init()
    {
        start();
    }

signals:

public slots:
    void checkClockTime()
    {
        qDebug() <<" start check clock time";
        checkVersionRet respone;
        SCServerComm::instance()->checkVersion(&respone);

        QString date = respone.systemTime();
        if(!date.isEmpty())
        {
            date = "\"" + date + "\"";
            QString temp = QString("date -s %1").arg(date);
            QByteArray clockData = temp.toLatin1();
            system(clockData.data());

            system("hwclock -u -w");
        }
    }

public:
    CUpdateSystemThrd(){}
    ~CUpdateSystemThrd(){}


    void run()
    {

        qsrand(time(0));

        while(1)
        {
            sleep(UPDATE_SLEEP_TIME);

            if (SCMainCtrl::instance()->curStatus() == 1)
            {
                QTime time = QTime::currentTime();
                int hour = time.hour();
                int updatetime;
                if(hour == 2)

                {
                    updatetime = qrand()%30;
                    qDebug() << "start update program" << updatetime;

                    sleep(updatetime*60);

                    updateHomePng();
                    updateAppSoftWare();

                }
            }
        }

    }
public:
    bool updateHomePng()
    {
        qDebug() << "--info--: update HomePng thread proccess ...";

        bool ret = false;
        checkHomePageRet respone;
        SCServerComm::instance()->checkHomePage(&respone);

        if(respone.resultCode() == RET_TRUE)
        {
            if(!respone.downloadURL().isEmpty())
            {
                if(SCServerComm::instance()->download(respone.downloadURL(),respone.md5Code(),2))
                {
                    QFile newFile("./file/homepage.png");
                    QFile oldFile("./images/homepage.png");

                    if(oldFile.exists())
                    {
                        oldFile.remove();
                    }

                    ret = newFile.copy("./images/homepage.png");
                    if(ret)
                    {
                        SCImageTime::instance()->mHomeTime = respone.modifyTime();
                        SCImageTime::instance()->updateImageTimeInfo();
                    }
                    newFile.remove();

                }
                else
                {
                    system("rm -rf ./file/*");
                }
            }
        }

        return ret;
    }

    qint8  updateAppSoftWare()
    {
        qDebug() << "--info--: update AppSoftWare thread proccess ...";

        qint8 ret = 0;
        checkVersionRet respone;
        SCServerComm::instance()->checkVersion(&respone);

        QString date = respone.systemTime();
        if(!date.isEmpty())
        {
            date = "\"" + date + "\"";
            QString temp = QString("date -s %1").arg(date);
            QByteArray clockData = temp.toLatin1();
            system(clockData.data());

            system("hwclock -u -w");
        }
        else
        {
            ret = -1;
        }
        if(respone.resultCode() == RET_TRUE)
        {           
            if(!respone.downloadURL().isEmpty())
            {
                if(SCServerComm::instance()->download(respone.downloadURL(),respone.md5Code(),1))
                {
                    ret = 1;

                   // qDebug() << "system update";
                    system("sh update.sh &");

                }
                else
                {
                    system("rm -rf ./file/*");
                }
            }
        }

        return ret;

    }

    DECLARE_SINGLETON_CLASS(CUpdateSystemThrd)
};

typedef Singleton<CUpdateSystemThrd> SCUpdateSystemThrd;

#endif // UPDATESYSTEM_H
