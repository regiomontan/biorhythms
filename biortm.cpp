//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("biortm.res");
USEFORM("mainU.cpp", mainForm);
USEUNIT("conv_dateU.cpp");
USEFORM("dialogU.cpp", dialogForm);
USEFORM("aboutU.cpp", AboutBox);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->HelpFile = "help\\BIO.HLP";
                 Application->CreateForm(__classid(TmainForm), &mainForm);
                 Application->CreateForm(__classid(TdialogForm), &dialogForm);
                 Application->CreateForm(__classid(TAboutBox), &AboutBox);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
