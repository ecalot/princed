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
  #RELEASE       = -g -Wall -pedantic -fPIC for .so files
  RELEASE       = -g -Wall -pedantic
  LINKERRELEASE =
else
  RELEASE       = -O3 
  LINKERRELEASE = -s
endif

ACTIONS  = .import.o .export.o .classify.o
XML      = .parse.o .search.o .unknown.o .translate.o .tree.o
COMPRESS = .lzg_compress.o .lzg_decompress.o .rle_compress.o .rle_decompress.o .rlev_decompress.o 
LAYERS   = .dat.o .memory.o .list.o .reslist.o .pallist.o .disk.o .idlist.o .autodetect.o .stringformat.o .resourcematch.o .auxiliary.o
FORMAT   = .bmp.o .mid.o .pal.o .wav.o .plv.o
OBJECT   = .object.o .pop2_256c.o .pop1_16c.o .image2.o .image16.o .image256.o .binary.o .sound_common.o .level_common.o .image_common.o
CONSOLE  = .main.o .filedir.o
MAIN     = .pr.o

EXEOBJ   = $(MAIN) $(XML) $(COMPRESS) $(ACTIONS) $(LAYERS) $(OBJECT) $(FORMAT) $(PORTS) $(CONSOLE)
LIBOBJ   = $(MAIN) $(XML) $(COMPRESS) $(ACTIONS) $(LAYERS) $(OBJECT) $(FORMAT) $(PORTS)

EXEFILE  = bin/pr
XMLFILE  = bin/resources.xml

ADDONS   = .python.o

#Use this to temporary remove an option
OPTIONS       = $(INCLUDE) $(DEFINES) $(RELEASE)
LINKEROPTIONS = $(LINKERRELEASE) $(LIBS)

#main file

$(EXEFILE): $(EXEOBJ) $(XMLFILE)
	$(INFO) Linking files...
	$(MAKEDIR) bin
	$(CC) $(OPTIONS) -o $(EXEFILE) $(EXEOBJ)
	$(INFO) Program successfully compiled
	$(INFO)
	$(INFO) Please read readme.txt for syntax information
	$(INFO)

#command options
.PHONY: clean cleanxml build all libs

clean:
	$(INFO) Erasing temporary object files...
	$(REMOVER) $(EXEOBJ) $(EXEFILE) $(ADDONS)

cleanxml:
	$(INFO) Erasing xml file...
	$(REMOVER) $(XMLFILE)

build: clean bin/pr

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

$(XMLFILE): xml/resources.xml
	$(INFO) Copying resources...
	$(MAKEDIR) bin
	$(COPY) xml/resources.xml $(XMLFILE)

.import.o: lib/actions/import.c include/common.h include/import.h include/autodetect.h include/dat.h include/disk.h include/idlist.h include/memory.h include/unknown.h include/object.h
	$(INFO) Compiling import module...
	$(CC) $(OPTIONS) -c lib/actions/import.c -o $@

.classify.o: lib/actions/classify.c include/autodetect.h include/classify.h include/common.h include/dat.h include/disk.h include/memory.h
	$(INFO) Compiling classification module...
	$(CC) $(OPTIONS) -c lib/actions/classify.c -o $@

.export.o: lib/actions/export.c include/common.h include/export.h include/autodetect.h include/dat.h include/disk.h include/idlist.h include/memory.h include/object.h include/palette.h include/pallist.h include/unknown.h
	$(INFO) Compiling export module...
	$(CC) $(OPTIONS) -c lib/actions/export.c -o $@

.autodetect.o: lib/layers/autodetect.c include/autodetect.h include/auxiliary.h
	$(INFO) Compiling resource autodetection module...
	$(CC) $(OPTIONS) -c lib/layers/autodetect.c -o $@

.idlist.o: lib/layers/idlist.c include/disk.h include/idlist.h include/memory.h include/resourcematch.h
	$(INFO) Compiling id list layer...
	$(CC) $(OPTIONS) -c lib/layers/idlist.c -o $@

.disk.o: lib/layers/disk.c include/memory.h include/disk.h include/common.h include/direntwin.h
	$(INFO) Compiling disk access library...
	$(CC) $(OPTIONS) -c lib/layers/disk.c -o $@

.dat.o: lib/layers/dat.c include/common.h include/dat.h include/disk.h include/reslist.h
	$(INFO) Compiling DAT editing library...
	$(CC) $(OPTIONS) -c lib/layers/dat.c -o $@

