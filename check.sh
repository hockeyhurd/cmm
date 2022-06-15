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
# Note: normally --enable=all is what we want, but since we are planning to use the script with the HSON library,
# we don't want errors such as 'unusedFunction' to be enabled.
time cppcheck --std=c++11 --enable=warning,style,performance,portability,information,missingInclude --inconclusive \
    --xml --xml-version=2 graph 2 -j $JOBS -Iinclude/cmm src include 2> $OUTFILE
echo "Done"

