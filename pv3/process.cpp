// process.cpp : implementation of the cProcess class
//

#include "stdafx.h"
#include "Princed V3.h"
#include <process.h>
#include <signal.h>

class cProcess {
private:
	int handle;
public:

	cProcess(char *cmdname) {
		handle=_spawnl(_P_NOWAIT, cmdname, "megahit","3", NULL);
		
	}

//int raise( int sig );


}
;