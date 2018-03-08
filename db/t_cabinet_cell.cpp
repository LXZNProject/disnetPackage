#include "t_cabinet_cell.h"
#include <QSqlQuery>
#include <QStringList>
#include <QDebug>
#include <QSqlRecord>
#include <QVariant>
#include "t_basic_sync_forbidden.h"


t_cabinet_cell::t_cabinet_cell(QObject *parent) :
    QObject(parent)
{
//    init();
}

void t_cabinet_cell::init()
{
    myBasicCell.cellId = "";
    myBasicCell.cabinetId = "";
    myBasicCell.cellType = 3;
    myBasicCell.status = 1;
    myBasicCell.hasGoods = 2;
    myBasicCell.goodsCheck = 2;
    myBasicCell.goodsCheckStatus = 1;

    QSqlQuery query;
    //柜子表t_basic_cabinet
    query.exec("create table t_basic_cabinet(cabinetId varchar(2), cabinetType integer,cellNum integer)");

    //格子表t_basic_cell
    query.exec("create table t_basic_cell(cellId varchar(4), cabinetId varchar(2), cellType integer, status integer, hasGoods integer, goodsCheck integer, goodsCheckStatus integer)");

    //格门异常表  t_basic_cell_error
    query.exec("create table t_basic_cell_error(errorId integer primary key, cellId varchar(4), cabinetId varchar(2), errorType integer, reporttime datetime, repairtime datetime, status integer,submit integer)");

    QSqlQuery q("select * from  t_basic_cell_error");
    QSqlRecord rec = q.record();
    if(!rec.contains("submit"))
    {
        query.exec("alter table t_basic_cell_error add column submit integer");
        query.prepare("update t_basic_cell_error set submit = 1");
        query.exec();
    }
}



/****************************************************************
**
*	函数名称：	checkCabinetIsExit.
*	函数功能：	查看柜子是否存在
* 	函数参数：	cabinetID
* 	返回值  :       true
*
****************************************************************/
bool t_cabinet_cell::checkCabinetIsExit(QString cabinetID)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM t_basic_cabinet where cabinetId = ?");
    query.addBindValue(cabinetID);
    query.exec();

    if(query.first())
    {
        return true;
    }
    return false;
}
/****************************************************************
**
*	函数名称：	insertCabinet
*	函数功能：	增加柜子
* 	函数参数：	cabinetID  cabinetNum
* 	返回值  :       true
*
****************************************************************/
bool t_cabinet_cell::insertCabinet(QString cabinetID, int type,int cabinetNum)
{
    if(checkCabinetIsExit(cabinetID))
    {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO t_basic_cabinet VALUES(?,?,?)");
    query.addBindValue(cabinetID);
    query.addBindValue(type);
    query.addBindValue(cabinetNum);
    return query.exec();
}
/****************************************************************
**
*	函数名称：	changeCabinet
*	函数功能：	修改柜子
* 	函数参数：	cabinetID  cabinetNum
* 	返回值  :       true
*
****************************************************************/
bool t_cabinet_cell::changeCabinet(QString cabinetID, int type,int cabinetNum)
{
    QSqlQuery query;
    if(checkCabinetIsExit(cabinetID))
    {
        query.prepare("update t_basic_cabinet set cabinetType = ?,cellNum = ?where cabinetId = ?");
        query.addBindValue(type);
        query.addBindValue(cabinetNum);
        query.addBindValue(cabinetID);
        return query.exec();
    }
    return false;
}
/****************************************************************
**
*	函数名称：	delectCabinet
*	函数功能：	删除柜子
* 	函数参数：	cabinetID
* 	返回值  :       true
*
****************************************************************/
void t_cabinet_cell::delectCabinet(QString cabinetID)
{
    QSqlQuery query;
    query.prepare("DELETE  FROM t_basic_cabinet where cabinetId = ?");
    query.addBindValue(cabinetID);
    query.exec();
    query.clear();

    query.prepare("DELETE  FROM t_basic_cell where cabinetId = ?");
    query.addBindValue(cabinetID);
    query.exec();
}
/****************************************************************
**
*	函数名称：	getCabinetNumber
*	函数功能：	获取柜子中格子个数
* 	函数参数：	cabinetID cabinetNum
* 	返回值  :
*
****************************************************************/
void t_cabinet_cell::getCabinetNumber(QString cabinetID, int &type,int &number)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM t_basic_cabinet where cabinetId = ?");
    query.addBindValue(cabinetID);
    query.exec();

    if(query.first())
    {
        type = query.value(1).toInt();
        number = query.value(2).toInt();
    }
}
/****************************************************************
**
*	函数名称：	getSaveCabinetInfo
*	函数功能：	获取柜子信息
* 	函数参数：	cabinetID cabinetNum
* 	返回值  :
*
****************************************************************/
void t_cabinet_cell::getSaveCabinetInfo(QStringList &cabinetID,QStringList &cabinetType, QStringList &cabinetNum)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM t_basic_cabinet");
    query.exec();

    while(query.next())
    {
        cabinetID.append(query.value(0).toString());
        cabinetType.append (QString::number(query.value(1).toInt()));
        cabinetNum.append (QString::number(query.value(2).toInt()));
    }
}



