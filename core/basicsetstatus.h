#ifndef BASICSETSTATUS_H
#define BASICSETSTATUS_H
#include "status.h"
#include "terminal.h"
class CBasicSetStatus: public CStatus
{
public:
    CBasicSetStatus(STATUS status = ST_BASIC_SET);
    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);
    void cardInputProc(QByteArray param);


private:
    terminal *mterminal;
};

#endif // BASICSETSTATUS_H
