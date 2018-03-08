#ifndef T_LOG_SMSLIMIT_H
#define T_LOG_SMSLIMIT_H

#include <QDate>
#include "singleton.h"
class CNoteLimit
{
public:
    CNoteLimit();

    void init();

    bool insertNote(QString phone,QDate date);
    bool getNoteInfo(QString phone,QDate &date,int &number);

    void deleteOverTimeNoteInfo();
    bool checkNoteSendInfo(QString phone,int limitNum,int &curNum);


    DECLARE_SINGLETON_CLASS(CNoteLimit)
};

typedef Singleton<CNoteLimit> SCNoteLimit;

#endif // T_LOG_SMSLIMIT_H
