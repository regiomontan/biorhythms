//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "mainU.h"
#include <math.h>
#include "conv_dateU.h"
#include "dialogU.h"
#include "aboutU.h"
#include <Printers.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TmainForm  *mainForm;
String      procent_fiz,procent_emo, procent_tnk, procent_int, procent_mid;
TDateTime   min_date, max_date;  // интервал вывода графиков
double      v_pos;      // позиция полос диаграмм;
double      h = 0;      // расстояние меж полосами диаграммы;
double      hh = 0;     // ширина полосы диаграммы;
TDateTime   segment;
double      width_bar;
BOOLEAN     SERIES_18 = FALSE; // флаг создания серий 1 - 8;
//---------------------------------------------------------------------------
__fastcall TmainForm::TmainForm(TComponent* Owner)
        : TForm(Owner)
{
}

void __fastcall TmainForm::ClearSeries(TObject *Sender)
{
  for(int i = 0; i <= Chart1 -> SeriesCount() - 1; i++ )
  Chart1 -> Series[i] -> Clear();
}
//---------------------------------------------------------------------------
void __fastcall TmainForm::DateTimePicker0Change(TObject *Sender)
{
ClearSeries( Sender );
SERIES_18 = FALSE;
}
//---------------------------------------------------------------------------
// при изменении текущей даты:
//---------------------------------------------------------------------------
void __fastcall TmainForm::DateTimePicker1Change(TObject *Sender)
{
ClearSeries( Sender );
min_date = DateTimePicker1 -> Date - 19;
max_date = DateTimePicker1 -> Date + 19;
Chart1 -> BottomAxis -> SetMinMax( min_date, max_date ) ;
SERIES_18 = FALSE;
}
//---------------------------------------------------------------------------
// дополнительные серединные горизонтальные оси  для синусоид,
// "ящики" для диаграмм и штриховка
//---------------------------------------------------------------------------
// вспомогательная процедура - мужской половой знак:
void man_signum( TCanvas3D *canva, int r, int x0, int y0 )
{
canva->Ellipse( x0 - r, y0 - r, x0 + r, y0 + r );
canva->MoveTo( x0, y0 );
canva->LineTo( x0 + 2*r, y0 - 2*r );
canva->LineTo( x0 + 1.7*r, y0 - r );
canva->MoveTo( x0 + 2*r, y0 - 2*r );
canva->LineTo( x0 + r, y0 - 1.7*r );
}
// вспомогательная процедура - женский половой знак:
void woman_signum( TCanvas3D *canva, int r, int x0, int y0 )
{
canva->Ellipse( x0 - r, y0 - r, x0 + r, y0 + r );
canva->MoveTo( x0, y0 );
canva->LineTo( x0 + 2*r, y0 - 2*r );
canva->MoveTo( x0 + 1.7*r, y0 - r );
canva->LineTo( x0 + r, y0 - 1.7*r );
}

