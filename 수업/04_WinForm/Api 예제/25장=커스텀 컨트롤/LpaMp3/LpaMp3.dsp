# Microsoft Developer Studio Project File - Name="LpaMp3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=LpaMp3 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LpaMp3.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LpaMp3.mak" CFG="LpaMp3 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LpaMp3 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "LpaMp3 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LpaMp3 - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "LpaMp3 - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "LpaMp3 - Win32 Release"
# Name "LpaMp3 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GrpBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\GrpScrl.cpp
# End Source File
# Begin Source File

SOURCE=.\LpaMp3.cpp
# End Source File
# Begin Source File

SOURCE=.\LpaMp3.rc
# End Source File
# Begin Source File

SOURCE=.\ShReg.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\fmod.h
# End Source File
# Begin Source File

SOURCE=.\GrpBtn.h
# End Source File
# Begin Source File

SOURCE=.\GrpScrl.h
# End Source File
# Begin Source File

SOURCE=.\ShReg.h
# End Source File
# Begin Source File

SOURCE=.\Util.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\atopdown.bmp
# End Source File
# Begin Source File

SOURCE=.\atopnorm.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\closedown.bmp
# End Source File
# Begin Source File

SOURCE=.\closenor.bmp
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\listnorm.bmp
# End Source File
# Begin Source File

SOURCE=.\mindown.bmp
# End Source File
# Begin Source File

SOURCE=.\minnorma.bmp
# End Source File
# Begin Source File

SOURCE=.\nextdown.bmp
# End Source File
# Begin Source File

SOURCE=.\nexthot.bmp
# End Source File
# Begin Source File

SOURCE=.\nextnorm.bmp
# End Source File
# Begin Source File

SOURCE=.\opendown.bmp
# End Source File
# Begin Source File

SOURCE=.\openhot.bmp
# End Source File
# Begin Source File

SOURCE=.\opennorm.bmp
# End Source File
# Begin Source File

SOURCE=.\optiondo.bmp
# End Source File
# Begin Source File

SOURCE=.\optionho.bmp
# End Source File
# Begin Source File

SOURCE=.\optionno.bmp
# End Source File
# Begin Source File

SOURCE=.\pausedow.bmp
# End Source File
# Begin Source File

SOURCE=.\pausehot.bmp
# End Source File
# Begin Source File

SOURCE=.\pausenor.bmp
# End Source File
# Begin Source File

SOURCE=.\playdown.bmp
# End Source File
# Begin Source File

SOURCE=.\plistdow.bmp
# End Source File
# Begin Source File

SOURCE=.\prevdown.bmp
# End Source File
# Begin Source File

SOURCE=.\prevhot.bmp
# End Source File
# Begin Source File

SOURCE=.\prevnorm.bmp
# End Source File
# Begin Source File

SOURCE=.\repeatno.bmp
# End Source File
# Begin Source File

SOURCE=.\stopdown.bmp
# End Source File
# Begin Source File

SOURCE=.\stophot.bmp
# End Source File
# Begin Source File

SOURCE=.\stopnorm.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\fmodvc.lib
# End Source File
# End Target
# End Project
