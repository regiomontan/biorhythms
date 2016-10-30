//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "dialogU.h"
#include "conv_dateU.h"
#include <math.h>
#include <stdio.h>
#include <dstring.h>
#include <iostream.h>
#include <fstream.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TdialogForm *dialogForm;
// "������� �����":
struct person
       {
       String     FIO;    // �.�.�.
       TDateTime  birth;  // ��� ��������
       String     sex;    // ���
       BOOL       lacuna; // ���� ������� � ������
       };
// ����� ������ �����:
struct common_data
       {
       float  fiz;
       float  emoz;
       float  intell;
       float  tonk;
       int    count;
       };

struct person family[13];        // ����� <= 12 �������
common_data   common_family;
const         family_count = 12; // ���������� ����� � �����
bool          SELECTED;
int           h_offset = 40;     // �������� ������ �� ������ �����
//---------------------------------------------------------------------------
__fastcall TdialogForm::TdialogForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
//  ����� ���� �������� ������  ������� �����  StringGrid
//---------------------------------------------------------------------------
void __fastcall TdialogForm::Clear_data( TObject *Sender )
{
 for ( int i = 1; i <= family_count; i++ )
  {
  family[i].FIO    = "";
  family[i].lacuna = TRUE;
  family[i].birth  = EncodeDate( 1977, 7, 7 );
  family[i].sex    = "";
    if ( i > 4 ) StringGrid1->Cells[0][i] = IntToStr(i - 4);
  }
StringGrid1->Cells[0][1] = "����";
StringGrid1->Cells[0][2] = "(����)";
StringGrid1->Cells[0][3] = "����";
StringGrid1->Cells[0][4] = "(����)";
StringGrid1->Cells[1][0] = "���� ����.";
StringGrid1->Cells[2][0] = "���";
StringGrid1->Cells[3][0] = "�.�.�";

common_family.fiz    = 0;
common_family.emoz   = 0;
common_family.intell = 0;
common_family.tonk   = 0;
common_family.count  = 0;
}
//---------------------------------------------------------------------------
//  ��������� �����e, ����������� �� �� �������  family_count
//   �� �����  StringGrid1
//---------------------------------------------------------------------------
void __fastcall TdialogForm::Refresh_date(TObject *Sender)
{
  for ( int row = 1; row <= family_count; row++)
  {
   if ( family[row].lacuna == FALSE)
   {
     try
     {
     StringGrid1->Cells[1][row] = DateToStr( family[row].birth );
     }
     catch(...)
     {
     ShowMessage("�������� ���� ����");
     }
   }
   else
   {
   StringGrid1->Cells[1][row] = "";
   }  // if - else ( family_lacuna == FALSE)
  StringGrid1->Cells[2][row] = family[row].sex;
  StringGrid1->Cells[3][row] = family[row].FIO;
  } // for( int i...
}
//---------------------------------------------------------------------------
//  ������ ���������
//---------------------------------------------------------------------------

