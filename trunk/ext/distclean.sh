#!/bin/sh

BASE=`pwd`
export BASE

if [ ! -e ${BASE}/distclean.sh ]; then
echo "please run as ./distclean.sh from the ext directory"
exit
fi

/bin/rm -f ${BASE}/bin/lemon \
    ${BASE}/bin/re2c \
    ${BASE}/bin/re2c \
    ${BASE}/share/man/man1/re2c.1

/bin/rm -rf ${BASE}/build

/bin/rmdir ${BASE}/bin 
/bin/rmdir ${BASE}/share/man/man1
/bin/rmdir ${BASE}/share/man
/bin/rmdir ${BASE}/share




