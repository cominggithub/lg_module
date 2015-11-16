#!/bin/bash


#!/bin/bash
date_str=$(date +%Y_%m_%d_%H_%M_%S)
PROCESS_COUNT=1
PREFIX="_lg_module_${date_str}"

OUTPUT_FOLDER=${date_str}
is_verbose=0
is_parallel=1

function print_help {
	echo "-v is_verbose"
	echo "-s is_single"
}

for para in $@
do
	if [ $para == "-v" ]; then
		is_verbose=1
	fi

	if [ $para == "-s" ]; then
		is_parallel=0
	fi 

	if [ $para == "help"]; then
		print_help
		exit 0
	fi

done

if [ ${is_parallel} == "1" ]; then
	echo "parallel"
else
	echo "sequential"
fi

if [ ${is_verbose} == "1" ]; then
	echo "verbose"
fi

cd output
mkdir $date_str

START=$(date +%s)
ps aux | grep ray_handler | awk '{print $1}' | xargs kill -9 &> /dev/null


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


##########################################################
# backup, run windows single

# cd output
# START=$(date +%s)

# ./lg_module
# END=$(date +%s)

# gnuplot gnuplot_script/plot.data2d.txt
# gnuplot gnuplot_script/plot.data3dm.txt
# open output.2d.svg &
# open output3d_matrix.svg &
# DIFF=$(echo "$END - $START" | bc)
# echo "execute time ${DIFF} seconds"

# cd ..
# exit 0