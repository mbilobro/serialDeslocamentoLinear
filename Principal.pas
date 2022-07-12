unit Principal;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, CPort, Vcl.StdCtrls, CPortCtl,
  Vcl.ExtCtrls, Vcl.ImgList, Vcl.Buttons;

type
  TfrmPrincipal = class(TForm)
    ComPort1: TComPort;
    ComDataPacket1: TComDataPacket;
    gtnEnviar: TButton;
    Panel1: TPanel;
    edtDeslocamento: TEdit;
    chkckCRLF: TCheckBox;
    Label3: TLabel;
    ImageList1: TImageList;
    btnHome: TButton;
    btnCalibrar: TButton;
    btnEsquerda: TButton;
    btnDireita: TButton;
    Label4: TLabel;
    Button1: TButton;
    Button2: TButton;
    Label1: TLabel;
    ComTerminal1: TComTerminal;
    procedure btnCalibrarClick(Sender: TObject);
    procedure btnHomeClick(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure btnEsquerdaClick(Sender: TObject);
    procedure btnDireitaClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmPrincipal: TfrmPrincipal;

implementation

{$R *.dfm}

procedure TfrmPrincipal.btnCalibrarClick(Sender: TObject);
begin
  if chkckCRLF.Checked
  then
    ComPort1.WriteStr('c' + #13 + #10)
  else
    ComPort1.WriteStr('c');
end;

procedure TfrmPrincipal.btnDireitaClick(Sender: TObject);
begin
  if chkckCRLF.Checked
  then
    ComPort1.WriteStr('x ' + edtDeslocamento.Text + #13 + #10)
  else
    ComPort1.WriteStr('x ' + edtDeslocamento.Text);
end;

procedure TfrmPrincipal.btnEsquerdaClick(Sender: TObject);
begin
  if chkckCRLF.Checked
  then
    ComPort1.WriteStr('x -' + edtDeslocamento.Text + #13 + #10)
  else
    ComPort1.WriteStr('x -' + edtDeslocamento.Text);
end;

procedure TfrmPrincipal.btnHomeClick(Sender: TObject);
begin
  if chkckCRLF.Checked
  then
    ComPort1.WriteStr('h' + #13 + #10)
  else
    ComPort1.WriteStr('h');
end;

procedure TfrmPrincipal.Button1Click(Sender: TObject);
begin
  edtDeslocamento.Clear;
  edtDeslocamento.SetFocus;
  ComTerminal1.ClearScreen;
end;

procedure TfrmPrincipal.Button2Click(Sender: TObject);
begin
  Application.Terminate;
end;

procedure TfrmPrincipal.FormShow(Sender: TObject);
begin
  edtDeslocamento.SetFocus;
end;

end.
