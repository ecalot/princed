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
AWK        = @awk

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
                input.o kid.o states.o tiles.o
EXEFILE       = bin/freeprince

GENERATEDRESHEADERS = include/res_conf_parts.h include/res_conf_files.h\
                      include/res_conf_types.h include/res_conf_mods.h\
                      include/res_conf_resources.h
GENERATEDSTAHEADERS = include/states_conf_static.h include/states_conf_properties.h\
                      include/states_conf_flags.h

GENERATEDTILHEADERS = include/tiles_conf_types.h include/tiles_conf_groups.h

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

check: checkcompiler checksdl checkawk

checkcompiler:
	$(INFO) Checking C compiler...
	$(CC) --version>/dev/null

checksdl:
	$(INFO) Checking SDL library...
	@sdl-config --version>/dev/null

checkawk:
	$(INFO) Checking AWK...
	@which awk>/dev/null

###################
# command options #
###################

clean:
	$(INFO) Erasing temporary object files...
	$(REMOVER) $(OBJFILES) $(EXEFILE)\
	           $(GENERATEDRESHEADERS) $(GENERATEDSTAHEADERS) $(GENERATEDTILHEADERS)\
						 $(SRC2)

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

main.o: main.c include/kernel.h include/main.h include/maps.h\
        include/map_defs.h include/tiles.h include/tiles_conf.h
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

kid.o: ker/kid.c include/kid.h include/resources.h include/res_conf.h\
       include/states.h include/states_conf.h $(GENERATEDRESHEADERS)\
			 $(GENERATEDTILHEADERS) include/res_conf.h\
       include/types.h include/tiles.h include/tiles_conf.h
	$(INFO) Compiling kernel kid object...
	$(CC) -c ker/kid.c $(OPTIONS)

titles.o: ker/titles.c include/resources.h include/res_conf.h
	$(INFO) Compiling kernel titles module...
	$(CC) -c ker/titles.c $(OPTIONS)

compress.o: res/compress.c include/compress.h include/memory.h \
            include/disk.h
	$(INFO) Compiling resource compression module...
	$(CC) -c res/compress.c $(OPTIONS)

maps.o: res/maps.c include/maps.h include/map_defs.h include/tiles.h include/tiles_conf.h
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

states.o: ker/states.c include/states.h include/states_conf.h\
          include/resources.h include/res_conf.h include/types.h\
					include/tiles.h include/tiles_conf.h $(GENERATEDRESHEADERS)\
					$(GENERATEDSTAHEADERS) $(GENERATEDTILHEADERS)
	$(INFO) Compiling kernel states module...
	$(CC) -c ker/states.c $(OPTIONS)

tiles.o: res/tiles.c include/tiles.h include/tiles_conf.h
	$(INFO) Compiling resource tile classification module...
	$(CC) -c res/tiles.c $(OPTIONS)


#AWK scripts: header generation
include/res_conf.h: $(GENERATEDRESHEADERS)

include/res_conf_parts.h: conf/parts.conf conf/awk/res_conf_parts.awk
	$(INFO) Creating resource id parts configuration file...
	$(AWK) -f conf/awk/res_conf_parts.awk conf/parts.conf>include/res_conf_parts.h

include/res_conf_files.h: conf/files.conf conf/awk/res_conf_files.awk
	$(INFO) Creating resource dat files list configuration file...
	$(AWK) -f conf/awk/res_conf_files.awk conf/files.conf>include/res_conf_files.h

include/res_conf_types.h: conf/types.conf conf/awk/res_conf_types.awk
	$(INFO) Creating resource types configuration file...
	$(AWK) -f conf/awk/res_conf_types.awk conf/types.conf>include/res_conf_types.h

include/res_conf_mods.h: conf/mods.conf conf/awk/res_conf_mods.awk
	$(INFO) Creating resource modifiers configuration file...
	$(AWK) -f conf/awk/res_conf_mods.awk conf/mods.conf>include/res_conf_mods.h

include/res_conf_resources.h: conf/resources.conf conf/awk/res_conf_resources.awk
	$(INFO) Creating resource list configuration file...
	$(AWK) -f conf/awk/res_conf_resources.awk conf/resources.conf>include/res_conf_resources.h

include/states_conf.h: $(GENERATEDSTAHEADERS)

include/states_conf_static.h: conf/states.conf conf/awk/states_conf_static.awk
	$(INFO) Creating states list configuration file...
	$(AWK) -f conf/awk/states_conf_static.awk conf/states.conf>include/states_conf_static.h

include/states_conf_properties.h: conf/statesproperties.conf conf/awk/states_conf_properties.awk
	$(INFO) Creating states condition properties configuration file...
	$(AWK) -f conf/awk/states_conf_properties.awk conf/statesproperties.conf>include/states_conf_properties.h

include/states_conf_flags.h: conf/flags.conf conf/awk/states_conf_flags.awk
	$(INFO) Creating states frame flags configuration file...
	$(AWK) -f conf/awk/states_conf_flags.awk conf/flags.conf>include/states_conf_flags.h

include/tiles_conf.h: $(GENERATEDTILHEADERS)

include/tiles_conf_types.h: conf/tiles.conf conf/awk/tiles_conf_types.awk
	$(INFO) Creating tile list configuration file...
	$(AWK) -f conf/awk/tiles_conf_types.awk conf/tiles.conf>include/tiles_conf_types.h

include/tiles_conf_groups.h: conf/tiles.conf conf/awk/tiles_conf_groups.awk
	$(INFO) Creating tile groups configuration file...
	$(AWK) -f conf/awk/tiles_conf_groups.awk conf/tiles.conf>include/tiles_conf_groups.h

