# Microsoft Developer Studio Project File - Name="libcomponents" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libcomponents - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libcomponents.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libcomponents.mak" CFG="libcomponents - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libcomponents - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libcomponents - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libcomponents - Win32 Release"

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

!ELSEIF  "$(CFG)" == "libcomponents - Win32 Debug"

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

# Name "libcomponents - Win32 Release"
# Name "libcomponents - Win32 Debug"
# Begin Source File

SOURCE="ac_sim.cpp"
# End Source File
# Begin Source File

SOURCE="am_modulator.cpp"
# End Source File
# Begin Source File

SOURCE="ampere_ac.cpp"
# End Source File
# Begin Source File

SOURCE="ampere_dc.cpp"
# End Source File
# Begin Source File

SOURCE="ampere_noise.cpp"
# End Source File
# Begin Source File

SOURCE="amplifier.cpp"
# End Source File
# Begin Source File

SOURCE="andor4x2.cpp"
# End Source File
# Begin Source File

SOURCE="andor4x3.cpp"
# End Source File
# Begin Source File

SOURCE="andor4x4.cpp"
# End Source File
# Begin Source File

SOURCE="attenuator.cpp"
# End Source File
# Begin Source File

SOURCE="biast.cpp"
# End Source File
# Begin Source File

SOURCE="binarytogrey4bit.cpp"
# End Source File
# Begin Source File

SOURCE="bjt.cpp"
# End Source File
# Begin Source File

SOURCE="bjtsub.cpp"
# End Source File
# Begin Source File

SOURCE="bondwire.cpp"
# End Source File
# Begin Source File

SOURCE="capacitor.cpp"
# End Source File
# Begin Source File

SOURCE="cccs.cpp"
# End Source File
# Begin Source File

SOURCE="ccvs.cpp"
# End Source File
# Begin Source File

SOURCE="circulator.cpp"
# End Source File
# Begin Source File

SOURCE="coaxialline.cpp"
# End Source File
# Begin Source File

SOURCE="comp_1bit.cpp"
# End Source File
# Begin Source File

SOURCE="comp_2bit.cpp"
# End Source File
# Begin Source File

SOURCE="comp_4bit.cpp"
# End Source File
# Begin Source File

SOURCE="component.cpp"
# End Source File
# Begin Source File

SOURCE="componentdialog.cpp"
# End Source File
# Begin Source File

SOURCE="componentdialog.h"

!IF  "$(CFG)" == "libcomponents - Win32 Release"

# Begin Custom Build - MOC'ing componentdialog.h ...
InputPath="componentdialog.h"
InputName=componentdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libcomponents - Win32 Debug"

# Begin Custom Build - MOC'ing componentdialog.h ...
InputPath="componentdialog.h"
InputName=componentdialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="componentdialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="coplanar.cpp"
# End Source File
# Begin Source File

SOURCE="coupler.cpp"
# End Source File
# Begin Source File

SOURCE="cpwgap.cpp"
# End Source File
# Begin Source File

SOURCE="cpwopen.cpp"
# End Source File
# Begin Source File

SOURCE="cpwshort.cpp"
# End Source File
# Begin Source File

SOURCE="cpwstep.cpp"
# End Source File
# Begin Source File

SOURCE="ctline.cpp"
# End Source File
# Begin Source File

SOURCE="d_flipflop.cpp"
# End Source File
# Begin Source File

SOURCE="dc_sim.cpp"
# End Source File
# Begin Source File

SOURCE="dcblock.cpp"
# End Source File
# Begin Source File

SOURCE="dcfeed.cpp"
# End Source File
# Begin Source File

SOURCE="dff_SR.cpp"
# End Source File
# Begin Source File

SOURCE="diac.cpp"
# End Source File
# Begin Source File

SOURCE="digi_sim.cpp"
# End Source File
# Begin Source File

SOURCE="digi_source.cpp"
# End Source File
# Begin Source File

SOURCE="diode.cpp"
# End Source File
# Begin Source File

SOURCE="DLS_1ton.cpp"
# End Source File
# Begin Source File

SOURCE="DLS_nto1.cpp"
# End Source File
# Begin Source File

SOURCE="dmux2to4.cpp"
# End Source File
# Begin Source File

SOURCE="dmux3to8.cpp"
# End Source File
# Begin Source File

SOURCE="dmux4to16.cpp"
# End Source File
# Begin Source File

SOURCE="EKV26MOS.cpp"
# End Source File
# Begin Source File

SOURCE="eqndefined.cpp"
# End Source File
# Begin Source File

SOURCE="equation.cpp"
# End Source File
# Begin Source File

SOURCE="fa1b.cpp"
# End Source File
# Begin Source File

SOURCE="fa2b.cpp"
# End Source File
# Begin Source File

SOURCE="gatedDlatch.cpp"
# End Source File
# Begin Source File

SOURCE="greytobinary4bit.cpp"
# End Source File
# Begin Source File

SOURCE="ground.cpp"
# End Source File
# Begin Source File

