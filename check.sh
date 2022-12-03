#!/bin/bash

OUTFILE=cppcheck.xml
JOBS=

if [ -z "$1" ]; then
    JOBS=1
else
    JOBS=$1
fi

echo "Job count: " $JOBS

echo "Cleaning old file..."
rm -f $OUTFILE

echo "Starting cppcheck"

time cppcheck --std=c++11 --enable=all --inconclusive \
    --xml --xml-version=2 graph 2 -j $JOBS -Iinclude/cmm src include 2> $OUTFILE
echo "Done"

