#include "portcomm.h"

#include <QDebug>
#include <QByteArray>
#include <QThread>
#include <QTextCodec>
#include <QStringList>
#include <QString>
#include <QThread>
#include "stdio.h"
#include "stdlib.h"
#include "t_basic_terminal.h"
#include "t_basic_sync_forbidden.h"

#define  STARTBLOCK 1  //读写的开始块
#define  RWBLOCK  14   //读写数据块的个数
portcomm::portcomm(QObject *parent) :
    QObject(parent)
{
    myCom = NULL;
    key = 11;
}
void portcomm::init()
{
    if(myCom)
    {
        myCom->close();
        myCom = NULL;
    }
    comName = "/dev/ttyO1";

    myCom = new QextSerialPort(comName);
    myCom->open(QIODevice::ReadWrite);
    myCom->setBaudRate(BAUD9600);
    myCom->setDataBits(DATA_8);
    myCom->setParity(PAR_NONE);
    myCom->setStopBits(STOP_1);
    myCom->setFlowControl(FLOW_OFF);
    myCom->setTimeout(0,100);
}

qint8 portcomm::queryCardInfo(QStringList &readInfoList)
{
    QByteArray sendData;
    sendData.append(0x01);
    sendData.append(0x08);
    sendData.append(0xAB);
    sendData.append(0x20);
    sendData.append(STARTBLOCK);
    sendData.append((char)0x00); //蜂鸣器开关
    sendData.append(RWBLOCK);
    sendData.append(Crc16Value(sendData));

   //qDebug() << "queryCardInfor write :" << sendData.toHex();
    myCom->write(sendData);

    usleep(60000);

    QByteArray readData = myCom->readAll();

    qDebug() << "queryCardInfor readAll:" << readData.toHex();

    //首先校验CRC
    bool recv = checkCrc16Value(readData);
    if(!recv)
    {
        //qDebug() << "crc is error!";
         return -1;
    }

    int len = (unsigned char)readData.at(1);
    if(len != readData.size())
    {
  //      qDebug() << "len is error!";
        return -1;
    }

    if((unsigned char)readData.at(4) != 0)
    {
   //     qDebug() << "read is error!";
        return -1;
    }
    readData.remove(0,5);

    readData.remove((readData.size() -1),1);

    QString tempStr = QString::fromAscii(readData.data());

    tempStr = getXorEncryptDecrypt(tempStr,key);

    qDebug() << "tempStr:" << tempStr;

    readInfoList = tempStr.split("|");
    readInfoList.removeLast();

    if(readInfoList.isEmpty())
    {
        return -2;
    }


    if(readInfoList.size() > 8)
    {
        if((readInfoList.at(0) != "1") && (readInfoList.at(0) != "8"))
        {
            if(!SCTerminal::instance()->terminalInfo.companyId.startsWith(readInfoList.at(1)))
            {
                return 0;
            }
        }

        if(SBalckList::instance()->lossUserInfo.contains(readInfoList.at(7)))
        {
            return 1;
        }
    }

    return 2;
}

bool portcomm::writeCardInfo(QString tempWriteData)
{
    bool ret = false;
    QStringList tempList = tempWriteData.split("|");
    if(tempList.at(7).size() != 32)
    {
        return false;
    }


    QByteArray sendData;
    tempWriteData = getXorEncryptDecrypt(tempWriteData,key);
    quint8 dataLen = 8 + tempWriteData.size();

    int blockNum = tempWriteData.size()/16;
    int elseNum = tempWriteData.size()%16;
    if(elseNum > 0)
    {
        blockNum++;
    }

    sendData.append(0x01);
    sendData.append(dataLen);
    sendData.append(0xAA);
    sendData.append(0x20);
    sendData.append(STARTBLOCK);
    sendData.append((char)0x01);
    sendData.append(blockNum);
    sendData.append(tempWriteData);

    sendData.append(Crc16Value(sendData));

    qDebug() << "writeCardInfor write :" << sendData.toHex();

    int i = 0;
    do
    {
        myCom->write(sendData);
        usleep(350000);
        QByteArray readData = myCom->readAll();

        qDebug() << "writeCardInfor readAll :" << readData.toHex();

        ret = checkWriteCard(readData);
        if(ret)
        {
            break;
        }

        usleep(10000);
        i++;
    }while(i < 3);

    return ret;
}

bool portcomm::checkWriteCard(QByteArray readData)
{
    bool recv = checkCrc16Value(readData);
    if(!recv)
    {
        qDebug() << "CRC is error!";
        return false;
    }

    int len = (unsigned char)readData.at(1);
    if(len != readData.size())
    {
        qDebug() << "len is error!";
        return false;
    }
    if(readData.at(4)!=0) //0: success 1: fail
    {
        qDebug() << "write is error!";
        return false;
    }

    return true;
}

bool portcomm::readCardNumber(QString &tempData)
{
    QByteArray sendData;
    sendData.append(0x01);
    sendData.append(0x08);
    sendData.append(0xA1);
    sendData.append(0x20);
    sendData.append((char)0x00);
    sendData.append((char)0x00); //蜂鸣器开关
    sendData.append((char)0x00);
    sendData.append(Crc16Value(sendData));

    //qDebug() << "queryCardInfor write :" << sendData.toHex();
    myCom->write(sendData);

    //usleep(60000);

    QByteArray readData = myCom->readAll();

    //qDebug() << "queryCardInfor readAll :" << readData.toHex();

    bool recv = checkCrc16Value(readData);
    if(!recv)
    {
 //       qDebug() << "CRC is error!";
        return false;
    }

    int len = (unsigned char)readData.at(1);
    if(len != readData.size())
    {
  //      qDebug() << "len is error!";
        return false;
    }

    if((unsigned char)readData.at(4) != 0)
    {
   //     qDebug() << "read is error!";
        return false;
    }
    if((unsigned char)readData.at(1) != 0x0C)
    {
   //     qDebug() << "read is error!";
        return false;
    }

    unsigned int u_iABA10 = 0;
    unsigned char index = 0;
    for(index = 7; index < 11; index++)
    {
        u_iABA10 |= ((unsigned char)readData.at(index) << ((index - 7) * 8));
    }

    tempData =  QString::number(u_iABA10,10);
    return true;
}

qint8 portcomm::Crc16Value(QByteArray data)
{
    qint8 checksum = 0;
    int lenth = (quint8)data.at(1) - 1;

    for(int i = 0;i<= lenth; i++)
    {
        checksum ^= data.at(i); //异或
    }
    checksum = ~checksum;
    return checksum;
}

bool portcomm::checkCrc16Value(QByteArray tempArray)
{
    qint8 checksum = 0;
    int len = (quint8)tempArray.at(1) - 1;

    for(int i = 0; i < len; i++)
    {
      checksum ^= (qint8)tempArray.at(i); //异或
    }
    checksum = ~checksum;

    if(checksum == (qint8)tempArray.at(len))
    {
       return true;
    }
    return false;
}

QString portcomm::getXorEncryptDecrypt(const QString &str, const char &key)
{
  QString result;
  QByteArray bs = qstringToByte(str);

  for(int i=0; i<bs.size(); i++){
    bs[i] = bs[i] ^ key;
  }

  result = byteToQString(bs);
  return result;
}

QString portcomm::byteToQString(const QByteArray &byte)
{
  QString result;
  if(byte.size() > 0){
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    result = codec->toUnicode(byte);
  }

  return result;
}

QByteArray portcomm::qstringToByte(const QString &strInfo)
{
  QByteArray result;
  if(strInfo.length() > 0){
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    result = codec->fromUnicode(strInfo);
  }
  return result;
}
