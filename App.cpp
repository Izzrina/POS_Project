#include "App.hpp"
#include "MainFrame.hpp"
#include  "SQLiteDB.hpp"

#include <wx/wx.h>

bool App::OnInit()
{
    MainFrame *mainframe = new MainFrame("Nadine's CASH REGISTER");
    mainframe->SetClientSize(1300,800);
    mainframe->Center();
    mainframe->SetMinSize(wxSize(1300, 800));
    mainframe->Show();

    return true;
}

wxIMPLEMENT_APP(App);
