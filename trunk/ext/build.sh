#!/bin/sh

BASE=`pwd`
export BASE

if [ ! -e ${BASE}/build.sh ]; then
echo "please run as ./build.sh from the ext directory"
exit
fi

install -d ${BASE}/bin
install -d ${BASE}/build

cd ${BASE}/bin
gunzip < ${BASE}/src/tkdiff.gz > tkdiff
chmod 755 tkdiff

cd ${BASE}/build

tar zxvf ../src/lemon.tgz

cd lemon
gcc -O3 -o lemon lemon.c
install lemon ${BASE}/bin/lemon

/bin/rm lemon
cd ..

tar zxvf ../src/re2c-0.12.3.tgz

cd re2c-0.12.3
./configure --prefix=${BASE}
make 
make install
make distclean


echo "Jenarix EXT build complete"

