#include "openemptycellstatus.h"

COpenEmptyCellStatus::COpenEmptyCellStatus(STATUS status):
    CStatus(status)
{
    m_openEmptyCellPage = new openEmptyCellPage();
    connect(m_openEmptyCellPage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void COpenEmptyCellStatus::enter(QByteArray param)
{
    openTimer();
    m_openEmptyCellPage->show();
}

void COpenEmptyCellStatus::leave(QByteArray param)
{
    stopTimer();
    m_openEmptyCellPage->hide();
}

void COpenEmptyCellStatus::resetStart()
{
    clearTimer();
}

void COpenEmptyCellStatus::jumpPage()
{
    m_openEmptyCellPage->cancelBtnClicked();
}

