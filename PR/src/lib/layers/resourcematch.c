
/* TODO:
 * add headers
 * rename this file to resourcematch.c 
 */

#include <stdio.h>
#include "memory.h"
#include "search.h" /* getOrder */
#include "resourcematch.h"
#include "translate.h" /* translate */

#define dVal 1
#define dInd 2
#define dOrd 4
#define dPat 8

typedef enum {eDone=0,eVal,eInd,eOrd,ePat} tStatus;

char aux[200];

/* default values */
static tResourceMatch result;

void freeRM(tResourceMatch *r) {
	freeAllocation(r->index);
	freeAllocation(r->path);
}

void xemit(tStatus s, char c) {
	static tStatus old=eDone;
	static char* a=aux;
	static int x=0;

	if (s!=old) { /* status change */
		*a=0;x=0;
		switch (old) {
		case eVal:result.value=atoi(aux);break;
		case eInd:result.index=strallocandcopy(aux);break;
		case eOrd:result.order=getOrder(aux);break;
		case ePat:result.path =strallocandcopy(aux);break;
		default:break;
		}
		old=s;
		a=aux;
	}
	if (x==199) {*a=0;return;} /* oups, aux size limit has been reached. this return avoids a buffer overflow */
 	*(a++)=c;
	x++;
}

int initRM(const char* text, tResourceMatch *r) {
	tStatus status=eVal;
	const char* t;
	
	/* set up default values */
	result.flag=0;
	result.value=0;
	result.order=0;
	result.index=NULL;
	result.path=NULL;

	for (t=text;status!=eDone;t++) {
		switch (*t) {
		case '/': /* path */
			status=ePat;
			result.flag|=dPat;
			 xemit(status,'/'); /* starts with / */
			break;
		case ':': /* index */
			if (status!=ePat) { /* after path, no index is allowed */
				status=eInd;
				if (result.flag&dInd) {
					freeRM(&result);
					return -1; /* ind flag has been set, duplicated index? */
				}
				result.flag|=dInd;
			} else xemit(status,*t);
			break;
		case '#': /* order */
			if (status!=ePat) { /* after path, no order is allowed */
				status=eOrd;
				if (result.flag&dOrd) {
					freeRM(&result);
					return -1; /* ord flag has been set, duplicated order? */
				}
				result.flag|=dOrd;
			} else xemit(status,*t);
			break;
		case 0:
			status=eDone;
			break;
		default: /* by default "value" */
			if (status==eVal) result.flag|=dVal;
			xemit(status,*t);
			break;
		}
	}
	xemit(eDone,0);

	*r=result;
	
	return 0; /* Ok */
}	

/* matches becomes false only if the flag is true and the match is false */
#define compare(x,a) m=m&&( (!(r->flag&x)) || (a) )

int runRM(const tResourceMatch *r, const char* path, const tResourceId *id) {
	int m=1; /* by default it matches */

#ifdef MATCH_DEBUG
	printf("Matching: path='%s', id=(%d,%s,%d) <=> flag=%x path='%s' id=(%d,%s,%d)\n",
		path,
		id->value,id->index,id->order,
		r->flag,
		r->path,
		r->value,r->index,r->order
	);
#endif

	/* compare each field */
	compare(dOrd,r->order==id->order);
	compare(dVal,r->value==id->value);
	compare(dInd,matchesIn(id->index,r->index)||matchesIn(translateInt2Ext(id->index),r->index));
	compare(dPat,matchesIn(path,r->path));
	
	return m;
}

