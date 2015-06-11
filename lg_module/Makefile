

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

#CPP_FILES := $(wildcard *.cpp)
#OBJ_FILES := $(addprefix ${OBJ_DIR}/,$(notdir $(CPP_FILES:.cpp=.o)))
OBJ_FILES := \
	conf_util.o \
	data_file_util.o \
	dbg_log.o \
	debug_fun.o \
	den_to_pos.o \
	gen_source_ray.o \
	globalstr.o \
	ini_var.o \
	mem_func.o \
	microstr.o \
	Module_IV.o \
	plt_figure.o \
	time_util.o \
	var_def.o


all: prepare lg_module dbg_main data_precessor ray_handler


prepare:
	mkdir -p ${OUTPUT_DIR}
	mkdir -p ${OBJ_DIR}

	cp -rf ${SRC_DIR}/gnuplot_script ${OUTPUT_DIR}/
	cp -f ${DATA_FILE} ${OUTPUT_DIR}

lg_module: $(OBJ_FILES) lg_module.o
	g++ $(LD_FLAGS) -o ${OUTPUT_DIR}/$@ $^

dbg_main: $(OBJ_FILES) dbg_main.o
	g++ $(LD_FLAGS) -o ${OUTPUT_DIR}/$@ $^
	

data_precessor: $(OBJ_FILES) data_precessor.o
	g++ $(LD_FLAGS) -o ${OUTPUT_DIR}/$@ $^

ray_handler: $(OBJ_FILES) ray_handler.o
	g++ $(LD_FLAGS) -o ${OUTPUT_DIR}/$@ $^


${OBJ_DIR}/%.o: %.cpp
	g++ $(CPPFLAGS) -c -o $@ $<

# lg_module: ${SRC_DIR}/lg_module.cpp
# 	gcc ${CPPFLAGS} $< -o ${OUTPUT_DIR}/$@ ${LDFLAGS}

clean:
	rm -f *.o *.png *.svg ${OUTPUT_DATA}
	rm -rf ${OUTPUT_DIR}
	rm -rf ${OBJ_DIR}
