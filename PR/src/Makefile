############
# Programs #
############

CC         = @gcc
LINKER     = @ld
INFO       = @echo
MAKEDIR    = @mkdir -p
REMOVER    = @rm -f
GZIP       = @gzip

#####################
# Operating Systems #
#####################

OS      := $(shell uname)
ifeq ($(OS),Linux)
  LINUX = -DLINUX
  OS    = GNU/Linux
  SRC2  =
else
  LINUX = -DNOLINUX
  SRC2  = getopt.o getopt1.o
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
  RELEASE       = -g -Wall -pedantic 
  LINKERRELEASE =
else
  RELEASE       = -O2 
  LINKERRELEASE = -s
endif

OBJFILES = compile.o compress.o extract.o resources.o tasks.o disk.o\
           xmlparse.o xmlsearch.o dat.o bmp.o mid.o pal.o wav.o plv.o\
           memory.o $(SRC2) pr.o

SRC3     = prlib.o

EXEFILE  = bin/pr
XMLFILE  = bin/resources.xml

#Use this to temporary remove an option
OPTIONS       = $(INCLUDE) $(DEFINES) $(RELEASE)
LINKEROPTIONS = $(LINKERRELEASE)

#main file

$(EXEFILE): $(OBJFILES) $(XMLFILE)
	$(INFO) Linking files...
	$(MAKEDIR) bin
	$(CC) $(OPTIONS) -o bin/pr $(OBJFILES) 
	$(INFO) Program successfully compiled
	$(INFO)
	$(INFO) Please read readme.txt for syntax information
	$(INFO)

#command options

clean:
	$(INFO) Erasing temporary object files...
	$(REMOVER) $(OBJFILES) $(SRC3) $(EXEFILE)

cleanxml:
	$(INFO) Erasing xml file...
	$(REMOVER) $(XMLFILE)

build: clean bin/pr

all: $(EXEFILE)

lib: clean $(OBJFILES) $(SRC3)
	$(MAKEDIR) bin
	$(INFO) Making dynamic library...
	$(CC) $(OPTIONS) -o bin/pr.so $(OBJFILES) $(SRC3) -Llibc -shared -dynamic
	$(INFO) Library successfully compiled
	$(INFO)
	$(INFO) Please read readme.coders.txt and pr.h for interfaces
	$(INFO)

#files

$(XMLFILE): resources.xml.gz
	$(INFO) Uncompressing resources...
	$(MAKEDIR) bin
	$(GZIP) -cd resources.xml.gz >bin/resources.xml

compile.o: compile.c
	$(INFO) Compiling import module...
	$(CC) -c compile.c $(OPTIONS)

memory.o: memory.c
	$(INFO) Compiling memory manager...
	$(CC) -c memory.c $(OPTIONS)

extract.o: extract.c
	$(INFO) Compiling export module...
	$(CC) -c extract.c $(OPTIONS)

compress.o: compress.c
	$(INFO) Compiling compression module...
	$(CC) -c compress.c $(OPTIONS)

pr.o: pr.c
	$(INFO) Compiling main module in standard mode...
	$(CC) -c pr.c $(OPTIONS)

prlib.o: pr.c
	$(INFO) Compiling main module in library mode...
	$(CC) -c pr.c $(OPTIONS) -DDLL -o prlib.o

resources.o: resources.c
	$(INFO) Compiling resource manager module...
	$(CC) -c resources.c $(OPTIONS)

tasks.o: tasks.c
	$(INFO) Compiling extra tasks module...
	$(CC) -c tasks.c $(OPTIONS)

xmlparse.o: xmlparse.c
	$(INFO) Compiling xml parsing module...
	$(CC) -c xmlparse.c $(OPTIONS)

xmlsearch.o: xmlsearch.c
	$(INFO) Compiling xml search features...
	$(CC) -c xmlsearch.c $(OPTIONS)

disk.o: disk.c
	$(INFO) Compiling disk access library...
	$(CC) -c disk.c $(OPTIONS)

dat.o: dat.c
	$(INFO) Compiling dat editing library...
	$(CC) -c dat.c $(OPTIONS)

bmp.o: formats/bmp.c
	$(INFO) Compiling bitmap files support \(bmp\)...
	$(CC) -c formats/bmp.c $(OPTIONS)

mid.o: formats/mid.c
	$(INFO) Compiling midi audio files support \(mid\)...
	$(CC) -c formats/mid.c $(OPTIONS)

plv.o: formats/plv.c
	$(INFO) Compiling prince level files support \(plv\)...
	$(CC) -c formats/plv.c $(OPTIONS)

pal.o: formats/pal.c
	$(INFO) Compiling JASC palette support \(pal\)...
	$(CC) -c formats/pal.c $(OPTIONS)

wav.o: formats/wav.c
	$(INFO) Compiling digital wave audio support \(wav\)...
	$(CC) -c formats/wav.c $(OPTIONS)

dirent.o: ports/dirent.c
	$(INFO) Porting the dirent structure...
	$(CC) $(OPTIONS) -c ports/dirent.c
	
getopt.o: ports/getopt.c
	$(INFO) Porting the Unix-like getopt function \(first part\)...
	$(CC) $(OPTIONS) -c ports/getopt.c

getopt1.o: ports/getopt1.c
	$(INFO) Porting the Unix-like getopt function \(second part\)...
	$(CC) $(OPTIONS) -c ports/getopt1.c

