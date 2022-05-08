#!/usr/bin/env bash

set -e
set -o pipefail
set -u
set -x

BASEDIR=$(dirname "$0")
cd $BASEDIR

doxygen doxygen.cfg