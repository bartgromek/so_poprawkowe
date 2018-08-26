#!/bin/bash
cd /usr/src/minix/servers/pm 
make && make install
cd /usr/src/releasetools 
make do-hdboot

