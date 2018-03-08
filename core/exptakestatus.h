#ifndef EXPTAKESTATUS_H
#define EXPTAKESTATUS_H

#include "status.h"
#include "exptakepage.h"

class CExpTakeStatus : public CStatus
{
    Q_OBJECT
public:
    CExpTakeStatus(STATUS status = ST_COURIER_PACK);

    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();
private:
    exptakepage *page;

};

#endif // EXPTAKESTATUS_H
