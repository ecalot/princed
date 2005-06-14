############
# Programs #
############

CC         = @gcc
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

ACTIONS  = .import.o .export.o .classify.o
XML      = .parse.o .search.o .resources.o
COMPRESS = .compress.o .lzg_compress.o .lzg_uncompress.o .rle_compress.o .rle_uncompress.o 
LAYERS   = .dat.o .memory.o .list.o .reslist.o .disk.o .idlist.o .autodetect.o
FORMAT   = .bmp.o .mid.o .pal.o .wav.o .plv.o
CONSOLE  = .main.o .filedir.o
MAIN     = .pr.o

EXEOBJ   = $(MAIN) $(XML) $(COMPRESS) $(ACTIONS) $(LAYERS) $(FORMAT) $(PORTS) $(CONSOLE)
LIBOBJ   = $(MAIN) $(XML) $(COMPRESS) $(ACTIONS) $(LAYERS) $(FORMAT) $(PORTS)

EXEFILE  = bin/pr
XMLFILE  = bin/resources.xml

ADDONS   = .python.o

#Use this to temporary remove an option
OPTIONS       = $(INCLUDE) $(DEFINES) $(RELEASE)
LINKEROPTIONS = $(LINKERRELEASE)

#main file

$(EXEFILE): $(EXEOBJ) $(XMLFILE)
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

$(XMLFILE): xml/resources.xml
	$(INFO) Copying resources...
	$(MAKEDIR) bin
	$(COPY) xml/resources.xml $(XMLFILE)

.import.o: lib/actions/import.c include/autodetect.h include/bmp.h include/common.h include/dat.h include/disk.h include/idlist.h include/import.h include/memory.h include/mid.h include/pal.h include/plv.h include/resources.h include/wav.h
	$(INFO) Compiling import module...
	$(CC) $(OPTIONS) -c lib/actions/import.c -o $@

.classify.o: lib/actions/classify.c include/autodetect.h include/classify.h include/common.h include/dat.h include/disk.h include/memory.h
	$(INFO) Compiling classification module...
	$(CC) $(OPTIONS) -c lib/actions/classify.c -o $@

.export.o: lib/actions/export.c include/autodetect.h include/bmp.h include/common.h include/dat.h include/disk.h include/export.h include/idlist.h include/memory.h include/mid.h include/pal.h include/plv.h include/reslist.h include/resources.h include/wav.h
	$(INFO) Compiling export module...
	$(CC) $(OPTIONS) -c lib/actions/export.c -o $@

.autodetect.o: lib/layers/autodetect.c include/autodetect.h
	$(INFO) Compiling resource autodetection module...
	$(CC) $(OPTIONS) -c lib/layers/autodetect.c -o $@

.idlist.o: lib/layers/idlist.c include/disk.h include/idlist.h include/memory.h
	$(INFO) Compiling id list layer...
	$(CC) $(OPTIONS) -c lib/layers/idlist.c -o $@

.disk.o: lib/layers/disk.c include/common.h include/direntwin.h include/disk.h include/memory.h
	$(INFO) Compiling disk access library...
	$(CC) $(OPTIONS) -c lib/layers/disk.c -o $@

.dat.o: lib/layers/dat.c include/common.h include/dat.h include/disk.h include/reslist.h
	$(INFO) Compiling DAT editing library...
	$(CC) $(OPTIONS) -c lib/layers/dat.c -o $@

.memory.o: lib/layers/memory.c include/memory.h
	$(INFO) Compiling memory manager...
	$(CC) $(OPTIONS) -c lib/layers/memory.c -o $@

.list.o: lib/layers/list.c include/list.h
	$(INFO) Compiling list implementation...
	$(CC) $(OPTIONS) -c lib/layers/list.c -o $@

.reslist.o: lib/layers/reslist.c include/memory.h include/reslist.h
	$(INFO) Compiling resource list module...
	$(CC) $(OPTIONS) -c lib/layers/reslist.c -o $@

.compress.o: lib/compression/main.c include/compress.h include/disk.h include/memory.h
	$(INFO) Compiling main compression library...
	$(CC) $(OPTIONS) -c lib/compression/main.c -o .compress.o -o $@

.lzg_compress.o: lib/compression/lzg_compress.c include/compress.h
	$(INFO) Compiling LZG compression module...
	$(CC) $(OPTIONS) -c lib/compression/lzg_compress.c -o $@

