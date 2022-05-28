#!/bin/bash

set -xeuo  pipefail


export SONAR_SCANNER_VERSION=4.2.0.1873
export SONAR_SCANNER_HOME=$HOME/.sonar/sonar-scanner-$SONAR_SCANNER_VERSION-linux
rm -rf $SONAR_SCANNER_HOME
mkdir -p $SONAR_SCANNER_HOME
curl -sSLo $HOME/.sonar/sonar-scanner.zip https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-$SONAR_SCANNER_VERSION-linux.zip
unzip $HOME/.sonar/sonar-scanner.zip -d $HOME/.sonar/
rm $HOME/.sonar/sonar-scanner.zip
export PATH=$SONAR_SCANNER_HOME/bin:$PATH
export SONAR_SCANNER_OPTS="-server"

#download build wrapper 
curl -sSLo build-wrapper-linux-x86.zip   https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip
unzip  build-wrapper-linux-x86.zip

./build-wrapper-linux-x86/build-wrapper-linux-x86-64 --out-dir bw-outputs bash  ./devops/run-test.sh

sonar-scanner \
  -Dsonar.projectKey=trumanz_cppbind \
  -Dsonar.organization=trumanz \
  -Dsonar.sources=. \
  -Dsonar.host.url=https://sonarcloud.io \
  -Dsonar.login=ed79d3f3b5286851a2265f9b060d466dbba121d0
