# Microsoft Developer Studio Project File - Name="tasks" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=tasks - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pr.mak" CFG="tasks - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tasks - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "tasks - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "tasks - Win32 Dynamic Link Library" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tasks - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "include\\" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR /YX /c
# ADD BASE RSC /l 0x2c0a /d "NDEBUG"
# ADD RSC /l 0x2c0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"bin/pr.exe"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=del winbins\pr.exe	copy bin\pr.exe winbins\pr.exe >nul	scripts\upx.exe winbins\pr.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "include\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x2c0a /d "_DEBUG"
# ADD RSC /l 0x2c0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "tasks___Win32_Dynamic_Link_Library"
# PROP BASE Intermediate_Dir "tasks___Win32_Dynamic_Link_Library"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Dll"
# PROP Intermediate_Dir "Dll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "include\\" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /W3 /GX /O2 /I "include\\" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "DLL" /FR /YX /c
# ADD BASE RSC /l 0x2c0a /d "NDEBUG"
# ADD RSC /l 0x2c0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"bin/pr.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /dll /machine:I386 /def:"winbins/pr.def" /out:"winbins/pr.dll" /implib:"winbins/pr.lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=scripts\upx.exe winbins\pr.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "tasks - Win32 Release"
# Name "tasks - Win32 Debug"
# Name "tasks - Win32 Dynamic Link Library"
# Begin Group "src"

# PROP Default_Filter ""
# Begin Group "formats"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\formats\bmp.c
DEP_CPP_BMP_C=\
	".\include\bmp.h"\
	".\include\compile.h"\
	".\include\compress.h"\
	".\include\disk.h"\
	".\include\resources.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\formats\mid.c
DEP_CPP_MID_C=\
	".\include\compile.h"\
	".\include\disk.h"\
	".\include\mid.h"\
	".\include\resources.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\formats\pal.c
DEP_CPP_PAL_C=\
	".\include\compress.h"\
	".\include\disk.h"\
	".\include\pal.h"\
	".\include\resources.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\formats\plv.c
DEP_CPP_PLV_C=\
	".\include\compile.h"\
	".\include\disk.h"\
	".\include\plv.h"\
	".\include\resources.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\formats\wav.c
DEP_CPP_WAV_C=\
	".\include\compile.h"\
	".\include\resources.h"\
	".\include\wav.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# End Group
# Begin Group "ports"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ports\getopt.c
DEP_CPP_GETOP=\
	".\include\getopt.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ports\getopt1.c
DEP_CPP_GETOPT=\
	".\include\getopt.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\compile.c
DEP_CPP_COMPI=\
	".\include\bmp.h"\
	".\include\compile.h"\
	".\include\compress.h"\
	".\include\disk.h"\
	".\include\mid.h"\
	".\include\pal.h"\
	".\include\plv.h"\
	".\include\pr.h"\
	".\include\resources.h"\
	".\include\wav.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\compress.c
DEP_CPP_COMPR=\
	".\include\compress.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\disk.c
DEP_CPP_DISK_=\
	".\include\disk.h"\
	".\include\pr.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\extract.c
DEP_CPP_EXTRA=\
	".\include\bmp.h"\
	".\include\compress.h"\
	".\include\disk.h"\
	".\include\extract.h"\
	".\include\mid.h"\
	".\include\pal.h"\
	".\include\plv.h"\
	".\include\pr.h"\
	".\include\resources.h"\
	".\include\wav.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pr.c
DEP_CPP_PR_C16=\
	".\include\bmp.h"\
	".\include\compile.h"\
	".\include\compress.h"\
	".\include\disk.h"\
	".\include\extract.h"\
	".\include\getopt.h"\
	".\include\pr.h"\
	".\include\resources.h"\
	".\include\tasks.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources.c
DEP_CPP_RESOU=\
	".\include\compress.h"\
	".\include\disk.h"\
	".\include\pr.h"\
	".\include\resources.h"\
	".\include\xml.h"\
	".\include\xmlsearch.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tasks.c
DEP_CPP_TASKS=\
	".\include\resources.h"\
	".\include\tasks.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\xml.c
DEP_CPP_XML_C=\
	".\include\disk.h"\
	".\include\pr.h"\
	".\include\xml.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\xmlsearch.c
DEP_CPP_XMLSE=\
	".\include\resources.h"\
	".\include\xml.h"\
	".\include\xmlsearch.h"\
	

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ELSEIF  "$(CFG)" == "tasks - Win32 Dynamic Link Library"

!ENDIF 

# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\bmp.h
# End Source File
# Begin Source File

SOURCE=.\include\compile.h
# End Source File
# Begin Source File

SOURCE=.\include\compress.h
# End Source File
# Begin Source File

SOURCE=.\include\disk.h
# End Source File
# Begin Source File

SOURCE=.\include\extract.h
# End Source File
# Begin Source File

SOURCE=.\include\getopt.h
# End Source File
# Begin Source File

SOURCE=.\include\memory.h
# End Source File
# Begin Source File

SOURCE=.\include\mid.h
# End Source File
# Begin Source File

SOURCE=.\include\pal.h
# End Source File
# Begin Source File

SOURCE=.\include\plv.h
# End Source File
# Begin Source File

SOURCE=.\include\pr.h
# End Source File
# Begin Source File

SOURCE=.\include\resources.h
# End Source File
# Begin Source File

SOURCE=.\include\tasks.h
# End Source File
# Begin Source File

SOURCE=.\include\wav.h
# End Source File
# Begin Source File

SOURCE=.\include\xml.h
# End Source File
# Begin Source File

SOURCE=include\xmlsearch.h
# End Source File
# End Group
# Begin Group "Text Files"

# PROP Default_Filter "*.txt"
# Begin Source File

SOURCE=..\changelog.txt
# End Source File
# Begin Source File

SOURCE=..\gpl.txt
# End Source File
# Begin Source File

SOURCE=..\readme.coders.txt
# End Source File
# Begin Source File

SOURCE=..\readme.txt
# End Source File
# End Group
# Begin Group "Other Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Makefile
# End Source File
# Begin Source File

SOURCE=.\Makefile.lcc
# End Source File
# Begin Source File

SOURCE=.\winbins\pr.def
# End Source File
# Begin Source File

SOURCE=.\bin\resources.sample.xml
# End Source File
# Begin Source File

SOURCE=.\bin\resources.xml
# End Source File
# End Group
# End Target
# End Project
