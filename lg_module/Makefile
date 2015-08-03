

CPPFLAGS= -I. -I../lg_module_lib -I/usr/include/malloc -std=c++0x -ggdb
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
	../lg_module_lib/conf_util.o \
	../lg_module_lib/data_file_util.o \
	../lg_module_lib/dbg_log.o \
	../lg_module_lib/debug_fun.o \
	../lg_module_lib/den_to_pos.o \
	../lg_module_lib/gen_source_ray.o \
	../lg_module_lib/globalstr.o \
	../lg_module_lib/ini_var.o \
	../lg_module_lib/mem_func.o \
	../lg_module_lib/microstr.o \
	../lg_module_lib/Module_IV.o \
	../lg_module_lib/plt_figure.o \
	../lg_module_lib/time_util.o \
	../lg_module_lib/ray_tracer.o \
	../lg_module_lib/opt_reflector.o \
	../lg_module_lib/glist.o \
	../lg_module_lib/opt_record.o \
	../lg_module_lib/mem_alloc.o \
	../lg_module_lib/var_def.o


all: prepare lg_module data_preprocessor ray_handler data_postprocessor


prepare:
	mkdir -p ${OUTPUT_DIR}
	mkdir -p ${OBJ_DIR}

	cp -rf ${SRC_DIR}/gnuplot_script ${OUTPUT_DIR}/
	cp -f ${DATA_FILE} ${OUTPUT_DIR}

lg_module: $(OBJ_FILES) lg_module.o
	g++ $(LD_FLAGS) -o ${OUTPUT_DIR}/$@ $^

dbg_main: $(OBJ_FILES) dbg_main.o
	g++ $(LD_FLAGS) -o ${OUTPUT_DIR}/$@ $^
	

data_preprocessor: $(OBJ_FILES) ../data_preprocessor/data_preprocessor.o
	g++ $(LD_FLAGS) -o ${OUTPUT_DIR}/$@ $^

data_postprocessor: $(OBJ_FILES) ../data_postprocessor/data_postprocessor.o
	g++ $(LD_FLAGS) -o ${OUTPUT_DIR}/$@ $^	

ray_handler: $(OBJ_FILES) ../ray_handler/ray_handler.o
	g++ $(LD_FLAGS) -o ${OUTPUT_DIR}/$@ $^

${OBJ_DIR}/%.o: %.cpp
	g++ $(CPPFLAGS) -c -o $@ $<

# lg_module: ${SRC_DIR}/lg_module.cpp
# 	gcc ${CPPFLAGS} $< -o ${OUTPUT_DIR}/$@ ${LDFLAGS}

clean:
	rm -f *.o *.png *.svg ${OUTPUT_DATA}
	rm -f ${OBJ_FILES}
	rm -rf ${OUTPUT_DIR}
	rm -rf ${OBJ_DIR}
	rm -f ../data_preprocessor/*.o
	rm -f ../data_postprocessor/*.o
	rm -f ../ray_handler/*.o

