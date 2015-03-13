
CPPFLAGS= -I. -I/usr/include/malloc 
LDFLAGS= -lstdc++
OUTPUT_DATA = \
	data2d.txt \
	data3dm.txt \
	*.edge \
	*.ele \
	*.face \
	*.mtr \
	*.node \
	*.p2t \
	*.mtr \
	*.smesh 

SRC_DIR = ../
OUTPUT_DIR = output
all: prepare lg_module


prepare:
	mkdir -p ${OUTPUT_DIR}
	cp -rf ${SRC_DIR}/gnuplot_script ${OUTPUT_DIR}/

lg_module: ${SRC_DIR}/lg_module.cpp
	gcc ${CPPFLAGS} $< -o ${OUTPUT_DIR}/$@ ${LDFLAGS}

clean:
	rm -f *.o *.png *.svg ${OUTPUT_DATA}
	rm -rf output
