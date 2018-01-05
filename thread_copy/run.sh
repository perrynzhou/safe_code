#!/bin/bash
for i in {1..40}
do
    ./test -n=$i mp4 log$i
done
for i in {1..40}
do
    md5sum log$i
done


