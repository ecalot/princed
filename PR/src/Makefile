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
  PORTS =
else
  LINUX = -DNOLINUX
  PORTS = getopt.o getopt1.o
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

FILES    = import.o export.o resources.o classify.o disk.o\
           dat.o bmp.o mid.o pal.o wav.o plv.o\
           memory.o pr.o

XML      = parse.o search.o
COMPRESS = compress.o lzg_compress.o lzg_uncompress.o rle_compress.o rle_uncompress.o 
CONSOLE  = main.o filedir.o

EXEOBJ   = $(FILES) $(XML) $(COMPRESS) $(CONSOLE) $(PORTS)
LIBOBJ   = $(FILES) $(XML) $(COMPRESS) $(PORTS)

EXEFILE  = bin/pr
XMLFILE  = bin/resources.xml

#Use this to temporary remove an option
OPTIONS       = $(INCLUDE) $(DEFINES) $(RELEASE)
LINKEROPTIONS = $(LINKERRELEASE)

#main file

$(EXEFILE): $(EXEOBJ)
	$(INFO) Linking files...
	$(MAKEDIR) bin
	$(CC) $(OPTIONS) -o bin/pr $(EXEOBJ) 
	$(INFO) Program successfully compiled
	$(INFO)
	$(INFO) Please read readme.txt for syntax information
	$(INFO)

#command options
.PHONY: clean cleanxml build all lib

clean:
	$(INFO) Erasing temporary object files...
	$(REMOVER) $(EXEOBJ) $(EXEFILE)

cleanxml:
	$(INFO) Erasing xml file...
	$(REMOVER) $(XMLFILE)

build: clean bin/pr

all: $(EXEFILE)

lib: $(LIBOBJ)
	$(MAKEDIR) bin
	$(INFO) Making dynamic library...
	$(CC) $(OPTIONS) -o bin/pr.so $(LIBOBJ) -Llibc -shared -dynamic
	$(INFO) Library successfully compiled
	$(INFO)
	$(INFO) Please read readme.coders.txt and pr.h for interfaces
	$(INFO)

#files

$(XMLFILE): resources.xml.gz
	$(INFO) Uncompressing resources...
	$(MAKEDIR) bin
	$(GZIP) -cd resources.xml.gz >bin/resources.xml

import.o: lib/import.c
	$(INFO) Compiling import module...
	$(CC) -c lib/import.c $(OPTIONS)

memory.o: lib/memory.c
	$(INFO) Compiling memory manager...
	$(CC) -c lib/memory.c $(OPTIONS)

export.o: lib/export.c
	$(INFO) Compiling export module...
	$(CC) -c lib/export.c $(OPTIONS)

compress.o: lib/compression/main.c
	$(INFO) Compiling compression module...
	$(CC) -c lib/compression/main.c -o compress.o $(OPTIONS)

lzg_compress.o: lib/compression/lzg_compress.c
	$(INFO) Compiling LZG compression module...
	$(CC) -c lib/compression/lzg_compress.c $(OPTIONS)

rle_compress.o: lib/compression/rle_compress.c
	$(INFO) Compiling RLE compression module...
	$(CC) -c lib/compression/rle_compress.c $(OPTIONS)

lzg_uncompress.o: lib/compression/lzg_uncompress.c
	$(INFO) Compiling LZG uncompression module...
	$(CC) -c lib/compression/lzg_uncompress.c $(OPTIONS)

rle_uncompress.o: lib/compression/rle_uncompress.c
	$(INFO) Compiling RLE uncompression module...
	$(CC) -c lib/compression/rle_uncompress.c $(OPTIONS)

main.o: console/main.c
	$(INFO) Compiling command parsing module for standard mode...
	$(CC) -c console/main.c $(OPTIONS)

pr.o: lib/pr.c
	$(INFO) Compiling main library primitives for both modes...
	$(CC) -c lib/pr.c $(OPTIONS)
	
filedir.o: console/filedir.c
	$(INFO) Compiling directory and recursive file reading module for standard mode...
	$(CC) -c console/filedir.c $(OPTIONS)

resources.o: lib/resources.c
	$(INFO) Compiling resource manager module...
	$(CC) -c lib/resources.c $(OPTIONS)

classify.o: lib/classify.c
	$(INFO) Compiling classification module...
	$(CC) -c lib/classify.c $(OPTIONS)

parse.o: lib/xml/parse.c
	$(INFO) Compiling XML parsing module...
	$(CC) -c lib/xml/parse.c $(OPTIONS)

search.o: lib/xml/search.c
	$(INFO) Compiling XML search features...
	$(CC) -c lib/xml/search.c $(OPTIONS)

disk.o: lib/disk.c
	$(INFO) Compiling disk access library...
	$(CC) -c lib/disk.c $(OPTIONS)

dat.o: lib/dat.c
	$(INFO) Compiling DAT editing library...
	$(CC) -c lib/dat.c $(OPTIONS)

bmp.o: lib/formats/bmp.c
	$(INFO) Compiling bitmap files support \(bmp\)...
	$(CC) -c lib/formats/bmp.c $(OPTIONS)

mid.o: lib/formats/mid.c
	$(INFO) Compiling MIDI audio files support \(mid\)...
	$(CC) -c lib/formats/mid.c $(OPTIONS)

plv.o: lib/formats/plv.c
	$(INFO) Compiling prince level files support \(plv\)...
	$(CC) -c lib/formats/plv.c $(OPTIONS)

pal.o: lib/formats/pal.c
	$(INFO) Compiling JASC palette support \(pal\)...
	$(CC) -c lib/formats/pal.c $(OPTIONS)

wav.o: lib/formats/wav.c
	$(INFO) Compiling digital wave audio support \(wav\)...
	$(CC) -c lib/formats/wav.c $(OPTIONS)

dirent.o: ports/dirent.c
	$(INFO) Porting the dirent structure...
	$(CC) $(OPTIONS) -c ports/dirent.c
	
getopt.o: ports/getopt.c
	$(INFO) Porting the Unix-like getopt function \(first part\)...
	$(CC) $(OPTIONS) -c ports/getopt.c

getopt1.o: ports/getopt1.c
	$(INFO) Porting the Unix-like getopt function \(second part\)...
	$(CC) $(OPTIONS) -c ports/getopt1.c

