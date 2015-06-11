#!/bin/bash
PROCESS_COUNT=1
PREFIX=sample
cd output
START=$(date +%s)

ps aux | grep ray_handler | awk '{print $1}' | xargs kill -9 > /dev/null
./data_precessor ${PREFIX} $PROCESS_COUNT

# for i in { 1..$PROCESS_COUNT }
i=0
while [ $i -lt $PROCESS_COUNT ]
do
	echo "./ray_handler ray_handler_${i} ${PREFIX}_ray_source_${i}.dat" 
	./ray_handler "ray_handler_${i}" "${PREFIX}_ray_source_${i}.dat"  > /dev/null &
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