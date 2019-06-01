#!/usr/bin/env bash
echo "Test 2  arguments test"
if ! ./server 27015 27015 27015
then
    echo -e "Test passed\n"
else
    echo -e "Test failed\n"
    exit 1
fi
