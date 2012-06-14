#!/bin/bash
PWD=`pwd`
CONFIG_DIR="$PWD"/DDNASCONFIG

echo Now,mv DDNAS_defconfig.config to .config
cp $CONFIG_DIR/DDNAS_defconfig.config $PWD
echo mkdir dl,and cp linux-2.6.31.8.tar.bz2 to dl
cp $PWD/linux-2.6.31.8.tar.bz2 $PWD/dl/

make V=99
echo make V=99, first time, dot use make V=99 -jx