/****************************************************************
**
*	函数名称：	initCell
*	函数功能：	初始化格子
* 	函数参数：	cabinetID cabinetNum
* 	返回值  :
*
****************************************************************/
void t_cabinet_cell::initCell(QString cabinetID, int type,int cabinetNum)
{
    myBasicCell.cabinetId = cabinetID;
    for(int i = 0; i < cabinetNum;i++)
    {
        myBasicCell.cellId = cabinetID + QString("%1").arg(i+1,2,10,QChar('0'));
        if(type == 1)
        {
            if(i < 6)
            {
                myBasicCell.cellType = 2;
                insertCell(myBasicCell);
            }
            else if(i>=6 && i< cabinetNum - 6)
            {
                myBasicCell.cellType = 3;
                insertCell(myBasicCell);
            }
            else if(i>=(cabinetNum - 6) && i< (cabinetNum - 2))
            {
                myBasicCell.cellType = 2;
                insertCell(myBasicCell);
            }
            else
            {
                myBasicCell.cellType = 1;
                insertCell(myBasicCell);
            }
        }
        else
        {
            if(i < 4)
            {
                myBasicCell.cellType = 2;
                insertCell(myBasicCell);
            }
            else if(i>=4 && i< cabinetNum - 6)
            {
                myBasicCell.cellType = 3;
                insertCell(myBasicCell);
            }
            else if(i>=(cabinetNum - 6) && i< (cabinetNum - 2))
            {
                myBasicCell.cellType = 2;
                insertCell(myBasicCell);
            }
            else
            {
                myBasicCell.cellType = 1;
                insertCell(myBasicCell);
            }
        }
    }
}
/****************************************************************
**
*	函数名称：	checkCellIsExit
*	函数功能：	查看格子是否存在
* 	函数参数：	basicCell
* 	返回值  :       true
*
****************************************************************/
bool t_cabinet_cell::checkCellIsExit(QString cellID)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM t_basic_cell where cellId = ?");
    query.addBindValue(cellID);
    query.exec();

    if(query.first())
    {
        return true;
    }
    return false;
}

