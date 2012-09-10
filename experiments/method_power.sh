#!/bin/bash

root_dir="$(dirname $0)/.."

# Gather parameters
while getopts "a:b:d:h:n:" opt;
do
    case $opt in
        a) maf1=$OPTARG ;;
        b) maf2=$OPTARG ;;
        d) ncases=$OPTARG ;;
        h) ncontrols=$OPTARG ;;
        n) npairs=$OPTARG ;;
        *) echo "Usage: method_power -a maf1 -b maf2 -d ncases -h ncontrols -n npairs model_file output_dir" ; exit 1 ;;
    esac
done
shift $((OPTIND-1))

# Check that we got all arguments
if [ $# != 2 ]
then
    echo "Usage: method_power -a maf1 -b maf2 -d ncases -h ncontrols -n npairs model_file output_dir"
    exit 1
fi

if [ ! "$maf1" ] || [ ! "$maf2" ] || [ ! "$ncases" ] || [ ! "$ncontrols" ] || [ ! "$npairs" ];
then
    echo "error: Option -a, -b, -d, -h or -n not specified."
    exit 1
fi

# Contains models
model_file=$1

# Output directory
output_dir=$2

# Path to methods to test
generate_path="$root_dir/tools/generate_ped/generate_ped.py"
glm_path="$root_dir/scripts/glm_interaction.r"
rocmdr_path="$root_dir/src/build_cmake/programs/rocmdr_snp/rocmdr_snp"

generated_path=$TMPDIR/generated
maf="$maf1 $maf2"

# Generate data and run methods for each model
i=1
while read model; do
    python $generate_path --model $model --maf $maf --ncases $ncases --ncontrols $ncontrols --npairs $npairs --out $generated_path
    
    $rocmdr_path -j 2 -s 1 -p "$generated_path.pair" $generated_path > "$output_dir/rocmdr_model$i.out"
    Rscript $glm_path "$generated_path.pair" $generated_path > "$output_dir/glm_model$i.out"

    let "i=$i+1"
done < $model_file

# Output parameters
param_file="$output_dir/params.out"
cat $model_file > $param_file
echo "maf: $maf" >> $param_file
echo "ncases: $ncases ncontrols: $ncontrols" >> $param_file
