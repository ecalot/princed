Princed Resources v1.0 beta
(c) Copyright 2003 - Princed Development Team
http://www.princed.com.ar
This program is open source under the GNU General Public License terms

Please read this readme file throughly before starting to use Princed
Resource Editor.

1) Supported file formats:

 The extractor supports only a few file formats, it uses the most commonly
 supported versions of the formats to allow maximum compatibility. 
 As we haven't got enough time to code the program we decided to support
 only those kinds of formats, and let the user make the necessary
 conversions.

Resource Types:
 Name       Extension  Description

 level      plv        Prince of Persia level
 image      bmp        Graphic in windows bitmap format
 wave       wav        Wave digital audio sounds
 midi       mid        Game music in midi format
 binary     bin        Binary files
 palette    pal        Image palette in the Jasc format
 pcspeaker  pcs        Internal PC Speaker Sounds

Type "level":
 We decided to give Level files the extension ".plv".  These files
 can be edited in RoomShaker from version 1.66 and Princed V3.
 PV3 can also edit the whole levels.dat file.
 Recommended editor: RoomShaker.

Type "image":
 We use Windows Bitmaps (BMP) to extract the files.
 The file specifications are:
  - 16 color-palette in paletted images and a 2 color-palette in B/W images
  - No compression.
 Recommended editor: Paint Shop Pro, but even MS Paint works.
 
Type "wave":
 For digital audio we are using the standard .wav format.
 The file specifications are:
  - Size of Format: 16
  - Format: PCM
  - Attributes: 8 bit, mono, unsigned
  - Channels: 1
  - Sample rate: 11025
  - Bytes/Sec: 11025
  - Block Align: 1
 Recommended editors: GoldWave, CoolEdit. 

Type "midi":
 For musical instrument digital interface we are using the standard midi
 format (mid) and type 0 midi format.
 The file specifications are:
  - Unknown (see standard MIDI specifications)
 Recommended editor: Power Tracks.

Type "binary":
 Some binary garbage or untested stuff.
 Recommended editor: xvi32 or any other hex editor.

Type "palette":
 For the bitmap palettes we are using the Jasc Pal format (Not Microsoft
  .pal format)
 Recommended editor: Paint Shop Pro.

Type "pcspeaker":
 For the internal PC Speaker sound we are using binary data saved in pcs
  format
 The formas is saved this way:
  2 unique bytes for headers
  3 bytes per note (2 for frequency and 1 for duration)
 Recommended editor: xvi32 or any other hex editor.

2) Bugs:
 All known bugs were solved. If you find a bug (program crashes, corrupted
 resources, typos, etc), it's very probable that we haven't found it. So
 please report it. To report bugs just post them in
 http://sourceforge.net/tracker/?func=add&group_id=91915&atid=598845

3) Usage: 
 Run the program without parameters to get the usage help.
 
 Syntax samples:
 
 C:\PRINCED\PR> pr -xext dat\vdungeon.dat
 
 This will extract the bitmaps and palettes allocated in 
 C:\princed\pr\dat\vdungeon.dat into the c:\princed\pr\ext. 
 Note: ext must exist.
 
 C:\PRINCE> pr -x vdungeon.dat
 
 This will extract all the bitmaps and palettes allocated in
 c:\prince\vdungeon.dat into c:\prince.
 
 C:\PRINCED\PR> pr -cext compiled\vdungeon.dat
 
 This will compile the bitmaps and palettes that are in c:\princed\pr\ext
 and associated to vdungeon.dat in the resource file and generate the file
 c:\princed\pr\compiled\vdungeon.dat.
 
4) New versions
 It's strongly recommended that you download a newer stable version of
 this program as soon as it comes available in the official url
 http://www.princed.com.ar

5) Requirements
  
  25127199 bytes in ram are approx required for a normal full directory task
  
  Recomended:
   AMD K6-II at 350Mhz or higher
   32 Mb ram

6) Credits

 Coding & main routines
  Enrique Calot
  
 Coding Assistant
  Santiago Zamora

 Graphic compression algorithms
  Tammo Jan Dijkema
  Enrique Calot

 Graphic format development
  Tammo Jan Dijkema
  Anke Balderer

 MID Sound format development
  Christian Lundheim

 Resources.xml edition
  Steven Fayers

Please enjoy using Princed Resource Editor!
