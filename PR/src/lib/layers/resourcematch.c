
#include <stdio.h>
#include "memory.h"
#include "search.h" /* getOrder */

#define dVal 1
#define dInd 2
#define dOrd 4
#define dPat 8

typedef enum {eVal=0,eInd,eOrd,ePat,eDone} tStatus;

char aux[200];

/* default values */
int xvalue=0;
int xorder=0;
char* xindex=NULL;
char* xpath=NULL;

void xemit(tStatus s, char c) {
	static tStatus old=eDone;
	static char* a=aux;
	if (s!=old) { /* status change */
		*a=0;
		switch (old) {
		case eVal:xvalue=atoi(aux);break;
		case eInd:xindex=strallocandcopy(aux);break;
		case eOrd:xorder=getOrder(aux);break;
		case ePat:xpath =strallocandcopy(aux);break;
		default:break;
		}
		old=s;
		a=aux;
	}
 	*(a++)=c;
/*	printf("emiting '%c' (%d)\n",x,a);*/
}

int parse(const char* text, int* flag, int* value, char** index, int* order,char** path) {
	tStatus status=eVal;
	const char* t;

	*flag=0;

	for (t=text;status!=eDone;t++) {
		switch (*t) {
		case '/': /* path */
			status=ePat;
			*flag|=dPat;
			break;
		case ':': /* index */
			if (status!=ePat) { /* after path, no index is allowed */
				status=eInd;
				if (*flag&dInd) {
					freeAllocation(xindex);
					freeAllocation(xpath);
					return -1; /* ind flag has been set, duplicated index? */
				}
				*flag|=dInd;
			} else xemit(status,*t);
			break;
		case '#': /* order */
			if (status!=ePat) { /* after path, no order is allowed */
				status=eOrd;
				if (*flag&dOrd) {
					freeAllocation(xindex);
					freeAllocation(xpath);
					return -1; /* ord flag has been set, duplicated order? */
				}
				*flag|=dOrd;
			} else xemit(status,*t);
			break;
		case 0:
			status=eDone;
			break;
		default: /* by default "value" */
			if (status==eVal) *flag|=dVal;
			xemit(status,*t);
			break;
		}
	}
	xemit(eDone,0);

	*value=xvalue;
	*order=xorder;
	*index=xindex;
	*path=xpath;
	
	return 0; /* Ok */
}	
