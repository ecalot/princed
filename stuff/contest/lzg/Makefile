# Makefile for the lzg compressor/extractor

CC=gcc
CPPFLAGS=-Wall -ansi -pedantic -g

all: lzg

lzg: lzg.o lzg_compress.o lzg_uncompress.o

lzg.o: lzg.c lzg_compress.h lzg_uncompress.h
lzg_compress.o: lzg_compress.h
lzg_uncompress.o: lzg_uncompress.h

clean:
	rm -fv *.o lzg
