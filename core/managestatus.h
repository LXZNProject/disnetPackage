#ifndef MANAGESTATUS_H
#define MANAGESTATUS_H
#include "status.h"
#include "managepage.h"
class CManageStatus: public CStatus
{
    Q_OBJECT
public:
    CManageStatus(STATUS status = ST_MANAGE);

    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();

private:

    managePage *m_managePage;
};

#endif // MANAGESTATUS_H
