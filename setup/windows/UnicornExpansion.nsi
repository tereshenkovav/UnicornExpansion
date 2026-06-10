!ifndef GAMELANG
  !error "Value of GAMELANG not defined"
!endif

Unicode True
RequestExecutionLevel admin
SetCompressor /SOLID zlib
AutoCloseWindow true
Icon main.ico
XPStyle on

!include LangData_${GAMELANG}.nsi

!include "FileFunc.nsh"
!insertmacro GetTime

!define TEMP1 $R0 

ReserveFile /plugin InstallOptions.dll
ReserveFile "runapp_${GAMELANG}.ini"

OutFile "UnicornExpansion-${UPPERLANG}-${VERSION}-Win64.exe"

var is_update

Page directory
Page components
Page instfiles
Page custom SetRunApp ValidateRunApp "$(AfterParams)" 

UninstPage uninstConfirm
UninstPage instfiles

Name $(GameGameName)

Function .onInit
  InitPluginsDir
  File /oname=$PLUGINSDIR\runapp_${GAMELANG}.ini "runapp_${GAMELANG}.ini"

  StrCpy $INSTDIR $PROGRAMFILES64\UnicornExpansion

  IfFileExists $INSTDIR\UnicornExpansion.exe +3
  StrCpy $is_update "0"
  Goto +2
  StrCpy $is_update "1"
  
FunctionEnd

Function .onInstSuccess
  StrCmp $is_update "1" SkipAll

  ReadINIStr ${TEMP1} "$PLUGINSDIR\runapp_${GAMELANG}.ini" "Field 1" "State"
  StrCmp ${TEMP1} "0" SkipDesktop

  SetOutPath $INSTDIR
  CreateShortCut "$DESKTOP\$(GameNameLink).lnk" "$INSTDIR\UnicornExpansion.exe" "" "$INSTDIR\main.ico"

SkipDesktop:

  ReadINIStr ${TEMP1} "$PLUGINSDIR\runapp_${GAMELANG}.ini" "Field 2" "State"
  StrCmp ${TEMP1} "0" SkipRun

  Exec $INSTDIR\UnicornExpansion.exe

  SkipRun:
  SkipAll:

FunctionEnd

Function un.onUninstSuccess
  MessageBox MB_OK "$(MsgUninstOK)"
FunctionEnd

Function un.onUninstFailed
  MessageBox MB_OK "$(MsgUninstError)"
FunctionEnd

Function .onInstFailed
  MessageBox MB_OK "$(MsgInstError)"
FunctionEnd

Section "$(GameGameName)"
  SectionIn RO

  StrCmp $is_update "0" SkipSleep
  Sleep 3000
  SkipSleep:

  SetOutPath $INSTDIR
  File main.ico

  SetOutPath $INSTDIR
  File ${BINDIR}\UnicornExpansion.exe
  File ${BINDIR}\sfml-*.dll

  SetOutPath $INSTDIR\data
  File /r ..\..\data\*

  FileOpen $0 "$INSTDIR\data\deflang" w
  FileWrite $0 "${GAMELANG}"
  FileClose $0

  StrCmp $is_update "1" Skip2
  
  WriteUninstaller $INSTDIR\Uninst.exe

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UnicornExpansion" \
                 "DisplayName" "$(GameGameName)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UnicornExpansion" \
                 "UninstallString" "$\"$INSTDIR\Uninst.exe$\""
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UnicornExpansion" \
                 "EstimatedSize" 0x00002000
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UnicornExpansion" \
                 "DisplayIcon" $INSTDIR\main.ico

  ${GetTime} "" "L" $0 $1 $2 $3 $4 $5 $6
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UnicornExpansion" \
                 "InstallDate"  "$2$1$0"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UnicornExpansion" \
                 "Publisher"  "$(PublisherName)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UnicornExpansion" \
                 "DisplayVersion"  "${VERSION}"

  SetOutPath $INSTDIR
  CreateDirectory "$SMPROGRAMS\$(GameNameLink)"
  CreateShortCut "$SMPROGRAMS\$(GameNameLink)\$(GameNameLink).lnk" "$INSTDIR\UnicornExpansion.exe" "" "$INSTDIR\main.ico"

Skip2:

SectionEnd

Section "Uninstall"
  RMDir /r $INSTDIR
  RMDir /r "$SMPROGRAMS\$(GameNameLink)"
  Delete "$DESKTOP\$(GameNameLink).lnk"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UnicornExpansion"
SectionEnd

Function SetRunApp

  Push ${TEMP1}

  InstallOptions::dialog "$PLUGINSDIR\runapp_${GAMELANG}.ini"
    Pop ${TEMP1}
  
  Pop ${TEMP1}

FunctionEnd

Function ValidateRunApp

FunctionEnd
