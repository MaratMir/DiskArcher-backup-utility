# Microsoft Developer Studio Project File - Name="MArc2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MArc2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MArc2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MArc2.mak" CFG="MArc2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MArc2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MArc2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MArc/MArc2", PPAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MArc2 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /Zi /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Release/DiskArc.exe" /OPT:ICF /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MArc2 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /version:0.2 /subsystem:windows /debug /machine:I386 /out:"Debug/DiskArc.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MArc2 - Win32 Release"
# Name "MArc2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CAboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CArchiveDB.cpp

!IF  "$(CFG)" == "MArc2 - Win32 Release"

!ELSEIF  "$(CFG)" == "MArc2 - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CBundle.cpp
# End Source File
# Begin Source File

SOURCE=.\CBundles.cpp
# End Source File
# Begin Source File

SOURCE=.\CComputer.cpp
# End Source File
# Begin Source File

SOURCE=.\CCopiesDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CCopiesFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CCopiesView.cpp
# End Source File
# Begin Source File

SOURCE=.\CDiskItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CDrive.cpp
# End Source File
# Begin Source File

SOURCE=.\CFileCompressor.cpp
# End Source File
# Begin Source File

SOURCE=.\CFileCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\CFileOnDisk.cpp
# End Source File
# Begin Source File

SOURCE=.\CFilesCopies.cpp
# End Source File
# Begin Source File

SOURCE=.\CFilesOnDisk.cpp
# End Source File
# Begin Source File

SOURCE=.\CFilesToArc.cpp
# End Source File
# Begin Source File

SOURCE=.\CFilesToArcDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CFilesToArcFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CFilesToArcView.cpp
# End Source File
# Begin Source File

SOURCE=.\CFileToArc.cpp
# End Source File
# Begin Source File

SOURCE=.\CFileToArcPropertiesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\CFoldersToArc.cpp
# End Source File
# Begin Source File

SOURCE=.\CFolderToArc.cpp
# End Source File
# Begin Source File

SOURCE=.\CInputStringDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CInsertDiskDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CLocatorExcludedDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CLocatorExcludedFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CLocatorExcludedView.cpp
# End Source File
# Begin Source File

SOURCE=.\CLocatorFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\CLogDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CLogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CLogFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CLogView.cpp
# End Source File
# Begin Source File

SOURCE=.\CMyArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\CMyListView.cpp
# End Source File
# Begin Source File

SOURCE=.\CNag.cpp
# End Source File
# Begin Source File

SOURCE=.\CNewFilesLocator.cpp
# End Source File
# Begin Source File

SOURCE=.\CNewFilesLocatorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CNewFilesLocatorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CNewFilesLocatorFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CNewFilesLocatorView.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CProgressDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CRegisterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CRoom.cpp
# End Source File
# Begin Source File

SOURCE=.\CRoomLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\CRoomPropDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CRooms.cpp
# End Source File
# Begin Source File

SOURCE=.\CRoomsDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CRoomsFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CRoomsView.cpp
# End Source File
# Begin Source File

SOURCE=.\CTipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CZipBundle.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MArc2.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\MArc2.hpj

!IF  "$(CFG)" == "MArc2 - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__MARC2="hlp\AfxCore.rtf"	"hlp\$(TargetName).hm"	
# Begin Custom Build - Making help file...
OutDir=.\Release
TargetName=DiskArc
InputPath=.\hlp\MArc2.hpj
InputName=MArc2

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(TargetName).hlp" goto :Error 
	copy "hlp\$(TargetName).hlp" "$(OutDir)\$(TargetName).hlp" 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MArc2 - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__MARC2="hlp\AfxCore.rtf"	"hlp\$(TargetName).hm"	
# Begin Custom Build - Making help file...
OutDir=.\Debug
TargetName=DiskArc
InputPath=.\hlp\MArc2.hpj
InputName=MArc2

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(TargetName).hlp" goto :Error 
	copy "hlp\$(TargetName).hlp" "$(OutDir)\$(TargetName).hlp" 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MArc2.rc
# End Source File
# Begin Source File

