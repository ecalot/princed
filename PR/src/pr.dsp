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
# ADD CPP /nologo /W3 /GX /O2 /I "include\\" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /YX /c
# ADD BASE RSC /l 0x2c0a /d "NDEBUG"
# ADD RSC /l 0x2c0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"bin/pr.exe"

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

!ENDIF 

# Begin Target

# Name "tasks - Win32 Release"
# Name "tasks - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter ""
# Begin Group "formats"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\formats\bmp.c

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\formats\mid.c

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\formats\pal.c

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\formats\wav.c

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\compile.c

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\compress.c

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\extract.c

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\parser.c

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pr.c

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources.c

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tasks.c

!IF  "$(CFG)" == "tasks - Win32 Release"

!ELSEIF  "$(CFG)" == "tasks - Win32 Debug"

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

SOURCE=.\include\extract.h
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

SOURCE=.\include\parser.h
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
# End Group
# End Target
# End Project
