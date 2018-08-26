#!/bin/bash
path=/usr/src/minix
scp -r -P 15892 ./unistd.h root@localhost:/usr/include/
scp -r -P 15892 ./unistd.h root@localhost:/usr/src/include/
scp -r -P 15892 ./callnr.h root@localhost:/usr/include/minix/
scp -r -P 15892 ./callnr.h root@localhost:$path"/include/minix/"
scp -r -P 15892 ./pm root@localhost:$path"servers" 
