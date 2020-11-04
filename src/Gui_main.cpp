//
// Gui_main.cpp
//

#include "../include/Gui_main.h"

wxIMPLEMENT_APP(Application);

Application::Application() {};

Application::~Application() {};

bool Application::OnInit()
{
    main_win = new Main_win("Downtime Network Monitor",wxPoint(30,30),wxSize(400,200));
    main_win->Show();

    return true;
}