void __fastcall TdialogForm::Krug( int cx, int cy, int R,
                                   float fiz,    float emoz,
                                   float intell, float tonk )
{
float  summa  = fiz + emoz + intell + tonk;
float fi_0, fi_1; // ���� ��������;
  // ���������� �������������:
  if ( fiz > 0 )
  {
  fi_0 = 0;
  fi_1 = fi_0 + fiz/summa*2.0*PI;
  Chart1->Canvas->Brush->Color = clRed;
  Chart1->Canvas->Pie(cx - R, cy - R, cx + R, cy + R,
                      cx + (int)(R*cos(fi_0)), cy - (int)(R*sin(fi_0)),
                      cx + (int)(R*cos(fi_1)), cy - (int)(R*sin(fi_1)) );
  }
  // ������������� �������������:
  if ( emoz > 0 )
  {
  fi_0 = fi_1;
  fi_1 = fi_0 + emoz/summa*2.0*PI;
  Chart1->Canvas->Brush->Color = clYellow;
  Chart1->Canvas->Pie(cx - R, cy - R, cx + R, cy + R,
                      cx + (int)(R*cos(fi_0)), cy - (int)(R*sin(fi_0)),
                      cx + (int)(R*cos(fi_1)), cy - (int)(R*sin(fi_1)) );
  }
  // ���������������� �������������:
  if ( intell > 0 )
  {
  fi_0 = fi_1;
  fi_1 = fi_0 + intell/summa*2.0*PI;
  Chart1->Canvas->Brush->Color = clBlue;
  Chart1->Canvas->Pie(cx - R, cy - R, cx + R, cy + R,
                      cx + (int)(R*cos(fi_0)), cy - (int)(R*sin(fi_0)),
                      cx + (int)(R*cos(fi_1)), cy - (int)(R*sin(fi_1)) );
  }
  // ���������������� �������������:
  if ( tonk > 0 )
  {
  fi_0 = fi_1;
  fi_1 = fi_0 + tonk/summa*2.0*PI;
  Chart1->Canvas->Brush->Color = clGreen;
  Chart1->Canvas->Pie(cx - R, cy - R, cx + R, cy + R,
                      cx + (int)(R*cos(fi_0)), cy - (int)(R*sin(fi_0)),
                      cx + (int)(R*cos(fi_1)), cy - (int)(R*sin(fi_1)) );
  }
Chart1->Canvas->Brush->Color = clWhite;

}
//---------------------------------------------------------------------------
//  ������ ��������� � ���������
//---------------------------------------------------------------------------
void __fastcall TdialogForm::Diagramma( int cy, TDateTime D1, TDateTime D2,
                                        int numb1, int numb2 )
{
// ���������� ���������� ������������� � ���������� ������
// � common_family:
float  fiz          = percent( D1, D2, 23 );
common_family.fiz  += fiz;
float  emoz         = percent( D1, D2, 28 );
common_family.emoz += emoz;
float  intell       = percent( D1, D2, 33 );
common_family.intell += intell;
float  tonk         = percent( D1, D2, 38 );
common_family.tonk += tonk;
common_family.count++;
// ����� ���������� ���������� �������������.
float  summa        = fiz + emoz + intell + tonk;
int    Chart_height = Chart1->ChartRect.Bottom - Chart1->ChartRect.Top;
int    cx = (int)( Chart1->BottomAxis->CalcPosValue(summa/4) );
int    R  = (int)( (Chart_height - h_offset)/10*summa/400 );//������ ���������
String str1 = "", str2 = "";
  // ������� ��� �����������:
  switch ( numb1 )
  {
  case 1:   str1 = "o";   break;  // ����
  case 3:   str1 = "�";   break;  // ����
  case 2:   str1 = "(o)"; break;  // ������ ����
  case 4:   str1 = "(�)"; break;  // ������ �����
  default:  str1 = IntToStr( numb1 - 4 );
  }
  switch ( numb2 )
  {
  case 1:   str2 = "o";   break;
  case 3:   str2 = "�";   break;
  case 2:   str2 = "(o)"; break;
  case 4:   str2 = "(�)"; break;
  default:  str2 = IntToStr( numb2 - 4 );
  }
 Chart1->Canvas->TextOutA( cx - Chart1->Canvas->TextWidth(" -"),
                           cy - R - Chart1->Canvas->TextHeight("�"),
                           str1 + " - " + str2  );
 // ����� ������ ��������.
Krug( cx, cy,  R, fiz, emoz, intell,  tonk );
}
//---------------------------------------------------------------------------
//  �������������
//---------------------------------------------------------------------------
void __fastcall TdialogForm::FormCreate(TObject *Sender)
{
Clear_data( Sender );
}
//---------------------------------------------------------------------------
// ��������� ���� �������� � ������ �����
//---------------------------------------------------------------------------
void __fastcall TdialogForm::ToListBtnClick(TObject *Sender)
{
StringGrid1->Cells[1][StringGrid1->Row] = DateToStr(DTPicker_birth->Date);
family[StringGrid1->Row].birth  = DTPicker_birth->Date;
family[StringGrid1->Row].lacuna = FALSE;
  if ( CheckBox_male->Checked )
  {
  family[StringGrid1->Row].sex = "�";
  StringGrid1->Cells[2][StringGrid1->Row] = "�";
  }
  if ( CheckBox_female->Checked )
  {
  family[StringGrid1->Row].sex = "�";
  StringGrid1->Cells[2][StringGrid1->Row] = "�";
  }
dialogForm->Caption = " ";  
}
//---------------------------------------------------------------------------
// ���������� ������
//---------------------------------------------------------------------------
void __fastcall TdialogForm::StringGrid1SetEditText(TObject *Sender,
                                   int ACol, int ARow, const AnsiString Value)
{
  switch ( ACol )
  {
  case 1 :if ( Value == "") // ���� ��� ���� ��������
          {
          family[ARow].lacuna = TRUE;
          }
          else
          {
          family[ARow].birth = StrToDate( Value );
          family[ARow].lacuna = FALSE;
          }
          break;
  case 2 :family[ARow].sex = Value;
          break;
  case 3 :family[ARow].FIO = Value;
          break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TdialogForm::SpeedButton1Click(TObject *Sender)
{
Refresh_date( Sender );
Series1->Clear();
//Series1->Active = FALSE;
int  member_family = 0; //
 for ( int i = 1; i <= family_count; i++ )
 {
   if ( !family[i].lacuna ) member_family++;
 }
 if ( member_family == 1)
 {
 ShowMessage("� ����� ������ ���� > 1 ��������");
 }
}
//---------------------------------------------------------------------------
// ����� ��������
//---------------------------------------------------------------------------
void __fastcall TdialogForm::Chart1AfterDraw(TObject *Sender)
{
common_family.fiz    = 0;
common_family.emoz   = 0;
common_family.intell = 0;
common_family.tonk   = 0;
common_family.count  = 0;
Chart1->Canvas->Font->Size = Chart1->BottomAxis->LabelsFont->Size + 1;
int max_h = Chart1->LeftAxis->Maximum; // max ������
// �������� ������:
 Windows::TPoint points[4];
 points[0] = Point( Chart1->ChartRect.Left, Chart1->ChartRect.Top );
 points[1] = Point( Chart1->ChartRect.Left,
                    Chart1->LeftAxis->CalcPosValue(max_h - h_offset) );
 points[2] = Point( Chart1->ChartRect.Right, Chart1->ChartRect.Top );

 Chart1->Canvas->Brush->Color = 0xDD9999; //clBlue;
 Chart1->Canvas->Polygon(points, 2);

 points[0] = Point( Chart1->ChartRect.Right, Chart1->ChartRect.Top );
 points[1] = Point( Chart1->ChartRect.Right,
                    Chart1->LeftAxis->CalcPosValue(max_h - h_offset));
 points[2] = Point( Chart1->ChartRect.Left,
                    Chart1->LeftAxis->CalcPosValue(max_h - h_offset) );
 Chart1->Canvas->Brush->Color = 0x9999FF;  //clRed;
 Chart1->Canvas->Polygon(points, 2);
 Chart1->Canvas->Brush->Color = clWhite;

  for ( int l = 1; l <= 4; l++ )
  {
  Chart1->Canvas->DoVertLine(Chart1->BottomAxis->CalcPosValue(
                                            Chart1->BottomAxis->Maximum/4*l ),
                             Chart1->LeftAxis->CalcPosValue( max_h ),
                             Chart1->LeftAxis->CalcPosValue(max_h - h_offset));
  }
 // ������������:
// Chart1->Canvas->MoveTo( Chart1->ChartRect.Left,
//                         Chart1->LeftAxis->CalcPosValue(max_h - h_offset) );
// Chart1->Canvas->LineTo( Chart1->ChartRect.Right, Chart1->ChartRect.Top );
 // ������ �����:
 Chart1->Canvas->MoveTo( Chart1->ChartRect.Left,
                         Chart1->LeftAxis->CalcPosValue(max_h - h_offset) );
 Chart1->Canvas->LineTo( Chart1->ChartRect.Right,
                        Chart1->LeftAxis->CalcPosValue(max_h - h_offset) );
 // ����� ��������.
 int diag_h2 = (max_h - h_offset)/10; // �������� ������ ������ ��� ��������
  // 5 �������������� �����:
  for ( int k = 1; k <= 5; k++)
  {
  Chart1->Canvas->MoveTo( Chart1->ChartRect.Left,
                          Chart1->LeftAxis->CalcPosValue(2*k*diag_h2) );
  Chart1->Canvas->LineTo( Chart1->ChartRect.Right,
                          Chart1->LeftAxis->CalcPosValue(2*k*diag_h2) );
  }
  //`````````````````````````````````````````````````````````````````````````
  // ���� ���� ��� ������:
  if ( !family[1].lacuna && !family[3].lacuna  )
  {
  Chart1->Canvas->TextOutA(Chart1->ChartRect.Left+3,
                           Chart1->LeftAxis->CalcPosValue( max_h - h_offset),
                           "�������");
  Diagramma( Chart1->LeftAxis->CalcPosValue( max_h - h_offset - diag_h2 ),
             family[1].birth, family[3].birth, 1, 3 );
  }
  //"������" �������:
  if ( !family[1].lacuna && !family[4].lacuna  )
  {
  Diagramma( Chart1->LeftAxis->CalcPosValue( max_h - h_offset - diag_h2 ),
             family[1].birth, family[4].birth, 1, 4 );
  }
  if ( !family[2].lacuna && !family[3].lacuna  )
  {
  Diagramma( Chart1->LeftAxis->CalcPosValue( max_h - h_offset - diag_h2 ),
             family[2].birth, family[3].birth, 2, 3 );
  }
  // ���� � ����������:
  for ( int i = 5; i <= family_count; i++ )
  {
    if ( !family[i].lacuna )
    {
       if ( !family[1].lacuna) // � �����
       {
       Chart1->Canvas->TextOutA(Chart1->ChartRect.Left+3,
                           Chart1->LeftAxis->CalcPosValue(
                                           max_h - h_offset - 2*diag_h2 - 2),
                           "����");
       Diagramma( Chart1->LeftAxis->CalcPosValue(max_h - h_offset - 3*diag_h2),
                  family[1].birth, family[i].birth, 1, i );

       }
       if ( !family[2].lacuna) // �� 2-� �����
       {
       Chart1->Canvas->TextOutA(Chart1->ChartRect.Left+3,
                           Chart1->LeftAxis->CalcPosValue(
                                           max_h - h_offset - 2*diag_h2 - 2),
                           "����");
       Diagramma( Chart1->LeftAxis->CalcPosValue(max_h - h_offset - 3*diag_h2),
                  family[2].birth, family[i].birth, 2, i );

       }
       if ( !family[3].lacuna) // � �������
       {
       Chart1->Canvas->TextOutA(Chart1->ChartRect.Left+3,
                           Chart1->LeftAxis->CalcPosValue(
                                           max_h - h_offset - 4*diag_h2 - 2),
                           "����");
       Diagramma( Chart1->LeftAxis->CalcPosValue(max_h - h_offset - 5*diag_h2),
                  family[3].birth, family[i].birth, 3, i );
       }
       if ( !family[4].lacuna) // �� 2-� �������
       {
       Chart1->Canvas->TextOutA(Chart1->ChartRect.Left+3,
                           Chart1->LeftAxis->CalcPosValue(
                                           max_h - h_offset - 4*diag_h2 - 2),
                           "����");
       Diagramma( Chart1->LeftAxis->CalcPosValue(max_h - h_offset - 5*diag_h2),
                  family[4].birth, family[i].birth, 4, i );
       }
    }
  }
  // ���� ����� �����:
  for ( int i = 5; i <= family_count; i++ )
  {
     for ( int j = i + 1; j <= family_count; j++ )
     {
        if ( !family[i].lacuna  && !family[j].lacuna)
        {
        Chart1->Canvas->TextOutA(Chart1->ChartRect.Left+3,
                           Chart1->LeftAxis->CalcPosValue(
                                    max_h - h_offset - 6*diag_h2 - 2),
                           "����");
        Diagramma( Chart1->LeftAxis->CalcPosValue(max_h - h_offset - 7*diag_h2),
                   family[i].birth, family[j].birth, i, j );
        }
     }
  }
  // �����:
  if ( common_family.count > 0 )
  {
  Chart1->Canvas->TextOutA( Chart1->ChartRect.Left+3,
                            Chart1->LeftAxis->CalcPosValue(
                                         max_h - h_offset - 8*diag_h2 - 2),
                            "����� " );

  float  summa = (common_family.fiz + common_family.emoz
              +common_family. intell + common_family.tonk)/common_family.count;
  int    Chart_height = Chart1->ChartRect.Bottom - Chart1->ChartRect.Top;
  int    cx = (int)( Chart1->BottomAxis->CalcPosValue(summa/4) );
  int    R  = (int)( (Chart_height - h_offset)/10*summa/400 );//������ ���������
  Krug( cx, Chart1->LeftAxis->CalcPosValue(max_h - h_offset - 9*diag_h2), R,
        common_family.fiz, common_family.emoz,
        common_family. intell, common_family.tonk );
  }
}
//---------------------------------------------------------------------------

void __fastcall TdialogForm::SpeedButton2Click(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------

void __fastcall TdialogForm::SpeedButton3Click(TObject *Sender)
{
if ( FileExists ( "help\\bio.hlp" ) )
  {
  Application->HelpFile = "help\\bio.hlp";
  Application->HelpContext( 30 );
  }
  else
  {
  ShowMessage( "�� ������ ���� ������� help\\bio.hlp");
  }
}
//---------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------
void __fastcall TdialogForm::SpeedButton4Click(TObject *Sender)
{
 if ( PrintDialog1->Execute() )
  {
  // ���������� ��������:
  Printer()->Orientation = poPortrait;
  Screen->Cursor = crHourGlass;
    // ������ ������:
    try
    {
    Printer()->BeginDoc();
    float  w = Chart1->ClientWidth;
    float  h = Chart1->ClientHeight;
    float  aspect_ratio = w/h;
     // ��������� ����. ���������� ������ � ������ �������� ��
     // �������� ��������
     if ( ( w < Printer()->PageWidth ) && ( h < Printer()->PageHeight) )
     {
       if ( w < h )
       {
       h = Printer()->PageHeight;
       w = h*aspect_ratio;
       }
       else
       {
       w = Printer()->PageWidth;
       h = w/aspect_ratio;
       }
     }
     if ( w > Printer()->PageWidth )
     {
     w = Printer()->PageWidth;
     h = w/aspect_ratio;
     }
     if ( h > Printer()->PageHeight)
     {
     h = Printer()->PageHeight;
     w = h*aspect_ratio;
     }
  // ������ Chart
  //Chart1->MarginTop = 5;

  Chart1->PrintPartial( Rect( 20, 50, (int)w + 20 , (int)h + 50 ) );

  Printer()->EndDoc(); // ����� ������;
  }
  catch (...)
  {
  // just in case an error happens
  Printer()->Abort();
  Printer()->EndDoc();
  throw;       // <-- rethrow the exception !!!
  }
  Screen->Cursor = crDefault; // <-- restore cursor
 }

}
 //---------------------------------------------------------------------------
 void __fastcall TdialogForm::CheckBox_maleClick(TObject *Sender)
 {
 CheckBox_female->Checked = FALSE;
 }
 //---------------------------------------------------------------------------
 void __fastcall TdialogForm::CheckBox_femaleClick(TObject *Sender)
 {
 CheckBox_male->Checked = FALSE;
 }
 //---------------------------------------------------------------------------
 // ���������� �����
 //---------------------------------------------------------------------------
 void __fastcall TdialogForm::SpeedButton5Click(TObject *Sender)
 {
  if ( SaveDialog1->Execute() )  
  {
    // �������� ������������ ����?:
    if (FileExists(SaveDialog1->FileName))
    {
      if ( MessageDlg("���� " + SaveDialog1->FileName +" ����������, ��������?",
                       mtConfirmation,
                       TMsgDlgButtons() << mbYes << mbNo, 0) == mrNo )
      goto ex;
    }
  ofstream out( SaveDialog1->FileName.c_str() );
    // �������� �� ������� ������:
    if ( !out )
    {
    ShowMessage("�������� ������");
    goto ex;
    }
    for ( int x = 0; x < StringGrid1->ColCount; x++ )
    {
      for ( int y = 0; y < StringGrid1->RowCount; y++ )
      {
      out << " " << StringGrid1->Cells[x][y].c_str() <<"\n";
      }
    }
  out.close();
  } // if ( SaveDialog1->Execute ...   file
ex: ;
 }
 //---------------------------------------------------------------------------
 // �������� � ������ �����
 //---------------------------------------------------------------------------
 void __fastcall TdialogForm::SpeedButton6Click(TObject *Sender)
 {
  if ( OpenDialog1->Execute() )
  {
    // ���������� ����?:
    if ( FileExists( OpenDialog1->FileName ) == FALSE )
    {
    ShowMessage("���� " + OpenDialog1->FileName +" �� ����������");
    goto ex;
    }
    try
    {
    fstream in;
    in.open( OpenDialog1->FileName.c_str() );
     for ( int x = 0; x < StringGrid1->ColCount; x++ )
     {
      for ( int y = 0; y < StringGrid1->RowCount; y++ )
      {
      char *temp_str = "";
      in.getline( temp_str, 20 );
      StringGrid1->Cells[x][y] =  Trim( temp_str );
        // ���������� ������� lacuna:
        if ( ( y > 0 ) && ( CompareStr( temp_str, "" ) != 0 ) )
        {
         switch (x)
         {
         // 1-� ������� - ���� ����.
         case 1: if ( strlen( temp_str ) > 4 )
                 {
                 //ShowMessage( temp_str );
                 family[y].lacuna = FALSE;
                 family[y].birth = StrToDate( temp_str );
                 }
                 break;
         // 2-� ������� - ���:
         case 2:  if ( CompareStr( temp_str, "�" ) != 0 )
                  family[y].sex = Trim( temp_str );
                  if ( CompareStr( temp_str, "�" ) != 0 )
                  family[y].sex = Trim( temp_str );
                  break;
         // 3-� ������� - ���:
         case 3:  family[y].FIO = temp_str;
         }// case ...
        } // if ( ( y > ...
      } // for ( int y...
     }
  in.close();
  dialogForm->Caption = OpenDialog1->FileName;
    }
    catch(...)
    {
    ShowMessage("���� ��� ������ ������ �� �����");
    }
  Series1->Clear();
  Refresh_date( Sender );
 } // if ( OpenDialog1...
ex: ;
 }
//---------------------------------------------------------------------------

void __fastcall TdialogForm::SpeedButton7Click(TObject *Sender)
{
Clear_data( Sender );
dialogForm->Caption = " ";
SpeedButton1Click( Sender );
}
//---------------------------------------------------------------------------

