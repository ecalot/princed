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
filedir.c: Princed Resources : read command line file arguments and generate a file list
¯¯¯¯¯¯
 Copyright 2005 Princed Development Team
  Created: 19 Mar 2005

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2005-Mar-19)

 Note:
  DO NOT remove this copyright notice
*/

#include "xmlsearch.h" /* tTag */
#include "memory.h" /* malloc, strallocandcopy */
#include <string.h> /* strcat */
#include "disk.h"

/* Private Searching Structures */

/* File List Structure */
typedef struct tFileDir {
	char* text;
	struct tFileDir* next;
}tFileDir;

typedef struct {
	tFileDir* filenames;
	tFileDir* options;
}tFileDir2;

/* layer 1: stack implementation */

int filedir_push(const char* text, tFileDir** list, int checkEquals) {
	/*
		Adds the file to the list only once
	*/
	tFileDir* node;

	/* Verify if the file exists */
	if (checkEquals) {
		node=*list;
		while (node) {
			if (equalsIgnoreCase(node->text,text)) /* TODO: use defines If file was in the list, do nothing */
			/*if (!strcmp(node->file,text)) * If file was in the list, do nothing */
				return -1;
			node=node->next;
		}
	}

	/* Add new node */
	node=(tFileDir*)malloc(sizeof(tFileDir));

	/* Use LIFO because its more probable to get a file with the same name */
	node->text=strallocandcopy(text);
	node->next=*list;
	*list=node;
	return 0;
}

char* filedir_pop(tFileDir** list) {
	/*
		Returns and removes one random file from the list
	*/
	char* result;
	tFileDir* aux;
	if (*list) {
		/* Remember node values */
		aux=*list;
		result=(*list)->text;
		/* move one position */
		*list=(*list)->next;
		/* free node */
		free(aux);

		return result;
	} else {
		return NULL;
	}
}

/* layer 2, module Import one directory */
typedef struct {
	const char* dir;
	const char* opt;
	tFileDir2*  list;
}tPassListAndDir;

void addFileToList(const char* file,void* pass2) {
	register tPassListAndDir* pass=pass2;
	char path[256];
	char* dat;
	if (pass->dir) {
		strcpy(path,pass->dir);
		strcat(path,"/");
		strcat(path,file);
	} else {
		strcpy(path,file);
	}
	dat=strallocandcopy(getFileNameFromPath(path));
	strcat(path,pass->opt);
	if(!filedir_push(path,&(pass->list->filenames),1))
		filedir_push(dat,&(pass->list->options),0);
	free(dat);
}

void addFileToListTag(const tTag* t,void* pass2) {
	addFileToList(t->file,pass2);
}

/* Search all files in the xml tree and returns them */
int listAllDatFiles(const char* vResFile, const char* directory, const char* opt, tFileDir2* list) {
	/* Declare error variable */
	tPassListAndDir pass;
	tTag* structure;
	int error;
	
	pass.dir=directory;
	pass.opt=opt;
	pass.list=list;

	/* Generate xml structure if doesn't exist */
	if ((error=parseStructure(vResFile,&structure))) return error;

	/* Use the xml structure to Generate the file list */
	workTree(structure,&pass,addFileToListTag);

	/* All done */
	list=pass.list;
	return 0;
}

/* layer 2, module check if a file is in the xml */
typedef struct {
	const char* file;
	int result;
}tPassFileAndResult;

void checkIfFileExists(const tTag* t,void* pass2) {
	register tPassFileAndResult* pass=pass2;
	if (equalsIgnoreCase(pass->file,t->file)) pass->result=1;
}

int isADatFile(const char* vResFile, const char* file) {
	/* Declare error variable */
	tPassFileAndResult pass;
	tTag* structure;
	int error;
	
	pass.file=getFileNameFromPath(file);
	pass.result=0;

	/* TODO: directory must end with / */
	
	/* Generate xml structure if doesn't exist */
	if ((error=parseStructure(vResFile,&structure))) return error;

	/* Use the xml structure to Generate the file list */
	workTree(structure,&pass,checkIfFileExists);

	/* All done */
	return pass.result;
}

