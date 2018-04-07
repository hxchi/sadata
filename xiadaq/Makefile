# makefile
# copy from why

OBJ = aout
sourcefile = main.cpp eventer.h r2root.h eventer.cpp r2root.cpp

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs)

CFLAGS = -g -Wall -O2 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cpp ,$(sourcefile))

clean:
	rm -f *~ *.o $(OBJ)
