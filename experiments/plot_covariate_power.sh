#!/bin/bash

if [ $# != 4 ]
then
    echo "Usage: plot_covariate_power.sh input_dir title ylabel output_dir"
    exit 1
fi

root_dir="$(dirname $0)/.."
input_dir=$1
title=$2
ylabel=$3
output_dir=$4

rscript "$root_dir/scripts/plot_covariate_power.r" $input_dir/model.out "$title" "$ylabel" "$output_dir/model.pdf"
