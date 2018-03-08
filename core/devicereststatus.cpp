#include "devicereststatus.h"

CDeviceRestStatus::CDeviceRestStatus(STATUS status):
    CStatus(status)
{
    devicerest = new devicerestpage();
}


void CDeviceRestStatus::enter(QByteArray param)
{
    devicerest->show();
}


void CDeviceRestStatus::leave(QByteArray param)
{
    devicerest->hide();
}
