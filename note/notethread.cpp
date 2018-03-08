#include "notethread.h"
#include "t_basic_lan_jian.h"
#include "t_basic_delivery.h"
#include "t_basic_temp_storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <stddef.h>
#include "pthread.h"
#include <iconv.h>
#include <termios.h>
#include "math.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <QApplication>
#include <QUuid>
#include <QRegExp>

#define MAX_SZ          1024
#define CR              '\r'              /* carriage return */
#define NL              '\n'              /* next line*/
#define RETRY           20                /* sms-status-report */

//sendmsg
#define RETRYMIN        15
#define CTRL_Z          "\x00\x1A"            /*finish sms input*/
#define CTRL_Z_LEN      2                /*length of CTRL_Z*/
#define ESC             "\x00\x1B"            /*abort sms input*/
#define ESC_LEN         2                /*length of ESC*/
#define ST_REP          "+CDS"                /*sms-status-report*/
#define SMS_LEN         140

//grps
#define TYPE_E_IOGPRSCTL 		0xFE
#define NR_GPRS_RESET        	0x56
#define GPRS_RESET         		_IO(TYPE_E_IOGPRSCTL, NR_GPRS_RESET)	/* define the IO crtl parameter */


CNoteThread::CNoteThread(QObject *parent) :
    QObject(parent)
{
}

void CNoteThread::init()
{
    fd_232_0 = CDMAOpenModem("/dev/ttyO4");
    msginit();
}


/*
 * close modem
 */
int CNoteThread::CDMACloseModem(int CDMAFd)
{
    return (close(CDMAFd));
}

int CNoteThread::CDMAOpenModem(char *CDMADevice)
{
    int fd;
    /*
     * open CDMA serial port, O_NOCTTY means it's
     * not a controll tty device, O_NDELAY means
     * it's not block
     */
    fd = open(CDMADevice, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {
        perror("open()");
        return (-1);
    }
    return (fd);
}

void CNoteThread::settty(int fd, int baud, int data_bits, int stop_bits, int parity)
{
    struct termios opt;
    tcgetattr(fd, &opt);

    opt.c_cflag |= (CLOCAL | CREAD);
    opt.c_cflag &= ~CRTSCTS;                 // ÎÞÓ²ŒþÁ÷¿ØÖÆ

    if(stop_bits == 2)
    {
        opt.c_cflag |= CSTOPB;                // 2 Î»Í£Ö¹Î»
    }
    else
    {
        opt.c_cflag &= ~CSTOPB;                // 1 Î»Í£Ö¹Î»
    }
    opt.c_cflag &= ~CSIZE;

    switch(data_bits)
    {
        case 5: opt.c_cflag |= CS5;break;
        case 6: opt.c_cflag |= CS6;break;
        case 7: opt.c_cflag |= CS7;break;
        case 8: opt.c_cflag |= CS8;break;
        default :
            opt.c_cflag |= CS8;
            break;
     }

    switch(parity)
    {
        case 0:                               //ÎÞÆæÅŒÐ£ÑéÎ»
        {
            opt.c_cflag &= ~PARENB;
            opt.c_iflag &= ~INPCK;
            break;
        }
        case 1:                              // ÆæÐ£Ñé
        {
            opt.c_cflag |= PARENB;
            opt.c_cflag |= PARODD;
            opt.c_iflag |= INPCK;
            opt.c_iflag &= ~ISTRIP;
            break;
        }
        case 2:                             // ÅŒÐ£Ñé
        {
            opt.c_cflag |= PARENB;
            opt.c_cflag &= ~PARODD;
            opt.c_iflag |= INPCK;
            opt.c_iflag &= ~ISTRIP;
            break;
        }
        default:
            opt.c_cflag &= ~PARENB;
            opt.c_iflag &= ~INPCK;
            break;
    }

    opt.c_iflag&=~(IXON|IXOFF|IXANY);
    opt.c_iflag &= ~IGNPAR;
    opt.c_iflag &= ~(ICRNL|IGNCR);
    opt.c_iflag &= ~ (INLCR | ICRNL | IGNCR);
    opt.c_oflag &= ~(ONLCR | OCRNL);
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Input*/
    opt.c_oflag &= ~OPOST;                          /*Output*/
    opt.c_cc[VTIME] = 0;
    opt.c_cc[VMIN]  = 0;

    int baudrate = B9600;
    /* get the propr baudrate */
    switch (baud)
    {
        case 75:
        {
            baudrate=B75;
            break;
        }
        case 110:
        {
            baudrate=B110;
            break;
        }
        case 150:
        {
            baudrate=B150;
            break;
        }
        case 300:
        {
            baudrate=B300;
            break;
        }
        case 600:
        {
            baudrate=B600;
            break;
        }
        case 1200:
        {
            baudrate=B1200;
            break;
        }
        case 2400:
        {
            baudrate=B2400;
            break;
        }
        case 4800:
        {
            baudrate=B4800;
            break;
        }
        case 9600:
        {
            baudrate=B9600;
            break;
        }
        case 19200:
        {
            baudrate=B19200;
            break;
        }
        case 38400:
        {
            baudrate=B38400;
            break;
        }
        case 57600:
        {
            baudrate=B57600;
            break;
        }
        case 115200:
        {
            baudrate=B115200;
            break;
        }
        default:
        {
            baudrate=B9600;
            break;
        }
    }

    cfsetispeed(&opt, baudrate);
    cfsetospeed(&opt, baudrate);
    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, &opt);
}