.memory.o: lib/layers/memory.c include/memory.h include/binary.h
	$(INFO) Compiling memory manager...
	$(CC) $(OPTIONS) -c lib/layers/memory.c -o $@

.list.o: lib/layers/list.c include/list.h
	$(INFO) Compiling list implementation...
	$(CC) $(OPTIONS) -c lib/layers/list.c -o $@

.reslist.o: lib/layers/reslist.c include/memory.h include/reslist.h
	$(INFO) Compiling resource list module...
	$(CC) $(OPTIONS) -c lib/layers/reslist.c -o $@

.level_common.o: lib/object/level/level_common.c include/binary.h include/common.h include/dat.h include/disk.h include/plv.h include/types.h
	$(INFO) Compiling level_common.class module...
	$(CC) $(OPTIONS) -c lib/object/level/level_common.c -o $@

.sound_common.o: lib/object/sound/sound_common.c include/binary.h include/common.h include/wav.h include/dat.h include/types.h
	$(INFO) Compiling common functions for sound type class...
	$(CC) $(OPTIONS) -c lib/object/sound/sound_common.c -o $@

.pallist.o: lib/layers/pallist.c include/pallist.h include/reslist.h include/object.h
	$(INFO) Compiling palette list module...
	$(CC) $(OPTIONS) -c lib/layers/pallist.c -o $@

.object.o: lib/object/object.c include/common.h include/object.h include/types.h include/image.h include/other.h include/palette.h include/sound.h
	$(INFO) Compiling main item class...
	$(CC) $(OPTIONS) -c lib/object/object.c -o $@

.image2.o: lib/object/image/image2.c include/compress.h include/common.h include/image.h include/memory.h include/disk.h include/dat.h include/object.h include/palette.h include/bitmap.h
	$(INFO) Compiling 2 colors image type class...
	$(CC) $(OPTIONS) -c lib/object/image/image2.c -o $@

.image16.o: lib/object/image/image16.c include/compress.h include/common.h include/image.h include/memory.h include/disk.h include/dat.h include/object.h include/palette.h include/bitmap.h
	$(INFO) Compiling 16 colors image type class...
	$(CC) $(OPTIONS) -c lib/object/image/image16.c -o $@

.image256.o: lib/object/image/image256.c include/compress.h include/common.h include/image.h include/memory.h include/disk.h include/dat.h include/object.h include/palette.h include/bitmap.h
	$(INFO) Compiling 256 colors image type class...
	$(CC) $(OPTIONS) -c lib/object/image/image256.c -o $@

.image_common.o: lib/object/image/image_common.c include/compress.h include/common.h include/image.h include/memory.h include/disk.h include/dat.h include/object.h include/palette.h include/bitmap.h
	$(INFO) Compiling common functions for image type class...
	$(CC) $(OPTIONS) -c lib/object/image/image_common.c -o $@

.lzg_compress.o: lib/compression/lzg_compress.c include/compress.h
	$(INFO) Compiling LZG compression module...
	$(CC) $(OPTIONS) -c lib/compression/lzg_compress.c -o $@

.rle_compress.o: lib/compression/rle_compress.c
	$(INFO) Compiling RLE compression module...
	$(CC) $(OPTIONS) -c lib/compression/rle_compress.c -o $@

.lzg_decompress.o: lib/compression/lzg_decompress.c include/compress.h
	$(INFO) Compiling LZG decompression module...
	$(CC) $(OPTIONS) -c lib/compression/lzg_decompress.c -o $@

.rle_decompress.o: lib/compression/rle_decompress.c include/compress.h
	$(INFO) Compiling RLE decompression module...
	$(CC) $(OPTIONS) -c lib/compression/rle_decompress.c -o $@

.rlev_decompress.o: lib/compression/rlev_decompress.c include/compress.h
	$(INFO) Compiling RLEC decompression module...
	$(CC) $(OPTIONS) -c lib/compression/rlev_decompress.c -o $@

.pr.o: lib/pr.c include/common.h include/classify.h include/export.h include/import.h include/disk.h include/idlist.h include/memory.h include/types.h include/search.h include/unknown.h
	$(INFO) Compiling main library primitives for both modes...
	$(CC) $(OPTIONS) -c lib/pr.c -o $@
	
.main.o: console/main.c include/getopt.h include/getopt.h include/common.h include/memory.h include/filedir.h include/disk.h include/compress.h include/parse.h
	$(INFO) Compiling command parsing module for standard mode...
	$(CC) $(OPTIONS) -c console/main.c -o $@

