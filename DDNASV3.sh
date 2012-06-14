#!/bin/bash
PWD=`pwd`
CONFIG_DIR="$PWD"/DDNASCONFIG

echo Now,mv DDNASV_defconfig.config to .config

cp $CONFIG_DIR/DDNASV_defconfig.config $PWD
make V=99
echo make V=99, first time, dot use make V=99 -jx


