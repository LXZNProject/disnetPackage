#include "httpupdate.h"
#include "md5.h"

#include <QUuid>
#include <QRegExp>
#include <QDebug>
#include <istream>

httpUpdate::httpUpdate(QObject *parent) :
    QObject(parent)
{
}


bool httpUpdate::getAppInfo(QString url,QString fileMd5,qint8 status)
{
    bool ret = false;

    nam = new QNetworkAccessManager(this);

    QNetworkRequest        request;
    request.setUrl(QUrl(url));

    QString updateName = "./file/";
    if(1 == status)
    {
        updateName = updateName + "Qt.zip";
    }
    else if(2 == status)
    {
        updateName = updateName + "homepage.png";
    }

    QFile f(updateName);
    if (f.open(QIODevice::WriteOnly))
    {
        reply = nam->get(request);
        QEventLoop loop;//事件机制，变为同步
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();// 进入事件循环， 直到reply的finished()信号发出， 这个语句才能结束

        readRespone(reply,f);
    }


    MD5 md5;
    md5.reset();
    std::ifstream file(updateName.toAscii());
    md5.update(file);
    if(md5.toString().c_str() == fileMd5)
    {
        ret = true;
    }

    qDebug() << "down MD5: " << md5.toString().c_str();
    delete nam;

    return ret;
}

void httpUpdate::readRespone(QNetworkReply *reply,QFile &f)
{
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();


    if (status == 200)
    {
        f.write(reply->readAll());
        f.close();
    }
    reply->close();
    delete reply;
}
