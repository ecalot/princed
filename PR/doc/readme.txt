Princed Resources V0.9
(c) Copyright 2003 - Princed Development Team
http://www.princed.com.ar
This program is open source under the GNU General Public License terms

Please read this readme file throughly before starting to use Princed Resource Editor.

1) Supported file formats:

The extractor supports only a few file formats, it uses the
most commonly supported versions of the formats to allow maximum compatibility. 
As we haven't got enough time to code the program we decided to support only those kinds of formats, and let the user make the necessary conversions.

Resource Types:
 01 Levels
 02 Bitmaps
 03 Waves
 04 Midis
 05 Undefined
 06 Palette
 07 Internal PC Speaker Sounds
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
  - Sample rate: 11025
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
 For the internal PC Speaker sound we are using binary data saved in pcs format
 Recommended editor: xvi32 or any other hex editor.

2) Bugs:
 This version has lots of new features but we didn't have enough time to test it.
 There are lots known bugs. This program was only tested using vdungeon.dat and
 vpalace.dat.
 so we know there are bugs in other dat files but we can't solve them very fast.
 It is recommended to upgrade to the next stable when it is released.
 To report bugs please go to http://sourceforge.net/tracker/?func=add&group_id=91915&atid=598845.

3) Usage: 
   pr [-x [EXTRACTDIR]|-c [COMPILEDIR]|-d] [DATFILEPATH] 
   pr [OPTIONS] [DATFILEPATH]
 
   Mandatory arguments to long options are mandatory for short options too.
 
    -c, --import[=DIRNAME]     imports from DIRNAME into given dat file
    -d, --classify             returns the DAT file type
    -x, -e, --export[=DIRNAME] extracts given dat file into DIRNAME
 
    -b, --backup[=EXTENSION]   backup your files
    -f, --force                default option, you cannot disable it,
                               so please make a backup of your files
    -g, --cgi                  run as CGI and output mime headers
    -h, --help                 display this help and exit
    -m, --resource=RESFILE     uses an user-specific resource xml file
    -r, --raw                  uses raw format
    -R, --recursive            searches for all dat files (only if DATFILEPATH
                               is not a dat file)
    -t, --datfile=DATFILE      specifies a dat file to read resources
                               different that the original file
        --unknown              generate the unknown file without performing
                               any extraction
    -v, --verbose              explain what is being done
        --version              output version information and exit

 Note: if DATFILEPATH is not provided or if it is a directory all dat files
       that are set on resources.xml will be selected
 
 Syntax samples:
 
 C:\PRINCED\PR> pr -xext dat\vdungeon.dat
 
 This will extract the bitmaps and palettes allocated in   C:\princed\pr\dat\vdungeon.dat into the c:\princed\pr\ext. 
 Note: ext must exist.
 
 C:\PRINCE> pr -x vdungeon.dat
 
 This will extract all the bitmaps and palettes allocated in  c:\prince\vdungeon.dat
 into c:\prince.
 
 C:\PRINCED\PR> pr -cext compiled\vdungeon.dat
 
 This will compile the bitmaps and palettes that are in c:\princed\pr\ext and
 associated to vdungeon.dat in the resource file and generate the file
 c:\princed\pr\compiled\vdungeon.dat.
 
4) New versions
 It's strongly recommended that you download a newer stable version of this program as soon as it comes available in the official url (http://www.princed.com.ar)

5) Credits

 Coding & main routines
  Enrique Calot

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
