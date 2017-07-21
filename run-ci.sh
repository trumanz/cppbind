#!/bin/bash
#cat /proc/cpuinfo
#cat /proc/meminfo
source source.env
make clean
make  all && cd test && make clean; make test
