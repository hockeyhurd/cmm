#!/bin/bash

BUILD_DIR=$1

if [ -z "$1" ]; then
    BUILD_DIR='build'
fi

echo "BUILD_DIR: " $BUILD_DIR

function unitTests()
{
    #echo $PWD
    local testFiles=`find $BUILD_DIR -type f -name "*Test" -print`
    echo "Tests to run: " $testFiles

    for test in $testFiles
    do
        echo "Starting test: " $test
        $test
        retVal=$?
        echo "Test: " $test " completed with exit code: " $retVal

        if [ $retVal -ne 0 ]; then
            exit $retVal
        fi
    done
}

time unitTests

echo "Done"

