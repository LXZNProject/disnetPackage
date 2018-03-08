#ifndef T_INCOME_DETAIL_H
#define T_INCOME_DETAIL_H


#include "singleton.h"
#include <QDateTime>

typedef struct{
    quint32 incomeId;
    QDateTime incomeTime;
    quint8   incomeType;
    quint8   incomeWay;
    quint32 incomeObject;
    double  incomePrice ;
}TIncomeDetail;

class t_income_detail
{
public:
    t_income_detail();
       void init();
       bool insertNote(TIncomeDetail date);

        DECLARE_SINGLETON_CLASS(t_income_detail)
    };

    typedef Singleton<t_income_detail> STIncomeDetail;

#endif // T_INCOME_DETAIL_H