SOURCE=.\Miscelaneous.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\bundleType.h
# End Source File
# Begin Source File

SOURCE=.\CAboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\CArchiveDB.h
# End Source File
# Begin Source File

SOURCE=.\CBundle.h
# End Source File
# Begin Source File

SOURCE=.\CBundles.h
# End Source File
# Begin Source File

SOURCE=.\CComputer.h
# End Source File
# Begin Source File

SOURCE=.\CCopiesDoc.h
# End Source File
# Begin Source File

SOURCE=.\CCopiesFrame.h
# End Source File
# Begin Source File

SOURCE=.\CCopiesView.h
# End Source File
# Begin Source File

SOURCE=.\CDiskItem.h
# End Source File
# Begin Source File

SOURCE=.\CDrive.h
# End Source File
# Begin Source File

SOURCE=.\CFileCompressor.h
# End Source File
# Begin Source File

SOURCE=.\CFileCopy.h
# End Source File
# Begin Source File

SOURCE=.\CFileOnDisk.h
# End Source File
# Begin Source File

SOURCE=.\CFilesCopies.h
# End Source File
# Begin Source File

SOURCE=.\CFilesOnDisk.h
# End Source File
# Begin Source File

SOURCE=.\CFilesToArc.h
# End Source File
# Begin Source File

SOURCE=.\CFilesToArcDoc.h
# End Source File
# Begin Source File

SOURCE=.\CFilesToArcFrame.h
# End Source File
# Begin Source File

SOURCE=.\CFilesToArcView.h
# End Source File
# Begin Source File

SOURCE=.\CFileToArc.h
# End Source File
# Begin Source File

SOURCE=.\CFileToArcPropertiesDialog.h
# End Source File
# Begin Source File

SOURCE=.\CFolder.h
# End Source File
# Begin Source File

SOURCE=.\CFoldersToArc.h
# End Source File
# Begin Source File

SOURCE=.\CFolderToArc.h
# End Source File
# Begin Source File

SOURCE=.\CInputStringDialog.h
# End Source File
# Begin Source File

SOURCE=.\CInsertDiskDlg.h
# End Source File
# Begin Source File

SOURCE=.\CLocatorExcludedDoc.h
# End Source File
# Begin Source File

SOURCE=.\CLocatorExcludedFrame.h
# End Source File
# Begin Source File

SOURCE=.\CLocatorExcludedView.h
# End Source File
# Begin Source File

SOURCE=.\CLocatorFolder.h
# End Source File
# Begin Source File

SOURCE=.\CLogDoc.h
# End Source File
# Begin Source File

SOURCE=.\CLogFile.h
# End Source File
# Begin Source File

SOURCE=.\CLogFrame.h
# End Source File
# Begin Source File

SOURCE=.\CLogView.h
# End Source File
# Begin Source File

SOURCE=.\CMyArchive.h
# End Source File
# Begin Source File

SOURCE=.\CMyListView.h
# End Source File
# Begin Source File

SOURCE=.\CNag.h
# End Source File
# Begin Source File

SOURCE=.\CNewFilesLocator.h
# End Source File
# Begin Source File

SOURCE=.\CNewFilesLocatorDlg.h
# End Source File
# Begin Source File

SOURCE=.\CNewFilesLocatorDoc.h
# End Source File
# Begin Source File

SOURCE=.\CNewFilesLocatorFrame.h
# End Source File
# Begin Source File

SOURCE=.\CNewFilesLocatorView.h
# End Source File
# Begin Source File

SOURCE=.\COptionsDialog.h
# End Source File
# Begin Source File

SOURCE=.\CProgressDialog.h
# End Source File
# Begin Source File

SOURCE=.\CRegisterDlg.h
# End Source File
# Begin Source File

SOURCE=.\CRoom.h
# End Source File
# Begin Source File

SOURCE=.\CRoomLabel.h
# End Source File
# Begin Source File

SOURCE=.\CRoomPropDialog.h
# End Source File
# Begin Source File

SOURCE=.\CRooms.h
# End Source File
# Begin Source File

SOURCE=.\CRoomsDoc.h
# End Source File
# Begin Source File

