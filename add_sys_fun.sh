#!/bin/bash
path=/usr/src/minix
yes | cp -rf ./unistd.h root@localhost:/usr/include/
yes | cp -rf ./unistd.h root@localhost:/usr/src/include/
yes | cp -rf  ./callnr.h root@localhost:/usr/include/minix/
yes | cp -rf ./callnr.h root@localhost:$path"/include/minix/"
yes | cp -rf ./pm root@localhost:$path"servers" 
yes | cp -rf ./check_if_ancestor.c root@localhost:/usr/src/lib/libc/misc
yes | cp -rf ./Makefile.inc root@localhost:/usr/src/lib/libc/misc
