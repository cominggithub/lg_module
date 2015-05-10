

CPPFLAGS= -I. -I/usr/include/malloc -std=c++0x -ggdb
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

DATA_FILE = \
	parameters.txt \
	microstr.txt

SRC_DIR = ./
OUTPUT_DIR = output
OBJ_DIR = obj

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix ${OBJ_DIR}/,$(notdir $(CPP_FILES:.cpp=.o)))

all: prepare lg_module dbg_main


prepare:
	mkdir -p ${OUTPUT_DIR}
	mkdir -p ${OBJ_DIR}

	cp -rf ${SRC_DIR}/gnuplot_script ${OUTPUT_DIR}/
	cp -f ${DATA_FILE} ${OUTPUT_DIR}

lg_module: $(OBJ_FILES)
	g++ $(LD_FLAGS) -o ${OUTPUT_DIR}/$@ $^

dbg_main: $(OBJ_FILES)
	g++ $(LD_FLAGS) -o ${OUTPUT_DIR}/$@ $^
	
${OBJ_DIR}/%.o: %.cpp
	g++ $(CPPFLAGS) -c -o $@ $<

# lg_module: ${SRC_DIR}/lg_module.cpp
# 	gcc ${CPPFLAGS} $< -o ${OUTPUT_DIR}/$@ ${LDFLAGS}

clean:
	rm -f *.o *.png *.svg ${OUTPUT_DATA}
	rm -rf ${OUTPUT_DIR}
	rm -rf ${OBJ_DIR}
