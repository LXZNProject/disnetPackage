#ifndef HTTPUPDATE_H
#define HTTPUPDATE_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QFile>
#include <QEventLoop>

class httpUpdate : public QObject
{
    Q_OBJECT
public:
    explicit httpUpdate(QObject *parent = 0);

    bool getAppInfo(QString url,QString fileMd5,qint8 status);

    
signals:
    
public slots:

private:
    QNetworkReply*         reply;
    QNetworkAccessManager* nam;
    void readRespone(QNetworkReply *reply,QFile &f);
    
};

#endif // HTTPUPDATE_H
