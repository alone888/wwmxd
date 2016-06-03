# Microsoft Developer Studio Project File - Name="libdialogs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libdialogs - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libdialogs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libdialogs.mak" CFG="libdialogs - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libdialogs - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libdialogs - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libdialogs - Win32 Release"

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

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

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

# Name "libdialogs - Win32 Release"
# Name "libdialogs - Win32 Debug"
# Begin Source File

SOURCE="changedialog.cpp"
# End Source File
# Begin Source File

SOURCE="changedialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing changedialog.h ...
InputPath="changedialog.h"
InputName=changedialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing changedialog.h ...
InputPath="changedialog.h"
InputName=changedialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="changedialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="digisettingsdialog.cpp"
# End Source File
# Begin Source File

SOURCE="digisettingsdialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing digisettingsdialog.h ...
InputPath="digisettingsdialog.h"
InputName=digisettingsdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing digisettingsdialog.h ...
InputPath="digisettingsdialog.h"
InputName=digisettingsdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="digisettingsdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="importdialog.cpp"
# End Source File
# Begin Source File

SOURCE="importdialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing importdialog.h ...
InputPath="importdialog.h"
InputName=importdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing importdialog.h ...
InputPath="importdialog.h"
InputName=importdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="importdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="labeldialog.cpp"
# End Source File
# Begin Source File

SOURCE="labeldialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing labeldialog.h ...
InputPath="labeldialog.h"
InputName=labeldialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing labeldialog.h ...
InputPath="labeldialog.h"
InputName=labeldialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="labeldialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="librarydialog.cpp"
# End Source File
# Begin Source File

SOURCE="librarydialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing librarydialog.h ...
InputPath="librarydialog.h"
InputName=librarydialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

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

SOURCE="matchdialog.cpp"
# End Source File
# Begin Source File

SOURCE="matchdialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing matchdialog.h ...
InputPath="matchdialog.h"
InputName=matchdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing matchdialog.h ...
InputPath="matchdialog.h"
InputName=matchdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="matchdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="newprojdialog.cpp"
# End Source File
# Begin Source File

SOURCE="packagedialog.cpp"
# End Source File
# Begin Source File

SOURCE="packagedialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing packagedialog.h ...
InputPath="packagedialog.h"
InputName=packagedialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing packagedialog.h ...
InputPath="packagedialog.h"
InputName=packagedialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="packagedialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="qucssettingsdialog.cpp"
# End Source File
# Begin Source File

SOURCE="qucssettingsdialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing qucssettingsdialog.h ...
InputPath="qucssettingsdialog.h"
InputName=qucssettingsdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing qucssettingsdialog.h ...
InputPath="qucssettingsdialog.h"
InputName=qucssettingsdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="qucssettingsdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="savedialog.cpp"
# End Source File
# Begin Source File

SOURCE="savedialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing savedialog.h ...
InputPath="savedialog.h"
InputName=savedialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing savedialog.h ...
InputPath="savedialog.h"
InputName=savedialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="savedialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="searchdialog.cpp"
# End Source File
# Begin Source File

SOURCE="searchdialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing searchdialog.h ...
InputPath="searchdialog.h"
InputName=searchdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

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

SOURCE="settingsdialog.cpp"
# End Source File
# Begin Source File

SOURCE="settingsdialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing settingsdialog.h ...
InputPath="settingsdialog.h"
InputName=settingsdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing settingsdialog.h ...
InputPath="settingsdialog.h"
InputName=settingsdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="settingsdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="simmessage.cpp"
# End Source File
# Begin Source File

SOURCE="simmessage.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing simmessage.h ...
InputPath="simmessage.h"
InputName=simmessage

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing simmessage.h ...
InputPath="simmessage.h"
InputName=simmessage

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="simmessage.moc.cpp"
# End Source File
# Begin Source File

SOURCE="sweepdialog.cpp"
# End Source File
# Begin Source File

SOURCE="sweepdialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing sweepdialog.h ...
InputPath="sweepdialog.h"
InputName=sweepdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing sweepdialog.h ...
InputPath="sweepdialog.h"
InputName=sweepdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="sweepdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="vasettingsdialog.cpp"
# End Source File
# Begin Source File

SOURCE="vasettingsdialog.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing vasettingsdialog.h ...
InputPath="vasettingsdialog.h"
InputName=vasettingsdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing vasettingsdialog.h ...
InputPath="vasettingsdialog.h"
InputName=vasettingsdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="vasettingsdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="vtabbar.cpp"
# End Source File
# Begin Source File

SOURCE="vtabbar.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing vtabbar.h ...
InputPath="vtabbar.h"
InputName=vtabbar

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing vtabbar.h ...
InputPath="vtabbar.h"
InputName=vtabbar

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="vtabbar.moc.cpp"
# End Source File
# Begin Source File

SOURCE="vtabbeddockwidget.cpp"
# End Source File
# Begin Source File

SOURCE="vtabbeddockwidget.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing vtabbeddockwidget.h ...
InputPath="vtabbeddockwidget.h"
InputName=vtabbeddockwidget

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing vtabbeddockwidget.h ...
InputPath="vtabbeddockwidget.h"
InputName=vtabbeddockwidget

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="vtabbeddockwidget.moc.cpp"
# End Source File
# Begin Source File

SOURCE="vtabbutton.cpp"
# End Source File
# Begin Source File

SOURCE="vtabbutton.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing vtabbutton.h ...
InputPath="vtabbutton.h"
InputName=vtabbutton

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing vtabbutton.h ...
InputPath="vtabbutton.h"
InputName=vtabbutton

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="vtabbutton.moc.cpp"
# End Source File
# Begin Source File

SOURCE="vtabwidget.cpp"
# End Source File
# Begin Source File

SOURCE="vtabwidget.h"

!IF  "$(CFG)" == "libdialogs - Win32 Release"

# Begin Custom Build - MOC'ing vtabwidget.h ...
InputPath="vtabwidget.h"
InputName=vtabwidget

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libdialogs - Win32 Debug"

# Begin Custom Build - MOC'ing vtabwidget.h ...
InputPath="vtabwidget.h"
InputName=vtabwidget

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="vtabwidget.moc.cpp"
# End Source File
# End Target
# End Project
