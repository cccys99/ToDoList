#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrame* mainframe = new MainFrame("To-Do List");
    mainframe->SetClientSize(800, 600);
    mainframe->CenterOnScreen();
    mainframe->Show();
    return true;
}
