#include "managestatus.h"

CManageStatus::CManageStatus(STATUS status):
    CStatus(status)
{
    m_managePage = new managePage();
    connect(m_managePage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CManageStatus::enter(QByteArray param)
{
    openTimer();
    m_managePage->show();
}

void CManageStatus::leave(QByteArray param)
{
    stopTimer();
    m_managePage->hide();
}

void CManageStatus::resetStart()
{
    clearTimer();
}

void CManageStatus::jumpPage()
{
    m_managePage->cancelBtnClicked();
}

