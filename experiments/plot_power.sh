#!/bin/bash

if [ $# != 1 ]
then
    echo "Usage: plot_power.sh levels"
    exit 1
fi

levels=$1
root_dir="$(dirname $0)/.."
result_dir="$root_dir/results/power2"

join_model="$root_dir/scripts/join_model_files.sh"

i=0
for model in {"Homozygot","Side","XOR","Linear","Independent"} 
do
    echo $model
    echo $i
    eval $join_model $result_dir/rocmdr_model{$((i*levels+1))..$(((i+1)*levels))}.out > "$result_dir/rocmdr_joint$((i+1)).out"
    eval $join_model $result_dir/glm_model{$((i*levels+1))..$(((i+1)*levels))}.out > "$result_dir/glm_joint$((i+1)).out"
    Rscript "$root_dir/scripts/plot_model_power.R" \"$model\" 0.05 $result_dir/{rocmdr,glm}_joint$((i+1)).out "$result_dir/model$((i+1)).pdf" &> /dev/null
    ((i+=1))
done