bool t_cabinet_cell::getCellInfo(QString cellId, basicCell &tempvalue)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM t_basic_cell where cellId = ?");
    query.addBindValue(cellId);
    query.exec();

    if(query.first())
    {
        tempvalue.cellId = query.value(0).toString();
        tempvalue.cabinetId = query.value(1).toString();
        tempvalue.cellType = query.value(2).toInt();
        tempvalue.status = query.value(3).toInt();
        tempvalue.hasGoods = query.value(4).toInt();
        tempvalue.goodsCheck = query.value(5).toInt();
        tempvalue.goodsCheckStatus = query.value(6).toInt();
        return true;
    }
    return false;
}
/****************************************************************
**
*	函数名称：	insertCell
*	函数功能：	增加格子
* 	函数参数：	basicCell
* 	返回值  :       true
*
****************************************************************/
bool t_cabinet_cell::insertCell(basicCell tempvalue)
{
    if(checkCellIsExit(tempvalue.cellId))
    {
        return false;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO t_basic_cell VALUES(?,?,?,?,?,?,?)");
    query.addBindValue(tempvalue.cellId);
    query.addBindValue(tempvalue.cabinetId);
    query.addBindValue(tempvalue.cellType);
    query.addBindValue(tempvalue.status);
    query.addBindValue(tempvalue.hasGoods);
    query.addBindValue(tempvalue.goodsCheck);
    query.addBindValue(tempvalue.goodsCheckStatus);
    return query.exec();
}
/****************************************************************
**
*	函数名称：	changeCell
*	函数功能：	修改格子
* 	函数参数：	basicCell
* 	返回值  :       true
*
****************************************************************/
bool t_cabinet_cell::changeCell(basicCell tempvalue)
{
    if(checkCellIsExit(tempvalue.cellId))
    {
        QSqlQuery query;
        query.prepare("update t_basic_cell set cellType = ?,status = ?,hasGoods = ?,goodsCheck =?,goodsCheckStatus =?where cellId = ?");
        query.addBindValue(tempvalue.cellType);
        query.addBindValue(tempvalue.status);
        query.addBindValue(tempvalue.hasGoods);
        query.addBindValue(tempvalue.goodsCheck);
        query.addBindValue(tempvalue.goodsCheckStatus);
        query.addBindValue(tempvalue.cellId);
        return query.exec();
    }
    return false;
}

bool t_cabinet_cell::changeCellStatus(QString cellId, qint8 status)
{
    QSqlQuery query;
    query.prepare("update t_basic_cell set status = ? where cellId = ?");
    query.addBindValue(status);
    query.addBindValue(cellId);

    return query.exec();
}

bool t_cabinet_cell::changeCellHasGoods(QString cellId, qint8 hasGoods)
{
    QSqlQuery query;
    query.prepare("update t_basic_cell set hasGoods = ? where cellId = ?");
    query.addBindValue(hasGoods);
    query.addBindValue(cellId);

    return query.exec();
}

bool t_cabinet_cell::changeCellGoodsCheck(QString cellId, qint8 goodsCheck)
{
    QSqlQuery query;
    query.prepare("update t_basic_cell set goodsCheck = ? where cellId = ?");
    query.addBindValue(goodsCheck);
    query.addBindValue(cellId);

    return query.exec();
}

bool t_cabinet_cell::changeCellGoodsCheckStatus(QString cellId, qint8 goodsCheckStatus)
{
    QSqlQuery query;
    query.prepare("update t_basic_cell set goodsCheckStatus = ? where cellId = ?");
    query.addBindValue(goodsCheckStatus);
    query.addBindValue(cellId);

    return query.exec();
}
/****************************************************************
**
*	函数名称：	delectCell
*	函数功能：	删除格子
* 	函数参数：	cellID
* 	返回值  :       true
*
****************************************************************/
bool t_cabinet_cell::delectCell(QString cellID)
{
    if(checkCellIsExit(cellID))
    {
        QSqlQuery query;
        query.prepare("DELETE  FROM t_basic_cell where cellId = ?");
        query.addBindValue(cellID);
       return  query.exec();
    }
    return false;
}

bool t_cabinet_cell::takeChangeCellInfo(QString cellID)
{
    QSqlQuery query;
    query.prepare("update t_basic_cell set status = 3,hasGoods = 2 where cellId = ?");
    query.bindValue(0,cellID);

    return query.exec();
}

/****************************************************************
**
*	函数名称：	getSaveCellInfo
*	函数功能：	获取格子的详细信息
* 	函数参数：	cabinetID tempvalue
* 	返回值  :       true
*
****************************************************************/
void t_cabinet_cell::getSaveCellInfo(QString cabinetID, QVector<basicCell> &tempvalueList)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM t_basic_cell where cabinetId = ?");
    query.addBindValue(cabinetID);
    query.exec();

    while(query.next())
    {
        basicCell tempvalue;
        tempvalue.cellId = query.value(0).toString();
        tempvalue.cabinetId = query.value(1).toString();
        tempvalue.cellType = query.value(2).toInt();
        tempvalue.status = query.value(3).toInt();
        tempvalue.hasGoods = query.value(4).toInt();
        tempvalue.goodsCheck = query.value(5).toInt();
        tempvalue.goodsCheckStatus = query.value(6).toInt();
        tempvalueList.append(tempvalue);
    }
}
/****************************************************************
**
*	函数名称：	setCellError
*	函数功能：	设置异常
* 	函数参数：	cellError
* 	返回值  :       true
*
****************************************************************/
bool t_cabinet_cell::setCellError(cellError tempvalue)
{
    QSqlQuery query;
    bool isExist = false;

    query.prepare("SELECT cellId FROM t_basic_cell_error");
    query.exec();

    while(query.next())
    {
        if(tempvalue.cellId == query.value(0).toString())
        {
            isExist = true;
            break;
        }
    }
    query.clear();

    if(isExist)
    {
        query.prepare("select errorType, reporttime,repairtime ,status from t_basic_cell_error where cellId = ?");

        query.addBindValue(tempvalue.cellId);
        query.exec();
        while(query.next())
        {
            if((tempvalue.errorType == query.value(0).toInt()) &&(tempvalue.reporttime == query.value(1).toDateTime())\
                    &&(tempvalue.repairtime == query.value(2).toDateTime())&&(tempvalue.status == query.value(3).toInt()))
            {
                return true;
            }
            else
            {
                query.prepare("update t_basic_cell_error set errorType = ?,reporttime = ?,repairtime = ?,status =? where cellId = ?");
                query.addBindValue(tempvalue.errorType);
                query.addBindValue(tempvalue.reporttime);
                query.addBindValue(tempvalue.repairtime);
                query.addBindValue(tempvalue.status);
                query.addBindValue(tempvalue.cellId);
            }
        }
    }
    else
    {
        query.prepare("INSERT INTO t_basic_cell_error VALUES(?,?,?,?,?,?,?)");
        query.addBindValue(tempvalue.errorId);
        query.addBindValue(tempvalue.cellId);
        query.addBindValue(tempvalue.cabinetId);
        query.addBindValue(tempvalue.errorType);
        query.addBindValue(tempvalue.reporttime);
        query.addBindValue(tempvalue.repairtime  );
        query.addBindValue(tempvalue.status);
    }
    return query.exec();
}


