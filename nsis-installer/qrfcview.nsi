;based on MUI Welcome/Finish Page Example Script written by Joost Verburg
;based on FFDShow install
;based on demexp installer by Christophe GISQUET <Christophe.Gisquet@free.fr>

!define MUI_COMPONENTSPAGE_SMALLDESC
!define MUI_LANGDLL_ALWAYSSHOW
!include "MUI.nsh"
!include "Sections.nsh"
!include "LogicLib.nsh"
!include "StrFunc.nsh"
${StrStr}

Name "qRFCView"
!define QRFCVIEW_VERSION "0.62"

;General
OutFile "qrfcview-${QRFCVIEW_VERSION}-setup.exe"
!ifndef COMPRESSION
  !define COMPRESSION lzma
!endif
SetCompressor ${COMPRESSION}

!define MUI_LANGDLL_REGISTRY_ROOT "HKLM" 
!define MUI_LANGDLL_REGISTRY_KEY "SOFTWARE\MELCO\qrfcview" 
!define MUI_LANGDLL_REGISTRY_VALUENAME "lang"

;--------------------------------
;Configuration

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE $(qrfcviewLicense)
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!define MUI_ABORTWARNING

;Language
!insertmacro MUI_LANGUAGE "English"

LicenseLangString qrfcviewLicense ${LANG_ENGLISH} "..\LICENSE"
LangString TITLE_qrfcview ${LANG_ENGLISH} "qRFCView"
LangString DESC_qrfcview  ${LANG_ENGLISH} "Installs qRFCView ${QRFCVIEW_VERSION}"

;Folder-selection page
InstallDir "$PROGRAMFILES\qrfcview"
; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM SOFTWARE\MELCO\qrfcview "pth"

AutoCloseWindow false

;--------------------------------
;Installer Sections

Section $(TITLE_qrfcview) Sec_qrfcview
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File "..\LICENSE"
  SetOutPath $INSTDIR\bin
  File "bin\mingwm10.dll"
  File "bin\QtCore4.dll"
  File "bin\QtDesignerComponents4.dll"
  File "bin\QtGui4.dll"
  File "bin\QtNetwork4.dll"
  File "bin\qRFCView.exe"

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\MELCO\qrfcview "pth" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\qrfcview" "DisplayName" "qrfcview (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\qrfcview" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteUninstaller "uninstall.exe"

  ; Shortcuts
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\qrfcview"
  CreateShortCut  "$SMPROGRAMS\qrfcview\qrfcview.lnk" "$INSTDIR\bin\qrfcview.exe" "" "$INSTDIR\bin\qrfcview.exe" 0
  CreateShortCut  "$SMPROGRAMS\qrfcview\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0

  ;Write language to the registry (for the uninstaller)
  WriteRegStr HKLM SOFTWARE\MELCO\qrfcview "Installer Language" $LANGUAGE

  GetFullPathName /SHORT $SHORTINSTDIR $INSTDIR
SectionEnd

;--------------------------------
;Descriptions

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${Sec_qrfcview} $(DESC_qrfcview)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"
  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\qrfcview"
  DeleteRegKey HKLM SOFTWARE\MELCO\qrfcview
  DeleteRegKey HKCU SOFTWARE\MELCO\qrfcview
  ; remove shortcuts, if any.
  SetShellVarContext all
  Delete "$SMPROGRAMS\qrfcview\*.*"
  RMDir  "$SMPROGRAMS\qrfcview"
  ; remove files
  Delete "$INSTDIR\bin\mingwm10.dll"
  Delete "$INSTDIR\bin\QtCore4.dll"
  Delete "$INSTDIR\bin\QtDesignerComponents4.dll"
  Delete "$INSTDIR\bin\QtGui4.dll"
  Delete "$INSTDIR\bin\QtNetwork4.dll"
  Delete "$INSTDIR\bin\qRFCView.exe"
  RMDir  "$INSTDIR\bin"

  ; MUST REMOVE UNINSTALLER, too
  Delete "$INSTDIR\uninstall.exe"
  Delete "$INSTDIR\LICENSE"
  RMDir  "$INSTDIR"
;  !insertmacro MUI_UNFINISHHEADER
SectionEnd

Function .onInit

  ;Language selection

  !insertmacro MUI_LANGDLL_DISPLAY

  SectionSetFlags ${Sec_qrfcview} 17

FunctionEnd

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd
