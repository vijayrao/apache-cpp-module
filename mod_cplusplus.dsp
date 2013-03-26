# Microsoft Developer Studio Project File - Name="mod_cplusplus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=mod_cplusplus - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mod_cplusplus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mod_cplusplus.mak" CFG="mod_cplusplus - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mod_cplusplus - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mod_cplusplus - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "mod_cplusplus"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mod_cplusplus - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../../../../../packages/apache/httpd-2.0.35/wnt/include" /I "../../../core/include" /I "../../../core/include/mod_cplusplus" /I "../../../core/include/rw/wnt" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FR /Fd"Release\mod_cplusplus" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib mod_cplusplus /nologo /subsystem:windows /dll /map /machine:I386 /out:"Release/mod_cplusplus.so"
# ADD LINK32 kernel32.lib user32.lib libhttpd.lib libapr.lib /nologo /subsystem:windows /dll /map /machine:I386 /out:"Release/mod_cplusplus.so" /libpath:"../lib/Release" /libpath:"../../../../../../packages/apache/httpd-2.0.35/wnt/lib"

!ELSEIF  "$(CFG)" == "mod_cplusplus - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /I "../../../../../../packages/apache/httpd-2.0.35/wnt/include" /I "../../../core/include" /I "../../../core/include/mod_cplusplus" /I "../../../core/include/rw/wnt" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /Fd"Debug\mod_cplusplus" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib mod_cplusplus /nologo /subsystem:windows /dll /incremental:no /map /debug /machine:I386 /out:"Debug/mod_cplusplus.so"
# ADD LINK32 kernel32.lib user32.lib libhttpd.lib libapr.lib libaprutil.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"Debug/mod_cplusplus.so" /libpath:"../lib/Debug" /libpath:"../../../../../../packages/apache/httpd-2.0.35/wnt/lib"
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "mod_cplusplus - Win32 Release"
# Name "mod_cplusplus - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "*.cpp,*.c"
# Begin Source File

SOURCE=..\..\..\core\src\mod_cplusplus\apache_filters.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\core\src\mod_cplusplus\apache_handler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\core\src\mod_cplusplus\apache_output_buffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\core\src\mod_cplusplus\apache_protocol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\core\src\mod_cplusplus\cpp_request.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\core\src\mod_cplusplus\cpp_server.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\core\src\mod_cplusplus\env_value.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\core\src\mod_cplusplus\mod_cplusplus.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\src\mod_cplusplus\request_env.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h,*.hpp"
# Begin Source File

SOURCE=..\..\..\core\include\mod_cplusplus\apache_filters.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\include\mod_cplusplus\apache_handler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\include\mod_cplusplus\apache_output_buffer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\include\mod_cplusplus\apache_protocol.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\include\mod_cplusplus\cpp_request.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\include\mod_cplusplus\cpp_server.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\include\mod_cplusplus\env_value.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\include\mod_cplusplus\mod_cplusplus.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\include\mod_cplusplus\request_env.h
# End Source File
# End Group
# End Target
# End Project
