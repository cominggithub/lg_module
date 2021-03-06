#!/bin/bash
cd output
START=$(date +%s)

./lg_module
END=$(date +%s)

gnuplot gnuplot_script/plot.data2d.txt
gnuplot gnuplot_script/plot.data3dm.txt
open output.2d.svg &
open output3d_matrix.svg &
DIFF=$(echo "$END - $START" | bc)
echo "execute time ${DIFF} seconds"