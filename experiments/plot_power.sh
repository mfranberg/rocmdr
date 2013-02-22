#!/bin/bash

if [ $# != 3 ]
then
    echo "Usage: plot_power.sh input_dir model output_dir"
    exit 1
fi

root_dir="$(dirname $0)/.."
input_dir=$1
model_file=$2
output_dir=$3

i=1
awk 'NF == 2' $model_file |
while read method plot_type;
do
    rscript "$root_dir/scripts/plot_model_power.r" \"$method\" $input_dir/model$i.out "$output_dir/model$i.pdf" $plot_type #&> /dev/null
    ((i+=1))
done
