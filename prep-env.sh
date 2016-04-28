#!/bin/sh

BASE_DIR=$(pwd)/3pp
[ -d $BASE_DIR ] || mkdir $BASE_DIR

INSTALL_DIR=$BASE_DIR/installdir
mkdir  $INSTALL_DIR

gtest(){
   if [ -d googletest ]; then
     echo "user donloaded googletest"
   else
     git clone https://github.com/google/googletest.git  \
     && cd googletest \
     && cmake  -DCMAKE_INSTALL_PREFIX:PATH=$INSTALL_DIR \
     && make -j4 && make  install
     if [ $? != 0 ]; then
        exit $?
     fi
  fi
}

cd $BASE_DIR
gtest

