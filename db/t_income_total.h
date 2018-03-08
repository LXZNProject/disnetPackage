#ifndef T_INCOME_TOTAL_H
#define T_INCOME_TOTAL_H

#include "singleton.h"
#include <QDateTime>
typedef struct{
    double  allCoin ;
    double  allIncome;
    QDateTime activeTime;
}TIncomeTotal;


class t_income_total
{
public:
    t_income_total();

    void init();
    bool updataNote(TIncomeTotal date);
    bool getdataNote(TIncomeTotal date);

    DECLARE_SINGLETON_CLASS(t_income_total)
};
typedef Singleton<t_income_total> STIncomeTotal;

#endif // T_INCOME_TOTAL_H
