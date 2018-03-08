#ifndef TEMPSTORAGESTATUS_H
#define TEMPSTORAGESTATUS_H
#include "status.h"
#include "tempstorage.h"
class CtempStorageStatus: public CStatus
{
    Q_OBJECT
public:
    CtempStorageStatus(STATUS status = ST_TEMP_STORAGE);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void cardInputProc(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();
private:
    tempStorage  * tempStoragePage;

};

#endif // TEMPSTORAGESTATUS_H
