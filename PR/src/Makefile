bin/pr: compile.o compress.o extract.o parser.o pr.o resources.o tasks.o bmp.o mid.o pal.o wav.o
	
	@echo Linking files...
	@gcc -o bin/pr compile.o compress.o extract.o parser.o pr.o resources.o tasks.o bmp.o mid.o pal.o wav.o -s
	@echo Program succesfully compiled
	@echo
	@echo Please read readme.txt for syntax information
	@echo
	
parser.o: parser.c
	@echo Compiling parsing library...
	@gcc -c parser.c -Iinclude/

compile.o: compile.c
	@echo Compiling import module...
	@gcc -c compile.c -Iinclude/ -O3

extract.o: extract.c
	@echo Compiling export module...
	@gcc -c extract.c -Iinclude/ -O3

compress.o: compress.c
	@echo Compiling compression module...
	@gcc -c compress.c -Iinclude/ -O3

pr.o: pr.c
	@echo Compiling main module...
	@gcc -c pr.c -Iinclude/ 

resources.o: resources.c
	@echo Compiling resource manager module...
	@gcc -c resources.c -Iinclude/

tasks.o: tasks.c
	@echo Compiling extra tasks module...
	@gcc -c tasks.c -Iinclude/ -O3

bmp.o: formats/bmp.c
	@echo Compiling bitmap files support \(bmp\)...
	@gcc -c formats/bmp.c -Iinclude/ -O3

mid.o: formats/mid.c
	@echo Compiling midi audio files support \(mid\)...
	@gcc -c formats/mid.c -Iinclude/

pal.o: formats/pal.c
	@echo Compiling JASC palette support \(pal\)...
	@gcc -c formats/pal.c -Iinclude/

wav.o: formats/wav.c
	@echo Compiling digital wave audio support \(wav\)...
	@gcc -c formats/wav.c -Iinclude/
