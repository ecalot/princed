UNIT GRAPH13H;

{
############################################
Biblioteka do trybu 13h
(rozdzielczosc : 320 x 200 ; max. 256 ró¿nych kolorów)

Autor : Piotr Kochanek
WWW   : www.pop2.princed.com.ar

Obsluguje :
* Bufor Wirtualny
* Obrazki (mozliwosc ladowania z BMP)
* Palete kolorow
* Czcionke wbudowana w program
* Czcionke wczytywana z pliku
* Efekty graficzne (wygaszanie ekranu, rozjasnianie ekranu ird.)

Wiekszosc procedur jest napisana obiektowo
############################################
}

INTERFACE
 USES
   CRT;
 TYPE
   { // Bufor }
   TBufor
     = ARRAY [0..63999] OF byte;
   { // Obrazek }
   TImage
     = OBJECT
         Use       : Boolean; { Czy obrazek jest uzywany }
         Width     : Word;    { Szerokosc obrazka }
         Height    : Word;    { Wysokosc obrazka }
         Size      : Word;    { Rozmiar obrazka }
         Handler   : ^TBufor;     { Wskaznik do obrazka w pamieci }
         { // Tworzy nowy obrazek }
         PROCEDURE NewImage (x,y : word);
         { // Kladzie piksel na obrazek }
         PROCEDURE PutPix (x,y : integer ; c : byte);
         { // Pobiera pixel z obrazka }
         FUNCTION  GetPix (x,y : integer ; c : byte) : byte;
         { // Kopiuje inny obrazek }
         PROCEDURE CopyImage (Image : TImage);
         { // Rysuje obrazek na buforze }
         PROCEDURE XY  ( x,y : integer );
         { // Rysunek przezroczysty }
         PROCEDURE XYV (X,Y : integer);
         { // Rysuje obrazek na buforze ( kolor przezroczysty ) }
         PROCEDURE XYT ( x,y : integer; TransparentColor : byte);
         { // NIEUZYWANE }
         FUNCTION  LoadAFG     (sciezka,nazwa : string) : Boolean;
         { // Laduje plik BMP (mozna takze automatycznie ustawic palete tego pliku) }
{!!!}{           FUNCTION  OldLoadBMP (sciezka : string) : Boolean;}
         FUNCTION  LoadBMP (FileName : string ; LoadPalette : Boolean) : Boolean;
         { // Usuwa obrazek z pamieci }
         PROCEDURE Unload;
       END;
   { // Czcionka }
   TFontData
     = ARRAY [0..255,0..15] OF byte;
   TFont
     = OBJECT
         Use  : Boolean;
         Font : ^TFontData;
         { // Laduje czcionke z pliku }
         FUNCTION  Load (NazwaPliku : string) : boolean;
         { // Usuwa czionke z pamieci }
         PROCEDURE Unload;
         { // Kladzie pojedynczy znak }
         PROCEDURE Ch (X,Y : Word ; Znak : Char ; Color, Shadow : Byte);

         PROCEDURE ChA (X,Y : Word ; Znak : Char ; Color, Shadow : Byte);
         { // ??? }
         PROCEDURE ColorChar (X,Y : Word ; Znak : Char ; Color,Shadow : Byte);
         { // Podaje dlugosc danego znaku }
         FUNCTION  Weidth (Znak : Char) : Byte;
         { // Podaje dlugosc danego tekstu }
         FUNCTION  StrLength (Tekst : String) : Word;
         { // Wyswietla napis na buforze }
         PROCEDURE XY (X,Y : Word ; Tekst : String ; C,S : Byte);
         { // Wczytuje tekst od u¿ytkownika }
         FUNCTION  Read (PolX,PolY : Integer ; Var Tekst : String ; Max,C,B : Byte) : String;
       END;
   { // Naglowek BMP }
   THandleBMP
     = RECORD
         FType           : Word;
         FSize           : Longint;
         FReserved1      : Word;
         FReserved2      : Word;
         FFirstPixel     : Longint;
         FInfoSize       : Longint;
         FWidth          : Longint;
         FHeight         : Longint;
         FPlanes         : Word;
         FBitPerPixel    : Word;
         FCompression    : Longint;
         FImageSize      : Longint;
         FDPIWidth       : Longint;
         FDPIHeight      : Longint;
         FColorUsed      : Longint;
         FImportantColor : Longint;
       END;
   { // Palety }
   TPalette
     = ARRAY [0..255] OF
       RECORD
         R : Byte;
         G : Byte;
         B : Byte;
       END;
   TPaletteBMP
     = ARRAY [0..255] OF
       RECORD
         B : Byte;
         G : Byte;
         R : Byte;
         N : Byte;
       END;
CONST
  GetMaxX       = 319;
  GetMaxY       = 199;
VAR
  Screen         : Pointer;
  Bufor          : ^TBufor;
  DefaultPalette : TPalette;
  Plik           : File;
  X              : Word;
  Y              : Word;
  I              : Integer;

 { ### Glowne funkcje do wlanczania / wylanczania trybu 13h ### }

 { // Wlacz tryb 13h }
 PROCEDURE Load13h;
 { // Wylacz tryb 13h }
 PROCEDURE Unload13h;
 { // Aktualny tryb }
 FUNCTION  GetVideoMode : Byte;
 { // Ustawia wybrany tryb }
 PROCEDURE SetVideoMode( Mode : Byte );
 { // Ustawia Bufor }
 PROCEDURE SetBufor (Adres : Pointer);
 { // Funkcja wyprowadza adres aktualnie uzywanego buforu }
 FUNCTION  GetBufor : Pointer;

 { ### Obsluga palety w 13h ### }

 { // Zapisz aktualna palete }
 PROCEDURE SavePalette (var Palette : TPalette);
 { // Zaladuj palete }
 PROCEDURE LoadPalette (Palette : TPalette);
 { // ??? }
 FUNCTION  GetAverageColor(nr : byte) : byte;
 { // Ustal kolor palety}
 PROCEDURE SetColor(numer, r, g, b : byte);
 { // Pobierz kolor palety }
 PROCEDURE GetColors(numer : byte; var r, g, b : byte);
 { // ??? }
 FUNCTION  GetColor(numer : byte ; color : char) : byte;
 { // Znajduje kolor w aktualnej palecie najbardziej pasujacy do szukanego }
 FUNCTION  Color (r,g,b : byte) : byte;

 { ### Funkcje do obslugi pixeli ### }

 { // Rysuje pixel na buforze }
 PROCEDURE PutPixel(x,y : integer ; c : byte);
 { // Rysuje pixel na ekranie }
 PROCEDURE FPutPixel(x,y : integer ; c : byte);
 { // Pobiera pixel z buforu }
 FUNCTION  GetPixel(x,y : integer) : byte;
 { // Pobiera pixel z ekranu }
 FUNCTION  FGetPixel(x,y : integer) : byte;

 { ### Figury geometryczne ### }

 { // Rysuje linie }
 PROCEDURE Line (x1,y1,x2,y2 : word; c : byte);
 { // Rysuje linie pozioma }
 PROCEDURE LineAcross(x,y,leng : integer ; color : byte);
 { // Rysuje linie pionowa }
 PROCEDURE LineDown(x,y,leng : integer ; color : byte);
 { // Rysuje prostokat }
 PROCEDURE Rectangle(x1,y1,x2,y2 : integer ; color : byte);
 { // Rysuje wypelniony prostokat }
 PROCEDURE RectangleFilled(x1,y1,x2,y2 : integer ; color : byte);
 { // Rysuje kolo }
 PROCEDURE Circle(X,Y,R : Longint; color: Byte);

 { ### Obsluga czcionki systemowej ### }

 { ### Dodatkowe procedury do obrazkow ### }

 { // Laduje palete z pliku BMP }
 PROCEDURE LoadBMPPalette (sciezka : string);

 { ### Przenoszenie bufora na ekran ### }

 { // Czeka na powrot pionowy }
 PROCEDURE WaitForStopWriting;
 { // Kopiuje bufor }
 PROCEDURE CopyBufor (BuforIn,BuforOut : Pointer);
 { // Kopiuje bufor na ekran usuwajac 'mruganie ekranu' }
 PROCEDURE ShowBufor (BuforIn : Pointer);
 { // Jezeli trwa powrot pionowy to wyswietla ekran }
 PROCEDURE TryShowBufor (BuforIn : Pointer);

 { ### Efekty ### }

 { // Czysci bufor na wybrany kolor }
 PROCEDURE Clr13h (color : byte);
 { // Ustawia kolor czarnobialy }
 PROCEDURE BlackWhite;
 { // Odwraca kolory }
 PROCEDURE RetreatColors;
 { // Wygasza ekran }
 PROCEDURE WygasEkran (przerwa : byte);
 { // Rozjasnia ekran }
 PROCEDURE RozjasnijEkran (paleta_k : TPalette ; przerwa : Byte);
 { // Rozmazuje ekran }
 PROCEDURE Blur;

 { ### Procedury pomocnicze ### }

 { // Zmienia palete bmp na rgb }
 PROCEDURE PaletteBMPToRGB (PaletteBMP : TPaletteBMP ; Var Palette : TPalette);
 { // Zmienia palete rgb na bmp }
 PROCEDURE PaletteRGBtoBMP (Palette : TPalette ; Var PaletteBMP : TPaletteBMP);

IMPLEMENTATION

PROCEDURE SetBufor (Adres : Pointer);
BEGIN
  Bufor := Adres;
END;

FUNCTION GetBufor : Pointer;
BEGIN
  GetBufor := Bufor;
END;

FUNCTION GetVideoMode : Byte;
VAR
  tempVMode : Byte;
BEGIN
  ASM
    mov ah,$0f
    int $10
    mov tempvmode,al
  END;
  GetVideoMode := tempVMode;
END;

PROCEDURE SetVideoMode( Mode : Byte );
BEGIN
 ASM
   mov ah, Mode;
   int $10
 END;
END;

PROCEDURE Load13h;
BEGIN
 ASM
   mov ax, 13h;
   int 10h;
 END;
 Clr13h (0);
 SavePalette (DefaultPalette);
END;

PROCEDURE Unload13h;
BEGIN
 ASM
   mov ax, 3h;
   int 10h;
 END;
END;

PROCEDURE SavePalette (var Palette : TPalette);
VAR r,g,b : byte;
BEGIN
 FOR x := 0 TO 255 DO
 BEGIN
   GetColors(x,r,g,b);
   palette [x].r := r;
   palette [x].g := g;
   palette [x].b := b;
 END;
END;

PROCEDURE LoadPalette (Palette : TPalette);
BEGIN
 FOR x := 0 TO 255 DO
 BEGIN
   SetColor(x,palette [x].r,palette [x].g,palette [x].b);
 END;
END;

FUNCTION GetAverageColor(nr : byte) : byte;
Var
 r,g,b : byte;
BEGIN
 GetColors(nr,r,g,b);
 GetAverageColor := (r + g + b) div 3;
END;

PROCEDURE SetColor(numer, r, g, b : byte);
BEGIN
 port[$3c8] := numer;
 port[$3c9] := r;
 port[$3c9] := g;
 port[$3c9] := b;
END;

PROCEDURE GetColors(numer : byte; var r, g, b : byte);
BEGIN
 port[$3c7] := numer;
 r := port[$3c9];
 g := port[$3c9];
 b := port[$3c9];
END;

FUNCTION GetColor(numer : byte ; color : char) : byte;
var r,g,b : byte;
BEGIN
  GetColors(numer,r,g,b);
  IF color = 'r' THEN GetColor := r;
  IF color = 'g' THEN GetColor := g;
  IF color = 'b' THEN GetColor := b;
END;

PROCEDURE PutPixel(x,y : integer ; c : byte); Assembler;
ASM
 cmp x, 0
 jl @end
 cmp y, 0
 jl @end
 cmp x, 319
 jg @end
 cmp y, 199
 jg @end
 mov ax, y
 mov di, ax
 shl ax, 8
 shl di, 6
 add di, ax
 add di, x
 les ax, bufor  {???}
 mov al, c
 mov byte ptr es:[di], al
 @end:
END;
{ exit;

 IF (X <= GetMaxX) and (Y <= GetMaxY) and (X >= 0) and (Y  >= 0) THEN
 BUFOR^ [320 * y + x] := c;
END;}

PROCEDURE FPutPixel(x,y : integer ; c : byte); assembler;
ASM
 mov ax, $a000
 mov es, ax

 mov dx, y
 mov di, x
 xchg dh, dl
 add di, dx
 shr dx, 2
 add di, dx

 mov al, c
 mov es:[di], al
END;

FUNCTION GetPixel(x,y : integer) : byte;
BEGIN
 GetPixel := BUFOR^[320 * y + x];
END;

FUNCTION FGetPixel(x,y : integer) : byte; assembler;
ASM
 mov ax, $a000
 mov es, ax

 mov dx, y
 mov di, x
 xchg dh, dl
 add di, dx
 shr dx, 2
 add di, dx

 mov al, es:[di]
END;

PROCEDURE Line (x1,y1,x2,y2 : word; c : byte);

FUNCTION sgn(a:real):integer;
BEGIN
  if a > 0 then sgn := +1;
  if a < 0 then sgn := -1;
  if a = 0 then sgn := 0;
END;

VAR
  s : real;
  u,i,v,d1x,d1y,d2x,d2y,m,n : integer;
BEGIN
 u := x2 - x1;
 v := y2 - y1;
 d1x := SGN (u);
 d1y := SGN (v);
 d2x := SGN (u);
 d2y := 0;
 m := ABS (u);
 n := ABS (v);
 IF M <= N then
 BEGIN
   d2x := 0 ;
   d2y := SGN (v);
   m := ABS (v);
   n := ABS (u);
 END;
 s := INT (m / 2);
 FOR i := 0 TO m DO
 BEGIN
   PutPixel (x1,y1,c);
   s := s + n;
   IF s >= m THEN
   BEGIN
     s := s - m;
     x1 := x1 + d1x;
     y1 := y1 + d1y;
   END
   ELSE
   BEGIN
     x1 := x1 + d2x;
     y1 := y1 + d2y;
   END;
 END;
END;

PROCEDURE lineacross(x,y,leng : integer ; color : byte);
 BEGIN
  FOR x := x to x + leng - 1 do PutPixel(x,y,color);
 END;

PROCEDURE linedown(x,y,leng : integer ; color : byte);
 BEGIN
  FOR y := y to y + leng - 1 do PutPixel(x,y,color);
 END;

PROCEDURE Rectangle(x1,y1,x2,y2 : integer ; color : byte);
 BEGIN
  FOR x := x1 to x2 do
   BEGIN
    PutPixel(x,y1,color);
    PutPixel(x,y2,color);
   END;
  FOR y := y1 to y2 do
   BEGIN
    PutPixel(x1,y,color);
    PutPixel(x2,y,color);
   END;
 END;

PROCEDURE RectangleFilled(x1,y1,x2,y2 : integer ; color : byte);
BEGIN
 FOR x := x1 to x2 do
  FOR y := y1 to y2 do
   PutPixel(x,y,color);
END;

PROCEDURE Circle(X,Y,R : longint; color: byte);
Var Xl,Yl : LongInt;
BEGIN
  IF R=0 THEN
  BEGIN
    PutPixel(X,Y,color);
    Exit;
  END;
  Xl := 0;
  Yl := R;
  R := R*R+1;
  Repeat
    PutPixel(X+Xl,Y+Yl,color);
    PutPixel(X-Xl,Y+Yl,color);
    PutPixel(X+Xl,Y-Yl,color);
    PutPixel(X-Xl,Y-Yl,color);
    IF Xl*Xl+Yl*Yl >= R THEN Dec(Yl)
    Else Inc(Xl);
  Until Yl = 0;
  PutPixel(X+Xl,Y+Yl,color);
  PutPixel(X-Xl,Y+Yl,color);
  PutPixel(X+Xl,Y-Yl,color);
  PutPixel(X-Xl,Y-Yl,color);
END;

{PROCEDURE kopiuj(BUFOR1,BUFOR2:pointer);assembler;
asm
push ds
mov cx, 16000
lds si, BUFOR1
les di, BUFOR2
cld
db $F3, $66, $A5
pop ds
end;}

PROCEDURE WaitForStopWriting; Assembler;
ASM
  mov dx, 3dah
  @wait:
  in al, dx
  test al, 00001000b
  jnz @wait
  @end:
  in al, dx
  test al, 00001000b
  jz @end
END;


PROCEDURE CopyBufor (BuforIn,BuforOut : Pointer);Assembler;
{var
 p : pointer;
{ p := mem[$a000:0];}
{ IF (not UseBUFOR) THEN exit;}

{ kopiuj (BUFOR,mem [$a000:0]);
 Move(BUFOR^,mem[$a000:0],64000);}
ASM
 push ds
 lds si,buforin
 les di,buforout
 cld
 mov cx,32000
 rep movsw
 pop ds
END;

PROCEDURE ShowBufor (BuforIn : Pointer); ASSEMBLER;
ASM
 mov dx, 3dah
 @wait:
 in al, dx
 test al, 00001000b
 jnz @wait
 @end:
 in al, dx
 test al, 00001000b
 jz @end

 push ds
 lds si,buforin
 mov ax, $A000
 mov es, ax
 mov di, 0
 cld
 mov cx,32000
 rep movsw
 pop ds
END;

PROCEDURE TryShowBufor (BuforIn : Pointer); ASSEMBLER;
ASM
 mov dx, 3dah
{ @wait:}
 in al, dx
 test al, 00001000b
 {jnz}jz @exit
{ @end:
 in al, dx
 test al, 00001000b
 jz @exit}

 push ds
 lds si,buforin
 mov ax, $A000
 mov es, ax
 mov di, 0
 cld
 mov cx,32000
 rep movsw
 pop ds

 @exit:
END;

FUNCTION TFont.Load (NazwaPliku : string) : boolean;
Begin
 Load := False;
 If (Use) then Unload;

 Assign (Plik,NazwaPliku);
 {$I-}
 Reset (Plik,1);
 {$I+}
 If (IOResult <> 0) then Exit;
 New (Font);
 BlockRead (Plik,Font^,SizeOf (Font^));
 Close (Plik);

 Use := True;
 Load := True;
End;

PROCEDURE TFont.Unload;
Begin
 If (not Use) then Exit;
 Dispose (Font);
 Use := False;
End;

PROCEDURE TFont.Ch (X,Y : Word ; Znak : Char ; Color, Shadow : Byte);
var
 MaskaBitowa  : Byte;
 LicznikY     : Byte;
 LicznikBitow : Byte;
Begin
 If (not Use) then exit;
 For LicznikY := 0 to 15 do
 Begin
   MaskaBitowa := $80; { 80h = 10000000b }
   For LicznikBitow := 7 downto 0 do
   Begin
     If ((Font^ [Ord (Znak),LicznikY] and MaskaBitowa) = MaskaBitowa)
      then
      Begin
        If (Color <> Shadow) then
          PutPixel (X + LicznikBitow + 1,Y + LicznikY + 1,Shadow);
        PutPixel (X + LicznikBitow,Y + LicznikY,Color);
      End;
     MaskaBitowa := MaskaBitowa shr 1;
   End;
 End;
End;

PROCEDURE TFont.ChA (X,Y : Word ; Znak : Char ; Color, Shadow : Byte); Assembler;
ASM
  {czy czcionka jest zaladowana}
  mov ax, use
  cmp ax, 0
  jz @end
  {glowna petla - od 15 do 0}
  mov cx, 16
  @petla:
  push cx
  mov ax, 10000000b
  mov cx, 8
  @petla2:
  loop @petla2
  pop cx
  loop @petla
  {koniec procedury}
  @end:
END;

PROCEDURE TFont.ColorChar (X,Y : Word ; Znak : Char ; Color,Shadow : Byte);
var
 MaskaBitowa  : Byte;
 LicznikY     : Byte;
 LicznikBitow : Byte;
 ActualColor  : Byte;
Begin
 If (not Use) then exit;
 ActualColor := Color;
 For LicznikY := 0 to 15 do
 Begin
   MaskaBitowa := $80; { 80h = 10000000b }
   For LicznikBitow := 7 downto 0 do
   Begin
      If ((Font^ [Ord (Znak),LicznikY] and MaskaBitowa) = MaskaBitowa)
      then
      Begin
        If (Color <> Shadow) then
          PutPixel (X + LicznikBitow + 1,Y + LicznikY + 1,Shadow);
        PutPixel (X + LicznikBitow,Y + LicznikY,ActualColor);
      End;
     MaskaBitowa := MaskaBitowa shr 1;
   End;
   Dec (ActualColor);
 End;
End;

FUNCTION TFont.Weidth (Znak : Char) : Byte;
var
 X,Y,Leng,MaskaBitowa : Byte;
Begin
 Leng := 0;
 For Y := 0 to 15 do
 Begin
   MaskaBitowa := $01;
   For X := 0 to 7 do
   Begin
     If ((Font^ [Ord (Znak),Y] and MaskaBitowa) = MaskaBitowa) then
     Begin
       If (X > Leng) then Leng := X;
     End;
     MaskaBitowa := MaskaBitowa shl 1;
   End;
 End;
 Weidth := Leng + 1;
End;

FUNCTION TFont.StrLength (Tekst : String) : Word;
var
 Licznik : Byte;
 Dlugosc : Word;
Begin
 Dlugosc := 0;
 For licznik := 1 to length (Tekst) do
 Begin
   Inc (Dlugosc,Weidth (Tekst [licznik]) + 1);
   If (Tekst [Licznik] = #32) then Inc (Dlugosc,2);
 End;
 StrLength := Dlugosc;
End;

PROCEDURE TFont.XY (X,Y : Word ; Tekst : String ; C,S : Byte);
var
 Licznik : Byte;
Begin
 For Licznik := 1 to Length (Tekst) do
 Begin
   Ch (X,Y,Tekst [Licznik],C,S);
   If (Tekst [Licznik] = #32) then Inc (X,2);
   Inc (X,Weidth (Tekst [Licznik]) + 1);
 End;
End;

{!!! - procedura zostaje ze wzgledu na algorytm dzialania}
FUNCTION TFont.Read (PolX,PolY : Integer ; Var Tekst : String ; Max,C,B : Byte) : String;
VAR
 Kl       : Char;
 Ilosc    : Byte;
 OldBufor : Pointer;
BEGIN
 Ilosc := Length (Tekst);
 OldBufor := GetBufor;
 SetBufor (Screen);
 RectangleFilled (PolX,PolY + 1,PolX + 2 + (Max * 8),PolY + 17,B);
 REPEAT
   RectangleFilled (PolX + 2 + StrLength (Tekst),PolY + 1,PolX + 2 + (Max * 8),PolY + 17,B);
   XY (PolX + 2,PolY,Tekst,C,C);
   If (Keypressed) then Kl := Readkey else Kl := #1;
   If (Kl = #0) THEN Readkey
   Else If (Kl = #8) THEN
   BEGIN
     IF (Ilosc > 0) THEN
     BEGIN
       delete(tekst,length(tekst),1);
       dec(ilosc);
     END;
   END
   ELSE
   BEGIN
     IF (ord(kl) < 32) or (ord(kl) > 255) THEN continue;
     IF ilosc >= max THEN continue;
     inc(ilosc);
     tekst := tekst + kl;
   END;
 UNTIL (Kl = #13);
 SetBufor (OldBufor);
END;

        {  EFFECTS  }

PROCEDURE Clr13h (color : byte); Assembler;
ASM
  les di, Bufor;
  mov ah, color
  mov al, ah
  mov cx, 32000
  cld
  rep stosw
END;

PROCEDURE BlackWhite;
BEGIN
 FOR x := 0 to 255 do
 BEGIN
   y := (GetColor(x,'r') + GetColor(x,'g') + GetColor(x,'b') div 3);
   SetColor(x,y,y,y);
 END;
END;

PROCEDURE RetreatColors;
Var
 r,g,b : byte;
BEGIN
 FOR x := 0 to 255 do
 BEGIN
   GetColors(x,r,g,b);
   r := not r;
   g := not g;
   b := not b;
   SetColor(x,r,g,b);
 END;
END;

     { ADDITIONAL PROCEDURES }

{FUNCTION _Color (R,G,B : Byte) : Byte;
Var
  licznik           : byte;
  roznica           : word;
  r2,g2,b2          : byte;
  najlepsza_roznica : word;
  najlepszy_kolor   : byte;
BEGIN
 ASM
   mov najlepsza_roznica, 1000
   mov cx, 256
   @petla:
     push cx
     dec cx

     mov roznica, 0

     mov dx, 3c7h
     mov ax, cx
     out dx, ax
     mov dx, 3c9h
     in al, dx
     mov r2, al
     in al, dx
     mov g2, al
     in al, dx
     mov b2, al

     mov al, r
     cmp al, r2
     jb @nextr
       mov ax, roznica
       xor bx, bx
       mov bh, r
       add ax, bx
       xor bx, bx
       mov bh, r2
       sub ax, bx
       add roznica, ax
       jmp @nextr2
     @nextr:
       mov ax, roznica
       xor bx, bx
       mov bh, r2
       add ax, bx
       xor bx, bx
       mov bh, r
       sub ax, bx
       add roznica, ax
     @nextr2:

     mov al, g
     cmp al, g2
     jb @nextg
       mov ax, roznica
       xor bx, bx
       mov bh, g
       add ax, bx
       xor bx, bx
       mov bh, g2
       sub ax, bx
       add roznica, ax
       jmp @nextg2
     @nextg:
       mov ax, roznica
       xor bx, bx
       mov bh, g2
       add ax, bx
       xor bx, bx
       mov bh, g
       sub ax, bx
       add roznica, ax
     @nextg2:

     mov al, b
     cmp al, b2
     jb @nextb
       mov ax, roznica
       xor bx, bx
       mov bh, b
       add ax, bx
       xor bx, bx
       mov bh, b2
       sub ax, bx
       add roznica, ax
       jmp @nextb2
     @nextb:
       mov ax, roznica
       xor bx, bx
       mov bh, b2
       add ax, bx
       xor bx, bx
       mov bh, b
       sub ax, bx
       add roznica, ax
     @nextb2:

     mov ax, roznica
     cmp ax, najlepsza_roznica
     jae @next
       mov ax, roznica
       mov najlepsza_roznica, ax
       mov ax, cx
       mov najlepszy_kolor, ah
     @next:

     pop cx
   dec cx
   cmp cx, 0
   jne @petla
 END;
 _Color := najlepszy_kolor;
END;}

FUNCTION Color (r,g,b : byte) : byte;
Var
  licznik           : byte;
  roznica           : word;
  r2,g2,b2          : byte;
  najlepsza_roznica : word;
  najlepszy_kolor   : byte;
BEGIN
 najlepsza_roznica := 1000;
 FOR licznik := 0 to 255 do
 BEGIN
  roznica := 0;
  GetColors (licznik,r2,g2,b2);
  IF (r >= r2)
   THEN roznica := roznica + r - r2
   else roznica := roznica + r2 - r;
  IF (g >= g2)
   THEN roznica := roznica + g - g2
   else roznica := roznica + g2 - g;
  IF (b >= b2)
   THEN roznica := roznica + b - b2
   else roznica := roznica + b2 - b;
  IF roznica < najlepsza_roznica THEN
   BEGIN
    najlepsza_roznica := roznica;
    najlepszy_kolor   := licznik;
   END;
 END;
 Color := najlepszy_kolor;
END;

(* EFEKTY GRAFICZNE *)

PROCEDURE WygasEkran (przerwa : byte);
var licznik1,licznik2 : byte;
    wzorzec : record
               R,G,B :Byte;
              END;
BEGIN
 FOR licznik2 := 0 to 63 do
 BEGIN
  FOR licznik1 := 0 to 255 do
  BEGIN
   GetColors (licznik1,wzorzec.r,wzorzec.g,wzorzec.b);
   IF (wzorzec.r > 0) THEN dec (wzorzec.r);
   IF (wzorzec.g > 0) THEN dec (wzorzec.g);
   IF (wzorzec.b > 0) THEN dec (wzorzec.b);
   SetColor (licznik1,wzorzec.r,wzorzec.g,wzorzec.b);
  END;
  IF (przerwa <> 0) THEN Delay (przerwa);
 END;
END;

PROCEDURE RozjasnijEkran (paleta_k : TPalette ; przerwa : Byte);
VAR
 licznik1, licznik2 : Byte;
 wzorzec : record
            R,G,B : Byte;
           END;
BEGIN
 FOR licznik2 := 0 to 63 do
 BEGIN
  FOR licznik1 := 0 to 255 do
  BEGIN
   GetColors (licznik1,wzorzec.r,wzorzec.g,wzorzec.b);
   IF (wzorzec.r < paleta_k [licznik1].r) THEN Inc (wzorzec.r);
   IF (wzorzec.g < paleta_k [licznik1].g) THEN Inc (wzorzec.g);
   IF (wzorzec.b < paleta_k [licznik1].b) THEN Inc (wzorzec.b);
   SetColor (licznik1,wzorzec.r,wzorzec.g,wzorzec.b);
  END;
  IF (przerwa <> 0) THEN delay (przerwa);
 END;
END;

{PROCEDURE Blur;
Begin
  For x := 0 to 319 do
  For y := 0 to 199 do
    PutPixel (x,y,GetPixel (x - 1 + random (3),y - 1 + random (3)));
End;}

PROCEDURE Blur;
VAR
  c : integer;
BEGIN
 for x:= 0 to 319 do
 for y:= 0 to 198 do
 begin
   c:=(BUFOR^[320*y+x]+BUFOR^[320*(y+1)+x+1]+BUFOR^[320*y+1+x]+BUFOR^[320*(y+1)+x]) div 4;
   BUFOR^[320*y+x]:=c;
 end;
END;

(* OBRAZY *)

PROCEDURE TImage.NewImage (x,y : word);
Begin
 If (Use) then Unload;
 Size := x * y;
 Width := x;
 Height := y;
 GetMem (Handler,Size);
 For I := 0 to Size - 1 do Handler^ [I] := 0;
End;

PROCEDURE TImage.PutPix (x,y : integer ; c : byte);
Begin
 If (Use) and (x > 0) and (y > 0) and
    (x < Width) and (y < Height)
  then Handler^ [((y - 1) * Width) + (x - 1)] := C;
End;

FUNCTION TImage.GetPix (x,y : integer ; c : byte) : byte;
Begin
 If (Use) and (x > 0) and (y > 0) and
    (x < Width) and (y < Height)
  then GetPix := Handler^ [((y - 1) * Width) + (x - 1)]
  else GetPix := 0;
End;

PROCEDURE TImage.CopyImage (Image : TImage);
Var
 W : Word;
Begin
 If (not Image.Use) then Exit;
 If (Use) then Unload;
 GetMem (Handler,Image.Size);
 Size := Image.Size;
 Width := Image.Width;
 Height := Image.Height;
 For w := 0 to Size - 1 do Handler^ [w] := Image.Handler^ [w];
 Use := True;
End;

FUNCTION TImage.LoadAFG (sciezka,nazwa : string) : Boolean;
VAR
 plik : file;
 nagl : record
          typ : string [3];
          ver : string [3];
          images : word;
        END;
 fnagl : record
            adres : longint;
            nazwa : string [10];
            rozmiar : word;
            szerokosc : word;
            wysokosc : word;
          END;
BEGIN
 LoadAFG := false;
 If (Use) then Unload;
 assign (plik,sciezka);
 {$I-}
 reset  (plik, 1);
 {$I+}
 IF (IOResult <> 0) THEN exit;

 BlockRead (plik, nagl, 10);
 IF (nagl.typ <> 'AFG') or (nagl.ver <> '1.0') THEN
 BEGIN
   close (plik);
   exit;
 END;

 Repeat
  BlockRead (plik,fnagl,sizeof (fnagl));
 Until (fnagl.nazwa = nazwa) or (FilePos (plik) - 1 >= (nagl.images * 21) + 10);

 IF (FilePos (plik) - 1 >= (nagl.images * 21) + 10) THEN
 BEGIN
   close (plik);
   exit;
 END;

 Getmem (Handler, fnagl.rozmiar);
 Seek (plik,fnagl.adres);

 writeln (fnagl.rozmiar);
 writeln (FilePos (plik));
 writeln (FileSize (plik));

 Size := Fnagl.Rozmiar;
 Width := Fnagl.Szerokosc;
 Height := Fnagl.Wysokosc;

 BlockRead (plik,Handler^ [0],fnagl.rozmiar);

 close  (plik);
 Use := True;
 LoadAFG := true;
END;

{FUNCTION TImage.OldLoadBMP (sciezka : string) : Boolean;
VAR
 plik : file;
 naglowek : naglowek_BMP;
 licznik, wynik : word;
 licznik_odczytu : word;
BEGIN
 OldLoadBMP := False;
 If (Use) then Unload;
 assign (plik, sciezka);
 {$I-}
{ reset  (plik, 1);
 {$I+}
{ If (IOResult <> 0) then exit;
 licznik := sizeof (naglowek_BMP);
 blockread (plik, naglowek, licznik, wynik);
 seek   (plik, 1078);
 Width := naglowek.szerokosc_obrazu;
 Height  := naglowek.wysokosc_obrazu;
 Size   := naglowek.szerokosc_obrazu * naglowek.wysokosc_obrazu;
 IF (maxavail < Size) THEN
  BEGIN
   close (plik);
   exit;
  END;
 getmem (Handler, Size);
 licznik := naglowek.szerokosc_obrazu;

 FOR licznik_odczytu := naglowek.wysokosc_obrazu - 1 downto 0 do
 BEGIN
  blockread (plik, Handler^ [(licznik_odczytu) * (Width)], licznik, wynik);
  IF Width MOD 4 <> 0 THEN Seek(plik,FilePos (plik) + (4 - Width MOD 4));
 END;

 close (plik);
 Use := True;
 OldLoadBMP := True;
END;}

PROCEDURE PaletteBMPToRGB (PaletteBMP : TPaletteBMP ; Var Palette : TPalette);
BEGIN
   FOR I := 0 to 255 do
   BEGIN
     Palette [I]. R := PaletteBMP [I]. R shr 2;
     Palette [I]. G := PaletteBMP [I]. G shr 2;
     Palette [I]. B := PaletteBMP [I]. B shr 2;
   END;
END;

PROCEDURE PaletteRGBtoBMP (Palette : TPalette ; Var PaletteBMP : TPaletteBMP);
BEGIN
   FOR I := 0 to 255 do
   BEGIN
     PaletteBMP [I]. R := Palette [I]. R shl 2;
     PaletteBMP [I]. G := Palette [I]. G shl 2;
     PaletteBMP [I]. B := Palette [I]. B shl 2;
   END;
END;

FUNCTION LoadHandleBMP (FileName : String; Var HandleBMP : THandleBMP) : Boolean;
BEGIN
  LoadHandleBMP := FALSE;
  Assign (Plik, FileName);
  {$I-}
  Reset (Plik, 1);
  {$I+}
  If (IOResult <> 0) then EXIT;
  BlockRead (Plik, HandleBMP, SizeOf (HandleBMP));
  Close (Plik);

{  UNLOAD13H;
    Writeln ('HandleBMP. FWidth = ',HandleBMP. FWidth);
    Writeln ('HandleBMP. FHeight = ',HandleBMP. FHeight);
    Writeln ('HandleBMP. FBitPerPixel = ',HandleBMP. FBitPerPixel);
    Writeln ('HandleBMP. FCompression = ',HandleBMP. FCompression);
    ReadLn;
  LOAD13H;}

  If (HandleBMP. FWidth > 320) then EXIT;
  If (HandleBMP. FHeight > 200) then EXIT;
  If (HandleBMP. FBitPerPixel <> 8) and
     (HandleBMP. FBitPerPixel <> 24) then EXIT;
  If (HandleBMP. FCompression <> 0) then EXIT;
  LoadHandleBMP := TRUE;
END;

FUNCTION TImage.LoadBMP (FileName : string ; LoadPalette : Boolean) : Boolean;
VAR
 HandleBMP : THandleBMP;
 Palette : TPalette;
 PaletteBMP : TPaletteBMP;
 Temp : Array [0 .. (320 * 3) - 1] of Byte;
 I2 : Integer;
BEGIN
 LoadBMP := FALSE;
 If (NOT LoadHandleBMP (FileName,HandleBMP)) then EXIT;
 If (Use) then Unload;
 Assign (Plik, FileName);
 {$I-}
 Reset  (Plik, 1);
 {$I+}
 If (IOResult <> 0) then EXIT;
 Width := HandleBMP. FWidth;
 Height  := HandleBMP. FHeight;
 Size   := Width * Height;
 IF (MaxAvail < Size) THEN
 BEGIN
   Close (Plik);
   EXIT;
 END;
 CASE HandleBMP. FBitPerPixel OF
 8  : BEGIN
        GetMem (Handler,Size);
        IF (LoadPalette) THEN
        BEGIN
          Seek (Plik, 54);
          BlockRead (Plik,PaletteBMP,1024);
          PaletteBMPToRGB (PaletteBMP,Palette);
        END;
        Seek (Plik, 1078);
        FOR I := Height - 1 downto 0 do
        BEGIN
          BlockRead (Plik, Handler^ [I * Width], Width);
          IF (Width MOD 4) <> 0 THEN Seek (Plik,FilePos (Plik) + (4 - Width MOD 4));
        END;
      END;
   ELSE
   BEGIN
     Close (plik);
     EXIT;
   END;
 END;
 Close (plik);
 Use := True;
 LoadBMP := True;
END;

PROCEDURE LoadBMPPalette (sciezka : string);
VAR
 plik : file;
 paleta_BMP : array [0..255] of record
                                 b : byte;
                                 g : byte;
                                 r : byte;
                                 z : byte;
                                END;
 licznik,wynik : word;
BEGIN
 assign (plik, sciezka);
 reset  (plik, 1);
 seek   (plik, sizeof (THandleBMP));
 licznik := sizeof (paleta_BMP);
 blockread (plik, paleta_BMP, licznik, wynik);
 port [$3c8] := 0;
 FOR licznik := 0 to 255 do
 BEGIN
  port[$3c9] := paleta_BMP [licznik].r shr 2;
  port[$3c9] := paleta_BMP [licznik].g shr 2;
  port[$3c9] := paleta_BMP [licznik].b shr 2;
 END;
 close (plik);
END;

PROCEDURE TImage.XY (X,Y : integer);
VAR
 LicznikX,LicznikY : word;
 { dodatkowe zmienne do asma }
 PrzesBuforu,PrzesEkranu : word;
BEGIN
{ ASM

   mov ax, use
   cmp ax, 0
   jne @end

   mov ax, y
   mov przesekranu, 320
   mul przesekranu
   mov przesekranu, ax
   mov ax, x
   add przesekranu, ax

   mov przesbuforu, 0

   mov cx, height
   @petla:
     push cx
     push ds

     mov ax, height
     sub ax, cx

     les di, bufor
     add di, przesbuforu

     mov ax, seg handler
     mov ds, ax
     mov si, offset handler
     add si, przesekranu

     mov cx, width

     cld
     rep movsb

     add przesekranu, 320
     sub przesekranu, width

     pop cx
     pop ds
   loop @petla

   @end:
 END;
 EXIT;}

 If (not Use) then Exit;
 FOR LicznikY := 0 to Height - 1 do
 FOR LicznikX := 0 to Width - 1 do
 BEGIN
   PutPixel (X + LicznikX,Y + LicznikY,Handler^ [(LicznikY * (Width)) + LicznikX]);
 END;
END;

PROCEDURE TImage.XYV (X,Y : integer);
VAR
 LicznikX,LicznikY : word;
 { dodatkowe zmienne do asma }
 PrzesBuforu,PrzesEkranu : word;
 Visible : Boolean;
BEGIN
 If (not Use) then Exit;
 FOR LicznikY := 0 to Height - 1 do
 FOR LicznikX := 0 to Width - 1 do
 BEGIN
   If (Visible) then
   Begin
     PutPixel (X + LicznikX,Y + LicznikY,Handler^ [(LicznikY * (Width)) + LicznikX]);
     Visible := False
   End
   Else
     Visible := True;
   If (Width mod 2 = 0) and (LicznikX = Width - 1) then
   If (Visible) then Visible := False else Visible := True;
 END;
END;

PROCEDURE TImage.XYT (x,y : integer ; TransparentColor : byte);
 VAR
  LicznikX,LicznikY : longint;
BEGIN
 If (not Use) then Exit;
 FOR LicznikY := 0 to Height - 1 do
 FOR LicznikX := 0 to Width - 1 do
 BEGIN
   If (Handler^ [(LicznikY * (Width)) + LicznikX] <> TransparentColor) then
     PutPixel (X + LicznikX,Y + LicznikY,Handler^ [(LicznikY * (Width)) + LicznikX]);
 END;
END;

PROCEDURE TImage.Unload;
BEGIN
 If (not Use) then Exit;
 FreeMem (Handler,Size);
 Width := 0;
 Height  := 0;
 Size   := 0;
 Use := False;
END;

{
###############################
Koniec modulu M_13H.
Masz jakies uwagi lub pytania ?
Pisz : pt_k@wp.pl
###############################
}

BEGIN
 Screen := Ptr ($A000,0);
 SetBufor (Screen);
END.