#include "gprsreset.h"
#include "sys/ioctl.h"
#include "fcntl.h"


#define TYPE_E_IOGPRSCTL 		0xFE
#define NR_GPRS_RESET        	    0x56
#define GPRS_RESET         		    _IO(TYPE_E_IOGPRSCTL, NR_GPRS_RESET)


gprsReset::gprsReset(QObject *parent) :
    QObject(parent)
{
    fd_watchdog = open( "/dev/gprs", O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd_watchdog < 0)
    {
            qDebug("the ext gprsReset  open failed.\n");
    }
}

void gprsReset::reset()
{
    if (fd_watchdog < 0)
        return;

    ioctl(fd_watchdog, GPRS_RESET, 0);
}