void __fastcall TmainForm::Chart1AfterDraw(TObject *Sender)
{
  for ( int i = 4; i >= 1; i-- )
  {
  // оси:
  Chart1->BottomAxis->CustomDraw( Chart1->ChartRect.Bottom + 5, 0,
                                  Chart1->LeftAxis->CalcYPosValue(hh/2 + hh*i),
                                  FALSE );
  // разделительные линии:
  Chart1->Canvas->DoHorizLine( Chart1->ChartRect.Left, Chart1->ChartRect.Right,
                               Chart1->LeftAxis->CalcYPosValue( hh*i ) );
  }
// разметка для диаграмм("ящики"):
Chart1->Canvas->Rectangle(Chart1->BottomAxis->CalcXPosValue( min_date + 3 ),
                          Chart1->LeftAxis->CalcYPosValue(v_pos + width_bar/2),
                          Chart1->BottomAxis->CalcXPosValue(min_date + 3 + segment),
                          Chart1->LeftAxis->CalcYPosValue(v_pos - 3*h - width_bar/2));
Chart1->Canvas->Rectangle(Chart1->BottomAxis->CalcXPosValue( min_date + 3 ),
                          Chart1->LeftAxis->CalcYPosValue(v_pos - 3*h - width_bar),
                          Chart1->BottomAxis->CalcXPosValue(min_date + 3 + segment),
                          Chart1->LeftAxis->CalcYPosValue(v_pos - 6*h - 2*width_bar));
if ( SERIES_18 )
{
// "половые знаки":
 if ( CheckBox1_male->Checked )
 man_signum( Chart1->Canvas,
              7,
              Chart1->BottomAxis->CalcXPosValue(min_date + 1.5),
              Chart1->LeftAxis->CalcYPosValue(v_pos + width_bar/2 - 1.9*h)
              );
 if ( CheckBox2_male->Checked )
 man_signum( Chart1->Canvas,
              7,
              Chart1->BottomAxis->CalcXPosValue(min_date + 1.5),
              Chart1->LeftAxis->CalcYPosValue(v_pos + width_bar/2 - 6.9*h)
              );
 if ( CheckBox1_female->Checked )
 woman_signum(Chart1->Canvas,
              7,
              Chart1->BottomAxis->CalcXPosValue(min_date + 1.5),
              Chart1->LeftAxis->CalcYPosValue(v_pos + width_bar/2 - 1.9*h)
              );
 if ( CheckBox2_female->Checked )
 woman_signum(Chart1->Canvas,
              7,
              Chart1->BottomAxis->CalcXPosValue(min_date + 1.5),
              Chart1->LeftAxis->CalcYPosValue(v_pos + width_bar/2 - 6.9*h)
              );
// конец вывода "половых знаков".
// штриховка областей:
TDateTime   x = min_date, x0;
 while ( x <= max_date )
 {
 // физический:
 float ax = hh/2 + hh*4; // положение осей;
 double y1  = hh*4.5 + ( hh/2 - 2 )*
              sin( (double)(2.0*PI*(x - DateTimePicker0->Date))/23.0);
 double y2  = hh*4.5 + ( hh/2 - 2 )*
              sin( (double)(2.0*PI*(x - DateTimePicker00->Date))/23.0);
 Chart1->Canvas->Pen->Color = clRed;
   if ( fabs(y1 - y2) < 50 )
   {
   double yy = ax;
     if ( (y1 >= ax) && (y2 >= ax) )   yy = ( y1 < y2 ) ? y1 : y2;
     if ( (y1 <= ax) && (y2 <= ax) )   yy = ( y1 > y2 ) ? y1 : y2;
   Chart1->Canvas->MoveTo( Series1->CalcXPosValue( x ),
                           Series1->CalcYPosValue( yy ) );
   Chart1->Canvas->LineTo( Series1->CalcXPosValue( x ),
                           Chart1->LeftAxis->CalcYPosValue( ax) );
   }
 // эмоциональный:
 ax = hh/2 + hh*3;
 y1  = hh*3.5 + (hh/2 - 2)*sin( (double)(2.0*PI*(x - DateTimePicker0->Date))/28.0);
 y2  = hh*3.5 + (hh/2 - 2)*sin( (double)(2.0*PI*(x - DateTimePicker00->Date))/28.0);
 Chart1->Canvas->Pen->Color = RGB(239, 177, 16);
   if ( fabs(y1 - y2) < 50 )
   {
   double yy = ax;
     if ( (y1 >= ax) && (y2 >= ax) )   yy = ( y1 < y2 ) ? y1 : y2;
     if ( (y1 <= ax) && (y2 <= ax) )   yy = ( y1 > y2 ) ? y1 : y2;
   Chart1->Canvas->MoveTo( Series1->CalcXPosValue( x ),
                           Series1->CalcYPosValue( yy ) );
   Chart1->Canvas->LineTo( Series1->CalcXPosValue( x ),
                           Chart1->LeftAxis->CalcYPosValue(ax) );
   }
 // интеллектуальный:
 ax = hh/2 + hh*2;
 y1  = hh*2.5 + (hh/2 - 2)*sin( (double)(2.0*PI*(x - DateTimePicker0->Date))/33.0);
 y2  = hh*2.5 + (hh/2 - 2)*sin( (double)(2.0*PI*(x - DateTimePicker00->Date))/33.0);
 Chart1->Canvas->Pen->Color = clNavy;
   if ( fabs(y1 - y2) < 50 )
   {
   double yy = ax;
     if ( (y1 >= ax) && (y2 >= ax) )   yy = ( y1 < y2 ) ? y1 : y2;
     if ( (y1 <= ax) && (y2 <= ax) )   yy = ( y1 > y2 ) ? y1 : y2;
   Chart1->Canvas->MoveTo( Series1->CalcXPosValue( x ),
                           Series1->CalcYPosValue( yy ) );
   Chart1->Canvas->LineTo( Series1->CalcXPosValue( x ),
                           Chart1->LeftAxis->CalcYPosValue(ax) );
   }
 // тонкие чувства:
 ax = hh/2 + hh;
 y1  = hh*1.5 + (hh/2 - 2)*sin( (double)(2.0*PI*(x - DateTimePicker0->Date))/38.0);
 y2  = hh*1.5 + (hh/2 - 2)*sin( (double)(2.0*PI*(x - DateTimePicker00->Date))/38.0);
 Chart1->Canvas->Pen->Color = clGreen;
   if ( fabs(y1 - y2) < 50 )
   {
   double yy = ax;
     if ( (y1 >= ax) && (y2 >= ax) )   yy = ( y1 < y2 ) ? y1 : y2;
     if ( (y1 <= ax) && (y2 <= ax) )   yy = ( y1 > y2 ) ? y1 : y2;
   Chart1->Canvas->MoveTo( Series1->CalcXPosValue( x ),
                           Series1->CalcYPosValue( yy ) );
   Chart1->Canvas->LineTo( Series1->CalcXPosValue( x ),
                           Chart1->LeftAxis->CalcYPosValue(ax) );
   }
 x += 0.2;
 } //  while ( x <= max_date )...
Chart1->Canvas->Pen->Color = clBlack;
} // if ( SERIES_18 )...
}
//---------------------------------------------------------------------------
//  "исправляет" текст этикеток на Oy ( отключено )
//---------------------------------------------------------------------------
void __fastcall TmainForm::Chart1GetAxisLabel( TChartAxis *Sender,
                   TChartSeries *Series, int ValueIndex, AnsiString &LabelText)
{
/*
  if ( Sender == Chart1 -> LeftAxis )
  {
  float x0 = 400;
   while ( x0 >= 100 )
   {
   float  x1 = x0 + 100, x = x0;
    while ( x <= x1 )
    {
     if   ( StrToFloat( LabelText ) == x )
     {
     LabelText = FloatToStr( 200/(x1 - x0)*(x - (x1 + x0)/2) );
     if ( LabelText == "-100" ) LabelText = "100";
     }
    x += 5;
    } // while ( x <= x1 )...
   x0 -= 100;
   } // while
  } // if ( Sender ...
*/  
}
//---------------------------------------------------------------------------
// инициализация
//---------------------------------------------------------------------------
void __fastcall TmainForm::FormCreate(TObject *Sender)
{
DateTimePicker1->Date = Date(); // текущяя дата
Chart1->BottomAxis->Increment = DateTimeStep[ dtOneDay ] ;
min_date = DateTimePicker1->Date - 19;
max_date = DateTimePicker1->Date + 19;
Chart1->BottomAxis->Automatic = FALSE;
Chart1->BottomAxis->SetMinMax( min_date, max_date ) ;
}
//---------------------------------------------------------------------------
//  перевод мышиных координат в реальные
//---------------------------------------------------------------------------
void __fastcall TmainForm::Chart1MouseMove(TObject *Sender,
                              TShiftState Shift, int X, int Y)
{
 StatusBar1->SimpleText = "  " + DateToStr( Series1->XScreenToValue(X));
/*
float  y = Series1->YScreenToValue(Y);
 if ( (y < 100) && (y > -100) )
 {
 StatusBar1->SimpleText =  "  "
               + DateToStr( Series1->XScreenToValue(X)) + "    "
               + FloatToStrF(y, ffFixed, 3, 0 ) +" %";
  }
  else
  {
  StatusBar1->SimpleText =  "  "
                + DateToStr( Series1 ->  XScreenToValue(X) );
  }
*/
}
//---------------------------------------------------------------------------
void __fastcall TmainForm::Panel1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
StatusBar1->SimpleText =  "  ";
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::Panel2MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
StatusBar1->SimpleText =  "  ";
}
//---------------------------------------------------------------------------


