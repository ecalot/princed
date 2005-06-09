Princed Resources v1.1
(c) Copyright 2003 - 2005 - Princed Development Team
http://www.princed.com.ar
This program is open source under the GNU General Public License terms

Please read this readme file throughly before starting to use Princed
Resource Editor.

1) Supported file formats:

 The extractor supports only a few file formats, it uses the most commonly
 supported versions of the formats to allow maximum compatibility. 
 As we haven't got enough time to code the program we decided to support
 only those kinds of formats, and let the user make the necessary
 conversions. See the table above and specifications for more information.

Supported Resource Types:
 Name       Extension  Description
 ~~~~       ~~~~~~~~~  ~~~~~~~~~~~
 binary     bin        Binary files
 image      bmp        Graphic in windows bitmap format
 level      plv        Prince of Persia level
 midi       mid        Game music in midi format
 palette    pal        Image palette in the Jasc format
 pcspeaker  pcs        Internal PC Speaker Sounds
 wave       wav        Wave digital audio sounds

Levels:
 * Type "level":
 We decided to give Level files the extension ".plv".  These files
 can be edited in RoomShaker from version 1.66 and Princed V3.
 PV3 can also edit the whole levels.dat file.
 Format supported by: RoomShaker.

Images:
 * Type "image":
 We use Windows Bitmaps (BMP) to extract the files.
 The file specifications are:
  - 16 color-palette in paletted images and a 2 color-palette in B/W images
  - No compression.
 Format supported by: The Gimp, ImageMagick (command line conversor),
                      Paint Shop Pro, any other image editor.

 * Type "palette":
 For the bitmap palettes we are using the Jasc Pal format (Not Microsoft
  .pal format)
 Format supported by: Paint Shop Pro.

Sound:
 * Type "wave":
 For digital audio we are using the standard .wav format.
 The file specifications are:
  - Size of Format: 16
  - Format: PCM
  - Attributes: 8 bit, mono, unsigned
  - Channels: 1
  - Sample rate: 11025
  - Bytes/Sec: 11025
  - Block Align: 1
 Format supported by: SoX (command line conversor), GoldWave,
                      CoolEdit (editors).

 * Type "midi":
 For musical instrument digital interface we are using the standard midi
 format (mid) and type 0 midi format.
 The file specifications are:
  - Unknown (see standard MIDI specifications)
 Format supported by: Power Tracks.

Binary:
 * Type "binary":
 Some binary garbage or untested stuff.
 Format supported by: xvi32 or any other hex editor.

 * Type "pcspeaker":
 For the internal PC Speaker sound we are using binary data saved in pcs
 format.
 The format is saved this way:
  2 unique bytes for headers
  3 bytes per note (2 for frequency and 1 for duration)
 Format supported by: not supported yet, use xvi32 or any other hex editor.

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
  
  Approximately 25Mb in RAM are required for a normal full directory task
  
  Recomended:
   AMD K6-II at 350Mhz or higher
   32 Mb RAM
	
	Benchmark results:
   Performed on an AMD Athlon(tm) XP 2000+ with 1667.022 MHz
	 The file tested was the original vdungeon.dat, sized 14220 bytes

	 Normal benchmark results:
	  Compression level 1. Size 38031 bytes. Time 0.028137 seconds.
	  Compression level 2. Size 23756 bytes. Time 0.031273 seconds.
	  Compression level 3. Size 15912 bytes. Time 0.031836 seconds.
	  Compression level 4. Size 12519 bytes. Time 0.066246 seconds.
	  Compression level 5. Size 11347 bytes. Time 0.087975 seconds.
	  Compression level 6. Size 11218 bytes. Time 1.506183 seconds.
	  Compression level 7. Size 10932 bytes. Time 2.916612 seconds.

	 Benchamrk using valgrind: 
	  Compression level 1. Size 38031 bytes. Time 1.239898 seconds.
	  Compression level 2. Size 23756 bytes. Time 1.244662 seconds.
	  Compression level 3. Size 15912 bytes. Time 1.305006 seconds.
	  Compression level 4. Size 12519 bytes. Time 2.251174 seconds.
	  Compression level 5. Size 11347 bytes. Time 2.908357 seconds.
	  Compression level 6. Size 11218 bytes. Time 73.004200 seconds.
	  Compression level 7. Size 10932 bytes. Time 144.786515 seconds.

6) Credits

 Coding & main routines
  Enrique Calot
  
 Coding Assistant
  Santiago Zamora

 Graphic compression algorithms
  Tammo Jan Dijkema
  Enrique Calot
	Diego Essaya

 Graphic format development
  Tammo Jan Dijkema
  Anke Balderer

 MID Sound format development
  Christian Lundheim

 Resources.xml edition
  Steven Fayers

Type pr --version to see translation credits for an specific language 
Pr is also using some GPL compatible and GPL components made by
«Kevlin Henney» (dirent stucture port for windows) and «the Free Software
Foundation, Inc.» (getopt parsing functions ports).
Special thanks to:
 - the authors of Valgrind and GDB for the great debugging features.
 - the authors of GCC and LCC for making excelent free compilers.
 - SourceForge.net for the home page hosting and the development
   enviornment provided.
 - Lugfi for all the support. 

Please enjoy using Princed Resource Editor!

