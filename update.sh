# !bin/bash

#cd /home/development/lxyz/smartpackage/update
#unzip Qt.zip
#./smartpackage

#killall portal-arm
#./portal-arm -qws &
#./hardware &
#./smartpackage
cd /usr/lxyz
rm -f disnet-arm
unzip -o file/Qt.zip
sleep 15
chmod 755 disnet-arm
rm file/Qt.zip
reboot
