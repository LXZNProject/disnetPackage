#ifndef HEARTBEATREPORT_H
#define HEARTBEATREPORT_H
#include <QThread>
#include <QObject>
#include <QTime>
#include <QDebug>
#include <QString>
#include <QHostAddress>
#include <QNetworkInterface>

#include "singleton.h"
#include "mainctrl.h"
#include "networkinit.h"
#include "heartBeatReportRet.h"
#include "servercomm.h"
#include "db.h"
#include "networkthread.h"
#include "loseBlackUserRet.h"
#include "t_basic_terminal.h"
#include "syncOrderCellRet.h"
#include "t_basic_cell_order.h"
#define SLEEP_TIME  600
class heartBeatReportThrd : public QThread
{
    Q_OBJECT
public:

    void init()
    {
        qrandTime = qrand()%240;
        start();
    }

signals:
    void checkRunStatusSignal(qint8);

private:
    heartBeatReportThrd(){}
    ~heartBeatReportThrd(){}

    void run()
    {
        while(1)
        {
            sleep(SLEEP_TIME + qrandTime);

            if(SCMainCtrl::instance()->curStatus() == 1)
            {
                //远程命令
                syncRemoteCommandRet ret;
                QList<QRemoteCommandList*> cmdList;
                SCServerComm::instance()->syncRemoteCommand(&ret);

                if(ret.resultCode() == RET_TRUE)
                {
                    cmdList = ret.getCmdInfo();

                    for(int i = 0; i  < cmdList.size();i++)
                    {
                        if(cmdList.at(i)->commandType() == "1")
                        {
                            cmdList.removeAt(i);
                        }
                    }

                    if(cmdList.size() > 0)
                    {
                        QList<CmdDesc_t> retCmdList = SCDatBase::instance()->assistTerminalInfo(cmdList);
                        NetworkDesc_t cmdNetWorkInfo = SCServerComm::instance()->remoteCommandCallBack(retCmdList);
                        SCNetworkThread::instance()->sendNetWorkEvent(cmdNetWorkInfo);
                    }
                }

                //承包格子
                syncOrderCellRet orderCell;
                SCServerComm::instance()->syncOrderCell(&orderCell);
                if(orderCell.resultCode() == RET_TRUE)
                {
                    STCellOrder::instance()->insertOrderInfo(orderCell.orderCellSyncTime(),orderCell.getExpOrderList(),orderCell.getUserOrderList());
                }


            }

            //ip
            list.clear();
            list = QNetworkInterface::allAddresses();
            foreach (QHostAddress address, list)
            {
                if(address.protocol() == QAbstractSocket::IPv4Protocol)
                {
                    //IPv4地址
                    if (address.toString().contains("127.0."))
                    {
                        continue;
                    }
                    tempStr = address.toString();
                }
            }
            if (tempStr == "127.0.0.1")
            {
                qDebug() << "get local ip fail :" << tempStr;
            }
            else
            {
                qDebug() << "get local ip report time：" << tempStr ;

                heartBeatReportRet respone;
                SCServerComm::instance()->heartBeatReport(tempStr,&respone);
                if(respone.resultCode() == "0000")
                {
                    if(respone.terStatus().toInt()!= SCTerminal::instance()->terminalInfo.runStatus)
                    {
                        emit checkRunStatusSignal(respone.terStatus().toInt());
                    }
                }
                else
                {
                    qDebug() << "local ip fail";
                }
            }
        }
    }

private:

    QList<QHostAddress> list;
    QString tempStr;
    int qrandTime;

    DECLARE_SINGLETON_CLASS(heartBeatReportThrd)
};

typedef Singleton<heartBeatReportThrd> SReartBeatReportThrd;

#endif // HEARTBEATREPORT_H
