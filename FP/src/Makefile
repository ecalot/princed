#Programs
CC      = @gcc
LINKER  = @ld
INFO    = @echo
MAKEDIR = @mkdir -p

#Operating Systems
OS      := $(shell uname)
ifeq ($(OS),Linux)
  LINUX = -DLINUX
  OS    = GNU/Linux
  SRC2  =
else
  LINUX = -DNOLINUX
  SRC2  = getopt.o getopt1.o
endif

#Compiler options
INCLUDE = -Iinclude/
DEFINES = -DOS=\"$(OS)\" $(LINUX) 
OPTIONS = $(INCLUDE) $(DEFINES)

OBJFILES = resources.o disk.o\
           dat.o main.o\
           $(SRC2)


EXEFILE  = bin/freeprince

#main file

$(EXEFILE): $(OBJFILES)
	$(INFO) Linking files...
	$(MAKEDIR) bin
	$(CC) $(OPTIONS) -o $(EXEFILE) $(OBJFILES) -O2
	$(INFO) Program successfully compiled
	$(INFO)
	$(INFO) Please read readme.txt for syntax information
	$(INFO)

#command options

clean:
	$(INFO) Erasing temporary object files...
	@rm -f *.o bin/freeprince

build: clean  $(EXEFILE)

all: $(EXEFILE)

#files

#compress.o: compress.c
#	$(INFO) Compiling compression module...
#	$(CC) -c compress.c $(OPTIONS)

main.o: main.c
	$(INFO) Compiling main program...
	$(CC) -c main.c $(OPTIONS)

resources.o: res/resources.c
	$(INFO) Compiling resource manager module...
	$(CC) -c res/resources.c $(OPTIONS)

disk.o: res/disk.c
	$(INFO) Compiling disk access library...
	$(CC) -c res/disk.c $(OPTIONS)

dat.o: res/dat.c
	$(INFO) Compiling dat editing library...
	$(CC) -c res/dat.c $(OPTIONS)

