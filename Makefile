CPP = g++

OBJ_DIR := obj
EXECUTABLE := bin/cdbg.exe
SRC_DIR := src


OBJ_FILES := $(wildcard $(OBJ_DIR)/*.o)
EXT_LIBS := external/libelfin/dwarf/libdwarf++.so external/libelfin/elf/libelf++.so
READ_LINE := -lreadline

EXTERNING = export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/qfour/learning/cdebugger/external/libelfin/dwarf


cdbg:	$(OBJ_FILES) 
	$(MAKE) -C $(SRC_DIR) 
	$(CPP) $^ $(EXT_LIBS) $(READ_LINE) -o $(EXECUTABLE)
