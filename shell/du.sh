#!/bin/bash
xx=$(find ./ -type d -maxdepth 1)

for x in $xx
do
        du -sh $x
done
