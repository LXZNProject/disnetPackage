#ifndef T_IMAGE_TIME_H
#define T_IMAGE_TIME_H


#include "singleton.h"
#include <QDateTime>

class CImageTime
{
public:
     QString mHomeTime;
     QString mAdvertTime;
public:
    CImageTime();

    void init();
    void getImageTimeInfo();
    void updateImageTimeInfo();

    DECLARE_SINGLETON_CLASS(CImageTime)

};

typedef Singleton<CImageTime> SCImageTime;

#endif // T_IMAGE_TIME_H
