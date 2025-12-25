unit ToDelphi_f;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TForm1 = class(TForm)
    Button1: TButton;
    Label1: TLabel;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

function AddInteger(a:Integer;b:Integer):Integer;stdcall;external 'Mydll';
implementation

{$R *.DFM}

procedure TForm1.Button1Click(Sender: TObject);
var
	str:String;
	Result:Integer;
begin
	Result:=AddInteger(3,4);
	str:=Format('3+4 = %d',[Result]);
	Label1.Caption:=str;
end;

end.