int CNoteThread::CDMASendSMSInit(int CDMAFd)
{
    char buf[MAX_SZ];           /*command and response buffer*/
    int len;                    /*length written*/
    int cmd_len;                /*AT command's length*/
    int retry;                  /*retry time*/

    /* AT+CMGF=1\r set in TEXT mode */
    memset(buf, 0, MAX_SZ);
    cmd_len = snprintf(buf, MAX_SZ, "AT+CMGF=1\r");
    len = write(CDMAFd, buf, cmd_len);
    if (len != cmd_len)
    {
        printf("write()\n");
        return (-1);
    }
    for (retry = 0; retry < RETRY; retry++)
    {
        /*wait for 1 second*/
        sleep(1);

        memset(buf, 0, MAX_SZ);
        len = read(CDMAFd, buf, MAX_SZ);
        if (len < 0)
        {
            printf("read()\n");
            continue;
        }
        else if (len > 0)
        {
            printf("%s\n", buf);
            if (!strstr(buf, "OK"))
                return (-1);
            else
                break;
        }
    }

    memset(buf, 0, MAX_SZ);
    cmd_len = snprintf(buf, MAX_SZ, "AT+CNMI=1,2,0,1,0\r");
    len = write(CDMAFd, buf, cmd_len);
    if (len != cmd_len)
    {
        printf("write()\n");
        return (-1);
    }
    for (retry = 0; retry < RETRY; retry++)
    {
        /*wait for 1 second*/
        sleep(1);

        memset(buf, 0, MAX_SZ);
        len = read(CDMAFd, buf, MAX_SZ);
        if (len < 0)
        {
            printf("read()\n");
            continue;
        }
        else if (len > 0)
        {
            printf("%s\n", buf);
            if (!strstr(buf, "OK"))
                return (-1);
            else
                break;
        }
    }

    memset(buf, 0, MAX_SZ);
    cmd_len = snprintf(buf, MAX_SZ, "AT^HSMSSS=0,0,6,0\r");
    len = write(CDMAFd, buf, cmd_len);
    if (len != cmd_len)
    {
        printf("write()\n");
        return (-1);
    }
    for (retry = 0; retry < RETRY; retry++)
    {
        /*wait for 1 second*/
        sleep(1);

        memset(buf, 0, MAX_SZ);
        len = read(CDMAFd, buf, MAX_SZ);
        if (len < 0)
        {
            printf("read()\n");
            continue;
        }
        else if (len > 0)
        {
            printf("%s\n", buf);
            if (!strstr(buf, "OK"))
                return (-1);
            else
                break;
        }
    }
    return (0);
}

int CNoteThread::CDMAInitModem(int CDMAFd)
{
    char cmd_buf[MAX_SZ];
    int retry;
    int len;
    int cmd_len;
    /*flush data received and transimitted*/
    tcflush(CDMAFd, TCIOFLUSH);

    /*issue AT command*/
    memset(cmd_buf, 0, MAX_SZ);
    cmd_len = snprintf(cmd_buf, MAX_SZ, "AT\r");
   // strncpy(cmd_buf, "AT\r", MAX_SZ);
    len = write(CDMAFd, cmd_buf, strnlen(cmd_buf, MAX_SZ));
    if (len != cmd_len)
    {
        printf("write()\n");
        return (-1);
    }

    for (retry = 0; retry < RETRY; retry++)
    {
        /*wait for 1 second*/
        sleep(1);

        memset(cmd_buf, 0, MAX_SZ);
        len = read(CDMAFd, cmd_buf, MAX_SZ);
        if (len < 0)
        {
            printf("read()\n");
            continue;
        }

        if (strstr(cmd_buf, "OK"))
            return (0);
    }

    return (-1);
}