/****************************************************************
**
*	函数名称：	insertCellError
*	函数功能：	插入异常格子
* 	函数参数：	cellError
* 	返回值  :       true
*
****************************************************************/

bool t_cabinet_cell::insertCellError(cellError tempValue)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM t_basic_cell_error WHERE cellId = ? and status = 1");
    query.bindValue(0,tempValue.cellId);
    query.exec();

    if(query.first())
    {
//        qDebug() << "update t_basic_cell_error: " << tempValue.cellId;
        query.prepare("update t_basic_cell_error set errorType = ?,reporttime = ?,repairtime = ?,status =? where cellId = ? and status = 1");
        query.addBindValue(tempValue.errorType);
        query.addBindValue(tempValue.reporttime);
        query.addBindValue(tempValue.repairtime);
        query.addBindValue(tempValue.status);
        query.addBindValue(tempValue.cellId);
    }
    else
    {
//        qDebug() << "INSERT t_basic_cell_error: " << tempValue.cellId;
        query.prepare("INSERT INTO t_basic_cell_error VALUES(null,?,?,?,?,?,?,1)");
        query.addBindValue(tempValue.cellId);
        query.addBindValue(tempValue.cabinetId);
        query.addBindValue(tempValue.errorType);
        query.addBindValue(tempValue.reporttime);
        query.addBindValue(tempValue.repairtime);
        query.addBindValue(tempValue.status);
    }

    return query.exec();
}

bool t_cabinet_cell::changeCellError(cellError tempValue)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM t_basic_cell_error WHERE cellId = ? ");
    query.bindValue(0,tempValue.cellId);
    query.exec();

    if(query.next())
    {
        query.prepare("update t_basic_cell_error set repairtime = ?,status = ? where cellId = ?");
        query.addBindValue(tempValue.repairtime);
        query.addBindValue(tempValue.status);
        query.addBindValue(tempValue.cellId);
    }
    return query.exec();
}

bool t_cabinet_cell::queryOpenCellError(QList<cellError> &tempValue)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM t_basic_cell_error WHERE errorType = 3 and status = 1 ");
    query.exec();

    while(query.next())
    {
        cellError temp;
        temp.cellId = query.value(1).toString();
        temp.cabinetId = query.value(2).toString();
        temp.errorType = query.value(3).toInt();
        temp.reporttime = query.value(4).toDateTime();
        temp.repairtime = query.value(5).toDateTime();
        temp.status = query.value(6).toInt();
        temp.submit = query.value(7).toInt();

        tempValue.append(temp);

//        qDebug() << "queryOpenCellError:" << temp.cellId;
    }
    return query.exec();
}

bool t_cabinet_cell::updateExceptionCellError(cellError tempValue)
{
    QSqlQuery query;
    query.prepare("update t_basic_cell_error set repairtime = ?,status = 2 where cellId = ? and status = 1");
    query.bindValue(0,tempValue.repairtime);
    query.bindValue(1,tempValue.cellId);

    return query.exec();
}

