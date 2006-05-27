############
# Programs #
############

CC         = @g++
LINKER     = @ld
INFO       = @echo
MAKEDIR    = @mkdir -p
REMOVER    = @rm -f
GZIP       = @gzip
COPY       = @ln -f

#####################
# Operating Systems #
#####################

OS      := $(shell uname)
ifeq ($(OS),Linux)
  LINUX = -DLINUX
  OS    = GNU/Linux
  PORTS =
else
  LINUX = -DNOLINUX
  PORTS = 
	ifeq ($(OS),Darwin)
		LINUX = -DNOLINUX -DMACOS
	endif
endif

####################
# Compiler options #
####################

#Libraries: include path and linked libs
INCLUDE       = -Iinclude/
LIBS          = 
DEFINES       = -DOS=\"$(OS)\" $(LINUX)

#Release type
TYPE          = Debug
# RELEASE may be:
#  -g -Wall -ansi -pedantic        for debug
#  -O2                             for release
# LINKERRELEASE may be:
#  -s                              for release
ifeq ($(TYPE),Debug)
  #RELEASE       = -g -Wall -pedantic -fPIC for .so files
  RELEASE       = -g -Wall 
#-pedantic
  LINKERRELEASE =
else
  RELEASE       = -O2 
  LINKERRELEASE = -s
endif

OBJECTS  = .guard.o .level.o .levelformat.o .outertile.o .pop1levelformat.o .pop1tile.o .pop2tile.o .tile.o
LAYERS   = .endiansafestream.o
SUPPORT  = .main.o

EXEOBJ   = $(SUPPORT) $(LAYERS) $(OBJECTS)
#LIBOBJ   = $(MAIN) $(XML) $(COMPRESS) $(ACTIONS) $(LAYERS) $(OBJECT) $(FORMAT) $(PORTS)

EXEFILE  = bin/pv4

#Use this to temporary remove an option
OPTIONS       = $(INCLUDE) $(DEFINES) $(RELEASE)
LINKEROPTIONS = $(LINKERRELEASE) $(LIBS)

#main file

$(EXEFILE): $(EXEOBJ)
	$(INFO) Linking files...
	$(MAKEDIR) bin
	$(CC) $(OPTIONS) -o $(EXEFILE) $(EXEOBJ)
	$(INFO) Program successfully compiled
	$(INFO)
	$(INFO) Please read readme.txt for syntax information
	$(INFO)

#command options
.PHONY: clean build all libs

clean:
	$(INFO) Erasing temporary object files...
	$(REMOVER) $(EXEOBJ) $(EXEFILE)

build: clean bin/pv4

all: $(EXEFILE)

libs: $(LIBOBJ)
	$(MAKEDIR) bin
	$(INFO) Making dynamic library...
	$(CC) $(OPTIONS) -o bin/pr.so $(LIBOBJ) -Llibc -shared -dynamic
	$(INFO) Library successfully compiled
	$(INFO)
	$(INFO) Please read readme.coders.txt and pr.h for interfaces
	$(INFO)

#files

.endiansafestream.o: src/endiansafestream.cpp include/endiansafestream.h
	$(INFO) Compiling endian abstraction layer...
	$(CC) $(OPTIONS) -c src/endiansafestream.cpp -o $@

.guard.o: src/guard.cpp include/guard.h
	$(INFO) Compiling main guard class...
	$(CC) $(OPTIONS) -c src/guard.cpp -o $@

.level.o: src/level.cpp include/level.h
	$(INFO) Compiling main level bridge class...
	$(CC) $(OPTIONS) -c src/level.cpp -o $@

.levelformat.o: src/levelformat.cpp include/levelformat.h
	$(INFO) Compiling main level format class...
	$(CC) $(OPTIONS) -c src/levelformat.cpp -o $@

.main.o: src/main.cpp
	$(INFO) Compiling main dummy class...
	$(CC) $(OPTIONS) -c src/main.cpp -o $@

.outertile.o: src/outertile.cpp include/outertile.h
	$(INFO) Compiling outer tile class...
	$(CC) $(OPTIONS) -c src/outertile.cpp -o $@

.pop1levelformat.o: src/pop1levelformat.cpp include/pop1levelformat.h
	$(INFO) Compiling pop1 level support class...
	$(CC) $(OPTIONS) -c src/pop1levelformat.cpp -o $@

.pop1tile.o: src/pop1tile.cpp include/pop1tile.h
	$(INFO) Compiling pop1 tile support class...
	$(CC) $(OPTIONS) -c src/pop1tile.cpp -o $@

.pop2tile.o: src/pop2tile.cpp include/pop2tile.h
	$(INFO) Compiling pop2 tile support class...
	$(CC) $(OPTIONS) -c src/pop2tile.cpp -o $@

.tile.o: src/tile.cpp include/tile.h
	$(INFO) Compiling main tile class...
	$(CC) $(OPTIONS) -c src/tile.cpp -o $@

