#!/bin/bash
cat /proc/cpuinfo
cat /proc/meminfo

source source.env
make clean; make  all && make -f Makefile.test clean; make -f Makefile.test test
