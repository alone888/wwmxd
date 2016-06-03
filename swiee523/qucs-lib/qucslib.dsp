# Microsoft Developer Studio Project File - Name="qucslib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=qucslib - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "qucslib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "qucslib.mak" CFG="qucslib - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "qucslib - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "qucslib - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "qucslib - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Opt"
# PROP Intermediate_Dir "Opt"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD LIB32 /nologo
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "." /I ".." /I "$(QTDIR)/include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__MINGW32__" /D "HAVE_CONFIG_H" /D __STDC__=0 /D "QT_THREAD_SUPPORT" /D "_REENTRANT" /D "UNICODE" /D "QT_DLL" /D "QT_NO_DEBUG" /FD /c
# ADD BASE RSC /l 0x804
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 kernel32.lib qtmain.lib qt-mt3.lib /nologo /subsystem:windows /pdb:none /machine:I386 /nodefaultlib:"msvcrtd" /libpath:"$(QTDIR)/lib"

!ELSEIF  "$(CFG)" == "qucslib - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Dbg"
# PROP Intermediate_Dir "Dbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD LIB32 /nologo
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "." /I ".." /I "$(QTDIR)/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__MINGW32__" /D "HAVE_CONFIG_H" /D __STDC__=0 /D "QT_THREAD_SUPPORT" /D "_REENTRANT" /D "UNICODE" /D "QT_DLL" /D "DEBUG" /FD /c
# ADD BASE RSC /l 0x804
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 kernel32.lib qtmain.lib qt-mt3.lib /nologo /subsystem:windows /pdb:none /debug /machine:I386 /libpath:"$(QTDIR)/lib"

!ENDIF 

# Begin Target

# Name "qucslib - Win32 Release"
# Name "qucslib - Win32 Debug"
# Begin Source File

SOURCE="displaydialog.cpp"
# End Source File
# Begin Source File

SOURCE="displaydialog.h"

!IF  "$(CFG)" == "qucslib - Win32 Release"

# Begin Custom Build - MOC'ing displaydialog.h ...
InputPath="displaydialog.h"
InputName=displaydialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "qucslib - Win32 Debug"

# Begin Custom Build - MOC'ing displaydialog.h ...
InputPath="displaydialog.h"
InputName=displaydialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="displaydialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="librarydialog.cpp"
# End Source File
# Begin Source File

SOURCE="librarydialog.h"

!IF  "$(CFG)" == "qucslib - Win32 Release"

# Begin Custom Build - MOC'ing librarydialog.h ...
InputPath="librarydialog.h"
InputName=librarydialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "qucslib - Win32 Debug"

# Begin Custom Build - MOC'ing librarydialog.h ...
InputPath="librarydialog.h"
InputName=librarydialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="librarydialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="main.cpp"
# End Source File
# Begin Source File

SOURCE="qucslib.cpp"
# End Source File
# Begin Source File

SOURCE="qucslib.h"

!IF  "$(CFG)" == "qucslib - Win32 Release"

# Begin Custom Build - MOC'ing qucslib.h ...
InputPath="qucslib.h"
InputName=qucslib

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "qucslib - Win32 Debug"

# Begin Custom Build - MOC'ing qucslib.h ...
InputPath="qucslib.h"
InputName=qucslib

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="qucslib.moc.cpp"
# End Source File
# Begin Source File

SOURCE="searchdialog.cpp"
# End Source File
# Begin Source File

SOURCE="searchdialog.h"

!IF  "$(CFG)" == "qucslib - Win32 Release"

# Begin Custom Build - MOC'ing searchdialog.h ...
InputPath="searchdialog.h"
InputName=searchdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "qucslib - Win32 Debug"

# Begin Custom Build - MOC'ing searchdialog.h ...
InputPath="searchdialog.h"
InputName=searchdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="searchdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="symbolwidget.cpp"
# End Source File
# Begin Source File

SOURCE="symbolwidget.h"

!IF  "$(CFG)" == "qucslib - Win32 Release"

# Begin Custom Build - MOC'ing symbolwidget.h ...
InputPath="symbolwidget.h"
InputName=symbolwidget

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "qucslib - Win32 Debug"

# Begin Custom Build - MOC'ing symbolwidget.h ...
InputPath="symbolwidget.h"
InputName=symbolwidget

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="symbolwidget.moc.cpp"
# End Source File
# End Target
# End Project