.filedir.o: console/filedir.c include/disk.h include/memory.h include/search.h
	$(INFO) Compiling directory and recursive file reading module for standard mode...
	$(CC) $(OPTIONS) -c console/filedir.c -o $@

.unknown.o: lib/xml/unknown.c include/common.h include/disk.h include/memory.h include/parse.h include/stringformat.h include/translate.h include/tree.h include/unknown.h
	$(INFO) Compiling unknown.xml generating module...
	$(CC) $(OPTIONS) -c lib/xml/unknown.c -o $@

.tree.o: lib/xml/tree.c include/common.h include/list.h include/memory.h include/parse.h include/unknown.h
	$(INFO) Compiling XML tree special handling routines...
	$(CC) $(OPTIONS) -c lib/xml/tree.c -o $@

.parse.o: lib/xml/parse.c include/parse.h include/disk.h include/memory.h include/common.h
	$(INFO) Compiling XML parsing module...
	$(CC) $(OPTIONS) -c lib/xml/parse.c -o $@

.search.o: lib/xml/search.c include/common.h include/memory.h include/parse.h include/search.h include/translate.h
	$(INFO) Compiling XML search features...
	$(CC) $(OPTIONS) -c lib/xml/search.c -o $@

.translate.o: lib/xml/translate.c include/translate.h
	$(INFO) Compiling array tranlation features...
	$(CC) $(OPTIONS) -c lib/xml/translate.c -o $@

.stringformat.o: lib/layers/stringformat.c include/translate.h include/stringformat.h
	$(INFO) Compiling unknown string format parsing feature...
	$(CC) $(OPTIONS) -c lib/layers/stringformat.c -o $@

.pop2_256c.o: lib/object/palette/pop2_256c.c include/palette.h include/memory.h include/dat.h include/disk.h include/auxiliary.h include/pal.h
	$(INFO) Compiling the POP2 256 colors palette class support...
	$(CC) $(OPTIONS) -c  lib/object/palette/pop2_256c.c -o $@

.pop1_16c.o: lib/object/palette/pop1_16c.c include/palette.h include/memory.h include/dat.h include/disk.h include/pal.h
	$(INFO) Compiling the POP1 16 colors palette class support...
	$(CC) $(OPTIONS) -c  lib/object/palette/pop1_16c.c -o $@

.binary.o: lib/object/other/binary.c include/binary.h include/common.h include/dat.h include/types.h include/disk.h
	$(INFO) Compiling the binary item class support...
	$(CC) $(OPTIONS) -c  lib/object/other/binary.c -o $@

.resourcematch.o: lib/layers/resourcematch.c include/memory.h include/resourcematch.h include/translate.h
	$(INFO) Compiling resource list parsing feature...
	$(CC) $(OPTIONS) -c lib/layers/resourcematch.c -o $@

.auxiliary.o: lib/layers/auxiliary.c include/auxiliary.h include/disk.h
	$(INFO) Compiling object auxiliary routines...
	$(CC) $(OPTIONS) -c lib/layers/auxiliary.c -o $@

.bmp.o: lib/formats/bmp.c include/bmp.h include/common.h include/compress.h include/dat.h include/disk.h include/memory.h include/palette.h
	$(INFO) Compiling bitmap files support \(bmp\)...
	$(CC) $(OPTIONS) -c lib/formats/bmp.c -o $@

.mid.o: lib/formats/mid.c include/common.h include/dat.h include/disk.h include/mid.h
	$(INFO) Compiling MIDI audio files support \(mid\)...
	$(CC) $(OPTIONS) -c lib/formats/mid.c -o $@

.plv.o: lib/formats/plv.c include/plv.h include/disk.h include/dat.h
	$(INFO) Compiling prince level files support \(plv\)...
	$(CC) $(OPTIONS) -c lib/formats/plv.c -o $@

.pal.o: lib/formats/pal.c include/pal.h include/disk.h include/dat.h
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

#cpp
.cppdat.o: addons/cpp/dat.cpp include/pr.h
	$(INFO) Building the C++ DAT handling interface
	g++ -c addons/cpp/dat.cpp -Iinclude -o .cppdat.o

.cppxml.o: addons/cpp/xml.cpp include/pr.h
	$(INFO) Building the C++ XML handling interface
	g++ -c addons/cpp/xml.cpp -Iinclude -o .cppxml.o

cpp: .cppdat.o .cppxml.o

