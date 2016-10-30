//---------------------------------------------------------------------------
#ifndef dialogUH
#define dialogUH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <BubbleCh.hpp>
#include <Dialogs.hpp>
#include <TeeShape.hpp>
//---------------------------------------------------------------------------
class TdialogForm : public TForm
{
__published:	// IDE-managed Components
        TPanel          *Panel1;
        TStringGrid *StringGrid1;
        TButton *ToListBtn;
        TDateTimePicker *DTPicker_birth;
        TChart *Chart1;
        TPrintDialog *PrintDialog1;
        TPanel *Panel2;
        TSpeedButton *SpeedButton3;
        TSpeedButton *SpeedButton4;
        TSpeedButton *SpeedButton2;
        TSpeedButton *SpeedButton1;
        TRadioGroup *RadioGroup1;
        TCheckBox *CheckBox_male;
        TCheckBox *CheckBox_female;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TSpeedButton *SpeedButton6;
        TSpeedButton *SpeedButton5;
        TSpeedButton *SpeedButton7;
        TBarSeries *Series1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall ToListBtnClick(TObject *Sender);
        void __fastcall StringGrid1SetEditText(TObject *Sender, int ACol,
                                   int ARow, const AnsiString Value);
        void __fastcall SpeedButton1Click(TObject *Sender);
        void __fastcall Chart1AfterDraw(TObject *Sender);
        void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall SpeedButton3Click(TObject *Sender);
        void __fastcall SpeedButton4Click(TObject *Sender);
        void __fastcall CheckBox_maleClick(TObject *Sender);
        void __fastcall CheckBox_femaleClick(TObject *Sender);
        void __fastcall SpeedButton5Click(TObject *Sender);
        void __fastcall SpeedButton6Click(TObject *Sender);
        void __fastcall SpeedButton7Click(TObject *Sender);
private:	// User declarations
        void __fastcall Refresh_date(TObject *Sender);
        void __fastcall Diagramma( int cy, TDateTime D1, TDateTime D2,
                                   int numb1, int numb2 );
        void __fastcall Clear_data(TObject *Sender);
        void __fastcall Krug( int cx, int cy, int R,
                              float fiz, float emoz,
                              float intell, float tonk );
public:		// User declarations

        __fastcall TdialogForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TdialogForm *dialogForm;
//---------------------------------------------------------------------------
#endif
