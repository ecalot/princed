Princed Resources Editor V0.8 beta
(c) Copyright 2003 - Princed Development Team
http://www.princed.com.ar
This program is open source under the GNU General Public License terms

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
 07 PC Speaker Sounds
 08 to 10 reserved

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
 For the bitmaps palettes we are using the Jasc Pal format (Not Microsoft pal format)
 Recommended editor: Paint Shop Pro.

Type 07:
 For the PC Speaker sounf we are using binary data saved in pcs format
 Recommended editor: xvi32 or any other hex editor.

2) Bugs:
 This is the first stable release, we hadn't tested enough as to say it has no bugs,
 and tehre are also lots known bugs. This program was only tested using vdungeon.dat
 so we know there are bugs in other dat files but we can't solve them very fast.
 It is recommended to upgrade to the next stable when it is released.

3) Syntax:
 The command syntax is very easy:
 
 pr [datfile] -mainoption[suboptions] optionParameters
 
 where mainoption must be:
  
 -t [resFile]
   to clear the resource file
 -k [resFile] [optionalDirectory]
   to kill an extracion
 -x[rnuso] [optionalDirectory] [resFile]
   for extract
  r: raw extraction
  n: don't extract
  u: update res file in case there were records
  s: don't save res file
  o: to specify ResFile
 -c[ro] [optionalDirectory] [resFile]
   for compile
  r: raw compiling
  o: to specify ResFile
 -d
   for type
 -h
   for help
 
 This will perform the option to the datafile using the auxiliary files in
 the optionalDirectory or current if not set.
 
 Available options are -x for extract, -c for compile and -t for clearing the  resource file (it doesn't clear the extracted files).
 
 Syntax samples:
 
 C:\PRINCED\PR> pr dat\vdungeon.dat -x ext
 
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
 It's strongly recommended that you download a newer stable version of this program as soon as it comes available in the official url (http://www.princed.com.ar)

Please enjoy using Princed Resource Editor!