SOURCE=.\CRoomsFrame.h
# End Source File
# Begin Source File

SOURCE=.\CRoomsView.h
# End Source File
# Begin Source File

SOURCE=.\CTipDlg.h
# End Source File
# Begin Source File

SOURCE=.\CZipBundle.h
# End Source File
# Begin Source File

SOURCE=.\enums.h
# End Source File
# Begin Source File

SOURCE=.\fileCommand.h
# End Source File
# Begin Source File

SOURCE=.\fileStatus.h
# End Source File
# Begin Source File

SOURCE=.\LeftView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MArc2.h
# End Source File
# Begin Source File

SOURCE=.\Miscelaneous.h
# End Source File
# Begin Source File

SOURCE=.\MyCommon.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "MArc2 - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=DiskArc
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MArc2 - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=DiskArc
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\roomCompressionMode.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\__dummy_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Copies.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Copies.ico
# End Source File
# Begin Source File

SOURCE=.\res\CopiesImageList.bmp
# End Source File
# Begin Source File

SOURCE=.\res\darclogo.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Delete.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Excluded.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Extract.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FilesToA.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FToA_ima.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\litebulb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Locator.ico
# End Source File
# Begin Source File

SOURCE=.\res\locatorExcl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LocatorExcl.ico
# End Source File
# Begin Source File

SOURCE=.\res\LocatoTB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Log.ico
# End Source File
# Begin Source File

SOURCE=.\res\main.ico
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MArc2.ico
# End Source File
# Begin Source File

SOURCE=.\res\MArc2.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MArc2Doc.ico
# End Source File
# Begin Source File

SOURCE=.\res\rooms_im.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RoomsTB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tick.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter "cnt;rtf"
# Begin Source File

SOURCE=.\hlp\AfxCore.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\Changed.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Continue.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw2.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurHelp.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\DiskArc.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\EditPast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileCopies.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileOpen.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileToAProperties.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Go.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpSBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpTBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\MArc2.cnt

!IF  "$(CFG)" == "MArc2 - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Release
InputPath=.\hlp\MArc2.cnt
InputName=MArc2

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "MArc2 - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Debug
InputPath=.\hlp\MArc2.cnt
InputName=MArc2

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hlp\NewFile.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\NotFound.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Older.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Paused.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Room.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RoomBig.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RoomNew.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\ScMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmin.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Update.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\UpToDate.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section MArc2 : {72ADFD54-2C39-11D0-9903-00A0C91BC942}
# 	1:17:CG_IDS_DIDYOUKNOW:106
# 	1:22:CG_IDS_TIPOFTHEDAYMENU:105
# 	1:18:CG_IDS_TIPOFTHEDAY:104
# 	1:22:CG_IDS_TIPOFTHEDAYHELP:109
# 	1:19:CG_IDP_FILE_CORRUPT:108
# 	1:7:IDD_TIP:103
# 	1:13:IDB_LIGHTBULB:102
# 	1:18:CG_IDS_FILE_ABSENT:107
# 	2:17:CG_IDS_DIDYOUKNOW:CG_IDS_DIDYOUKNOW
# 	2:7:CTipDlg:CTipDlg
# 	2:22:CG_IDS_TIPOFTHEDAYMENU:CG_IDS_TIPOFTHEDAYMENU
# 	2:18:CG_IDS_TIPOFTHEDAY:CG_IDS_TIPOFTHEDAY
# 	2:12:CTIP_Written:OK
# 	2:22:CG_IDS_TIPOFTHEDAYHELP:CG_IDS_TIPOFTHEDAYHELP
# 	2:2:BH:
# 	2:19:CG_IDP_FILE_CORRUPT:CG_IDP_FILE_CORRUPT
# 	2:7:IDD_TIP:IDD_TIP
# 	2:8:TipDlg.h:CTipDlg.h
# 	2:13:IDB_LIGHTBULB:IDB_LIGHTBULB
# 	2:18:CG_IDS_FILE_ABSENT:CG_IDS_FILE_ABSENT
# 	2:10:TipDlg.cpp:CTipDlg.cpp
# End Section
