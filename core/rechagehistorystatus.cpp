#include "rechagehistorystatus.h"

CRechageHistoryStatus::CRechageHistoryStatus(STATUS status):
    CStatus(status)
{
    myrechageHistoryPage =new rechageHistoryPage();
}

void CRechageHistoryStatus::enter(QByteArray param)
{
    myrechageHistoryPage->show();
}

void CRechageHistoryStatus::leave(QByteArray param)
{
    myrechageHistoryPage->hide();
}

void CRechageHistoryStatus::coinInputProc(QByteArray param)
{
}

void CRechageHistoryStatus::cardInputProc(QByteArray param)
{
}
