# Microsoft Developer Studio Project File - Name="rfftw" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=rfftw - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rfftw.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rfftw.mak" CFG="rfftw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rfftw - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "rfftw - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rfftw - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "." /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "rfftw - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "rfftw - Win32 Release"
# Name "rfftw - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\config.c
# End Source File
# Begin Source File

SOURCE=.\executor.c
# End Source File
# Begin Source File

SOURCE=.\fcr_1.c
# End Source File
# Begin Source File

SOURCE=.\fcr_10.c
# End Source File
# Begin Source File

SOURCE=.\fcr_11.c
# End Source File
# Begin Source File

SOURCE=.\fcr_12.c
# End Source File
# Begin Source File

SOURCE=.\fcr_128.c
# End Source File
# Begin Source File

SOURCE=.\fcr_13.c
# End Source File
# Begin Source File

SOURCE=.\fcr_14.c
# End Source File
# Begin Source File

SOURCE=.\fcr_15.c
# End Source File
# Begin Source File

SOURCE=.\fcr_16.c
# End Source File
# Begin Source File

SOURCE=.\fcr_2.c
# End Source File
# Begin Source File

SOURCE=.\fcr_3.c
# End Source File
# Begin Source File

SOURCE=.\fcr_32.c
# End Source File
# Begin Source File

SOURCE=.\fcr_4.c
# End Source File
# Begin Source File

SOURCE=.\fcr_5.c
# End Source File
# Begin Source File

SOURCE=.\fcr_6.c
# End Source File
# Begin Source File

SOURCE=.\fcr_64.c
# End Source File
# Begin Source File

SOURCE=.\fcr_7.c
# End Source File
# Begin Source File

SOURCE=.\fcr_8.c
# End Source File
# Begin Source File

SOURCE=.\fcr_9.c
# End Source File
# Begin Source File

SOURCE=.\fftwnd.c
# End Source File
# Begin Source File

SOURCE=.\fhb_10.c
# End Source File
# Begin Source File

SOURCE=.\fhb_16.c
# End Source File
# Begin Source File

SOURCE=.\fhb_2.c
# End Source File
# Begin Source File

SOURCE=.\fhb_3.c
# End Source File
# Begin Source File

SOURCE=.\fhb_32.c
# End Source File
# Begin Source File

SOURCE=.\fhb_4.c
# End Source File
# Begin Source File

SOURCE=.\fhb_5.c
# End Source File
# Begin Source File

SOURCE=.\fhb_6.c
# End Source File
# Begin Source File

SOURCE=.\fhb_7.c
# End Source File
# Begin Source File

SOURCE=.\fhb_8.c
# End Source File
# Begin Source File

SOURCE=.\fhb_9.c
# End Source File
# Begin Source File

SOURCE=.\fhf_10.c
# End Source File
# Begin Source File

SOURCE=.\fhf_16.c
# End Source File
# Begin Source File

SOURCE=.\fhf_2.c
# End Source File
# Begin Source File

SOURCE=.\fhf_3.c
# End Source File
# Begin Source File

SOURCE=.\fhf_32.c
# End Source File
# Begin Source File

SOURCE=.\fhf_4.c
# End Source File
# Begin Source File

SOURCE=.\fhf_5.c
# End Source File
# Begin Source File

SOURCE=.\fhf_6.c
# End Source File
# Begin Source File

SOURCE=.\fhf_7.c
# End Source File
# Begin Source File

SOURCE=.\fhf_8.c
# End Source File
# Begin Source File

SOURCE=.\fhf_9.c
# End Source File
# Begin Source File

SOURCE=.\fn_1.c
# End Source File
# Begin Source File

SOURCE=.\fn_10.c
# End Source File
# Begin Source File

SOURCE=.\fn_11.c
# End Source File
# Begin Source File

SOURCE=.\fn_12.c
# End Source File
# Begin Source File

SOURCE=.\fn_13.c
# End Source File
# Begin Source File

SOURCE=.\fn_14.c
# End Source File
# Begin Source File

SOURCE=.\fn_15.c
# End Source File
# Begin Source File

SOURCE=.\fn_16.c
# End Source File
# Begin Source File

SOURCE=.\fn_2.c
# End Source File
# Begin Source File

SOURCE=.\fn_3.c
# End Source File
# Begin Source File

SOURCE=.\fn_32.c
# End Source File
# Begin Source File

SOURCE=.\fn_4.c
# End Source File
# Begin Source File

SOURCE=.\fn_5.c
# End Source File
# Begin Source File

SOURCE=.\fn_6.c
# End Source File
# Begin Source File

SOURCE=.\fn_64.c
# End Source File
# Begin Source File

SOURCE=.\fn_7.c
# End Source File
# Begin Source File

SOURCE=.\fn_8.c
# End Source File
# Begin Source File

SOURCE=.\fn_9.c
# End Source File
# Begin Source File

SOURCE=.\fni_1.c
# End Source File
# Begin Source File

SOURCE=.\fni_10.c
# End Source File
# Begin Source File

SOURCE=.\fni_11.c
# End Source File
# Begin Source File

SOURCE=.\fni_12.c
# End Source File
# Begin Source File

SOURCE=.\fni_13.c
# End Source File
# Begin Source File

SOURCE=.\fni_14.c
# End Source File
# Begin Source File

