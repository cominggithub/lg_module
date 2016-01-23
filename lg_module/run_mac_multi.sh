#!/bin/bash
date_str=$(date +%Y_%m_%d_%H_%M_%S)
PROCESS_COUNT=1
PREFIX="_lg_module_${date_str}"

OUTPUT_FOLDER=${date_str}
is_verbose=0
is_parallel=0


function help_msg {
    echo "-v                    verbose mode";
    echo "-p                    parallel mode, default sequential mode"
    echo "-n=<PROCESS_COUNT>    process count, default 1 process"
    echo "-D <name>=<value>      user defined key value pair in paramerters.txt"
    echo "e.g., "
    echo "$0 -v     run in verbose mode"
    echo "$0 -p     run in parallel mode"
    echo "$0 -n=5   run in parallel mode, and the process count is 5"
    exit 0;
}

# setup parameters in parameters.tmp.txt
# keep parameters.txt unchanged

if [ ! -d "output" ]; then
    echo "output folder not found"
    exit 1;
fi

cd output
cp -f parameters.txt parameters.tmp.txt

while [[ $# > 0 ]]
do
key="$1"
case $key in
    -v|-V)
        is_verbose=1
        ;;
    -p|-P)
        is_parallel=1
        ;;
    -n=*)
        PROCESS_COUNT="${key#*=}"
        # shift # past argument
        ;;
    # user defined parameters values
    -D)
        shift
        key=$(echo $1 | cut -f1 -d=)
        value=$(echo $1 | cut -f2 -d=)
        sed -i '' "s/\($key *= *\).*/\1$value/" parameters.tmp.txt
        ;;
    -h)
        help_msg
        ;;
    *)
        # unknown option
        echo ${key}
    ;;
esac
shift # past argument or value
done


if [ ${is_parallel} == "1" ]; then
    echo "parallel"
else
    echo "sequential"
fi

if [ ${is_verbose} == "1" ]; then
    echo "verbose"
fi

echo "process count: ${PROCESS_COUNT}"

if [ $PROCESS_COUNT -lt 1 ]; then
    exit 0;
fi


mkdir $date_str

# create process temporary folders by process count
# for i in $(seq 1 $PROCESS_COUNT); do 
for (( i=0; i<$PROCESS_COUNT; i++ ))
do
    mkdir -p "$date_str/process_$i"
done

START=$(date +%s)
ps aux | grep ray_handler | awk '{print $2}' | xargs kill -9 &> /dev/null

cp parameters.tmp.txt "${OUTPUT_FOLDER}/parameters.txt"
cp microstr.txt "${OUTPUT_FOLDER}/microstr.txt"
cp dot_density.txt "${OUTPUT_FOLDER}/dot_density.txt"

rm -f last
ln -s "${OUTPUT_FOLDER}" last

if [ ${is_verbose} == "1" ]; then 
    ./data_preprocessor ${PROCESS_COUNT} ${OUTPUT_FOLDER}
else
    ./data_preprocessor ${PROCESS_COUNT} ${OUTPUT_FOLDER} > /dev/null
fi

# for i in { 1..$PROCESS_COUNT }
i=0
while [ $i -lt $PROCESS_COUNT ]
do
    # echo "./ray_handler ${OUTPUT_FOLDER} ray_handler_${i} ${i} ${OUTPUT_FOLDER}/ray_source_${i}.dat"
    echo -e "\nprocess $i started...."
    if [ ${is_parallel} == "1" ]; then
        if [ ${is_verbose} == "1" ]; then
            ./ray_handler ${OUTPUT_FOLDER} "ray_handler_${i}" ${i} "${OUTPUT_FOLDER}/ray_source_${i}.dat" &
        else
            ./ray_handler ${OUTPUT_FOLDER} "ray_handler_${i}" ${i} "${OUTPUT_FOLDER}/ray_source_${i}.dat"  > /dev/null &
        fi
    else
        if [ ${is_verbose} == "1" ]; then
            ./ray_handler ${OUTPUT_FOLDER} "ray_handler_${i}" ${i} "${OUTPUT_FOLDER}/ray_source_${i}.dat"
        else
            ./ray_handler ${OUTPUT_FOLDER} "ray_handler_${i}" ${i} "${OUTPUT_FOLDER}/ray_source_${i}.dat"  > /dev/null
        fi
    fi

    pids[$i]=$!
    let i++ 1
done


if [ ${is_parallel} == "1" ]; then
    sleep 3
fi
i=0

while [ $i -lt $PROCESS_COUNT ]
do
    #echo ${pids[$i]}
    wait ${pids[$i]}
    let i++ 1
done

echo "Post Processing....."
./data_postprocessor ${PROCESS_COUNT} ${OUTPUT_FOLDER}

END=$(date +%s)

# gnuplot gnuplot_script/plot.data2d.txt
# gnuplot gnuplot_script/plot.data3dm.txt
# open output.2d.svg &
# open output3d_matrix.svg &
DIFF=$(echo "$END - $START" | bc)
#DIFF=$(echo "$END - $START")
echo "execute time ${DIFF} seconds"
cd ..
exit 0