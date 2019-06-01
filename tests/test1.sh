#!/usr/bin/env bash
echo "Test 1 - launch"
if !./server
then 
    echo -e "Test passed\n"
    exit 1
else 
    echo -e "Test not passed"
fi