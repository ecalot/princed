Princed Resources Handler V0.6 beta
(c) Copyright 2003 - Princed Development Team
http://www.princed.com.ar
This program is open source under the GNU General Public License terms

1) Unix-based OS compiling:

GCC with the C99 standard will work:

-shell-2.05b$ gcc -std=c99 pr.c -o pr
-shell-2.05b$ ./pr
Princed resources (PR) V0.5 beta.
(c) Copyright 2003 - Princed Development Team
http://www.princed.com.ar

Syntax:
 pr datfile option [extract dir]

Valid options:
 -x[rnus] for extract
  r: raw extraction
  n: don't extract
  u: update res file in case there were records
  s: don't save res file
 -c for compile
  r: raw compiling
 -d for type
 -t to clear the resource file.
-shell-2.05b$

2) Dos/Win32 Console compiling

LCC will work

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

4) Need assistanse?
 Just ask here http://sourceforge.net/tracker/?func=add&group_id=91915&atid=598846

5) Bug reporting?
 http://sourceforge.net/tracker/?func=add&group_id=91915&atid=598845

6) Feature requests?
 http://sourceforge.net/tracker/?func=add&group_id=91915&atid=598848

7) SourceForge project home page
 http://project.princed.com.ar
