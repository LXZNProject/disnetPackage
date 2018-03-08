#ifndef CHECKNOTETHREAD_H
#define CHECKNOTETHREAD_H

#include <QThread>
#include "singleton.h"
#include "mainctrl.h"
#include "t_log_smslog.h"
#include "t_basic_terminal.h"
#include "notethread.h"
#include <QDebug>

#define CHECK_NOTE_TIME 5*60

class CheckNoteThread : public QThread
{
    Q_OBJECT
public:

    void init()
    {
        start();
    }


private:
    CheckNoteThread(){}
    ~CheckNoteThread(){}

    void run()
    {
        bool ret = true;
        while(1)
        {
            sleep(CHECK_NOTE_TIME);
            qDebug() << "--info--: check note thread proccess ...";

            if (SCMainCtrl::instance()->curStatus() == 1)
            {
               noteExceptionProc();

               QTime time = QTime::currentTime();
               int hour = time.hour();
               if(hour != 3)
               {
                   ret = true;
               }
               if((hour == 3) && ret)
               {
                   ret = false;
                   noteBreakProc();
               }
            }
        }

    }

    void noteExceptionProc()
    {
        qint8 loseNum = 10;
        QList<NoteDesc_t> noteList;
        SCSmslog::instance()->queryLoseNoteInfo(noteList,2,loseNum);

        for(int i = 0; i < noteList.size(); i++)
        {
            SCNoteThread::instance()->sendNoteEvent(noteList.at(i));
        }
    }

    void noteBreakProc()
    {
        qint8 loseNum = 10;
        QList<NoteDesc_t> noteList;
        SCSmslog::instance()->queryLoseNoteInfo(noteList,3,loseNum);

        for(int i = 0; i < noteList.size(); i++)
        {
            SCNoteThread::instance()->sendNoteEvent(noteList.at(i));
        }
    }



    DECLARE_SINGLETON_CLASS(CheckNoteThread)
};

typedef Singleton<CheckNoteThread> SCheckNoteThread;


#endif // CHECKNOTETHREAD_H
