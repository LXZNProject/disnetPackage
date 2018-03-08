#include "networkconigstatus.h"

CNetworkConigStatus::CNetworkConigStatus(STATUS status):
    CStatus(status)
{
    myPage = new networkConfigure();
}

void CNetworkConigStatus::enter(QByteArray param)
{
    myPage->readInfo();
    myPage->show();
}

void CNetworkConigStatus::leave(QByteArray param)
{
    myPage->hide();
}

void CNetworkConigStatus::coinInputProc(QByteArray param)
{
}

void CNetworkConigStatus::cardInputProc(QByteArray param)
{
}
