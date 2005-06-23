(*
  RECORDer source file

  Author   : Piotr Kochanek
  WWW site : www.pop2.princed.com.ar

  This software can capture an animation from 13h games (320 * 200 and 256 colors).
  After finish playing the game, it create as many bmp files as there was recorded frames.
  Then, with for example Animation Shop (Paint Shop Pro), You can link this files into one file (gif, avi, etc).
*)

PROGRAM SCREEN_RECORDER;

USES
 CRT, {!!!}
 DOS,
 HEAPMAN,
 GRAPH13H,
 M_XMS;
VAR
 OPT :
   RECORD
     ProgramPath : String;
     ProgramParameters : String;
     StartAtBegin : Boolean;
     {SkipSecFrame : Boolean;}

     UseMouse : Boolean;
     UseF9F12 : Boolean;
     UseCtrl : Boolean;

     UsePCSpeaker : Boolean;
     DelDuplicateFrames : Boolean;
     FilePrefix : String;
   END;

{ MButton : Byte;
 MLButton : Byte;
 MTime : Word;}

 KPress : Byte;
 LKPress : Byte;
 AltPressed : Boolean;
 MButton : Byte;
 MouseButtonPressed : Boolean;

 B : Byte;
 REC : Byte;
 W : Word;
 I,
 J : Integer;
 Bool : Boolean;
 AnimationHandle : array [0..19] of Word;
 ActualBufor : Byte;
 FramePalette : TPalette;
 FramePaletteBMP : TPaletteBMP;
 Old1Ch : Pointer;
 ActualFrame : Longint;
 ActualSaveFrame : Longint;
 ActualXMSFrame : Longint;
 MaxFrames : Longint;
 TotalFrames : Longint;
 OutputFile : File;
 BMPHandle : THandleBMP;
 FileName : String;
 Line : Array [0..319] of Byte;
 Line2 : Array [0..319] of Byte;
 STOP : BOOLEAN;
 Tmp : Longint;
 Nr : Longint;

PROCEDURE ERROR (Description : String ; HaltProgram : Boolean);
BEGIN
  If (HaltProgram) then
  Begin
    Writeln ('ERROR : ' + Description);
    Halt;
  End
  Else
  Begin
    Writeln ('WARNING : ' + Description);
    Writeln;
  End;
END;

PROCEDURE S (Snd, Del : Word);
BEGIN
  If (OPT. UsePCSpeaker) then
  Begin
    Sound (Snd);
    Delay (Del);
    NoSound;
  End;
END;

FUNCTION IntToStr (liczba : longint) : string;
VAR
 tekst : string;
BEGIN
 str (liczba,tekst);
 IntToStr := tekst;
END;