/* layer 2, module check Import type */
int fileDirGetFilesImport(tFileDir2* list1,tFileDir2* files,const char* resfile) {
	char* file;
	char* opt;
	whatIs type;
	int dirs=0;
	int fils=0;
	char output[255];
	int parseError=0;

	files->filenames=NULL;
	files->options=NULL;
	
	while ((file=filedir_pop(&(list1->filenames)))) {
		opt=filedir_pop(&(list1->options));
		type=isDir(file);
		
		/* a not found file may become either a directory or a file depending on the res file */
		if (type==eNotFound) {
			int isdat=isADatFile(resfile,file);
			if (isdat<0) parseError=isdat;

			if (isdat) {
				type=eFile;
			} else {
				type=eDirectory;
			}
		}
		
		if ((!dirs)&&type==eDirectory) {
			parseError=listAllDatFiles(resfile, file, opt, files);
		}
		if (type==eDirectory) dirs++;
		if (type==eFile) {
			fils++;
			strcpy(output,file);
			filedir_push(getFileNameFromPath(output), &(files->options), 0);
			strcat(output,opt);
			filedir_push(output, &(files->filenames), 0);
		}

		free(opt);
		free(file);
	}
	printf("fils=%d dirs=%d\n",fils,dirs);
	if (dirs>1) {
		while ((file=filedir_pop(&(files->filenames)))) free(file); /* empty list */
		return -20;
	}
	if (!fils&&!dirs)
		return -22; /* no files selected */

	if (parseError) {
		while ((file=filedir_pop(&(files->filenames)))) free(file); /* empty list */
		return parseError;
	}
	return 0;
}

/* layer 2, module Export */
int fileDirGetFilesExport(tFileDir2* list1,tFileDir2* files,int notHasRecursiveFlag) {
	char* file;
	char* opt;
	tPassListAndDir pass;
	whatIs type;
	char output[255];

	files->filenames=NULL;
	files->options=NULL;

	while ((file=filedir_pop(&(list1->filenames)))) {
		opt=filedir_pop(&(list1->options));
		type=isDir(file);
		if (type==eDirectory) {
			pass.dir=NULL;
			pass.opt=opt;
			pass.list=files;
			recurseDirectory(file,!notHasRecursiveFlag,&pass,addFileToList);
			files=pass.list;
		} else {
			strcpy(output,file);
			filedir_push(getFileNameFromPath(output), &(files->options), 0);
			strcat(output,opt);
			filedir_push(output, &(files->filenames), 0);
		}
		free(opt);
		free(file);
	}
	return 0;
}




/* layer 3, primitives */

void fileDirClearOptions(tFileDir2* list1) {
	list1->filenames=NULL;
	list1->options=NULL;
}

void fileDirAddOption(tFileDir2* list1, const char* option) {
	char fn[256];
	char op[256];
	enum {eop,efn} mode=efn;
	const char* pOpt=option;
	char* pFn=fn;
	char* pOp=op;

	do {
		if (*pOpt=='@')
			mode=eop;
		if (mode==eop) {
			*pOp=*pOpt;
			pOp++;
		} else {
			if (pOpt[0]!='/'/*DIR_SEPARATOR*/ || (pOpt[1]!='\0' && pOpt[1]!='@')) { /* ignore last "/" */
				*pFn=*pOpt;
				pFn++;
			}
		}
		pOpt++;
	} while (*pOpt);

	*pOp=0;
	*pFn=0;

	filedir_push(fn, &(list1->filenames),0);
	filedir_push(op, &(list1->options),0);
}



int fileDirGetFiles(tFileDir2* list1,tFileDir2* files,int hasExportFlag,int notHasRecursiveFlag,const char* resfile) {
/* case 1: * import from more than one directory */
 
	if (!hasExportFlag&&!notHasRecursiveFlag) {
		char* file;
		while ((file=filedir_pop(&(list1->filenames)))) {
			free(filedir_pop(&(list1->options)));
			free(file);
		}
		return -21; /* import with recursive flag is now allowed */
	}

	if (hasExportFlag)
		return fileDirGetFilesExport(list1,files,notHasRecursiveFlag);
	else
		return fileDirGetFilesImport(list1,files,resfile);
	
}

char* fileDirGetFile(tFileDir2* files, char** datfile) {
	*datfile=filedir_pop(&(files->options));
	return filedir_pop(&(files->filenames));
}

