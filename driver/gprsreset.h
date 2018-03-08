#ifndef GPRSRESET_H
#define GPRSRESET_H
#include <QObject>
#include <QTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>

#include "singleton.h"

class gprsReset: public QObject
{
    Q_OBJECT
public:
    gprsReset(QObject *parent = 0);

   void reset();

private:
   int fd_watchdog;

   DECLARE_SINGLETON_CLASS(gprsReset)
};
typedef Singleton<gprsReset> SGprsReset;

#endif // GPRSRESET_H
