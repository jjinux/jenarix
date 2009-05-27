#!/bin/sh

# compute the output filename
output=$2/`echo $1 | sed "s/jx_/$3_/"`
# create the output file
sed "s/jx_/$3_/g;s/JX_/$4_/g;s/Jx_/$5_/g" < $1 >> $output
echo "created $output"


