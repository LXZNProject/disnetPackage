#include "cabinetcellstatus.h"

CabinetCellStatus::CabinetCellStatus(STATUS status):
    CStatus(status)
{
    mcabinetCellPackage =new cabinetCellPackage();
}

void CabinetCellStatus::enter(QByteArray param)
{
    mcabinetCellPackage->readInfo();
    mcabinetCellPackage->show();
}

void CabinetCellStatus::leave(QByteArray param)
{
    mcabinetCellPackage->hide();
}

void CabinetCellStatus::coinInputProc(QByteArray param)
{
}

void CabinetCellStatus::cardInputProc(QByteArray param)
{
}
