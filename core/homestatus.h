#ifndef HOMESTATUS_H
#define HOMESTATUS_H

#include "status.h"
#include "mainctrl.h"
#include "disnetpackage.h"

class CHomeStatus : public CStatus
{
     Q_OBJECT
public:
    CHomeStatus(STATUS status = ST_HOME);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);
    void cardInputProc(QByteArray param);

    bool specialcard(QByteArray);

signals:


private:
    disnetPackage*   mPage;
    QTimer          *openQueryTimer;
};

#endif // HOMESTATUS_H
