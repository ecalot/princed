Attribute VB_Name = "Module1"
'pr.dll V2.0 declaration for Visual Basic
'PR is a Princed Development Team work
'check for the latest version at http://www.princed.com.ar

'Enumerations

'Resource type
enum prResourceType
 prResMemoryError=-2
 prResFileNotFoundError=-1
 
 prResInvalidDatFile=00
 
 prResLevelFile=01
 prResGraphicExtFile=02
 prResWaveFile=03
 prResMidiFile=04
 prResValidUnknownFile=05
 prResGraphicFile=06

end

'Export
enum prExportResult
 prExpMemoryError=-2
 prExpAccessDenied=-1
 prExpOk=0
end

enum prExportOpt 'To use more than one option separate them with "+".
 prExpExtract=1  'If this option is not given, the files won't be extracted
 prExpUpdate=2   'This will update the resource file
 prExpUseRaw=4   'This will extract to raw files
 prExpDontSave=8 'Running the function using this option will perform all
                 ' commands but without saving the changes in resource file
                 ' (update option will be ignored)
end

'Import
enum prImportResult
 prImpAccessDenied=-1
 prImpOk=0
 'Note: If the file was parcially compiled a positive number of files missing will be returned
end

enum prImportOpt
 prImpUseRaw=0
 prImpReadDataType=1
end

'This function will extract vDatFile into vDirName adding the extracted files in vResFilename
Public Declare Function prExportDat Lib "pr.dll" Alias "_prExportDat" (ByVal vDatFile As String, ByVal vDirName As String, ByVal vResFilename As String) As Integer
'This function will import the files located in vDirName into vDatFile acording to the information read in vResFilename
Public Declare Function prImportDat Lib "pr.dll" Alias "_prImportDat" (ByVal vDatFile As String, ByVal vDirName As String, ByVal vResFilename As String) As Integer
'This function will clear vResFilename information table but It will keep the file header and foot
Public Declare Function prClearRes Lib "pr.dll" Alias "_prClearRes" (ByVal vResFilename As String) As Integer
'This function will verify if the vDatFile exists, id it is a valid POP1 DAT file, and what kind of DAT file it is
Public Declare Function prVerifyDatType Lib "pr.dll" Alias "_prVerifyDatType" (ByVal vDatFile As String) As integer

'  verifyDatType return values may be:
'    -2 PR internal Memory error (only if RAM is too low)
'    -1 File not found
'
'    00 Not a valid POP1 DAT file
'
'    01 Levels file
'    02 Graphic file with an image in the first valid entry (not common)
'    03 Waves/Digital sound file
'    04 Midis file
'    05 Valid DAT file with Undefined content (example ibmsnd for the moment)
'    06 Graphic file with a palette in the first valid entry (common)

' extended functions

Public Declare Function prVerifyDatTypeRes Lib "pr.dll" Alias "_prVerifyDatType" (ByVal vDatFile As String) As prResourceType
Public Declare Function prClearResBool Lib "pr.dll" Alias "_prClearRes" (ByVal vResFilename As String) As Boolean
Public Declare Function prExportDatOpt Lib "pr.dll" Alias "_prExportDatOpt" (ByVal vDatFile As String, ByVal vDirName As String, ByVal vResFilename As String, ByVal vExportOpt as prExportOpt) As prExportResult
Public Declare Function prImportDatOpt Lib "pr.dll" Alias "_prImportDatOpt" (ByVal vDatFile As String, ByVal vDirName As String, ByVal vResFilename As String, ByVal vImportOpt as prImportOpt) As prImportResult
