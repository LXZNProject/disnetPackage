#ifndef CABINETCELLSTATUS_H
#define CABINETCELLSTATUS_H

#include "status.h"
#include "cabinetcellpackage.h"

class CabinetCellStatus: public CStatus
{
public:
    CabinetCellStatus(STATUS status = ST_CABINET_CELL_SET);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);
    void cardInputProc(QByteArray param);


private:
    cabinetCellPackage *mcabinetCellPackage;
};

#endif // CABINETCELLSTATUS_H