SOURCE="gyrator.cpp"
# End Source File
# Begin Source File

SOURCE="ha1b.cpp"
# End Source File
# Begin Source File

SOURCE="hb_sim.cpp"
# End Source File
# Begin Source File

SOURCE="HBT_X.cpp"
# End Source File
# Begin Source File

SOURCE="hic0_full.cpp"
# End Source File
# Begin Source File

SOURCE="hic2_full.cpp"
# End Source File
# Begin Source File

SOURCE="hicumL0V1p2.cpp"
# End Source File
# Begin Source File

SOURCE="hicumL0V1p2g.cpp"
# End Source File
# Begin Source File

SOURCE="hicumL0V1p3.cpp"
# End Source File
# Begin Source File

SOURCE="hicumL2V2p1.cpp"
# End Source File
# Begin Source File

SOURCE="hicumL2V2p23.cpp"
# End Source File
# Begin Source File

SOURCE="hicumL2V2p24.cpp"
# End Source File
# Begin Source File

SOURCE="hpribin4bit.cpp"
# End Source File
# Begin Source File

SOURCE="hybrid.cpp"
# End Source File
# Begin Source File

SOURCE="iexp.cpp"
# End Source File
# Begin Source File

SOURCE="ifile.cpp"
# End Source File
# Begin Source File

SOURCE="inductor.cpp"
# End Source File
# Begin Source File

SOURCE="iprobe.cpp"
# End Source File
# Begin Source File

SOURCE="ipulse.cpp"
# End Source File
# Begin Source File

SOURCE="irect.cpp"
# End Source File
# Begin Source File

SOURCE="isolator.cpp"
# End Source File
# Begin Source File

SOURCE="jfet.cpp"
# End Source File
# Begin Source File

SOURCE="jk_flipflop.cpp"
# End Source File
# Begin Source File

SOURCE="jkff_SR.cpp"
# End Source File
# Begin Source File

SOURCE="libcomp.cpp"
# End Source File
# Begin Source File

SOURCE="log_amp.cpp"
# End Source File
# Begin Source File

SOURCE="logic_0.cpp"
# End Source File
# Begin Source File

SOURCE="logic_1.cpp"
# End Source File
# Begin Source File

SOURCE="logical_and.cpp"
# End Source File
# Begin Source File

SOURCE="logical_buf.cpp"
# End Source File
# Begin Source File

SOURCE="logical_inv.cpp"
# End Source File
# Begin Source File

SOURCE="logical_nand.cpp"
# End Source File
# Begin Source File

SOURCE="logical_nor.cpp"
# End Source File
# Begin Source File

SOURCE="logical_or.cpp"
# End Source File
# Begin Source File

SOURCE="logical_xnor.cpp"
# End Source File
# Begin Source File

SOURCE="logical_xor.cpp"
# End Source File
# Begin Source File

SOURCE="MESFET.cpp"
# End Source File
# Begin Source File

SOURCE="mod_amp.cpp"
# End Source File
# Begin Source File

SOURCE="mosfet.cpp"
# End Source File
# Begin Source File

SOURCE="mosfet_sub.cpp"
# End Source File
# Begin Source File

SOURCE="mscorner.cpp"
# End Source File
# Begin Source File

SOURCE="mscoupled.cpp"
# End Source File
# Begin Source File

SOURCE="mscross.cpp"
# End Source File
# Begin Source File

SOURCE="msgap.cpp"
# End Source File
# Begin Source File

SOURCE="mslange.cpp"
# End Source File
# Begin Source File

SOURCE="msline.cpp"
# End Source File
# Begin Source File

SOURCE="msmbend.cpp"
# End Source File
# Begin Source File

SOURCE="msopen.cpp"
# End Source File
# Begin Source File

SOURCE="msrstub.cpp"
# End Source File
# Begin Source File

SOURCE="msstep.cpp"
# End Source File
# Begin Source File

SOURCE="mstee.cpp"
# End Source File
# Begin Source File

SOURCE="msvia.cpp"
# End Source File
# Begin Source File

SOURCE="mutual.cpp"
# End Source File
# Begin Source File

SOURCE="mutual2.cpp"
# End Source File
# Begin Source File

SOURCE="mux2to1.cpp"
# End Source File
# Begin Source File

SOURCE="mux4to1.cpp"
# End Source File
# Begin Source File

SOURCE="mux8to1.cpp"
# End Source File
# Begin Source File

SOURCE="nigbt.cpp"
# End Source File
# Begin Source File

SOURCE="noise_ii.cpp"
# End Source File
# Begin Source File

SOURCE="noise_iv.cpp"
# End Source File
# Begin Source File

SOURCE="noise_vv.cpp"
# End Source File
# Begin Source File

SOURCE="opamp.cpp"
# End Source File
# Begin Source File

SOURCE="opt_sim.cpp"
# End Source File
# Begin Source File

SOURCE="optimizedialog.cpp"
# End Source File
# Begin Source File

SOURCE="optimizedialog.h"

!IF  "$(CFG)" == "libcomponents - Win32 Release"

