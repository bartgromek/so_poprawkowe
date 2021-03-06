#!/bin/bash
path=/usr/src/minix
yes | cp -rf ./unistd.h /usr/include/
yes | cp -rf ./unistd.h /usr/src/include/
yes | cp -rf ./callnr.h /usr/include/minix/
yes | cp -rf ./callnr.h $path"/include/minix/"
yes | cp -rf ./pm $path/"servers" 
yes | cp -rf ./check_if_ancestor.c /usr/src/lib/libc/misc
yes | cp -rf ./Makefile.inc /usr/src/lib/libc/misc
yes | cp -rf ./system.conf /etc/system.conf
yes | cp -rf ./clipboard /usr/src/minix/drivers/examples
yes | cp -rf ./dmap.h /usr/src/minix/include/minix/
mknod /dev/clipboard c 17 0
