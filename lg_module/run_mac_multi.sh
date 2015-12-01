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
    echo "e.g., "
    echo "$0 -v     run in verbose mode"
    echo "$0 -p     run in parallel mode"
    echo "$0 -n=5   run in parallel mode, and the process count is 5"
    exit 0;
}

while [[ $# > 0 ]]
do
key="$1"
case $key in
    -v)
        is_verbose=1
        ;;
    -p)
        is_parallel=1
        ;;
    -n=*)
        PROCESS_COUNT="${key#*=}"
        # shift # past argument
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

cd output
mkdir $date_str

START=$(date +%s)
ps aux | grep ray_handler | awk '{print $1}' | xargs kill -9 > /dev/null


if [ ${is_verbose} == "1" ]; then 
    ./data_preprocessor ${PROCESS_COUNT} ${OUTPUT_FOLDER}
else
    ./data_preprocessor ${PROCESS_COUNT} ${OUTPUT_FOLDER} > /dev/null
fi

# for i in { 1..$PROCESS_COUNT }
i=0
while [ $i -lt $PROCESS_COUNT ]
do
    echo "./ray_handler ${OUTPUT_FOLDER} ray_handler_${i} ${i} ${OUTPUT_FOLDER}/ray_source_${i}.dat"
    cp parameters.txt "${OUTPUT_FOLDER}/parameters.txt"
    cp microstr.txt "${OUTPUT_FOLDER}/microstr.txt"
    cp dot_density.txt "${OUTPUT_FOLDER}/dot_density.txt"
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


sleep 3
i=0

while [ $i -lt $PROCESS_COUNT ]
do
    #echo ${pids[$i]}
    wait ${pids[$i]}
    let i++ 1
done

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