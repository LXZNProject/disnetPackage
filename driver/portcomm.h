#ifndef PORTCOMM_H
#define PORTCOMM_H

#include <QObject>
#include "qextserialport.h"
#include "singleton.h"

class portcomm : public QObject
{
    Q_OBJECT
public:
    explicit portcomm(QObject *parent = 0);
    void init();

    qint8 queryCardInfo(QStringList &readInfoList);  //-2 空卡 -1 读卡失败 0 不是该公司人 1 卡被禁用 2 成功
    bool writeCardInfo(QString tempWriteData);
    bool checkWriteCard(QByteArray readData);
    bool readCardNumber(QString &tempData);

private:
    qint8 Crc16Value(QByteArray data);
    bool checkCrc16Value(QByteArray tempArray);

    QByteArray qstringToByte(const QString &strInfo);
    QString byteToQString(const QByteArray &byte);
    QString getXorEncryptDecrypt(const QString &str, const char &key);

private:
    QString comName;
    QextSerialPort *myCom;
    char key;

    DECLARE_SINGLETON_CLASS(portcomm)
};

typedef Singleton<portcomm> SCPortComm;


#endif // PORTCOMM_H