FUNCTION YN (Text : string) : BOOLEAN;
VAR Kl : Char;
BEGIN
  While (Keypressed) do Readkey;
  YN := FALSE;
  Write (Text,' (Y/N)? ');
  REPEAT
    Kl := Readkey;
  UNTIL (Kl = #121) OR (Kl = #89) OR (Kl = #13) OR (Kl = #110) OR (Kl = #78) OR (Kl = #27);
  IF (Kl = #121) OR (Kl = #89) OR (Kl = #13) THEN
  BEGIN
    Writeln ('Y');
    YN := TRUE;
  END
  ELSE
    Writeln ('N');
END;

PROCEDURE ProgramEnd;
BEGIN
  For I := 0 to 19 do
  Begin
    If (I * 1035 >= MaxFrames) then Exit;
    If (not XMSFreeMem (AnimationHandle [I])) then Error ('Error while freeing XMS memory',False);
  End;

  Halt;
END;

PROCEDURE PrerareToRecord;
BEGIN
  Writeln ('13h Screen Recorder version 2.0 beta [2003/12/23]');
  Writeln ('Copyright by Peter_K { asm3@o2.pl }');
  Writeln ('Check for the latest version at www.princed.com.ar');
  Writeln;
  Writeln ('! Before do anything please read READ.txt file !');
  Writeln;

(* checking for XMS Memory *)
  If (NOT XMSSetup) then Error ('XMS not found',True);
  If (XMSMemAvail < 1000) then Error ('To little XMS memory',True);
  If (XMSMemAvail < 6000) then Error ('You haven"t too much XMS memory',False);

  For I := 0 to 19 do
  Begin
    Tmp := XMSMemAvail;
    If (NOT XMSGetMem (AnimationHandle [I],Tmp)) then Break;
    MaxFrames := MaxFrames + ((1024 * Tmp) div 64768) - 1; { = 1 GB }
  End;

(* info about memory *)
  Writeln ('Size of XMS bufor  : ',MaxFrames,' [frames]');
  Writeln ('Max recording time : ',MaxFrames div 1110,' min. ( ',MaxFrames div 18,' sec. ).');
  Writeln;

(* setting additional options *)
  {OPT. SkipSecFrame := True;}
  OPT. StartAtBegin := False;
  OPT. UsePCSpeaker := True;
  OPT. DelDuplicateFrames := False;

{  If (YN ('Advenced settings ?')) then
  Begin}
  Write('Type program name to exec (with .exe !) : ');
  Readln(OPT. ProgramPath);
  Write('Type program parameters: ');
  Readln(OPT. ProgramParameters);
  Write('Type prefix for BMP files (maximum 3 signs): ');
  Readln(OPT. FilePrefix);
  OPT. UseMouse := YN ('Use mouse?');
  OPT. UseF9F12 := YN ('Use keys F9 - F12?');
  OPT. UseCtrl := YN ('Use ctrl key?');
  {OPT. SkipSecFrame := FALSE;{YN ('Skip seconds frames - 2x more time and increase speed');}
  OPT. StartAtBegin := YN ('Start recording at beginning');
  OPT. UsePCSpeaker := YN ('Use PC Speaker');
  OPT. DelDuplicateFrames := YN ('Delete duplicate frames');
{  End;}
  If (OPT. StartAtBegin) Then
    REC := 2
  Else
    REC := 0;
END;

PROCEDURE SaveScreen; INTERRUPT;
BEGIN
  If (STOP) or (GetVideoMode <> $13) or (TotalFrames >= MaxFrames) then Begin S (200,100); Exit; End;

  STOP := TRUE;

  If (REC = 1) Then Rec := 0;

  If ((OPT. UseF9F12) OR (OPT. UseCtrl)) Then
  Begin
    {LKPress := KPress;}
    KPress := port[$60];
    If (KPress = $57) then Rec := 2;
    If (KPress = $58) then Rec := 0;
    {If (LKPress = 56) then Rec := False;}
    If (KPress = 56) then
    BEGIN
      if (NOT AltPressed) then Rec := 1;
      AltPressed := TRUE;
    END
    ELSE
      AltPressed := FALSE;
    If (KPress = $43) then
    Begin
      Graph13h.RectangleFilled (200,0,300,5,Graph13h.Color (63,63,63));
      Case REC Of
        0 : Graph13h.RectangleFilled (200,0,200 + ((TotalFrames * 100) div MaxFrames),5,Graph13h.Color (63,0,0));
        1 : Graph13h.RectangleFilled (200,0,200 + ((TotalFrames * 100) div MaxFrames),5,Graph13h.Color (0,63,0));
        2 : Graph13h.RectangleFilled (200,0,200 + ((TotalFrames * 100) div MaxFrames),5,Graph13h.Color (0,63,0));
      End;
    End;
  End;

{  If (OPT. UseMouse) then
  Begin
    MLButton := MButton;
    MButton := MouseButton;
    If (MButton = 1) Then
    Begin
      If (NOT MouseButtonPressed) Rec := 1;
      MouseButtonPressed := TRUE;
    End
    Else
      MouseButtonPressed := FALSE:
    If (MButton = 2) Then Rec := 1;
    If (MButton = 4) AND (MLButton <> 4) Then Rec := True;
    If (MLButton = 4) AND (MButton <> 1) Then Rec := False;
  End;}

  ActualBufor := TotalFrames div 1035;
  ActualXMSFrame := TotalFrames mod 1035;

  IF (REC > 0) THEN
  BEGIN
    {If (TotalFrames mod 20 = 0) Then Begin S (600,2); End;}
    SavePalette (FramePalette);
    XMSMoveMem (0,@FramePalette,AnimationHandle [ActualBufor],Pointer (ActualXMSFrame * 64768),768);
    XMSMoveMem (0,Screen,AnimationHandle [ActualBufor],Pointer ((ActualXMSFrame * 64768) + 768),64000);
    Inc (TotalFrames);
    Inc (ActualXMSFrame);
  END;

{  If (NOT REC) OR (MouseButton = 2) then
  Begin
    Graph13h.RectangleFilled (200,0,300,5,Graph13h.Color (63,63,63));
    Graph13h.RectangleFilled (200,0,200 + ((TotalFrames * 100) div MaxFrames),5,Graph13h.Color (0,63,0));
    Graph13h.RectangleFilled (200 + TotalFrames mod 100,0,201 + (TotalFrames mod 100),5,Graph13h.Color (63,0,0));
  End;}

  Inc (Nr);

  STOP := FALSE;
END;

PROCEDURE ExecuteProgram;
BEGIN
  Writeln ('Hit any key to execute program ...');
  While (Keypressed) do Readkey;
  Readkey;

  GetIntVec ($1C,Old1Ch);
  SetIntVec ($1C,@SaveScreen);

  If (Execute (OPT.ProgramPath,OPT.ProgramParameters) <> 0) then
  Begin
    SetIntVec ($1C,Old1Ch);
    Error ('Cannot execute file ' + OPT. ProgramPath,False);
    ProgramEnd;
  End;

  SetIntVec ($1C,Old1Ch);

  Writeln ('Recording finished succesfull');
  Writeln ('Total frames         : ',TotalFrames);
  Writeln ('BMPs size after save : ',(TotalFrames * 65078) div 1048576,' MB');
  Writeln;
END;

{PROCEDURE ViewAnimation;
BEGIN
  If (TotalFrames = 0) then Exit;
  Load13h;
  SetBufor (Bufor);
  ActualFrame := 0;
  REPEAT
    If (OPT.SavePalette) then
    Begin
      XMSMoveMem (AnimationHandle,Pointer (ActualFrame * 64768),0,@FramePalette,SizeOf (FramePalette));
      LoadPalette (FramePalette);
    End;
    XMSMoveMem (AnimationHandle,Pointer ((ActualFrame * 64768) + SizeOf (FramePalette)),0,Bufor,64000);
    ShowBufor (Bufor);
    Inc (ActualFrame);
    Delay (100);
    If (OPT.LoopAnimation) and (ActualFrame = TotalFrames) then ActualFrame := 0;
  UNTIL (ActualFrame = TotalFrames) OR (Keypressed);
  Unload13h;
END;}

PROCEDURE SaveAnimation;
VAR
  LastBufor : Byte;
  LastXMSFrame : Longint;
BEGIN
  {If (NOT YN ('Save bitmaps files')) Then If (YN ('Are You SURE')) Then Exit;
  OPT. DelDuplicateFrames := YN ('Delete duplicate frames');}

  {$I-}
  MkDir ('frames');
  {$I+}
  If (IOResult <> 0) then begin end;

  New (Bufor);

  BMPHandle.FType := $4D42;
  BMPHandle.FSize := 65078;
  BMPHandle.FReserved1 := 0;
  BMPHandle.FReserved2 := 0;
  BMPHandle.FFirstPixel := 1078;
  BMPHandle.FInfoSize := 40;
  BMPHandle.FWidth := 320;
  BMPHandle.FHeight := 200;
  BMPHandle.FPlanes := 1;
  BMPHandle.FBitPerPixel := 8;
  BMPHandle.FCompression := 0;
  BMPHandle.FImageSize := 64000;
  BMPHandle.FDPIWidth := 3937;
  BMPHandle.FDPIHeight := 3937;
  BMPHandle.FColorUsed := 0;
  BMPHandle.FImportantColor := 0;

  ActualSaveFrame := 0;
  For ActualFrame := 0 to (TotalFrames - 1) do
  Begin
    { for skipping the same frames }
    LastBufor := ActualBufor;
    LastXMSFrame := ActualXMSFrame;
    { ... }
    ActualBufor := ActualFrame div 1035;
    ActualXMSFrame := ActualFrame mod 1035;

    { for skipping the same frames }
    If (OPT. DelDuplicateFrames) and (ActualFrame > 0) then
    Begin
      Bool := False;
      For I := 0 to 199 do
      Begin
        XMSMoveMem (AnimationHandle [LastBufor],Pointer ((LastXMSFrame * 64768) + (I * 320) + SizeOf (FramePalette)),
                    0,Addr (Line),320);
        XMSMoveMem (AnimationHandle [ActualBufor],Pointer ((ActualXMSFrame * 64768) + (I * 320) + SizeOf (FramePalette)),
                    0,Addr (Line2),320);
        For J := 0 to 319 do
        If (Line [J] <> Line2 [J]) then
        Begin
          Bool := True;
          Break;
        End;
        If (Bool) then Break;
      End;
      If (NOT Bool) then
        Continue;
    End;
    { ... }

    Inc(ActualSaveFrame);
    FileName := IntToStr (ActualSaveFrame);
    While (Length (FileName) < 5) do FileName := '0' + FileName;
    FileName := OPT.FilePrefix + FileName + '.bmp';
    Assign (OutputFile,{OPT.SaveDir} + 'frames\' + FileName);
    {$I-}
    Rewrite (OutputFile, 1);
    {$I+}
    If (IOResult <> 0) then
    Begin
      Error ('Cannot save file ' + {OPT.SaveDir}'frames' + '\' + FileName,False);
      Continue;
    End;

{    BlockRead (InputFile, FramePalette, 768);}
    XMSMoveMem (AnimationHandle [ActualBufor],Pointer (ActualXMSFrame * 64768),0,@FramePalette,SizeOf (FramePalette));
    PaletteRGBToBMP (FramePalette,FramePaletteBMP);

{    BlockRead (InputFile, Bufor^, 64000);}
    XMSMoveMem (AnimationHandle [ActualBufor],Pointer ((ActualXMSFrame * 64768) + SizeOf (FramePalette)),0,Bufor,64000);

    BlockWrite (OutputFile,BMPHandle,SizeOf (BMPHandle));
    BlockWrite (OutputFile,FramePaletteBMP,SizeOf (FramePaletteBMP));
    FOR W:= 199 DOWNTO 0 DO
    BEGIN
      Move(Bufor^ [W * 320],Line,320);
      BlockWrite(OutputFile,Line,320);
    END;

    Close (OutputFile);

    GoToXY (1,WhereY);
    Write (((ActualFrame + 1) * 100) div TotalFrames,'%, ( ',ActualFrame + 1,' frames )          ');
  End;
  Dispose (Bufor);

  Writeln;
  Writeln;
  Writeln ('Frames now are stored in "frames" dir.');
END;

BEGIN
  PrerareToRecord;

  ExecuteProgram;

  If (TotalFrames > 0) then SaveAnimation;

  ProgramEnd;
END.