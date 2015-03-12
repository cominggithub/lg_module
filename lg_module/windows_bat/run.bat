lg_module

REM clear old svg files
del *.svg

REM plot new svg files
gnuplot "gnuplot_script\plot.data2d.txt"
gnuplot "gnuplot_script\plot.data3dm.txt"

REM open svg files
start "" %~dp0output.2d.svg
start "" %~dp0output3d_matrix.svg

REM pause