bool t_cabinet_cell::queryExceptionCellError(QList<QStringList> &modelData)
{
    quint8 tab = 0;
    QSqlQuery query;

    query.prepare("SELECT * FROM t_basic_cell_error WHERE errorType = 1 and status = 1 ");
    query.exec();
    while(query.next())
    {
        tab++;
        modelData.append(QStringList() << QString::number(tab) << query.value(1).toString() << "存件打不开" << "修复" << "1");
    }

    query.prepare("SELECT * FROM t_basic_cell_error WHERE errorType = 2 and status = 1 ");
    query.exec();
    while(query.next())
    {
        tab++;
        modelData.append(QStringList() << QString::number(tab) << query.value(1).toString() << "用户取件打不开" << "修复" << "2");
    }

    query.prepare("SELECT * FROM t_basic_cell_error WHERE errorType = 4 and status = 1 ");
    query.exec();
    while(query.next())
    {
        tab++;
        modelData.append(QStringList() << QString::number(tab) << query.value(1).toString() << "快递员取件打不开" << "修复" << "4");
    }

    query.prepare("SELECT * FROM t_basic_cell_error WHERE errorType = 5 and status = 1 ");
    query.exec();
    while(query.next())
    {
        tab++;
        modelData.append(QStringList() << QString::number(tab) << query.value(1).toString() << "寄存人取件打不开" << "修复" << "5");
    }

    query.prepare("SELECT * FROM t_basic_cell_error WHERE errorType = 6 and status = 1 ");
    query.exec();
    while(query.next())
    {
        tab++;
        modelData.append(QStringList() << QString::number(tab) << query.value(1).toString() << "快递员揽件打不开" << "修复" << "6");
    }


    return query.exec();
}

bool t_cabinet_cell::closeDeliveryInfo(QString cell)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("SELECT status FROM t_basic_cell where cellId = ?");
    query.addBindValue(cell);
    query.exec();

    if(query.first())
    {
        ret = true;
        qint8 tempStatus = query.value(0).toInt();

        query.prepare("update t_basic_cell set status = 1,hasGoods = 2 where cellId = ?");
        query.addBindValue(cell);
        query.exec();

        if(2 == tempStatus)
        {
            query.prepare("update t_basic_cell_error set status = 2 where cellId = ? and status = 1");
            query.bindValue(0,cell);
            query.exec();
        }
    }

    return ret;
}

QList<NetCellExcept> t_cabinet_cell::getNotCloseCellInfo()
{
    QList<NetCellExcept> notCloseCellInfo;
    QDateTime time = QDateTime::currentDateTime().addSecs(-24*3600);
    QSqlQuery query;
    query.prepare("SELECT * FROM t_basic_cell_error WHERE errorType = 3 and status = 1  and submit = 1 and reporttime < ?");
    query.bindValue(0,time);
    query.exec();

    while(query.next())
    {
        NetCellExcept notCloseInfo;
        notCloseInfo.cellId = query.value(1).toString();
        notCloseInfo.errorType = 3;
        notCloseInfo.flowType = 9;

        notCloseCellInfo.append(notCloseInfo);
    }

    if(!notCloseCellInfo.isEmpty())
    {
        query.prepare("update t_basic_cell_error set submit = 2 WHERE errorType = 3 and status = 1 and reporttime < ?");
        query.bindValue(0,time);
        query.exec();
    }

    return notCloseCellInfo;
}

void t_cabinet_cell::deleteOverTimeExcpInfo()
{
    QDateTime time = QDateTime::currentDateTime().addMonths(-1);
    QSqlQuery query;
    query.prepare("DELETE  FROM t_basic_cell_error WHERE repairtime < ?");
    query.bindValue(0,time);
    query.exec();
}




/****************************************************************
**
*	函数名称：	getSaveCellInfo
*	函数功能：	获取存件大 中 小 格子数
* 	函数参数：	QStringList
* 	返回值  :       无
*
****************************************************************/
void t_cabinet_cell::getSaveCellInfo(QStringList &bigList, QStringList &midList, QStringList &smlList)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM t_basic_cell");
    query.exec();

    while(query.next())
    {
        basicCell cell;
        cell.cellId = query.value(0).toString();
        cell.cabinetId = query.value(1).toString();
        cell.cellType = query.value(2).toInt();
        cell.status = query.value(3).toInt();
        cell.hasGoods = query.value(4).toInt();
        cell.goodsCheck = query.value(5).toInt();
        cell.goodsCheckStatus = query.value(6).toInt();

        if(SBalckList::instance()->forbidUserInfo.contains(cell.cellId))
        {
            continue;
        }

        if(2 == cell.goodsCheck)
        {
            if((1 == cell.status) && (2 == cell.hasGoods))
            {
                if(1 == cell.cellType)
                {
                    bigList.append(cell.cellId);
                }
                else if(2 == cell.cellType)
                {
                    midList.append(cell.cellId);
                }
                else if(3 == cell.cellType)
                {
                    smlList.append(cell.cellId);
                }
            }
        }
        else if(1 == cell.goodsCheckStatus)
        {
            if((1 == cell.status) && (2 == cell.hasGoods))
            {
                if(1 == cell.cellType)
                {
                    bigList.append(cell.cellId);
                }
                else if(2 == cell.cellType)
                {
                    midList.append(cell.cellId);
                }
                else if(3 == cell.cellType)
                {
                    smlList.append(cell.cellId);
                }
            }
        }

    }
}
