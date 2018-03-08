#ifndef T_BASIC_CELL_ORDER_H
#define T_BASIC_CELL_ORDER_H
#include "singleton.h"
#include "syncOrderCellRet.h"

typedef struct {
    QString phone;
    QString cellId;
    int     type;
}userOrder;

class t_basic_cell_order
{

public:
    QString orderCellTime;
public:
    t_basic_cell_order();

    void init();

    void insertOrderInfo(QString time,QList<expOrdersCellList*>expList,QList<userOrdersCellList*>userList);

    bool isExpOrderInfo(QString m1Card);

    void getExpOrderInfo(QString m1Card,QStringList &big,QStringList &middle,QStringList &small,QStringList &order);

    void getUserOrderInfo(QList<userOrder> &cellInfos);


    DECLARE_SINGLETON_CLASS(t_basic_cell_order)
};

typedef Singleton<t_basic_cell_order> STCellOrder;

#endif // T_BASIC_CELL_ORDER_H
