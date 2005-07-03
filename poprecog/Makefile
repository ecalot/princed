############
# Programs #
############

CC         = gcc
LINKER     = ld
INFO       = @echo
MAKEDIR    = @mkdir -p
REMOVER    = @rm -f
GZIP       = @gzip
COPY       = @ln -f

####################
# Compiler options #
####################

#Libraries: include path and linked libs
INCLUDE       =
LIBS          = `allegro-config --libs --cflags` 

RELEASE       = -g
#-g -Wall -pedantic 
LINKERRELEASE =

MAIN     = .poprecog.o

EXEOBJ   = $(MAIN)

EXEFILE  = poprecog

#Use this to temporary remove an option
OPTIONS       = $(INCLUDE) $(DEFINES) $(RELEASE)
LINKEROPTIONS = $(LINKERRELEASE) $(LIBS)

#main file

$(EXEFILE): $(EXEOBJ)
	$(INFO) Linking files...
	$(CC) $(LINKEROPTIONS) -o $(EXEFILE) $(EXEOBJ)
	$(INFO) Program successfully compiled
	$(INFO)
	$(INFO) Please read readme.txt for syntax information
	$(INFO)

#command options
.PHONY: clean build all lib

clean:
	$(INFO) Erasing temporary object files...
	$(REMOVER) $(EXEOBJ) $(EXEFILE)

build: clean $(EXEFILE)

all: $(EXEFILE)

#files

.poprecog.o: poprecog.c
	$(INFO) Compiling poprecog...
	$(CC) $(OPTIONS) -c poprecog.c -o $@