SOURCE=.\fni_15.c
# End Source File
# Begin Source File

SOURCE=.\fni_16.c
# End Source File
# Begin Source File

SOURCE=.\fni_2.c
# End Source File
# Begin Source File

SOURCE=.\fni_3.c
# End Source File
# Begin Source File

SOURCE=.\fni_32.c
# End Source File
# Begin Source File

SOURCE=.\fni_4.c
# End Source File
# Begin Source File

SOURCE=.\fni_5.c
# End Source File
# Begin Source File

SOURCE=.\fni_6.c
# End Source File
# Begin Source File

SOURCE=.\fni_64.c
# End Source File
# Begin Source File

SOURCE=.\fni_7.c
# End Source File
# Begin Source File

SOURCE=.\fni_8.c
# End Source File
# Begin Source File

SOURCE=.\fni_9.c
# End Source File
# Begin Source File

SOURCE=..\fourier.cpp
# End Source File
# Begin Source File

SOURCE=.\frc_1.c
# End Source File
# Begin Source File

SOURCE=.\frc_10.c
# End Source File
# Begin Source File

SOURCE=.\frc_11.c
# End Source File
# Begin Source File

SOURCE=.\frc_12.c
# End Source File
# Begin Source File

SOURCE=.\frc_128.c
# End Source File
# Begin Source File

SOURCE=.\frc_13.c
# End Source File
# Begin Source File

SOURCE=.\frc_14.c
# End Source File
# Begin Source File

SOURCE=.\frc_15.c
# End Source File
# Begin Source File

SOURCE=.\frc_16.c
# End Source File
# Begin Source File

SOURCE=.\frc_2.c
# End Source File
# Begin Source File

SOURCE=.\frc_3.c
# End Source File
# Begin Source File

SOURCE=.\frc_32.c
# End Source File
# Begin Source File

SOURCE=.\frc_4.c
# End Source File
# Begin Source File

SOURCE=.\frc_5.c
# End Source File
# Begin Source File

SOURCE=.\frc_6.c
# End Source File
# Begin Source File

SOURCE=.\frc_64.c
# End Source File
# Begin Source File

SOURCE=.\frc_7.c
# End Source File
# Begin Source File

SOURCE=.\frc_8.c
# End Source File
# Begin Source File

SOURCE=.\frc_9.c
# End Source File
# Begin Source File

SOURCE=.\ftw_10.c
# End Source File
# Begin Source File

SOURCE=.\ftw_16.c
# End Source File
# Begin Source File

SOURCE=.\ftw_2.c
# End Source File
# Begin Source File

SOURCE=.\ftw_3.c
# End Source File
# Begin Source File

SOURCE=.\ftw_32.c
# End Source File
# Begin Source File

SOURCE=.\ftw_4.c
# End Source File
# Begin Source File

SOURCE=.\ftw_5.c
# End Source File
# Begin Source File

SOURCE=.\ftw_6.c
# End Source File
# Begin Source File

SOURCE=.\ftw_64.c
# End Source File
# Begin Source File

SOURCE=.\ftw_7.c
# End Source File
# Begin Source File

SOURCE=.\ftw_8.c
# End Source File
# Begin Source File

SOURCE=.\ftw_9.c
# End Source File
# Begin Source File

SOURCE=.\ftwi_10.c
# End Source File
# Begin Source File

SOURCE=.\ftwi_16.c
# End Source File
# Begin Source File

SOURCE=.\ftwi_2.c
# End Source File
# Begin Source File

SOURCE=.\ftwi_3.c
# End Source File
# Begin Source File

SOURCE=.\ftwi_32.c
# End Source File
# Begin Source File

SOURCE=.\ftwi_4.c
# End Source File
# Begin Source File

SOURCE=.\ftwi_5.c
# End Source File
# Begin Source File

SOURCE=.\ftwi_6.c
# End Source File
# Begin Source File

SOURCE=.\ftwi_64.c
# End Source File
# Begin Source File

SOURCE=.\ftwi_7.c
# End Source File
# Begin Source File

SOURCE=.\ftwi_8.c
# End Source File
# Begin Source File

SOURCE=.\ftwi_9.c
# End Source File
# Begin Source File

SOURCE=.\generic.c
# End Source File
# Begin Source File

SOURCE=.\malloc.c
# End Source File
# Begin Source File

SOURCE=.\planner.c
# End Source File
# Begin Source File

SOURCE=.\putils.c
# End Source File
# Begin Source File

SOURCE=.\rader.c
# End Source File
# Begin Source File

SOURCE=.\rconfig.c
# End Source File
# Begin Source File

SOURCE=.\rexec.c
# End Source File
# Begin Source File

SOURCE=.\rexec2.c
# End Source File
# Begin Source File

SOURCE=.\rfftwf77.c
# End Source File
# Begin Source File

SOURCE=.\rfftwnd.c
# End Source File
# Begin Source File

SOURCE=.\rgeneric.c
# End Source File
# Begin Source File

SOURCE=.\rplanner.c
# End Source File
# Begin Source File

SOURCE=.\timer.c
# End Source File
# Begin Source File

SOURCE=.\twiddle.c
# End Source File
# Begin Source File

SOURCE=.\wisdom.c
# End Source File
# Begin Source File

SOURCE=.\wisdomio.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
