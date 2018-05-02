#!/bin/bash
HOSTNAME=`hostname`
echo "Friendly name changed : $HOSTNAME"
sed -e 's/TC_LINUX_BT/'$HOSTNAME'/g' ./TC_BT/inc/BD_ADDR.h > ./TC_BT/inc/tmp_BD_ADDR.h
mv ./TC_BT/inc/tmp_BD_ADDR.h ./TC_BT/inc/BD_ADDR.h
