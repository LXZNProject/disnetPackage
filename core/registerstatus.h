#ifndef REGISTERSTATUS_H
#define REGISTERSTATUS_H
#include "status.h"
#include "registerpackage.h"
class CRegisterStatus: public CStatus
{
    Q_OBJECT
public:
    CRegisterStatus(STATUS status = ST_REGISTER);
    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();
private:
   registerPackage *registerpage;
};

#endif // REGISTERSTATUS_H
