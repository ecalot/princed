############
# Programs #
############

CC         = @gcc
LINKER     = @gcc
INFO       = @echo
MAKEDIR    = @mkdir -p
INDEXER    = @cd ..;./install; cd src
DOWNLOADER = @cd ..;./install -d; cd src
REMOVER    = @rm -f
AWK        = awk
GREP       = @grep -v -e '^[[:space:]]*\(none[[:space:]]*\|\#.*\)\{0,1\}$$'

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
LIBS          = $(shell sdl-config --libs)

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
                output.o maps.o config.o room.o titles.o \
                input.o kid.o states.o
EXEFILE       = bin/freeprince

GENERATEDRESHEADERS = include/res_conf_parts.h include/res_conf_files.h\
                      include/res_conf_types.h include/res_conf_mods.h\
                      include/res_conf_resources.h
GENERATEDSTAHEADERS = include/states_conf_static.h include/states_conf_properties.h

#Use this to temporary remove an option
OPTIONS       = $(INCLUDE) $(DEFINES) $(RELEASE)
LINKEROPTIONS = $(LINKERRELEASE)

#############
# main file #
#############

$(EXEFILE): check $(OBJFILES)
	$(INFO) Linking files...
	$(MAKEDIR) bin
	$(LINKER) $(LINKEROPTIONS) -o $(EXEFILE) $(OBJFILES) $(LIBS) $(LINKEROPTIONS)
	$(INFO) Program successfully compiled

###################
# program checks  #
###################

check: checkcompiler checksdl checkawk checkgrep checksed

checkcompiler:
	$(INFO) Checking C compiler...
	$(CC) --version>/dev/null

checksdl:
	$(INFO) Checking SDL library...
	@sdl-config --version>/dev/null

checkawk:
	$(INFO) Checking AWK...
	@$(AWK)

checksed:
	$(INFO) Checking Sed...
	@sed --version>/dev/null

checkgrep:
	$(INFO) Checking Grep...
	$(GREP) --version>/dev/null

###################
# command options #
###################

clean:
	$(INFO) Erasing temporary object files...
	$(REMOVER) $(OBJFILES) $(EXEFILE) $(GENERATEDRESHEADERS) $(GENERATEDSTAHEADERS)

build: clean $(EXEFILE)

all: index build

install: download build

download:
	$(MAKEDIR) bin
	$(INFO) Downloading data art files...
	@wget http://fp.princed.com.ar/freeprince.script -O - -nv 2>/dev/null|tar xjC bin

downloadmore:
	$(MAKEDIR) bin
	$(INFO) Downloading extra files...
	@wget http://fp.princed.com.ar/freeprince2.script -O - -nv 2>/dev/null|tar xjC bin

################
# Source files #
################

main.o: main.c include/kernel.h include/main.h
	$(INFO) Compiling command line parser...
	$(CC) -c main.c $(OPTIONS)

resources.o: res/resources.c include/resources.h include/compress.h \
             include/dat.h include/disk.h include/res_conf.h
	$(INFO) Compiling main resource manager module...
	$(CC) -c res/resources.c $(OPTIONS)

disk.o: res/disk.c include/memory.h include/disk.h include/resources.h include/res_conf.h
	$(INFO) Compiling resource disk access library...
	$(CC) -c res/disk.c $(OPTIONS)

dat.o: res/dat.c include/disk.h include/dat.h
	$(INFO) Compiling resource dat editing library...
	$(CC) -c res/dat.c $(OPTIONS)

kernel.o: ker/kernel.c include/kernel.h include/resources.h include/res_conf.h include/output.h
	$(INFO) Compiling main kernel...
	$(CC) -c ker/kernel.c $(OPTIONS)

room.o: ker/room.c include/room.h include/resources.h include/res_conf.h
	$(INFO) Compiling kernel room object...
	$(CC) -c ker/room.c $(OPTIONS)

kid.o: ker/kid.c include/kid.h include/resources.h include/res_conf.h
	$(INFO) Compiling kernel kid object...
	$(CC) -c ker/kid.c $(OPTIONS)

titles.o: ker/titles.c include/resources.h include/res_conf.h
	$(INFO) Compiling kernel titles module...
	$(CC) -c ker/titles.c $(OPTIONS)

compress.o: res/compress.c include/compress.h include/memory.h \
            include/disk.h
	$(INFO) Compiling resource compression module...
	$(CC) -c res/compress.c $(OPTIONS)

maps.o: res/maps.c include/maps.h
	$(INFO) Compiling resource map handling module...
	$(CC) -c res/maps.c $(OPTIONS)

output.o: out/output.c include/resources.h include/res_conf.h
	$(INFO) Compiling main output module...
	$(CC) -c out/output.c $(OPTIONS)

input.o: out/input.c include/input.h
	$(INFO) Compiling main input module...
	$(CC) -c out/input.c $(OPTIONS)

config.o: res/config.c include/resources.h include/res_conf.h
	$(INFO) Compiling resource configuration module...
	$(CC) -c res/config.c $(OPTIONS)

states.o: ker/states.c include/states.h include/states_conf.h include/resources.h include/res_conf.h
	$(INFO) Compiling kernel states module...
	$(CC) -c ker/states.c $(OPTIONS)

#AWK scripts: header generation
include/res_conf.h: $(GENERATEDRESHEADERS)

include/res_conf_parts.h: conf/parts.conf conf/awk/res_conf_parts.awk
	$(INFO) Creating resource id parts configuration file...
	$(GREP) conf/parts.conf|$(AWK) -f conf/awk/res_conf_parts.awk >include/res_conf_parts.h

include/res_conf_files.h: conf/files.conf conf/awk/res_conf_files.awk
	$(INFO) Creating resource dat files list configuration file...
	$(GREP) conf/files.conf|$(AWK) -f conf/awk/res_conf_files.awk >include/res_conf_files.h

include/res_conf_types.h: conf/types.conf conf/awk/res_conf_types.awk
	$(INFO) Creating resource types configuration file...
	$(GREP) conf/types.conf|$(AWK) -f conf/awk/res_conf_types.awk >include/res_conf_types.h

include/res_conf_mods.h: conf/mods.conf conf/awk/res_conf_mods.awk
	$(INFO) Creating resource modifiers configuration file...
	$(GREP) conf/mods.conf|$(AWK) -f conf/awk/res_conf_mods.awk >include/res_conf_mods.h

include/res_conf_resources.h: conf/resources.conf conf/awk/res_conf_resources.awk
	$(INFO) Creating resource list configuration file...
	$(GREP) conf/resources.conf|$(AWK) -f conf/awk/res_conf_resources.awk >include/res_conf_resources.h

include/states_conf.h: $(GENERATEDSTAHEADERS)

include/states_conf_static.h: conf/states.conf conf/awk/states_conf_static.awk
	$(INFO) Creating states list configuration file...
	$(GREP) conf/states.conf|sed -e 's/^\t\t\t/- - - /g' -e 's/^\t\t/- - /g' -e 's/^\t/- /g' -e 's/:$$//g'|$(AWK) -f conf/awk/states_conf_static.awk>include/states_conf_static.h

include/states_conf_properties.h: conf/statesproperties.conf conf/awk/states_conf_properties.awk
	$(INFO) Creating states condition properties configuration file...
	$(GREP) conf/statesproperties.conf|$(AWK) -f conf/awk/states_conf_properties.awk >include/states_conf_properties.h

