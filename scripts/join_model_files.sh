#!/bin/bash

i=1
for file in $@
do
    cmd_string="{ print \$0, \"\t\", $i }"
    tail -n +2 $file | awk "$cmd_string"

    let "i=$i+1"
done
