#!/bin/bash

root_dir="$(dirname $0)/.."

# Gather parameters
while getopts "a:b:d:h:n:i:" opt;
do
    case $opt in
        a) maf1=$OPTARG ;;
        b) maf2=$OPTARG ;;
        d) ncases=$OPTARG ;;
        h) ncontrols=$OPTARG ;;
        n) npairs=$OPTARG ;;
        i) ninteractions=$OPTARG;; 
        *) echo "Usage: method_power -a maf1 -b maf2 -d ncases -h ncontrols -n npairs -i ninteractions model_file output_dir" ; exit 1 ;;
    esac
done
shift $((OPTIND-1))

# Check that we got all arguments
if [ $# != 2 ]
then
    echo "Usage: method_power -a maf1 -b maf2 -d ncases -h ncontrols -n npairs -i ninteractions model_file output_dir"
    exit 1
fi

if [ ! "$maf1" ] || [ ! "$maf2" ] || [ ! "$ncases" ] || [ ! "$ncontrols" ] || [ ! "$npairs" ] || [ ! "$ninteractions" ];
then
    echo "error: Option -a, -b, -d, -h, -n or -i not specified."
    exit 1
fi

# Exit on ctrl+C
trap 'exit' SIGINT SIGQUIT

# Contains models
model_file=$1

# Output directory
output_dir=$2
mkdir -p $output_dir

# Path to methods to test
calculate_power="python $root_dir/scripts/calculate_power.py"
generate_path="$root_dir/tools/generate_ped/generate_ped.py"
glm_path="$root_dir/scripts/glm_interaction.r"
ll_path="$root_dir/scripts/ll_interaction.r"
rocmdr_path="$root_dir/src/build_cmake/programs/rocmdr_snp/rocmdr_snp"
bayesic_path="$root_dir/../py-rocmdr/py-rocmdr.py"

generated_path=$TMPDIR/generated$RANDOM
maf="$maf1 $maf2"

# Generate data and run methods for each model
threshold=`echo 0.05/$ninteractions | bc -l`
# Increased for every set of parameters in a model
i=1

# Increased for every new model
model_id=0

# Determines whether it is an LD-model or not.
ld_flag=""

# Output file
output_file=""

while read model; do
    model_array=($model)

    # Check for new model
    if [ ${#model_array[@]} -eq 2 ];
    then
        i=1
        let "model_id=$model_id+1"

        ld_flag=""
        output_file="$output_dir/model$model_id.out"
        cat /dev/null > $output_file
        continue
    fi
    # Check for LD parameter
    if [ ${#model_array[@]} -eq 1 ];
    then
        ld_flag="--ld ${model_array[0]}"
        continue
    fi

    python $generate_path --model $model --maf $maf --ncases $ncases --ncontrols $ncontrols --npairs $npairs $ld_flag --out $generated_path

    $rocmdr_path -j 2 -s 1 -p "$generated_path.pair" $generated_path > "$output_dir/rocmdr_model$i.out"
    Rscript $glm_path "$generated_path.pair" $generated_path | awk 'NF == 3' | $calculate_power "Logistic" $i 2 le $threshold >> $output_file
    Rscript $ll_path "$generated_path.pair" $generated_path | awk 'NF == 3' | $calculate_power "Log-linear" $i 2 le $threshold >> $output_file
    python $bayesic_path -p "$generated_path.pair" -n $ninteractions $generated_path | awk 'NF == 3' | $calculate_power "Bayesic" $i 2 ge 0.95 >> $output_file

    let "i=$i+1"
done < $model_file

# Output parameters
param_file="$output_dir/params.out"
cat $model_file > $param_file
echo "maf: $maf" >> $param_file
echo "ncases: $ncases ncontrols: $ncontrols" >> $param_file
echo "ninteractions: $ninteractions" >> $param_file
