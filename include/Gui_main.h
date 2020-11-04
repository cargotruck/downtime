//
// Gui_main.h
//

#include <wx/wx.h>
#include "./Main_win.h"

class Application : public wxApp
{
public:
    Application();
    ~Application();

    virtual bool OnInit();

private:
    Main_win* main_win = nullptr;
};
