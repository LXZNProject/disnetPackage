#ifndef MODIFYINFOSTATUS_H
#define MODIFYINFOSTATUS_H
#include "status.h"
#include "modifyinfopage.h"
class CModifyInfoStatus: public CStatus
{
    Q_OBJECT
public:
    CModifyInfoStatus(STATUS status = ST_MODIFY_INFO);
    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();
private:
   modifyINfoPage *modifyPage;
};

#endif // MODIFYINFOSTATUS_H
