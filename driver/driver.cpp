#include <QDebug>
#include <QByteArray>
#include <QEventLoop>
#include <QStringList>
#include <QString>
#include <QProcess>
#include <QDateTime>
#include "driver.h"
#include "db.h"

#include <QDir>




extern qint8 gIndex;

CDriver::CDriver()
{
   // mServer = new Tcp::Server;
   // mObserver = new LxProtocolParser;
  //  mServer->addObserver(mObserver);
   // mMutex  = new QMutex;

    mWaitRespCommand = 0;
    mRespCommand     = 0;
    isWorking = false;
    isUpsWorking = false;

    /*
    QDir dir("/lxyz/image/");

    QStringList nameFilter;
    nameFilter << "*.gif";
    QStringList images = dir.entryList(nameFilter, QDir::NoFilter, QDir::Time);

    for (int i = 0; i < images.size(); i++)
        postFile(url ,  "/lxyz/image/" + images[i]);
*/
}



CDriver::~CDriver()
{
    soundProcess->close();
    videoProcess->close();
    delete soundProcess;
    delete videoProcess;
}

void CDriver::init()
{
    //mServer->start(serverPort);
    soundProcess = new QProcess;
    videoProcess = new QProcess;


    connect(videoProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(deletimage()));

}


/*
void CDriver::onCommandProc(qint8 index, qint8 cmd, QByteArray data)
{
    mRespCommand = cmd;
    mRespData = data;

  //  qDebug() << "index: "<< index << "cmd: " << cmd << "data: " << data.toHex();

    if(cmd == mWaitRespCommand + 1 && gIndex == index)
        emit signalProtoclResp();

    switch(cmd){
    case LX_KEY_IPUT:
        emit keyInput(data);
        break;
    case LX_M1_CARD_INPUT:
        emit cardInput(data);
        break;
    case LX_COIN_INPUT:
        emit coinInput(data);
        break;
    case LX_UPS_INPUT:
        emit upsInput(data);
        break;
    }
}

*/


void CDriver::deletimage()
{
    if(photoStatus)
    {
        system("rm ./picture/*.bmp");
    }
}
void CDriver::playVoice(QString mp3File, QString volume)
{
    QStringList list;
    list.append(mp3File);
    list.append("-A");
    list.append(volume);
    soundProcess->start("madplay", list);

   // mProcess->start("mplayer", list);
    //QProcess::execute("madplay", list);
}


void CDriver::catchVideo(QString imageFile)
{  
    QStringList list;

    imageFile = "/usr/lxyz/picture/" + imageFile + ".gif";
    list.append(imageFile);
    videoProcess->start("capture",list);

}


