program ToDelphi;

uses
  Forms,
  ToDelphi_f in 'ToDelphi_f.pas' {Form1};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
