############
# Programs #
############

CC         = @gcc
LINKER     = @ld
INFO       = @echo
MAKEDIR    = @mkdir -p
INDEXER    = @cd ..;./install; cd src
DOWNLOADER = @cd ..;./install -d; cd src
REMOVER    = @rm -f
	
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
endif


####################
# Compiler options #
####################

#Libraries: include path and linked libs
INCLUDE       = -Iinclude/
LIBS          = -lSDL -lpthread

#Defines
DEFINES       = -DOS=\"$(OS)\" $(LINUX) 

#Release type
# RELEASE may be:
#  -g -Wall -ansi -pedantic        for debug
#  -O2                             for release
# LINKERRELEASE may be:
#  -s                              for release
RELEASE       = -g -Wall -ansi -pedantic 
LINKERRELEASE =

#Binary code files
OBJFILES      = main.o kernel.o resources.o dat.o disk.o compress.o \
                output.o maps.o config.o
EXEFILE       = bin/freeprince

#Use this to temporary remove an option
OPTIONS       = $(INCLUDE) $(DEFINES) $(RELEASE)
LINKEROPTIONS = $(LINKERRELEASE)

#############
# main file #
#############

$(EXEFILE): $(OBJFILES)
	$(INFO) Linking files...
	$(MAKEDIR) bin
	$(CC) $(OPTIONS) -o $(EXEFILE) $(OBJFILES) $(LIBS) $(LINKEROPTIONS)
	$(INFO) Program successfully compiled
	$(INFO)
	$(INFO) Please read readme.txt for syntax information
	$(INFO)


###################
# command options #
###################

clean:
	$(INFO) Erasing temporary object files...
	$(REMOVER) $(OBJFILES) $(EXEFILE)

build: clean $(EXEFILE)

all: index build

install: download build

index: ../install
	$(INDEXER)

download: ../install
	$(DOWNLOADER)


################
# Source files #
################

main.o: main.c include/kernel.h include/main.h
	$(INFO) Compiling command line parser...
	$(CC) -c main.c $(OPTIONS)

resources.o: res/resources.c include/resources.h include/compress.h \
             include/dat.h include/disk.h
	$(INFO) Compiling main resource manager module...
	$(CC) -c res/resources.c $(OPTIONS)

disk.o: res/disk.c include/memory.h include/disk.h include/resources.h
	$(INFO) Compiling resource disk access library...
	$(CC) -c res/disk.c $(OPTIONS)

dat.o: res/dat.c include/disk.h include/dat.h
	$(INFO) Compiling resource dat editing library...
	$(CC) -c res/dat.c $(OPTIONS)

kernel.o: ker/kernel.c include/kernel.h include/resources.h include/output.h
	$(INFO) Compiling main kernel...
	$(CC) -c ker/kernel.c $(OPTIONS)

compress.o: res/compress.c include/compress.h include/memory.h \
            include/disk.h
	$(INFO) Compiling resource compression module...
	$(CC) -c res/compress.c $(OPTIONS)

maps.o: res/maps.c include/maps.h
	$(INFO) Compiling resource map handling module...
	$(CC) -c res/maps.c $(OPTIONS)

output.o: out/output.c include/resources.h
	$(INFO) Compiling main output module...
	$(CC) -c out/output.c $(OPTIONS)

config.o: res/config.c include/resources.h
	$(INFO) Compiling resource configuration module...
	$(CC) -c res/config.c $(OPTIONS)

