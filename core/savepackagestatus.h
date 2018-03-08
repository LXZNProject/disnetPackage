#ifndef SAVEPACKAGESTATUS_H
#define SAVEPACKAGESTATUS_H

#include "status.h"
#include "savepackage.h"
#include <QTimer>

class CSavepackageStatus : public CStatus
{
    Q_OBJECT

public:
    CSavepackageStatus(STATUS status = ST_SAVE_DELIVERY_MENU);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);
    void cardInputProc(QByteArray param);

public slots:
    void timeOutComing();
    void jumpPage();
    void resetStart();
    void leaveSlots();

private:
    savepackage *savepage;
    QByteArray data;
    QTimer mTimer;
};

#endif // SAVEPACKAGESTATUS_H
