Princed Resources Handler V0.5 beta
(c) Copyright 2003 - Princed Development Team
http://www.princed.com.ar

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