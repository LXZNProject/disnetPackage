#include "basicsetstatus.h"

CBasicSetStatus::CBasicSetStatus(STATUS status):
    CStatus(status)
{
    mterminal =new terminal();
}

void CBasicSetStatus::enter(QByteArray param)
{
    mterminal->readTerminalInfo( param);
}

void CBasicSetStatus::leave(QByteArray param)
{
    mterminal->hide();
}

void CBasicSetStatus::coinInputProc(QByteArray param)
{
}

void CBasicSetStatus::cardInputProc(QByteArray param)
{
}
