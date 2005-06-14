/*  Princed V3 - Prince of Persia Level Editor for PC Version
    Copyright (C) 2003 Princed Development Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    The authors of this program may be contacted at http://forum.princed.com.ar
*/

/*
memory.c: Princed Resources : Memory handling
¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 20 Dec 2003

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.01 (2004-Feb-13)

 Note:
  DO NOT remove this copyright notice
*/

#include "memory.h"
#include <string.h>

#ifndef MEM_CHECK
char* strallocandcopy(const char* text) {
	int size;
	char* aux;
	if (text==NULL) return NULL;
	size=strlen(text)+1;
	aux=(char*)malloc(size);
	if (aux) memcpy(aux,text,size);
	return aux;
}

#else
#include <stdlib.h>
#include <stdio.h>

typedef struct memm {
	struct memm* next;
	void* pointer;
	char  file[30];
	int   line;
	int   size;
}memm;

static memm* list=NULL;
static int memcounter=0;
static long int memsize=0;
static long int memsizef=0;

void* mymalloc(int size,char* fileName,int line) {
	memm* nodo;
	void* p;
	p=malloc(size);
	nodo=malloc(sizeof(memm));
	nodo->next=list;
	list=nodo;
	strcpy(nodo->file,fileName);
	nodo->line=line;
	nodo->size=size;
	nodo->pointer=p;
	/*if (!(memcounter%100))
	//fprintf(stderr,"Memory allocation: %d bytes at %p in %d@%s (%d/%d)\n",size,p,line,fileName,memsize,memcounter); */
	memcounter++;
	memsize+=size;
	memsizef+=size;
	return p;
}

void myfree(void* a,char* fileName,int line) {
	memm* currentNode;
	memm* priorNode=NULL;
	memcounter--;
	/*if ((!(memcounter%100))||((memcounter<100)&&(memcounter>-100)))
	//fprintf(stderr,"Liberando memoria? -> %p (%d/%d)\n",a,memsize,memcounter);*/

	free(a);

	if (list==NULL) {
		fprintf(stderr,"Error!!!! se pidio liberar %p pero no estaba alocado (lista vacia)\n",a);
		return; /* Empty list */
	}
	currentNode=list;
	while ((currentNode->pointer!=a)&&(currentNode->next!=NULL)) {
		priorNode=currentNode;
		currentNode=currentNode->next;
	}
	if (currentNode->pointer!=a) {
		fprintf(stderr,"Error!!!! se pidio liberar %p pero no estaba alocado %d@%s\n",a,line,fileName);
		return; /* Not found */
	}

	memsizef-=currentNode->size;

	/*fprintf(stderr,"Liberada: %d bytes at %p alloc: %d@%s | free  %d@%s  (%d/%d)\n",currentNode->size,currentNode->pointer,currentNode->line,currentNode->file,line,fileName,memsize,memcounter);*/

	/* free node and set prior pointer to the next */
	if (priorNode==NULL) {
		list=currentNode->next;
	} else {
		priorNode->next=currentNode->next;
	}
	free(currentNode);

}

void showInfo(void* a) {
	memm* currentNode;
	memm* priorNode=NULL;

	if (list==NULL) {
		fprintf(stderr,"lista vacia\n");
		return; /* Empty list */
	}
	currentNode=list;
	while ((currentNode->pointer!=a)&&(currentNode->next!=NULL)) {
		priorNode=currentNode;
		currentNode=currentNode->next;
	}
	if (currentNode->pointer!=a) {
		fprintf(stderr,"Puntero no alocado\n");
		return; /* Not found */
	}

	fprintf(stderr,"showInfo: %d bytes at %p alloc: %d@%s Mem tot: %d\n",currentNode->size,currentNode->pointer,currentNode->line,currentNode->file,memcounter);

}

void showStats() {
	fprintf(stderr,"Total de memoria manejada: %d. Allocaciones: %d. Memoria no liberada: %d\n",memsize,memcounter,memsizef);

	memm* currentNode;
	if (list==NULL) {
		fprintf(stderr,"Todo COOL!!!!!\n");
		return; /* Empty list */
	}
	currentNode=list;
	do {
		fprintf(stderr,"en %s@%d alocaste %d en %p y no los liberaste\n",currentNode->file,currentNode->line,currentNode->size,currentNode->pointer);
		currentNode=currentNode->next;
	} while (currentNode->next!=NULL);

}
#endif
