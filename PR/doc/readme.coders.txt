Princed Resources V0.9
(c) Copyright 2003 - Princed Development Team
http://www.princed.com.ar
This program is open source under the GNU General Public License terms

1) Unix-based OS compiling:

GCC will work:

-shell-2.05b$ make
Compiling import module...
Compiling compression module...
Compiling export module...
Compiling main module...
Compiling resource manager module...
Compiling extra tasks module...
Compiling disk access functions...
Compiling xml parsing module...
Compiling xml search features...
Compiling bitmap files support (bmp)...
Compiling midi audio files support (mid)...
Compiling JASC palette support (pal)...
Compiling digital wave audio support (wav)...
Linking files...
Program successfully compiled

Please read readme.txt for syntax information

-shell-2.05b$ cd bin
-shell-2.05b$ ./pr
Princed resources (PR) V0.9
(c) Copyright 2003 - Princed Development Team
http://www.princed.com.ar

Syntax:
 pr datfile option [extract dir]

Valid options:
 -x[rn] for extract
  r: raw extraction
  n: don't extract
 -c[r] for compile
  r: raw compiling
 -d for type
-shell-2.05b$

2) Dos/Win32 Console compiling

You can use the VC workspace file: pr.dsw and build the project (selecting Build/Set active configuration...), setting up Dll or Release compilation modes and pressing F7.

LCC should also work, use Makefile.lcc.

3) CVS download

Anonymous CVS access to SourceForge.net will work. Just type:

 cvs -d:pserver:anonymous@cvs.princed.com.ar:/cvsroot/princed login
Hit enter when prompted for password
 cvs -d:pserver:anonymous@cvs.princed.com.ar:/cvsroot/princed co PR

Sample:

-shell-2.05b$ cvs -d:pserver:anonymous@cvs.princed.com.ar:/cvsroot/princed login
Logging in to :pserver:anonymous@cvs.princed.com.ar:2401/cvsroot/princed
CVS password:
-shell-2.05b$ cvs -d:pserver:anonymous@cvs.princed.com.ar:/cvsroot/princed co PR
cvs server: Updating PR
U PR/compile.bat
(..)
U PR/zip/pr06.zip
U PR/zip/prlib.zip
U PR/zip/prvbdll.zip
-shell-2.05b$

4) Need assistance?
 Just ask here http://sourceforge.net/tracker/?func=add&group_id=91915&atid=598846

5) Bug reporting?
 http://sourceforge.net/tracker/?func=add&group_id=91915&atid=598845

6) Feature requests?
 http://sourceforge.net/tracker/?func=add&group_id=91915&atid=598848

7) SourceForge project home page
 http://project.princed.com.ar
