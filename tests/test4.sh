#!/usr/bin/env bash
echo "Test 4 - Connection with client"
if [[ $((./LinuxServer 8321) & (./LinuxClient)) == *"Test"* ]]
then
    echo -e "Test passed\n"
else
    echo -e "Test failed\n"
    exit 1
fi
