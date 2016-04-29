#!/bin/bash
cat /proc/cpuinfo
cat /proc/meminfo

source source.env
make  test
