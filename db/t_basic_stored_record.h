#ifndef T_BASIC_STORED_RECORD_H
#define T_BASIC_STORED_RECORD_H

#include "singleton.h"
#include <QDateTime>

typedef struct{
    QString price;
    QDateTime recordtime;
    QString storeCardNo;
    QString relM1CardId;
    QString relPhone;
    QString optM1CardId;
    QString optPhone;

}BasicStoredRecord;

class t_basic_stored_record
{
public:
    t_basic_stored_record();

    void init();
    bool insertNote(BasicStoredRecord date);
    void getRecordHistory(QString phoneNum,QList<BasicStoredRecord> &historyList);

    DECLARE_SINGLETON_CLASS(t_basic_stored_record)
};
typedef Singleton<t_basic_stored_record> SBasicStoredRecord;
#endif // T_BASIC_STORED_RECORD_H
