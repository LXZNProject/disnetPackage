#ifndef T_CABINET_CELL_H
#define T_CABINET_CELL_H

#include <QObject>
#include "singleton.h"
#include "t_log_network.h"
#include <QtSql/QSqlDatabase>
#include <QDateTime>
#include <QList>
typedef struct{
    QString cellId;
    QString cabinetId;
    qint8 cellType;
    qint8 status;
    qint8 hasGoods;
    qint8 goodsCheck;
    qint8 goodsCheckStatus  ;
}basicCell;

typedef struct{
    quint64 errorId;
    QString cellId;
    QString cabinetId;
    qint8 errorType;
    QDateTime reporttime;
    QDateTime repairtime  ;
    qint8 status  ;
    qint8 submit;
}cellError;

class t_cabinet_cell : public QObject
{
    Q_OBJECT
public:
    explicit t_cabinet_cell(QObject *parent = 0);
    ~t_cabinet_cell() {}

    //三张表格初始化，柜子表，格子表，格门异常表
    void init();

    //柜子表
    bool checkCabinetIsExit(QString cabinetID);//查看柜子是否存在
    bool insertCabinet(QString cabinetID, int type,int cabinetNum = 20);//增加柜子
    bool changeCabinet(QString cabinetID, int type,int cabinetNum);//修改柜子
    void delectCabinet(QString cabinetID);//删除
    void getCabinetNumber(QString cabinetID, int &type,int &number);//获取柜子个数
    void getSaveCabinetInfo(QStringList &cabinetID,QStringList &cabinetType,QStringList &cabinetNum);//获取柜子号与个数



    //格子表
    void initCell(QString cabinetID, int type,int cabinetNum = 20);//初始化格子数目以及格子的各种状态

    bool checkCellIsExit(QString cellID); //查看格子是否存在
    bool getCellInfo(QString cellId,basicCell &tempvalue);
    bool insertCell(basicCell tempvalue); //增加格子
    bool changeCell(basicCell tempvalue); //修改格子 整体修改
    bool changeCellStatus(QString cellId, qint8 status);
    bool changeCellHasGoods(QString cellId, qint8 hasGoods);
    bool changeCellGoodsCheck(QString cellId, qint8 goodsCheck);
    bool changeCellGoodsCheckStatus(QString cellId, qint8 goodsCheckStatus);
    bool delectCell(QString cellID); //删除
    bool takeChangeCellInfo(QString cellID); //取件完成改变格子信息

    void getSaveCellInfo(QString cabinetID,QVector <basicCell> &tempvalueList);//获取格子详细信息


    //设置格门异常表
    bool setCellError(cellError tempvalue);

    bool insertCellError(cellError tempValue);
    bool changeCellError(cellError tempValue);
    bool queryOpenCellError(QList<cellError> &tempValue);

    bool updateUseNotOpenCellError(QString cell);

    bool updateExceptionCellError(cellError tempValue);

    bool queryExceptionCellError(QList<QStringList> &modelData);

    //关闭派件信息
    bool closeDeliveryInfo(QString cell);

    QList<NetCellExcept> getNotCloseCellInfo();

    void deleteOverTimeExcpInfo();

    //存件取可用格子数
    void getSaveCellInfo(QStringList &bigList,QStringList &midList, QStringList &smlList);

    
signals:
    
public slots:

private:

    QSqlDatabase db;
    basicCell  myBasicCell;
    cellError   myCellError;

    DECLARE_SINGLETON_CLASS(t_cabinet_cell)
};
typedef Singleton<t_cabinet_cell> ST_cabinet_cell;

#endif // T_CABINET_CELL_H
