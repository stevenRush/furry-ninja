unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, Menus;

type
  TForm1 = class(TForm)
    Timer1: TTimer;
    Panel1: TPanel;
    Label1: TLabel;
    TimerL: TTimer;
    TimerR: TTimer;
    Panel2: TPanel;
    Label2: TLabel;
    MainMenu1: TMainMenu;
    N1: TMenuItem;
    N2: TMenuItem;
    N11: TMenuItem;
    N21: TMenuItem;
    N3: TMenuItem;
    N4: TMenuItem;
    N5: TMenuItem;
    N6: TMenuItem;
    N7: TMenuItem;
    N8: TMenuItem;
    N9: TMenuItem;
    N12: TMenuItem;
    N22: TMenuItem;
    N31: TMenuItem;
    N41: TMenuItem;
    N51: TMenuItem;
    Label3: TLabel;
    procedure FormShow(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure UpdateScore;
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure TimerRTimer(Sender: TObject);
    procedure FormKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure TimerLTimer(Sender: TObject);
    procedure NewGame;
    procedure LevelSelect(Sender: TObject);
    procedure N2Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

  TBall = class(TShape)
  public
    procedure Move;
    procedure ChangeDir;
    constructor Create(AOwner: TComponent); override;
  private
    DirectionX: integer;
    DirectionY: integer;
    Speed: integer;
  end;


  TPlat = class(TPanel)
  public
    Speed: integer;
    procedure MoveUp;
    procedure MoveDown;
    function isTouched(bl: TBall): boolean;
    constructor Create(AOwner: TComponent); override;
  end;

var
  Form1: TForm1;
  PlatL, PlatR: TPlat;
  Ball: TBall;
  PointR, PointL, MaxPoints, NumPress, SpeedX, BotSpeed: integer;
  UpL, UpR, DownL, DownR, Pause, isBot: boolean;

implementation

{$R *.dfm}

constructor TBall.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  Shape:= stCircle;
  Width:= 20;
  Height:= 20;
  randomize;
  DirectionY:= -2;
  if (random(2) = 1) then
     DirectionX:= 2
  else
     DirectionX:= -2;
  Speed:= 1;
  Color:= clWhite;
end;

procedure TBall.Move;
begin
  Top:= Top +  DirectionY;
  Left:= Left + DirectionX;
  if (Top <= 0) or (Top + Width >= Parent.Height) then
    DirectionY:= -DirectionY;
end;

procedure TBall.ChangeDir;
begin
  DirectionX:= -DirectionX;
end;

constructor TPlat.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  Parent:= TWinControl(AOwner);
  Width:= 5;
  Height:= 100;
  Color:= clWhite;
  Speed:= 1;
end;

function TPlat.isTouched(bl: TBall): boolean;
  var thisRt, BallRt, TempRt: TRect;
begin
  SetRect(thisRt, Left, Top, Left+Width, Top+Height);
  SetRect(BallRt, bl.Left, bl.Top, bl.Left+bl.Width, bl.Top+bl.Height);
  isTouched:= IntersectRect(TempRt, thisRt, BallRt);
end;

procedure TPlat.MoveUp;
  var i: integer;
begin
  for i:= 1 to Speed do
    if (Top > 0) then
      Top:= Top - 1;
end;

procedure TPlat.MoveDown;
  var i: integer;
begin
  for i:= 1 to Speed do
    if (Top + Height < Parent.Height) then
      Top:= Top + 1;
end;

procedure TForm1.FormShow(Sender: TObject);
begin
  PlatL:= TPlat.Create(Panel1);
  PlatL.Parent:= Panel1;
  PlatL.Left:= 5;
  PlatL.Top:= Panel1.Height div 2 - PlatL.Height div 2;
  PlatR:= TPlat.Create(Panel1);
  PlatR.Parent:= Panel1;
  PlatR.Left:= Panel1.Width - PlatR.Width - 12;
  PlatR.Top:= Panel1.Height div 2 - PlatR.Height div 2;
  Ball:= TBall.Create(Panel1);
  Ball.Parent:= Panel1;
  Ball.Top:= Panel1.Height div 2 - Ball.Height div 2;
  Ball.Left:= Panel1.Width div 2 - Ball.Width div 2;
  Ball.BringToFront;
  PointR:= 0;
  PointL:= 0;
  UpL:= false;
  UpR:= false;
  DownL:= false;
  DownR:= false;
  MaxPoints:= 5;
  NumPress:= 0;
  SpeedX:= 1;
end;

procedure TForm1.UpdateScore;
  var i: integer;
      st: PAnsiChar;
begin
  label1.Caption:= intToStr(PointL) + ' : ' + intToStr(PointR);
  i:= 0;
  NumPress:= 0;
  Label3.Visible:= true;
  Timer1.Enabled:= false;
  if (PointL = MaxPoints) then
    i:= 1;
  if (PointR = MaxPoints) then
    i:= 2;
  if (i <> 0) then
  begin
    Timer1.Enabled:= false;
    st:= PAnsiChar('Выйграл игрок №' + intToStr(i));
    if Application.MessageBox(st, 'Пинг Понг', MB_RETRYCANCEL) = IDCANCEL then
      Application.Terminate;
    Timer1.Enabled:= true;
    PointL:= 0;
    PointR:= 0;
    label1.Caption:= intToStr(PointL) + ' : ' + intToStr(PointR);
  end;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
   var pos: integer;
begin
  Ball.Move;
  if (isBot) then
  begin
    PlatR.Speed:= BotSpeed;
    if (Ball.Left + Ball.Width div 2) > Panel1.Width div 2 then
    begin
      pos:+ Ball.Top + Ball.Height div 2;
      if (not pos in [PlatR.Top..PlatR.Top + PlatR.Height]) and
         (pos > PlatR.Top + PlatR.Height div 2) then
         PlatR.MoveDown;
      if (not pos in [PlatR.Top..PlatR.Top + PlatR.Height]) and
         (pos < PlatR.Top + PlatR.Height div 2) then
         PlatL.MoveDown;

  if (PlatR.isTouched(Ball)) then
  begin
    Ball.ChangeDir;
    if UpR then
      Ball.DirectionY:= Ball.DirectionY - PlatR.Speed div 3;
    if DownR then
      Ball.DirectionY:= Ball.DirectionY + PlatR.Speed div 3;
  end;

  if (PlatL.isTouched(Ball)) then
  begin
    Ball.ChangeDir;
    if (UpL) then
      Ball.DirectionY:= Ball.DirectionY - PlatL.Speed div 3;
    if (DownL) then
      Ball.DirectionY:= Ball.DirectionY + PlatL.Speed div 3;
  end;
  if (Ball.DirectionY > 5) then
    Ball.DirectionY:= 5;
  if (Ball.DirectionY < -5) then
    Ball.DirectionY:= -5;
  if (Ball.Left <= 0) then
  begin
    inc(PointR);
    PlatL.Top:= Panel1.Height div 2 - PlatL.Height div 2;
    PlatR.Top:= Panel1.Height div 2 - PlatR.Height div 2;
    Ball.Top:= Panel1.Height div 2 - Ball.Height div 2;
    Ball.Left:= Panel1.Width div 2 - Ball.Width div 2;
    Ball.DirectionY:= -2;
    randomize;
    if (random(2) = 1) then
      SpeedX:= -SpeedX;
    Ball.DirectionX:= SpeedX;
    UpdateScore;
  end;
  if (Ball.Left >= Panel1.Width - Ball.Width) then
  begin
    inc(PointL);
    PlatL.Top:= Panel1.Height div 2 - PlatL.Height div 2;
    PlatR.Top:= Panel1.Height div 2 - PlatR.Height div 2;
    Ball.Top:= Panel1.Height div 2 - Ball.Height div 2;
    Ball.Left:= Panel1.Width div 2 - Ball.Width div 2;
    Ball.DirectionY:= -2;
    randomize;
    if (random(2) = 1) then
      SpeedX:= -SpeedX;
    Ball.DirectionX:= SpeedX;
    UpdateScore;
  end;

end;

procedure TForm1.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if (NumPress = 0) and not (Key = ord(' '))then
  begin
    exit;
  end;
  case Key of
    ord(' '):
    begin
      if (NumPress <> 0) then
         exit;
      Timer1.Enabled:= true;
      label3.Visible:= false;
      inc(NumPress);
    end;
    VK_UP:
    begin
      if (UpR) or (pause) then
        exit;
      if (DownR) then
        DownR:= false;
      UpR:= true;
      TimerR.Enabled:= true;
      PlatR.MoveUp;
    end;
    VK_DOWN:
    begin
      if (DownR) or (pause) then
        exit;
      if (UpR) then
        UpR:= false;
      DownR:= true;
      TimerR.Enabled:= true;
    end;
    ord('W'):
    begin
      if (UpL) or (pause) then
        exit;
      if (DownL) then
        DownL:= false;
      UpL:= true;
      TimerL.Enabled:= true;
    end;
    ord('S'):
    begin
      if (DownL) or (pause) then
        exit;
      if (UpL) then
        UpL:= false;
      DownL:= true;
      TimerL.Enabled:= true;
    end;
    ord('P'):
    begin
      Timer1.Enabled:= pause;
      if not pause then
      begin
        TimerL.Enabled:= pause;
        TimerR.Enabled:= pause;
      end;
      pause:= not pause;
      Label2.Visible:= pause;
    end;
  end;
end;

procedure TForm1.TimerRTimer(Sender: TObject);
begin
  if (DownR) then
    PlatR.MoveDown;
  if (UpR) then
    PlatR.MoveUp;
  inc(PlatR.Speed, 3);
end;

procedure TForm1.FormKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
   case Key of
    VK_UP:
    begin
      UpR:= false;
      TimerR.Enabled:= false;
      PlatR.Speed:= 1;
    end;
    VK_DOWN:
    begin
      DownR:= false;
      TimerR.Enabled:= false;
      PlatR.Speed:= 1;
    end;
    ord('W'):
    begin
      UpL:= false;
      TimerL.Enabled:= false;
      PlatL.Speed:= 1;
    end;
    ord('S'):
    begin
      DownL:= false;
      TimerL.Enabled:= false;
      PlatL.Speed:= 1;
    end;
  end;
end;

procedure TForm1.TimerLTimer(Sender: TObject);
begin
  if (DownL) then
    PlatL.MoveDown;
  if (UpL) then
    PlatL.MoveUp;
  inc(PlatL.Speed, 3);
end;

procedure TForm1.NewGame;
begin
   PointL:= 0;
   PointR:= 0;
   UpdateScore;
   PlatL.Top:= Panel1.Height div 2 - PlatL.Height div 2;
   PlatR.Top:= Panel1.Height div 2 - PlatR.Height div 2;
   Ball.Top:= Panel1.Height div 2 - Ball.Height div 2;
   Ball.Left:= Panel1.Width div 2 - Ball.Width div 2;
   NumPress:= 0;
   Label3.Visible:= true;
   Ball.DirectionX:= SpeedX;
   TimerR.Enabled:= false;
   TimerL.Enabled:= false;
end;
procedure TForm1.LevelSelect(Sender: TObject);
   var k: integer;
begin
    if (Ball.DirectionX < 0) then
      k:= -1
    else
      k:= 1;
    if (Sender = N12) then
         SpeedX:= k
    else if (Sender = N22) then
         SpeedX:= 2 * k
    else if (Sender = N31) then
         SpeedX:= 3 * k
    else if (Sender = N41) then
         SpeedX:= 4 * k
    else if (Sender = N51) then
         SpeedX:= 5 * k;
    Ball.DirectionX:= SpeedX;
    NewGame;
end;

procedure TForm1.N2Click(Sender: TObject);
begin
   NewGame;
end;

end.
