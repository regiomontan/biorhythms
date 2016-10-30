//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "conv_dateU.h"
#include <math.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// ѕреобразование григорианской даты в пор€дковый номер
// дн€ юлианского периода
//---------------------------------------------------------------------------
 unsigned long DateToLong(int day, int month, int year)
 {
   if(month>2)
   {
   month-=3;
   }
   else
   {
   month+=9;
   year--;
   }
long c  = year/100l;
long ya = year-100l*c;
return (146097l*c)/4l+(1461l*ya)/4l+(153l*month+2l)/5l+day+1721119l;
}
//---------------------------------------------------------------------------
// вычисл€ет процент совместимости
// D1, D2  - дни рождени€,
// period  - период( 23, 33 ... дней )
//---------------------------------------------------------------------------
float  percent( TDateTime D1, TDateTime D2, int period )
{
unsigned  short  year, m, d;
unsigned long    d1, d2;
DecodeDate( D1, year, m, d );
d1 = DateToLong( d, m, year );
DecodeDate( D2,  year, m, d );
d2 = DateToLong( d, m, year );
int delta_days = d1 - d2;

float  b = abs( delta_days ) % period;
 if ( b > period/2.0 )  b = period - b;
return  ( 100*(period - 2*b)/period );
}
//---------------------------------------------------------------------------

