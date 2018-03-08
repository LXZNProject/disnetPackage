#ifndef OPENEMPTYCELLSTATUS_H
#define OPENEMPTYCELLSTATUS_H
#include "status.h"
#include "openemptycellpage.h"
class COpenEmptyCellStatus: public CStatus
{
    Q_OBJECT
public:
    COpenEmptyCellStatus(STATUS status = ST_OPENEMPTYDOOR);

    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();
private:

       openEmptyCellPage *m_openEmptyCellPage;
};

#endif // OPENEMPTYCELLSTATUS_H
