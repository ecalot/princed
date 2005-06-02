############
# Programs #
############

CC         = @gcc
LINKER     = @ld
INFO       = @echo
MAKEDIR    = @mkdir -p
REMOVER    = @rm -f
GZIP       = @gzip
COPY       = @cp -f

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
  PORTS = .getopt.o .getopt1.o
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

FILES    = .import.o .export.o .resources.o .classify.o .disk.o\
           .dat.o .bmp.o .mid.o .pal.o .wav.o .plv.o\
           .memory.o .pr.o

XML      = .parse.o .search.o
COMPRESS = .compress.o .lzg_compress.o .lzg_uncompress.o .rle_compress.o .rle_uncompress.o 
CONSOLE  = .main.o .filedir.o

EXEOBJ   = $(FILES) $(XML) $(COMPRESS) $(CONSOLE) $(PORTS)
LIBOBJ   = $(FILES) $(XML) $(COMPRESS) $(PORTS)

EXEFILE  = bin/pr
XMLFILE  = bin/resources.xml

ADDONS   = .python.o

#Use this to temporary remove an option
OPTIONS       = $(INCLUDE) $(DEFINES) $(RELEASE)
LINKEROPTIONS = $(LINKERRELEASE)

#main file

$(EXEFILE): $(EXEOBJ)
	$(INFO) Linking files...
	$(MAKEDIR) bin
	$(CC) $(OPTIONS) -o $(EXEFILE) $(EXEOBJ)  -o $@
	$(INFO) Program successfully compiled
	$(INFO)
	$(INFO) Please read readme.txt for syntax information
	$(INFO)

#command options
.PHONY: clean cleanxml build all lib

clean:
	$(INFO) Erasing temporary object files...
	$(REMOVER) $(EXEOBJ) $(EXEFILE) $(ADDONS)

cleanxml:
	$(INFO) Erasing xml file...
	$(REMOVER) $(XMLFILE)

build: clean bin/pr

all: $(EXEFILE)

lib: $(LIBOBJ)
	$(MAKEDIR) bin
	$(INFO) Making dynamic library...
	$(CC) $(OPTIONS) -o bin/pr.so $(LIBOBJ) -Llibc -shared -dynamic -o $@
	$(INFO) Library successfully compiled
	$(INFO)
	$(INFO) Please read readme.coders.txt and pr.h for interfaces
	$(INFO)

#files

$(XMLFILE): resources.xml.gz
	$(INFO) Copying resources...
	$(MAKEDIR) bin
	$(CP) xml/resources.xml $(XMLFILE)

.import.o: lib/actions/import.c
	$(INFO) Compiling import module...
	$(CC) $(OPTIONS) -c lib/actions/import.c -o $@

.memory.o: lib/layers/memory.c
	$(INFO) Compiling memory manager...
	$(CC) $(OPTIONS) -c lib/layers/memory.c -o $@

.export.o: lib/actions/export.c
	$(INFO) Compiling export module...
	$(CC) $(OPTIONS) -c lib/actions/export.c -o $@

.compress.o: lib/compression/main.c
	$(INFO) Compiling compression module...
	$(CC) $(OPTIONS) -c lib/compression/main.c -o .compress.o -o $@

.lzg_compress.o: lib/compression/lzg_compress.c
	$(INFO) Compiling LZG compression module...
	$(CC) $(OPTIONS) -c lib/compression/lzg_compress.c -o $@

.rle_compress.o: lib/compression/rle_compress.c
	$(INFO) Compiling RLE compression module...
	$(CC) $(OPTIONS) -c lib/compression/rle_compress.c -o $@

.lzg_uncompress.o: lib/compression/lzg_uncompress.c
	$(INFO) Compiling LZG uncompression module...
	$(CC) $(OPTIONS) -c lib/compression/lzg_uncompress.c -o $@

.rle_uncompress.o: lib/compression/rle_uncompress.c
	$(INFO) Compiling RLE uncompression module...
	$(CC) $(OPTIONS) -c lib/compression/rle_uncompress.c -o $@

.main.o: console/main.c
	$(INFO) Compiling command parsing module for standard mode...
	$(CC) $(OPTIONS) -c console/main.c -o $@

.pr.o: lib/pr.c
	$(INFO) Compiling main library primitives for both modes...
	$(CC) $(OPTIONS) -c lib/pr.c -o $@
	
.filedir.o: console/filedir.c
	$(INFO) Compiling directory and recursive file reading module for standard mode...
	$(CC) $(OPTIONS) -c console/filedir.c -o $@

.resources.o: lib/resources.c
	$(INFO) Compiling resource manager module...
	$(CC) $(OPTIONS) -c lib/resources.c -o $@

.classify.o: lib/actions/classify.c
	$(INFO) Compiling classification module...
	$(CC) $(OPTIONS) -c lib/actions/classify.c -o $@

.parse.o: lib/xml/parse.c
	$(INFO) Compiling XML parsing module...
	$(CC) $(OPTIONS) -c lib/xml/parse.c -o $@

.search.o: lib/xml/search.c
	$(INFO) Compiling XML search features...
	$(CC) $(OPTIONS) -c lib/xml/search.c -o $@

.disk.o: lib/layers/disk.c
	$(INFO) Compiling disk access library...
	$(CC) $(OPTIONS) -c lib/layers/disk.c -o $@

.dat.o: lib/layers/dat.c
	$(INFO) Compiling DAT editing library...
	$(CC) $(OPTIONS) -c lib/layers/dat.c -o $@

.bmp.o: lib/formats/bmp.c
	$(INFO) Compiling bitmap files support \(bmp\)...
	$(CC) $(OPTIONS) -c lib/formats/bmp.c -o $@

.mid.o: lib/formats/mid.c
	$(INFO) Compiling MIDI audio files support \(mid\)...
	$(CC) $(OPTIONS) -c lib/formats/mid.c -o $@

.plv.o: lib/formats/plv.c
	$(INFO) Compiling prince level files support \(plv\)...
	$(CC) $(OPTIONS) -c lib/formats/plv.c -o $@

.pal.o: lib/formats/pal.c
	$(INFO) Compiling JASC palette support \(pal\)...
	$(CC) $(OPTIONS) -c lib/formats/pal.c -o $@

.wav.o: lib/formats/wav.c
	$(INFO) Compiling digital wave audio support \(wav\)...
	$(CC) $(OPTIONS) -c lib/formats/wav.c -o $@

.dirent.o: ports/dirent.c
	$(INFO) Porting the dirent structure...
	$(CC) $(OPTIONS) -c ports/dirent.c -o $@
	
.getopt.o: ports/getopt.c
	$(INFO) Porting the Unix-like getopt function \(first part\)...
	$(CC) $(OPTIONS) -c ports/getopt.c -o $@

.getopt1.o: ports/getopt1.c
	$(INFO) Porting the Unix-like getopt function \(second part\)...
	$(CC) $(OPTIONS) -c ports/getopt1.c -o $@

#python
.python.o: addons/python/python.c
	$(INFO) Building the python interfaces
	$(CC) -pthread -fno-strict-aliasing -DNDEBUG -g -O3 -Wall -fPIC -I/usr/include/python2.3 -c addons/python/python.c $(INCLUDE) -o $@
	
pylib: .python.o $(LIBOBJ)
	$(INFO) Building the python module
	$(MAKEDIR) addons/python/bin
	$(CC) -pthread -shared .python.o $(LIBOBJ) -o addons/python/bin/pr.so -o $@

