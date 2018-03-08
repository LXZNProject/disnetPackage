#include "balancestatus.h"

CBalanceStatus::CBalanceStatus(STATUS status):
    CStatus(status)
{
    balancepage =new balancePage();
}
void CBalanceStatus::enter(QByteArray param)
{
    balancepage->timeStart();
    balancepage->show();
}

void CBalanceStatus::leave(QByteArray param)
{
    balancepage->hide();
}

void CBalanceStatus::coinInputProc(QByteArray param)
{
}

void CBalanceStatus::cardInputProc(QByteArray param)
{
}
