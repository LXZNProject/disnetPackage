#include "notesetstatus.h"

CNoteSetStatus::CNoteSetStatus(STATUS status):
    CStatus(status)
{
    notetemplatePage =new notetemplate();
}

void CNoteSetStatus::enter(QByteArray param)
{
    notetemplatePage->readPageInfo();
}

void CNoteSetStatus::leave(QByteArray param)
{
    notetemplatePage->hide();
}

void CNoteSetStatus::coinInputProc(QByteArray param)
{
}

void CNoteSetStatus::cardInputProc(QByteArray param)
{
}
