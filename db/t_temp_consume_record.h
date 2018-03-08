#ifndef T_TEMP_CONSUME_RECORD_H
#define T_TEMP_CONSUME_RECORD_H

#include "singleton.h"
#include <QDateTime>

typedef struct{
    QString m1Card;
    QString companyId;
    QDateTime createTime;
    double curCash;
    QString usrUuid;
    QString cmmUuid;
} TempRecordDesc_t;


class t_temp_consume_record
{

public:
    explicit t_temp_consume_record();

    void init();
    void insertTempRecord(TempRecordDesc_t);
    bool queryTempRecord(QString usrUuid,double &cash,QString &cmmUuid);
    void clearTempRecord(QString usrUuid);
    void deleteTempRecordInfo();
    
signals:
    
public slots:
    
     DECLARE_SINGLETON_CLASS(t_temp_consume_record)
};

typedef Singleton<t_temp_consume_record> STempRecord;

#endif // T_TEMP_CONSUME_RECORD_H
