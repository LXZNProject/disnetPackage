#ifndef NOTESETSTATUS_H
#define NOTESETSTATUS_H
#include "status.h"
#include "notetemplate.h"
class CNoteSetStatus: public CStatus
{
public:
    CNoteSetStatus(STATUS status = ST_NOTE_SET);
    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);
    void cardInputProc(QByteArray param);

private:
    notetemplate *notetemplatePage;
};

#endif // NOTESETSTATUS_H
