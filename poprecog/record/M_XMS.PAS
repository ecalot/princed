(*
  XMS library
  
  Author   : Piotr Kochanek
  WWW site : www.pop2.princed.com.ar
*)

UNIT M_XMS;

INTERFACE
Var
 XMSError : Byte;

{ // Czy pamiec XMS istnieje ? }
FUNCTION XMSFind : Boolean;
{ // Funkcja zwraca adres programu XMM }
FUNCTION XMSGetAddress : Pointer;
{ // Zwraca wersje sterownika XMM }
FUNCTION XMSVersion : Word;
{ // Funkcja inicjujaca modul i spraqdzajaca czy wszystko OK }
FUNCTION XMSSetup : Boolean;
{ // Funkcja zwraca ilosc wolnej pamieci XMS w KB }
FUNCTION XMSMemAvail : Word;
{ // Funkcja zwraca wielkosc najwiekszego bloku pamieci XMS w KB }
FUNCTION XMSMaxAvail : Word;
{ // Funkcja alokuje pamiec XMS }
FUNCTION XMSGetMem (Var Handler : Word ; Size : Word) : Boolean;
{ // Zmienia rozmiar pamieci XMS pod wybranym uchwytem }
FUNCTION XMSReGetMem (Handler : Word ; Size : Word) : Boolean;
{ // Funkcja usuwa pamiec XMS }
FUNCTION XMSFreeMem (Handler : Word) : Boolean;
{ // Funkcja kopiuje bajty do/z pamieci XMS }
FUNCTION XMSMoveMem (SrcHandler : Word ; SrcOffset : Pointer ;
                     DstHandler : Word ; DstOffset : Pointer ; Size : Longint) : Boolean;

IMPLEMENTATION
VAR
 { // Adres sterownika pamieci XMS }
 XMSAddress : Pointer;
 { // Rekord potrzebny przy przesylaniu pamieci }
 XMSBlockInfo :
   RECORD
     Length : Longint;
     SourceHandler : Word;
     SourceOffset : Pointer;
     DestinationHandler : Word;
     DestinationOffset : Pointer;
   END;

(*
  Funkcja sprawdza czy istnieje pamiec XMS
*)
FUNCTION XMSFind : Boolean;
BEGIN
 ASM
   mov ax, $4300
   int $2f
   mov @result, FALSE
   cmp al, $80
   jne @end
   mov @result, TRUE
   @end:
 END;
END;

(*
  Zwraca adres sterownika pamieci XMS
*)
FUNCTION XMSGetAddress : Pointer;
VAR
 XMSSeg : Word;
 XMSOfs : Word;
BEGIN
 ASM
   mov ax, $4310
   int $2f
   mov XMSSeg, es
   mov XMSOfs, bx
 END;
 XMSGetAddress := Ptr (XMSSeg,XMSOfs);
END;

(*
  Zwraca wersje sterownika pamieci XMS
*)
FUNCTION XMSVersion : Word;
BEGIN
 ASM
   mov ax, $0000
   call [XMSAddress]
   mov @result, ax
 END;
END;

(*
  Sprawdza czy mozna uzywac pamiec XMS
*)
FUNCTION XMSSetup : Boolean;
BEGIN
 XMSSetup := TRUE;
 IF (XMSFind)
   THEN XMSAddress := XMSGetAddress
   ELSE XMSSetup := FALSE;
END;

(*
  Zwraca ilosc wolnej pamieci XMS w KB
*)
FUNCTION XMSMemAvail : Word;
BEGIN
 ASM
   mov ax, $0800
   call [XMSAddress]
   mov @result, dx
 END;
END;

(*
  Zwraca wielkosc najwiekszego wolnego bloku w KB
*)
FUNCTION XMSMaxAvail : Word;
BEGIN
 ASM
   mov ax, $0800
   call [XMSAddress]
   mov @result, ax
 END;
END;

FUNCTION XMSGetMem (Var Handler : Word ; Size : Word) : Boolean;
BEGIN
 ASM
   mov @result, TRUE
   mov ax, $0900
   mov dx, Size
   call [XMSAddress]
   les di, Handler
   mov [es:di], dx
   cmp ax, 1
   je @end
   mov XMSError, bl
   mov @result, FALSE
   @end:
 END;
END;

FUNCTION XMSReGetMem (Handler : Word ; Size : Word) : Boolean;
BEGIN
 ASM
   mov @result, TRUE
   mov ax, $0f00
   mov bx, Size
   mov dx, Handler
   call [XMSAddress]
   cmp al, 1
   je @end
   mov XMSError, bl
   mov @result, FALSE
   @end:
 END;
END;

FUNCTION XMSFreeMem (Handler : Word) : Boolean;
BEGIN
 ASM
   mov @result, TRUE
   mov ax, $0a00
   mov dx, Handler
   call [XMSAddress]
   cmp ax, 1
   je @end
   mov XMSError, bl
   mov @result, FALSE
   @end:
 END;
END;

FUNCTION XMSMoveMem (SrcHandler : Word ; SrcOffset : Pointer ;
                     DstHandler : Word ; DstOffset : Pointer ; Size : Longint) : Boolean;
BEGIN
 WITH XMSBlockInfo DO
 BEGIN
   Length := Size;
   SourceHandler := SrcHandler;
   SourceOffset := SrcOffset;
   DestinationHandler := DstHandler;
   DestinationOffset := DstOffset;
 END;
 ASM
   mov @result, TRUE
   mov ax, seg XMSBlockInfo
   mov ds, ax
   mov si, offset XMSBlockInfo
   mov ax, $0b00
   call [XMSAddress]
   cmp ax, 1
   je @end
   mov XMSError, bl
   mov @result, FALSE
   @end:
 END;
END;

FUNCTION XMSGetError (Error : Byte) : String;
VAR
 ErrorInfo : String;
BEGIN
 CASE Error OF
  $80 : ErrorInfo := '';
 END;
END;

BEGIN

END.
