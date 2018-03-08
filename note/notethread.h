#ifndef NOTETHREAD_H
#define NOTETHREAD_H

#include <QObject>
#include <QEvent>
#include "singleton.h"
#include "t_log_smslog.h"

static const QEvent::Type SEND_NOTE_TYPE = (QEvent::Type)QEvent::registerEventType(QEvent::User + 4); //发送短信

class CNoteEvent : public QEvent
{
public:
    CNoteEvent(QEvent::Type type,NoteDesc_t noteInfo):
        QEvent(type),
        mNoteInfo(noteInfo)
    {
    }

public:
  NoteDesc_t mNoteInfo;
};


class CNoteThread : public QObject
{
    Q_OBJECT
public:
    explicit CNoteThread(QObject *parent = 0);

    void init();
    bool sendNoteProc(QString phone,QString content);
    void sendNoteEvent(NoteDesc_t noteInfo);
    
signals:
    
public slots:
    
protected:
    bool event(QEvent *);
private:
    int fd_232_0;
    int CDMACloseModem(int CDMAFd);
    int CDMAOpenModem(char *CDMADevice);

    void settty(int fd, int baud, int data_bits, int stop_bits, int parity);
    //msginit模块
    int CDMASendSMSInit(int CDMAFd);
    int CDMAInitModem(int CDMAFd);
    bool msginit();

    //sendmsg模块
    int CDMASendSMS(int CDMAFd, char *phone, char *msg);
    static int CODEtoUNICODEBIG(char *in, size_t instrlen,char *out, size_t outbufsiz, char *codetype);
    int CDMASendInitModem(int CDMAFd);

    //GPRS
    bool GPRSInitModem();
    DECLARE_SINGLETON_CLASS(CNoteThread)
};

typedef Singleton<CNoteThread> SCNoteThread;

#endif // NOTETHREAD_H