.rle_compress.o: lib/compression/rle_compress.c
	$(INFO) Compiling RLE compression module...
	$(CC) $(OPTIONS) -c lib/compression/rle_compress.c -o $@

.lzg_uncompress.o: lib/compression/lzg_uncompress.c include/compress.h
	$(INFO) Compiling LZG uncompression module...
	$(CC) $(OPTIONS) -c lib/compression/lzg_uncompress.c -o $@

.rle_uncompress.o: lib/compression/rle_uncompress.c include/compress.h
	$(INFO) Compiling RLE uncompression module...
	$(CC) $(OPTIONS) -c lib/compression/rle_uncompress.c -o $@

.pr.o: lib/pr.c include/classify.h include/common.h include/disk.h include/export.h include/idlist.h include/import.h include/memory.h include/resources.h
	$(INFO) Compiling main library primitives for both modes...
	$(CC) $(OPTIONS) -c lib/pr.c -o $@
	
.main.o: console/main.c include/common.h include/compress.h include/disk.h include/filedir.h include/getopt.h include/memory.h include/parse.h
	$(INFO) Compiling command parsing module for standard mode...
	$(CC) $(OPTIONS) -c console/main.c -o $@

.filedir.o: console/filedir.c include/disk.h include/memory.h include/search.h
	$(INFO) Compiling directory and recursive file reading module for standard mode...
	$(CC) $(OPTIONS) -c console/filedir.c -o $@

.resources.o: lib/xml/resources.c include/common.h include/disk.h include/resources.h include/search.h
	$(INFO) Compiling XML resource reading module...
	$(CC) $(OPTIONS) -c lib/xml/resources.c -o $@

.parse.o: lib/xml/parse.c include/common.h include/disk.h include/memory.h include/parse.h
	$(INFO) Compiling XML parsing module...
	$(CC) $(OPTIONS) -c lib/xml/parse.c -o $@

.search.o: lib/xml/search.c include/common.h include/memory.h include/parse.h include/reslist.h include/search.h
	$(INFO) Compiling XML search features...
	$(CC) $(OPTIONS) -c lib/xml/search.c -o $@

.bmp.o: lib/formats/bmp.c include/bmp.h include/common.h include/dat.h include/disk.h include/memory.h
	$(INFO) Compiling bitmap files support \(bmp\)...
	$(CC) $(OPTIONS) -c lib/formats/bmp.c -o $@

.mid.o: lib/formats/mid.c include/common.h include/dat.h include/disk.h include/mid.h
	$(INFO) Compiling MIDI audio files support \(mid\)...
	$(CC) $(OPTIONS) -c lib/formats/mid.c -o $@

.plv.o: lib/formats/plv.c include/dat.h include/disk.h include/plv.h
	$(INFO) Compiling prince level files support \(plv\)...
	$(CC) $(OPTIONS) -c lib/formats/plv.c -o $@

.pal.o: lib/formats/pal.c include/dat.h include/disk.h include/pal.h
	$(INFO) Compiling JASC palette support \(pal\)...
	$(CC) $(OPTIONS) -c lib/formats/pal.c -o $@

.wav.o: lib/formats/wav.c include/dat.h include/disk.h include/wav.h
	$(INFO) Compiling digital wave audio support \(wav\)...
	$(CC) $(OPTIONS) -c lib/formats/wav.c -o $@

.dirent.o: ports/dirent.c include/direntwin.h
	$(INFO) Porting the dirent structure...
	$(CC) $(OPTIONS) -c ports/dirent.c -o $@
	
.getopt.o: ports/getopt.c include/getopt.h
	$(INFO) Porting the Unix-like getopt function \(first part\)...
	$(CC) $(OPTIONS) -c ports/getopt.c -o $@

.getopt1.o: ports/getopt1.c include/getopt.h
	$(INFO) Porting the Unix-like getopt function \(second part\)...
	$(CC) $(OPTIONS) -c ports/getopt1.c -o $@

#python
.python.o: addons/python/python.c include/pr.h
	$(INFO) Building the python interfaces
	$(CC) -pthread -fno-strict-aliasing -DNDEBUG -g -O3 -Wall -fPIC -I/usr/include/python2.3 -c addons/python/python.c $(INCLUDE) -o $@
	
pylib: .python.o $(LIBOBJ)
	$(INFO) Building the python module
	$(MAKEDIR) addons/python/bin
	$(CC) -pthread -shared .python.o $(LIBOBJ) -o addons/python/bin/pr.so -o $@