void __fastcall TmainForm::DateTimePicker00Change(TObject *Sender)
{
ClearSeries( Sender );
SERIES_18 = FALSE;
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::SpeedDiagrClick(TObject *Sender)
{
dialogForm -> Show();
}
//---------------------------------------------------------------------------
// вывод графиков
//---------------------------------------------------------------------------
void __fastcall TmainForm::SpeedGrafikClick(TObject *Sender)
{
ClearSeries( Sender );
Boolean FLAG0 = FALSE, FLAG00 = FALSE;  // флаги персон
Chart1->Canvas->Font->Size = Chart1->BottomAxis->LabelsFont->Size;
Chart1->Foot->Text->Text = "биоритмы ";
// первая персона:
TDateTime  D = DateTimePicker0->Date;
double     y;
hh = Chart1->LeftAxis->Maximum/5;  // ширина полос для графиков;
  while ( D <= max_date + 25 )
  {
    if ( ( D >= min_date - 25) && ( D >= DateTimePicker0 -> Date ) )
    {
     FLAG0 = TRUE;
     double arg_bio = (double)(2.0*PI*(D - DateTimePicker0 -> Date));
     // физический цикл:
     y =  hh*4.5 + ( hh/2 - 2 )*sin( arg_bio/23.0 );
     Series1->AddXY( (double)D, y, "", clRed );
     // эмоциональный:
     y =  hh*3.5  + ( hh/2 - 2 )*sin( arg_bio/28.0 );
     Series2 -> AddXY( (double)D, y, "", RGB(239, 177, 16) );
     // интеллектуальный:
     y =  hh*2.5  + ( hh/2 - 2 )*sin( arg_bio/33.0 );
     Series3 -> AddXY( (double)D, y, "", clNavy );
     // тонкие чувства:
     y =  hh*1.5  + ( hh/2 - 2 )*sin( arg_bio/38.0 );
     Series4 -> AddXY( (double)D, y, "", clGreen );

    }
  D++; //   while ( D <= max_date + 25 )...
  }
// вторая персона:
D = DateTimePicker00->Date;
  while ( D <= max_date + 25 )
  {
    if ( ( D >= min_date - 25) && ( D >= DateTimePicker00 -> Date ) )
    {
     FLAG00 = TRUE;
     double arg_bio = (double)(2.0*PI*(D - DateTimePicker00 -> Date));
     // физический цикл:
     y =  hh*4.5 + ( hh/2 - 2 )*sin( arg_bio/23.0 );
     Series5 -> AddXY( (double)D, y, "", clRed );
     // эмоциональный:
     y =  hh*3.5  + ( hh/2 - 2 )*sin( arg_bio/28.0 );
     Series6 -> AddXY( (double)D, y, "", RGB(239, 177, 16) );
     // интеллектуальный:
     y =  hh*2.5  + ( hh/2 - 2 )*sin( arg_bio/33.0 );
     Series7 -> AddXY( (double)D, y, "", clNavy );
     // тонкие чувства:
     y =  hh*1.5  + ( hh/2 - 2 )*sin( arg_bio/38.0 );
     Series8 -> AddXY( (double)D, y, "", clGreen );
    }
  D++;
  }
// диаграмма процентной совместимости:
v_pos = 0.8*hh; // позиция начала полос диаграмм - лев. верхн. угол;
h = hh/10; // расстояние меж полосами диаграммы;
TDateTime  t1 =  min_date + 3;  // нач. значение;
TDateTime  t2 =  DateTimePicker1->Date; // конечное значение - текущяя дата;
Series9->YOrigin = t1;
segment  = t2 - t1;
width_bar = 0.1*hh;
Series9->CustomBarHeight = width_bar; // ширина полосы диаграммы;
TDateTime D2 = DateTimePicker00 -> Date;
TDateTime D1 = DateTimePicker0 -> Date;
// проценты совметимости, вывод диаграмм:
float  p_fiz = percent( D1,D2, 23 );
procent_fiz = FloatToStrF(p_fiz, ffFixed, 3, 0) + "%";
TDateTime  segment_fiz = t1 + (double)segment*p_fiz/100.0;

float   p_emo = percent( D1,D2, 28 );
procent_emo = FloatToStrF(p_emo, ffFixed, 3, 0) + "%";
TDateTime  segment_emo = t1 + (double)segment*p_emo/100.0;

float  p_int = percent( D1,D2, 33 );
procent_int = FloatToStrF(p_int, ffFixed, 3, 0) + "%";
TDateTime  segment_int = t1 + (double)segment*p_int/100.0;

float p_tnk = percent( D1,D2, 38 );
procent_tnk = FloatToStrF(p_tnk, ffFixed, 3, 0) + "%";
TDateTime  segment_tnk = t1 + (double)segment*p_tnk/100.0;

float p_mid = ( p_fiz + p_emo + p_int + p_tnk )/4;
procent_mid = FloatToStrF(p_mid, ffFixed, 3, 0) + "%";
TDateTime  segment_mid = t1 + (double)segment*p_mid/100.0;

// кто "ведущий"? и вывод диаграмм - если ведущий 1-й, то соотв.
// диаграмма выводится в верхнем ящике, в противном случае - во втором
double    y_pos = 0;
TDateTime s_p;
 if ( D1 < D2 )
 {
 s_p =  D1;
   // физический:
   s_p =  D1;
   while ( s_p <= D2 )  s_p += 23.0;
   if   ( (double)(s_p - D2) > 23.0/2 ) y_pos = v_pos;
   else                                 y_pos = v_pos - 1.5*width_bar - 3*h;
   Series9->AddXY(segment_fiz, y_pos,"", clRed );
   // эмоциональный:
   s_p =  D1;
   while ( s_p <= D2 )  s_p += 28.0;
   if   ( (double)(s_p - D2) > 28.0/2 ) y_pos = v_pos - h;
   else                                 y_pos = v_pos - h - 1.5*width_bar - 3*h;
   Series9->AddXY(segment_emo, y_pos,   "", RGB(239,177,16) );
   // интеллектуальный:
   s_p =  D1;
   while ( s_p <= D2 )  s_p += 33.0;
   if   ( (double)(s_p - D2) > 33.0/2 ) y_pos = v_pos - 2*h;
   else                                 y_pos = v_pos - 2*h - 1.5*width_bar - 3*h;
   Series9->AddXY(segment_int, y_pos, "", clNavy );
   // тонкочувственный:
   s_p =  D1;
   while ( s_p <= D2 )  s_p += 38.0;
   if   ( (double)(s_p - D2) > 38.0/2 ) y_pos = v_pos - 3*h;
   else                                 y_pos = v_pos - 3*h - 1.5*width_bar - 3*h;
   Series9->AddXY(segment_tnk, y_pos, "", clGreen );
 }
 if ( D1 > D2 )
 {
 s_p = D2;
   // физический:
   s_p =  D2;
   while ( s_p <= D1 )  s_p += 23.0;
   if   ( (double)(s_p - D1) > 23.0/2 ) y_pos = v_pos - 1.5*width_bar - 3*h;
   else                                 y_pos = v_pos;
   Series9->AddXY(segment_fiz, y_pos,"", clRed );
   // эмоциональный:
   s_p =  D2;
   while ( s_p <= D1 )  s_p += 28.0;
   if   ( (double)(s_p - D1) > 28.0/2 ) y_pos = v_pos - h - 1.5*width_bar - 3*h;
   else                                 y_pos = v_pos - h;
   Series9->AddXY(segment_emo, y_pos,   "", RGB(239,177,16) );
   // интеллектуальный:
   s_p =  D2;
   while ( s_p <= D1 )  s_p += 33.0;
   if   ( (double)(s_p - D1) > 33.0/2 ) y_pos = v_pos - 2*h - 1.5*width_bar - 3*h;
   else                                 y_pos = v_pos - 2*h;
   Series9->AddXY(segment_int, y_pos, "", clNavy );
   // тонкочувственный:
   s_p =  D2;
   while ( s_p <= D1 )  s_p += 38.0;
   if   ( (double)(s_p - D1) > 38.0/2 ) y_pos = v_pos - 3*h - 1.5*width_bar - 3*h;
   else                                 y_pos = v_pos - 3*h;
   Series9->AddXY(segment_tnk, y_pos, "", clGreen );
 }
 if  ( D1 == D2 )
 {
 Series9->AddXY(segment_fiz, v_pos, "", clRed );
 Series9->AddXY(segment_fiz, v_pos - 1.5*width_bar - 3*h,"", clRed );
 Series9->AddXY(segment_emo, v_pos - h, "", RGB(239,177,16) );
 Series9->AddXY(segment_emo, v_pos - h - 1.5*width_bar - 3*h,"",RGB(239,177,16));
 Series9->AddXY(segment_int, v_pos - 2*h, "", clNavy );
 Series9->AddXY(segment_int, v_pos - 2*h - 1.5*width_bar - 3*h,"", clNavy );
 Series9->AddXY(segment_fiz, v_pos - 3*h,"", clGreen );
 Series9->AddXY(segment_fiz, v_pos - 3*h - 1.5*width_bar - 3*h,"", clGreen );
 }
// конец определения "ведущего".

// среднее значение:
Series9->AddXY(segment_mid, v_pos - 9*h, "", clGray );
// поясняющий текст для графиков:
  if ( FLAG0 && FLAG00)
  {
  Chart1->Title->Text->Text = "биосовместимость "
                             + DateToStr( DateTimePicker0 -> Date )
                             + " и " + DateToStr( DateTimePicker00 -> Date )
                             + ", расчитанная "
                             + DateToStr( DateTimePicker1 -> Date );
  CheckBox1->Checked = TRUE;
  }
  else
  {
     if ( FLAG0)
     Chart1->Title->Text->Text = "биоритмы "
                                + DateToStr( DateTimePicker0 -> Date )
                                + ", расчитанные "
                                + DateToStr( DateTimePicker1 -> Date );
     if ( FLAG00)
     Chart1->Title->Text->Text = "биоритмы "
                                + DateToStr( DateTimePicker00 -> Date )
                                + ", расчитанные "
                                + DateToStr( DateTimePicker1 -> Date );
  CheckBox1->Checked = FALSE;
  }
  // установка флага для штриховки:
  //( не сработает при датах, близких к дням рождений )
  if  (    (DateTimePicker00->Date < min_date )
        && ( DateTimePicker0->Date < min_date ) )  SERIES_18 = TRUE;
  else                                             SERIES_18 = FALSE;
}
//---------------------------------------------------------------------------


void __fastcall TmainForm::menuAboutClick(TObject *Sender)
{
AboutBox -> Show();
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::menuExitClick(TObject *Sender)
{
Close();        
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::menuHelpClick(TObject *Sender)
{
  if ( FileExists ( "help\\bio.hlp" ) )
  {
  Application->HelpFile = "help\\bio.hlp";
  Application->HelpCommand(HELP_CONTENTS, 0);
  }
  else
  {
  ShowMessage( "не найден файл справки help\\bio.hlp");
  }
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::CheckBox1Click(TObject *Sender)
{
Series9 -> Active = !( Series9 -> Active );
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::helpButtonClick(TObject *Sender)
{
if ( FileExists ( "help\\bio.hlp" ) )
  {
  Application->HelpFile = "help\\bio.hlp";
  Application->HelpContext( 20 );
  }
  else
  {
  ShowMessage( "не найден файл справки help\\bio.hlp");
  }
}
//---------------------------------------------------------------------------
// печать
//---------------------------------------------------------------------------
void __fastcall TmainForm::printButtonClick(TObject *Sender)
{
  if ( PrintDialog1->Execute() )
  {
  // подготовка принтера:
  Printer()->Orientation = poPortrait;
  Screen->Cursor = crHourGlass;
    // начало печати:
    try
    {
    Printer()->BeginDoc();
    float  w = Chart1->ClientWidth;
    float  h = Chart1->ClientHeight;
    float  aspect_ratio = w/h;
     // установка макс. допустимых высоты и ширины картинки на
     // странице принтера
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
  // печать Chart
  Chart1->PrintPartial( Rect( 20, 50, (int)w + 20, (int)h + 50) );
  Printer()->EndDoc(); // конец печати;
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


void __fastcall TmainForm::Series9AfterDrawValues(TObject *Sender)
{
Chart1->Canvas->TextOut( Series9->CalcXPos(0), Series9->CalcYPos(0)-2,
                         " " + procent_fiz);
Chart1->Canvas->TextOut(Series9->CalcXPos(1), Series9->CalcYPos(1)-2,
                         " " + procent_emo);
Chart1->Canvas->TextOut(Series9->CalcXPos(2), Series9->CalcYPos(2)-2,
                         " " + procent_int);
Chart1->Canvas->TextOut(Series9->CalcXPos(3), Series9->CalcYPos(3)-2,
                         " " + procent_tnk);
Chart1->Canvas->TextOut(Series9->CalcXPos(4), Series9->CalcYPos(4)-2,
                         " " + procent_mid);
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::Series1Click(TChartSeries *Sender,
      int ValueIndex, TMouseButton Button, TShiftState Shift, int X, int Y)
{
//ShowMessage( "!! " + FloatToStr( Sender->CalcYPosValue(Y) - 450) );
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::CheckBox1_maleClick(TObject *Sender)
{
CheckBox1_female->Checked = FALSE;
ClearSeries( Sender );
SERIES_18 = FALSE;
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::CheckBox1_femaleClick(TObject *Sender)
{
CheckBox1_male->Checked = FALSE;
ClearSeries( Sender );
SERIES_18 = FALSE;       
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::CheckBox2_maleClick(TObject *Sender)
{
CheckBox2_female->Checked = FALSE;
ClearSeries( Sender );
SERIES_18 = FALSE;
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::CheckBox2_femaleClick(TObject *Sender)
{
CheckBox2_male->Checked = FALSE;
ClearSeries( Sender );
SERIES_18 = FALSE;        
}
//---------------------------------------------------------------------------

