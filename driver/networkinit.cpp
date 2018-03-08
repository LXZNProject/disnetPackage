#include "networkinit.h"
#include <QSettings>
#include <QStringList>
   networkInit::networkInit(QObject *parent) :
       QObject(parent)
   {
       init();
   }
   void networkInit::init()
   {
       QSettings *configIniWrite = new QSettings("networkConfig.ini", QSettings::IniFormat);
       QStringList keys = configIniWrite->allKeys();
       if(keys.size() < 6)
       {
           configIniWrite->setValue("/pingIP/ip", "www.baidu.com");
           configIniWrite->setValue("/network/level", "0");
           configIniWrite->setValue("/network/ipaddr", "192.168.1.100");
           configIniWrite->setValue("/network/netmask", "255.255.255.0");
           configIniWrite->setValue("/network/defaultGw", "192.168.1.1");
           configIniWrite->setValue("/network/dns1", "192.168.1.1");
           configIniWrite->setValue("/network/dns2", "114.114.114.114");
       }
       delete configIniWrite;
   }
   QString networkInit::getNetworkMethods()
   {
       QSettings *configIniRead = new QSettings("networkConfig.ini", QSettings::IniFormat);
       QString level = configIniRead->value("/network/level").toString();
       delete configIniRead;
       return level;
   }

   QStringList networkInit::getNetwork()
   {
       QStringList tempValueList;
       QSettings *configIniRead = new QSettings("networkConfig.ini", QSettings::IniFormat);
       tempValueList << configIniRead->value("/pingIP/ip").toString();
       tempValueList << configIniRead->value("/network/level").toString();
       tempValueList << configIniRead->value("/network/ipaddr").toString();
       tempValueList << configIniRead->value("/network/netmask").toString();
       tempValueList << configIniRead->value("/network/defaultGw").toString();
       tempValueList << configIniRead->value("/network/dns1").toString();
       tempValueList << configIniRead->value("/network/dns2").toString();
       delete configIniRead;
       return tempValueList;
   }

   void networkInit::setNetwork(QStringList setValue)
   {
       int tempInit = 0;
       QSettings *configIniWrite = new QSettings("networkConfig.ini", QSettings::IniFormat);
       configIniWrite->setValue("/pingIP/ip", setValue.at(tempInit++));
       configIniWrite->setValue("/network/level", setValue.at(tempInit++));
       configIniWrite->setValue("/network/ipaddr", setValue.at(tempInit++));
       configIniWrite->setValue("/network/netmask", setValue.at(tempInit++));
       configIniWrite->setValue("/network/defaultGw", setValue.at(tempInit++));
       configIniWrite->setValue("/network/dns1", setValue.at(tempInit++));
       configIniWrite->setValue("/network/dns2", setValue.at(tempInit++));
       delete configIniWrite;
   }

   void networkInit::connetNetwork()
   {
        system("(sh mynetwork.sh &) >> debuglog.txt");
   }

   void networkInit::disconnectNetwork()
   {
       int index  = getNetworkMethods().toInt();
       switch(index)
       {
        case 0:
           break;
       case 1:
           system("killall -KILL pppd");
           break;
       case 2:
           system("killall udhcpc");
           system("ifconfig eth0 down");
           break;
       case 3:
           system("ifconfig eth0 down");
           break;
       }
   }


