#ifndef UPDATETERMINAL_H
#define UPDATETERMINAL_H

#include <QThread>
#include "singleton.h"
#include "mainctrl.h"
#include "servercomm.h"
#include <QObject>
#include <QTime>
#include <QDateTime>
#include <QDebug>

#define  UPDATE_TERMINAL_TIME  30*60

class CUpdateTerminal : public QThread
{
    Q_OBJECT
public:
    explicit CUpdateTerminal(QObject *parent = 0);
    void init()
    {
        start();
    }

    void run()
    {

        qsrand(time(0));

        while(1)
        {
            sleep(UPDATE_TERMINAL_TIME);

            if (SCMainCtrl::instance()->curStatus() == 1)
            {
                QTime time = QTime::currentTime();
                int hour = time.hour();
                int updatetime;
                if(hour == 3)

                {
                    updatetime = qrand()%30;
                    qDebug() << "update terminal program" << updatetime;

                    sleep(updatetime*60);

                    updateTerminalInfo();
                }
            }
        }

    }

    void updateTerminalInfo();
    void startUpdateTerminalInfo(syncTerminalDataRet&);


    

    DECLARE_SINGLETON_CLASS(CUpdateTerminal)
};

typedef Singleton<CUpdateTerminal> SCUpdateTerminal;

#endif // UPDATETERMINAL_H