bool CNoteThread::msginit()
{

    bool ret = false;

    settty(fd_232_0, 115200, 8, 1, 0);
    if(CDMAInitModem(fd_232_0) < 0)
    {
        return ret;
    }


    if(CDMASendSMSInit(fd_232_0) < 0)
    {
        return ret;
    }

    ret = true;

    return ret;
}


int CNoteThread::CDMASendSMS(int CDMAFd, char *phone, char *msg)
{
    char buf[MAX_SZ];            /*command and response buffer*/
    int len;                    /*length written*/
    int cmd_len;                /*AT command's length*/
    char *msg_p;                /*point to msg*/
    int msg_len;                /*msg's length, in bytes*/
    char conv_buf[MAX_SZ];        /*buff for encode conversion*/
    int conv_len;        /*converted msg's length*/
    int retry;            /*retry time*/
    char *meginputcode = "\r\n> ";
    char ctrl_z[] = {0x00, 0x1A};

    /*convert from GB2312 to UNICODE*/
    msg_len = strnlen(msg, MAX_SZ);
    memset(conv_buf, 0, MAX_SZ);
    if ((conv_len = CODEtoUNICODEBIG(msg, msg_len, conv_buf, MAX_SZ, "UTF-8")) < 0)
    {
        printf("convert encoding failed!\n");
        return (-1);
    }
    if (conv_len> SMS_LEN)
    {
        printf("too long SMS,SMS must not be more than"" 140 English words or 70 Chinese words\n");
        return (-1);
    }
    /*send message*/
    memset(buf, 0, MAX_SZ);
    cmd_len = snprintf(buf, MAX_SZ, "AT^HCMGS=\"%s\",0\r",
                phone);
    len = write(CDMAFd, buf, cmd_len);
    if (len != cmd_len)
    {
        perror("write()");
        return (-1);
    }

    for (retry = 0; retry < RETRYMIN; retry++)
    {
        /*wait for 2 second*/
        sleep(2);
        len = 0;
        memset(buf, 0, MAX_SZ);
        if ((len = read(CDMAFd, buf, MAX_SZ)) > 0)
        {
            printf("%s\n", buf);

            if (!strstr(buf, meginputcode))
            {
                if ( retry == (RETRYMIN - 1))
                    return (-1);
            }
            else
                break;
        }
    }

    memcpy(buf, conv_buf, conv_len);
    msg_p = buf + conv_len;
    /*below is critical area*/
    memcpy(msg_p, ctrl_z, CTRL_Z_LEN);
    len = write(CDMAFd, buf, conv_len + CTRL_Z_LEN);
    if (len != (conv_len + CTRL_Z_LEN))
    {
        perror("write()");
        return (-1);
    }
    for (retry = 0; retry < RETRYMIN; retry++)
    {
        /*wait for 1 second*/
        sleep(1);
        len = 0;
        memset(buf, 0, MAX_SZ);
        if ((len = read(CDMAFd, buf, MAX_SZ)) > 0)
        {
            printf("%s\n", buf);

            if (!strstr((char *)buf, "OK"))
            {
                if ( retry == (RETRYMIN - 1))
                    return (-1);
            }
            else
                break;
        }
    }
    return (0);
}

int CNoteThread::CODEtoUNICODEBIG(char *in, size_t instrlen, char *out, size_t outbufsiz, char *codetype)
{
    char *in_p = in;
    char *out_p = out;
    size_t inbytesleft = instrlen;
    size_t outbytesleft = outbufsiz;
    size_t err = 0;
    iconv_t cd;

    cd = iconv_open("UNICODEBIG", codetype);
    if (cd == (iconv_t)(-1)) {
        perror("iconv_open()");
        return (-1);
    }

    err = iconv(cd, &in_p, &inbytesleft,
            &out_p, &outbytesleft);
    if (err == (size_t)(-1)) {
        perror("iconv()");
        return (-1);
    }

    err = iconv_close(cd);
    if (err == -1) {
        perror("iconv_close()");
        return (-1);
    }

    return (outbufsiz - outbytesleft);    /*output encode length*/
}

