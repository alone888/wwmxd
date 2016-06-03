# Microsoft Developer Studio Project File - Name="libpaintings" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libpaintings - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libpaintings.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libpaintings.mak" CFG="libpaintings - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libpaintings - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libpaintings - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libpaintings - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Opt"
# PROP Intermediate_Dir "Opt"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe
# ADD LINK32 kernel32.lib /nologo /subsystem:windows /dll /pdb:none /incremental:no /machine:I386 /nodefaultlib:"msvcrtd"
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I ".." /I "../.." /I "$(QTDIR)/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "__MINGW32__" /D "HAVE_CONFIG_H" /D __STDC__=0 /D "QT_THREAD_SUPPORT" /D "_REENTRANT" /D "UNICODE" /D "QT_DLL" /D "QT_NO_DEBUG" /FD /c
# ADD BASE RSC /l 0x804
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libpaintings - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Dbg"
# PROP Intermediate_Dir "Dbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe
# ADD LINK32 kernel32.lib /nologo /subsystem:windows /dll /pdb:none /incremental:no /debug /machine:I386
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I ".." /I "../.." /I "$(QTDIR)/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__MINGW32__" /D "HAVE_CONFIG_H" /D __STDC__=0 /D "QT_THREAD_SUPPORT" /D "_REENTRANT" /D "UNICODE" /D "QT_DLL" /D "DEBUG" /FD /c
# ADD BASE RSC /l 0x804
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libpaintings - Win32 Release"
# Name "libpaintings - Win32 Debug"
# Begin Source File

SOURCE="arrow.cpp"
# End Source File
# Begin Source File

SOURCE="arrowdialog.cpp"
# End Source File
# Begin Source File

SOURCE="arrowdialog.h"

!IF  "$(CFG)" == "libpaintings - Win32 Release"

# Begin Custom Build - MOC'ing arrowdialog.h ...
InputPath="arrowdialog.h"
InputName=arrowdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libpaintings - Win32 Debug"

# Begin Custom Build - MOC'ing arrowdialog.h ...
InputPath="arrowdialog.h"
InputName=arrowdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="arrowdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="ellipse.cpp"
# End Source File
# Begin Source File

SOURCE="ellipsearc.cpp"
# End Source File
# Begin Source File

SOURCE="filldialog.cpp"
# End Source File
# Begin Source File

SOURCE="filldialog.h"

!IF  "$(CFG)" == "libpaintings - Win32 Release"

# Begin Custom Build - MOC'ing filldialog.h ...
InputPath="filldialog.h"
InputName=filldialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libpaintings - Win32 Debug"

# Begin Custom Build - MOC'ing filldialog.h ...
InputPath="filldialog.h"
InputName=filldialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="filldialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="graphicline.cpp"
# End Source File
# Begin Source File

SOURCE="graphictext.cpp"
# End Source File
# Begin Source File

SOURCE="graphictextdialog.cpp"
# End Source File
# Begin Source File

SOURCE="graphictextdialog.h"

!IF  "$(CFG)" == "libpaintings - Win32 Release"

# Begin Custom Build - MOC'ing graphictextdialog.h ...
InputPath="graphictextdialog.h"
InputName=graphictextdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libpaintings - Win32 Debug"

# Begin Custom Build - MOC'ing graphictextdialog.h ...
InputPath="graphictextdialog.h"
InputName=graphictextdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="graphictextdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="id_dialog.cpp"
# End Source File
# Begin Source File

SOURCE="id_dialog.h"

!IF  "$(CFG)" == "libpaintings - Win32 Release"

# Begin Custom Build - MOC'ing id_dialog.h ...
InputPath="id_dialog.h"
InputName=id_dialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libpaintings - Win32 Debug"

# Begin Custom Build - MOC'ing id_dialog.h ...
InputPath="id_dialog.h"
InputName=id_dialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="id_dialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="id_text.cpp"
# End Source File
# Begin Source File

SOURCE="painting.cpp"
# End Source File
# Begin Source File

SOURCE="portsymbol.cpp"
# End Source File
# Begin Source File

SOURCE="rectangle.cpp"
# End Source File
# End Target
# End Project
