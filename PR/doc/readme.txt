Princed Resources Editor V0.6 beta
(c) Copyright 2003 - Princed Development Team
http://www.princed.com.ar

Please read this readme file throughly before starting to use Princed Resource Editor.

1) Supported file formats:

The extractor supports only a few file formats, it uses the
most commonly supported versions of the formats to allow maximum compatibility. 
As we haven't got enough time to code the program we decided to support only those kinds of formats, and let the user make the nesessary conversions.

Resource Types:
 01 Levels
 02 Bitmaps
 03 Waves
 04 Midis
 05 Undefined
 06 Palette
 07 to 10 reserved

Type 01:
 We decided to give Level files the extension ".pet".  These files can be  edited Princed V3.  PV3 can also edit the whole levels.dat file.
 Recommended editor: PV3.

Type 02:
 We use Windows Bitmaps (BMP) to extract the files.
 The file specifications are:
  - 16 color-palette.
  - No compression.
  - Currently the images are saved upside down (We are going to change that).
 Recommended editor: Paint Shop Pro, perhaps even MS Paint.
 
Type 03:
 For digital audio we are using the standard .wav format.
 The file specifications are:
  - Size of Format: 16
  - Format: PCM
  - Attributes: 8 bit, mono, unsigned
  - Channels: 1
  - Samplerate: 11025
  - Bytes/Sec: 11025
  - Block Align: 1
 Recommended editors: GoldWave, CoolEdit.  Standard Windows programs do not
 understand the .wav format.

Type 04:
 For musical instrument digital interface we are using the standard midi
 format (mid) and type 0 midi format.
 The file specifications are:
  - Unknown (see standard MIDI specifications)
 Recommended editor: Power Tracks.

Type 05:
 Some binary garbage stuff.
 Recommended editor: xvi32 or any other hex editor.


Type 06:
 For the bitmaps palettes we are using the Jasc Pal format (Not Microsoft pal  format)
 Recommended editor: Paint Shop Pro.

2) Bugs:
 As Princed Resource Editor is still an Alpha version, bugs are very common, 
 and we are attempting to fix as many as possible.  After that we will release 
 the beta version and after that the final version.
 This program was published because lots of people asked for it in this
 unstable state, so do not expect the program to be flexible with the
 file format, program syntax, etc.

3) Syntax:
 The command syntax is very easy:
 
 pr -mainoption[suboptions] optionParameters
 
 where mainoption must be:
  
 -t [resFile]
   to clear the resource file
 -k [resFile] [optionalDirectory]
   to kill an extracion
 -x[rnuso] DatFile [optionalDirectory] [resFile]
   for extract
  r: raw extraction
  n: don't extract
  u: update res file in case there were records
  s: don't save res file
  o: to specify ResFile
 -c[ro] DatFile [optionalDirectory] [resFile]
   for compile
  r: raw compiling
  o: to specify ResFile
 -d DatFile
   for type
 -h
   for help
 
 This will perform the option to the datafile using the auxiliary files in
 the optionalDirectory or current if not set.
 
 Available options are -x for extract, -c for compile and -t for clearing the  resource file (it doesn't clear the extracted files).
 
 Syntax samples:
 
 C:\PRINCED\PR> pr -x dat\vdungeon.dat ext
 
 This will extract the bitmaps and palettes allocated in   C:\princed\pr\dat\vdungeon.dat into the c:\princed\pr\ext. 
 Note: ext must exist.
 
 C:\PRINCE> pr vdungeon.dat -x
 
 This will extract all the bitmaps and palettes allocated in  c:\prince\vdungeon.dat
 into c:\prince.
 
 C:\PRINCE> pr -t
 
 This will clear resources in c:\prince\resources.txt
 
 C:\PRINCED\PR> pr compiled\vdungeon.dat -c ext
 
 This will compile the bitmaps and palettes that are in c:\princed\pr\ext and
 associated to vdungeon.dat in the resource file and generate the file
 c:\princed\pr\compiled\vdungeon.dat.
 Note: c:\princed\pr\compiled must exist.
 
4) New versions
 It's strongly recommended that you download a newer stable version of this   program as soon as it comes available in the official url (http:://www.princed.com.ar)

5) ToDo List
 i)   Fix bug in palette compiling
 ii)  Improve support of bitmap compiling
 iii) Codify the compression algorithms (B1 to B4) to optimize the bitmap compiled dat files
 iv)  Move PG to bmp.c (only when all bitmaps are Ok)
 v)   Add full pop2 support
 vi)  Add resources tree in resources.txt with the following format:
  [Prince of Persia 1]
   [Sounds]
    [Wave]
     DIGISND1.DAT 10058 Door close
    [Midi]
    [Speaker]
   [Images]
    [Animations]
     [Jaffar walking to princess] Palette: TITLE.DAT 11022
      TITLE.DAT 11023 Frame 1
      TITLE.DAT 11024 Frame 2
      TITLE.DAT 11025 Frame 3
      TITLE.DAT 11026 Frame 4     
    [Titles] Palette: TITLE.DAT 200
     TITLE.DAT 203 Outside palace presentation
    [Eviornment]
     [Dungeon]
     [Palace]
    [Others]
  [Prince of Persia 2]

5.5) Versions:
  PR v0.6 beta: unestable, added ii, iv, first full working version
  PR v0.7:      tested & stable
  PR v0.8 beta: added i,iii,vi
  PR v0.9:      tested and stable
  PR v1.0:      First optimized & stable version for win32 and linux

6) Change Log
 * PG
  - Added support to export .bmp files
  - Added bitmap decompression
  - Added static palette extraction
 * PS
  - Added support to .mid files
  - Added support to indexed dat files
 * PR 0.1 Alpha
  - Merged PS and PG into one extractor.
  - Added resource.txt parser.
  - Solved several bugs.
  - Coded type header verification
  - Added dynamic palette extraction
  - Added support to export JASC palette for .pal files.
  - Added support to .wav files.
 * PR 0.2 Alpha
  - Added support to import .bmp files
  - Source files were abstracted
  - Added classifing dat type feature
  - Implemented CGI output support for UNIX versions
 * PR 0.3 beta
  - Added pr.dll compiling
  - Added modifiers -x[rnu] -c[r]
  - Solved bugs in bitmap format
  - Solved segmentation fault in title.dat
  - Images bigger than 256 pixels lenght or width now are recognized
  - Improved decompression algorithm in bigger images
  - Optimized bitmap handling
  - Bitmaps aren't upside down anymore 
 * PR 0.4 beta
  - Fixed a memory bug since v0.3
  - Pop2 DAT files detected (file type 11)
 * PR 0.5 beta
  - Optimized bmp saving algorithm
  - Abstracted format handling
  - Solved and optimized decompression for big images
  - Some Pop2 graphics are extracted (palettes aren't supported yet)
 * PR 0.6 beta
  - Optimized PG algorithms and reserved memory sizes
  - Completed and tested bmp reading algorithm
 

7) Platforms
 Binary files are compiled in DOS/Win32 console (v0.5) and in Unix/Alpha versions (v0.5). The DOS executable is pr.exe and the Unix executable is prunix. The Unix version will be used to automate the classification of server uploaded files in our unix-based web server.


Please enjoy using Princed Resource Editor!