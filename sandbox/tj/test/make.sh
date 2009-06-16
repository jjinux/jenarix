#!/bin/sh -v

gcc -I../../../trunk/src/libjx/include main_jxl.c show_ob.c ../../../trunk/src/libjx/libjx.a -o ctest

g++ -I../../../trunk/src/libjx/include main_jxl.cpp show_ob.cpp ../../../trunk/src/libjx/libjx.a -o cpptest

echo -e "now try:\n\n./ctest < drugs.json\n\n   and\n\n./cpptest < drugs.json"