int CNoteThread::CDMASendInitModem(int CDMAFd)
{
    char cmd_buf[MAX_SZ];
    int retry;
    int len;

    /*flush data received and transimitted*/
    tcflush(CDMAFd, TCIOFLUSH);

    /*issue AT command*/
    memset(cmd_buf, 0, MAX_SZ);
    strncpy(cmd_buf, "AT\r", MAX_SZ);
    len = write(CDMAFd, cmd_buf, strnlen(cmd_buf, MAX_SZ));
    if (len != strnlen(cmd_buf, MAX_SZ))
    {
        perror("write()");
        return (-1);
    }

    for (retry = 0; retry < RETRYMIN; retry++)
    {
        /*wait for 1 second*/
        sleep(1);

        memset(cmd_buf, 0, MAX_SZ);
        if ((len = read(CDMAFd, cmd_buf, MAX_SZ)) < 0)
        {
            perror("read()");
            continue;
        }

        if (strstr(cmd_buf, "OK"))
            return (0);
    }

    return (-1);
}

bool CNoteThread::GPRSInitModem()
{
    int fd_gprs = open( "/dev/gprs", O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd_gprs < 0)
    {
        return false;
    }

    ioctl(fd_gprs, GPRS_RESET, 0);
}


bool CNoteThread::sendNoteProc(QString phone, QString content)
{
    bool ret = false;

    QByteArray phoneByte = phone.toUtf8();
    QByteArray contentByte = content.toUtf8();

    settty(fd_232_0, 115200, 8, 1, 0);

    if(CDMASendInitModem(fd_232_0) < 0)
    {
        return ret;
    }
    if(CDMASendSMS(fd_232_0,phoneByte.data() ,contentByte.data()) < 0)
    {

        return ret;
    }

    ret = true;

    return ret;

}

void CNoteThread::sendNoteEvent(NoteDesc_t noteInfo)
{
    CNoteEvent* noteEvt = new CNoteEvent(SEND_NOTE_TYPE, noteInfo);
    QApplication::postEvent(this,noteEvt);
}

bool CNoteThread::event(QEvent *event)
{
    if(event->type() == SEND_NOTE_TYPE)
    {
        CNoteEvent *myEvent = static_cast<CNoteEvent *>(event);

        myEvent->mNoteInfo.mSendTime = QDateTime::currentDateTime();
        if(sendNoteProc(myEvent->mNoteInfo.mMobilePhone,myEvent->mNoteInfo.mSmsContent))
        {
            myEvent->mNoteInfo.mStatus = 1;

            if(1 == myEvent->mNoteInfo.mSmsType)
            {
                SCDelivery::instance()->updateDeliveryNoteSendTimeInfo(myEvent->mNoteInfo.mRelPriKey,myEvent->mNoteInfo.mSendTime);
            }
            else if(12 == myEvent->mNoteInfo.mSmsType || 32 == myEvent->mNoteInfo.mSmsType) //寄件
            {
                SCSender::instance()->updateSenderNoteSendTimeInfo(myEvent->mNoteInfo.mRelPriKey,myEvent->mNoteInfo.mSendTime);
            }
            else if(21 == myEvent->mNoteInfo.mSmsType) //临时存储
            {
                SBasicTempStorage::instance()->updateStorageNoteSendTimeInfo(myEvent->mNoteInfo.mRelPriKey,myEvent->mNoteInfo.mSendTime);
            }
        }
        else
        {
            myEvent->mNoteInfo.mStatus = 2;
            myEvent->mNoteInfo.mSendCount++;
        }
        if(myEvent->mNoteInfo.mSmsLogId.isEmpty())
        {
            //保存发送给后台的信息
            QUuid uuId = QUuid::createUuid();
            QString strUuId = uuId.toString();
            strUuId = strUuId.remove(QRegExp("[{}-]"));
            strUuId = strUuId.toUpper();

            myEvent->mNoteInfo.mSmsLogId = strUuId;
        }
        SCSmslog::instance()->insertNoteInfo(myEvent->mNoteInfo);
    }
    return  QObject::event(event);
}
