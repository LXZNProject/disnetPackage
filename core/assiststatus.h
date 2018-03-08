#ifndef ASSISTSTATUS_H
#define ASSISTSTATUS_H

#include "status.h"
#include "assistpage.h"
class CAssistStatus : public CStatus
{
    Q_OBJECT
public:
    CAssistStatus(STATUS status = ST_ASSIST);

    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();

private:
    assistpage *page;
};

#endif // ASSISTSTATUS_H
