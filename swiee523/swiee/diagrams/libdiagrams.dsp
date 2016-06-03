# Microsoft Developer Studio Project File - Name="libdiagrams" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libdiagrams - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libdiagrams.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libdiagrams.mak" CFG="libdiagrams - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libdiagrams - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libdiagrams - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libdiagrams - Win32 Release"

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

!ELSEIF  "$(CFG)" == "libdiagrams - Win32 Debug"

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

# Name "libdiagrams - Win32 Release"
# Name "libdiagrams - Win32 Debug"
# Begin Source File

SOURCE="curvediagram.cpp"
# End Source File
# Begin Source File

SOURCE="diagram.cpp"
# End Source File
# Begin Source File

SOURCE="diagramdialog.cpp"
# End Source File
# Begin Source File

SOURCE="diagramdialog.h"

!IF  "$(CFG)" == "libdiagrams - Win32 Release"

# Begin Custom Build - MOC'ing diagramdialog.h ...
InputPath="diagramdialog.h"
InputName=diagramdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdiagrams - Win32 Debug"

# Begin Custom Build - MOC'ing diagramdialog.h ...
InputPath="diagramdialog.h"
InputName=diagramdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="diagramdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="graph.cpp"
# End Source File
# Begin Source File

SOURCE="marker.cpp"
# End Source File
# Begin Source File

SOURCE="markerdialog.cpp"
# End Source File
# Begin Source File

SOURCE="markerdialog.h"

!IF  "$(CFG)" == "libdiagrams - Win32 Release"

# Begin Custom Build - MOC'ing markerdialog.h ...
InputPath="markerdialog.h"
InputName=markerdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdiagrams - Win32 Debug"

# Begin Custom Build - MOC'ing markerdialog.h ...
InputPath="markerdialog.h"
InputName=markerdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="markerdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="polardiagram.cpp"
# End Source File
# Begin Source File

SOURCE="psdiagram.cpp"
# End Source File
# Begin Source File

SOURCE="rect3ddiagram.cpp"
# End Source File
# Begin Source File

SOURCE="rectdiagram.cpp"
# End Source File
# Begin Source File

SOURCE="smithdiagram.cpp"
# End Source File
# Begin Source File

SOURCE="tabdiagram.cpp"
# End Source File
# Begin Source File

SOURCE="timingdiagram.cpp"
# End Source File
# Begin Source File

SOURCE="truthdiagram.cpp"
# End Source File
# End Target
# End Project
