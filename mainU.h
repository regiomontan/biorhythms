//---------------------------------------------------------------------------
#ifndef mainUH
#define mainUH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
//---------------------------------------------------------------------------
class TmainForm : public TForm
{
__published:	// IDE-managed Components
        TChart *Chart1;
        TFastLineSeries *Series2;
        TFastLineSeries *Series3;
        TFastLineSeries *Series4;
        TPanel *Panel2;
        TStatusBar *StatusBar1;
        TFastLineSeries *Series1;
        TLineSeries *Series8;
        TLineSeries *Series7;
        TLineSeries *Series6;
        TLineSeries *Series5;
        THorizBarSeries *Series9;
        TMainMenu *MainMenu1;
        TMenuItem *menuFile;
        TMenuItem *N1;
        TMenuItem *menuExit;
        TMenuItem *N2;
        TMenuItem *menuHelp;
        TMenuItem *menuAbout;
        TCheckBox *CheckBox1;
        TPrintDialog *PrintDialog1;
        TSavePictureDialog *SavePictureDialog1;
        TSpeedButton *helpButton;
        TSpeedButton *printButton;
        TDateTimePicker *DateTimePicker1;
        TSpeedButton *SpeedGrafik;
        TSpeedButton *SpeedDiagr;
        TGroupBox *GroupBox1;
        TDateTimePicker *DateTimePicker0;
        TCheckBox *CheckBox1_male;
        TCheckBox *CheckBox1_female;
        TGroupBox *GroupBox2;
        TDateTimePicker *DateTimePicker00;
        TCheckBox *CheckBox2_female;
        TCheckBox *CheckBox2_male;
        void __fastcall DateTimePicker0Change(TObject *Sender);
        void __fastcall DateTimePicker1Change(TObject *Sender);
        void __fastcall Chart1AfterDraw(TObject *Sender);
        void __fastcall Chart1GetAxisLabel(TChartAxis *Sender,
          TChartSeries *Series, int ValueIndex, AnsiString &LabelText);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Chart1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall Panel1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall Panel2MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall DateTimePicker00Change(TObject *Sender);
        void __fastcall SpeedDiagrClick(TObject *Sender);
        void __fastcall SpeedGrafikClick(TObject *Sender);
        void __fastcall menuAboutClick(TObject *Sender);
        void __fastcall menuExitClick(TObject *Sender);
        void __fastcall menuHelpClick(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall helpButtonClick(TObject *Sender);
        void __fastcall printButtonClick(TObject *Sender);
        void __fastcall Series9AfterDrawValues(TObject *Sender);
        void __fastcall Series1Click(TChartSeries *Sender, int ValueIndex,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall CheckBox1_maleClick(TObject *Sender);
        void __fastcall CheckBox1_femaleClick(TObject *Sender);
        void __fastcall CheckBox2_maleClick(TObject *Sender);
        void __fastcall CheckBox2_femaleClick(TObject *Sender);
private:	// User declarations
        void __fastcall ClearSeries(TObject *Sender);
public:		// User declarations
        __fastcall TmainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TmainForm *mainForm;
//---------------------------------------------------------------------------
#endif
