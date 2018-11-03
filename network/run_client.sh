#!/bin/bash
for i in {0..10}
do
    netcat localhost 5656|wc -c
done
