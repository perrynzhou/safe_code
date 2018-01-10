#!/bin/bash
for i in {1..6}
do
    echo "========task  running thread {$i}=========="
    time ./test -n=$i data log$i
    echo "finish {$i}"
done
for i in {1..6}
do
    md5sum log$i
done

echo "------using cp command------"
time cp data log_cp
md5sum log_cp


