set -xeuo pipefail
sed -i 's/jsoncpp_static/jsoncpp/g'  CMakeLists.txt
mkdir build
cd build   
cmake .. 
# jsoncpp path of ubunut apt installed and vpckg are diff 
export CPLUS_INCLUDE_PATH=/usr/include/jsoncpp/
make all
