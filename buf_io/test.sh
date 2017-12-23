#!/bin/bash
rm -rf log
./test 1 1000000
rm -rf log
./test 0 1000000