# Begin Custom Build - MOC'ing optimizedialog.h ...
InputPath="optimizedialog.h"
InputName=optimizedialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libcomponents - Win32 Debug"

# Begin Custom Build - MOC'ing optimizedialog.h ...
InputPath="optimizedialog.h"
InputName=optimizedialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="optimizedialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="pad2bit.cpp"
# End Source File
# Begin Source File

SOURCE="pad3bit.cpp"
# End Source File
# Begin Source File

SOURCE="pad4bit.cpp"
# End Source File
# Begin Source File

SOURCE="param_sweep.cpp"
# End Source File
# Begin Source File

SOURCE="phaseshifter.cpp"
# End Source File
# Begin Source File

SOURCE="photodiode.cpp"
# End Source File
# Begin Source File

SOURCE="phototransistor.cpp"
# End Source File
# Begin Source File

SOURCE="pm_modulator.cpp"
# End Source File
# Begin Source File

SOURCE="potentiometer.cpp"
# End Source File
# Begin Source File

SOURCE="rectline.cpp"
# End Source File
# Begin Source File

SOURCE="relais.cpp"
# End Source File
# Begin Source File

SOURCE="resistor.cpp"
# End Source File
# Begin Source File

SOURCE="rfedd.cpp"
# End Source File
# Begin Source File

SOURCE="rfedd2p.cpp"
# End Source File
# Begin Source File

SOURCE="rlcg.cpp"
# End Source File
# Begin Source File

SOURCE="rs_flipflop.cpp"
# End Source File
# Begin Source File

SOURCE="source_ac.cpp"
# End Source File
# Begin Source File

SOURCE="sp_sim.cpp"
# End Source File
# Begin Source File

SOURCE="sparamfile.cpp"
# End Source File
# Begin Source File

SOURCE="spicedialog.cpp"
# End Source File
# Begin Source File

SOURCE="spicedialog.h"

!IF  "$(CFG)" == "libcomponents - Win32 Release"

# Begin Custom Build - MOC'ing spicedialog.h ...
InputPath="spicedialog.h"
InputName=spicedialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libcomponents - Win32 Debug"

# Begin Custom Build - MOC'ing spicedialog.h ...
InputPath="spicedialog.h"
InputName=spicedialog

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="spicedialog.moc.cpp"
# End Source File
# Begin Source File

SOURCE="spicefile.cpp"
# End Source File
# Begin Source File

SOURCE="spicefile.h"

!IF  "$(CFG)" == "libcomponents - Win32 Release"

# Begin Custom Build - MOC'ing spicefile.h ...
InputPath="spicefile.h"
InputName=spicefile

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ELSEIF  "$(CFG)" == "libcomponents - Win32 Debug"

# Begin Custom Build - MOC'ing spicefile.h ...
InputPath="spicefile.h"
InputName=spicefile

"$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc -o $(InputName).moc.cpp $(InputName).h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="spicefile.moc.cpp"
# End Source File
# Begin Source File

SOURCE="subcircuit.cpp"
# End Source File
# Begin Source File

SOURCE="subcirport.cpp"
# End Source File
# Begin Source File

SOURCE="substrate.cpp"
# End Source File
# Begin Source File

SOURCE="switch.cpp"
# End Source File
# Begin Source File

SOURCE="symtrafo.cpp"
# End Source File
# Begin Source File

SOURCE="tff_SR.cpp"
# End Source File
# Begin Source File

SOURCE="thyristor.cpp"
# End Source File
# Begin Source File

SOURCE="tline.cpp"
# End Source File
# Begin Source File

SOURCE="tline_4port.cpp"
# End Source File
# Begin Source File

SOURCE="tr_sim.cpp"
# End Source File
# Begin Source File

SOURCE="transformer.cpp"
# End Source File
# Begin Source File

SOURCE="triac.cpp"
# End Source File
# Begin Source File

SOURCE="tunneldiode.cpp"
# End Source File
# Begin Source File

SOURCE="twistedpair.cpp"
# End Source File
# Begin Source File

SOURCE="vafile.cpp"
# End Source File
# Begin Source File

SOURCE="vccs.cpp"
# End Source File
# Begin Source File

SOURCE="vcvs.cpp"
# End Source File
# Begin Source File

SOURCE="verilogfile.cpp"
# End Source File
# Begin Source File

SOURCE="vexp.cpp"
# End Source File
# Begin Source File

SOURCE="vfile.cpp"
# End Source File
# Begin Source File

SOURCE="vhdlfile.cpp"
# End Source File
# Begin Source File

SOURCE="volt_ac.cpp"
# End Source File
# Begin Source File

SOURCE="volt_dc.cpp"
# End Source File
# Begin Source File

SOURCE="volt_noise.cpp"
# End Source File
# Begin Source File

SOURCE="vprobe.cpp"
# End Source File
# Begin Source File

SOURCE="vpulse.cpp"
# End Source File
# Begin Source File

SOURCE="vrect.cpp"
# End Source File
# End Target
# End Project
