bin/pr: compile.o compress.o extract.o pr.o resources.o tasks.o disk.o xml.o xmlsearch.o bmp.o mid.o pal.o wav.o plv.o prlib.o
	
	@echo Linking files...
	@mkdir -p bin
	@gcc -o bin/pr compile.o compress.o extract.o pr.o resources.o tasks.o disk.o xml.o xmlsearch.o bmp.o mid.o pal.o wav.o plv.o -s -O2
	@echo Making dynamic library...
	@gcc -o bin/pr.so compile.o compress.o extract.o prlib.o resources.o tasks.o disk.o xml.o xmlsearch.o bmp.o mid.o pal.o wav.o plv.o -O2 -Llibc -shared -dynamic
	@echo Program successfully compiled
	@echo
	@echo Please read readme.txt for syntax information
	@echo

compile.o: compile.c
	@echo Compiling import module...
	@gcc -c compile.c -Iinclude/

extract.o: extract.c
	@echo Compiling export module...
	@gcc -c extract.c -Iinclude/

compress.o: compress.c
	@echo Compiling compression module...
	@gcc -c compress.c -Iinclude/

pr.o: pr.c
	@echo Compiling main module in standard mode...
	@gcc -c pr.c -Iinclude/ -DOS=\"`uname`\" 

prlib.o: pr.c
	@echo Compiling main module in library mode...
	@gcc -c pr.c -Iinclude/ -DDLL -o prlib.o

resources.o: resources.c
	@echo Compiling resource manager module...
	@gcc -c resources.c -Iinclude/

tasks.o: tasks.c
	@echo Compiling extra tasks module...
	@gcc -c tasks.c -Iinclude/

xml.o: xml.c
	@echo Compiling xml parsing module...
	@gcc -c xml.c -Iinclude/

xmlsearch.o: xmlsearch.c
	@echo Compiling xml search features...
	@gcc -c xmlsearch.c -Iinclude/

disk.o: disk.c
	@echo Compiling disk access functions...
	@gcc -c disk.c -Iinclude/

bmp.o: formats/bmp.c
	@echo Compiling bitmap files support \(bmp\)...
	@gcc -c formats/bmp.c -Iinclude/

mid.o: formats/mid.c
	@echo Compiling midi audio files support \(mid\)...
	@gcc -c formats/mid.c -Iinclude/

plv.o: formats/plv.c
	@echo Compiling prince level files support \(plv\)...
	@gcc -c formats/plv.c -Iinclude/

pal.o: formats/pal.c
	@echo Compiling JASC palette support \(pal\)...
	@gcc -c formats/pal.c -Iinclude/

wav.o: formats/wav.c
	@echo Compiling digital wave audio support \(wav\)...
	@gcc -c formats/wav.c -Iinclude/
