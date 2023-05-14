#include "App.hpp"
#include "MainFrame.hpp"
#include  "SQLiteDB.hpp"


#include <wx/wx.h>

wxIMPLEMENT_APP(App);
bool App::OnInit() {



MainFrame* mainFrame = new MainFrame("Rechner");
mainFrame->SetClientSize(1300, 830); // Define size of the window
mainFrame->Center();
mainFrame->Show();
return true;

}
