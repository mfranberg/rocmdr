#!/bin/bash

if [ $# != 4 ]
then
    echo "Usage: plot_covariate_power.sh input_dir title plot_type output_dir"
    exit 1
fi

root_dir="$(dirname $0)/.."
input_dir=$1
title=$2
plot_type=$3
output_dir=$4

rscript "$root_dir/scripts/plot_model_power.r" "$title" $input_dir/model.out "$output_dir/model.pdf" $plot_type
