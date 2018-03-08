#ifndef CHECKDEVICESTATUS_H
#define CHECKDEVICESTATUS_H
#include "status.h"
#include "checkdevice.h"
class CCheckDeviceStatus: public CStatus
{
    Q_OBJECT
public:
    CCheckDeviceStatus(STATUS status = ST_CHECK_DEVICE);

    void enter(QByteArray param);
    void leave(QByteArray param);


public slots:
    void resetStart();
    void jumpPage();
private:

       checkDevice *myPage;
};

#endif // CHECKDEVICESTATUS_H
