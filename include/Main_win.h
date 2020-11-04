//
// Main_win.h
//

#ifndef MAIN_WIN_H
#define MAIN_WIN_H


#include <string>
#include <sstream>
#include <vector>

#include <wx/datectrl.h>
#include <wx/listctrl.h>
#include <wx/wx.h>

#include "Poco/Process.h"

#include "./Address_book_win.h"
#include "./Log_win.h"
#include "./Preferences_win.h"


class Main_win : public wxFrame
{
public:
    Main_win(const wxString& title,const wxPoint& pos,const wxSize& size);
    ~Main_win();

    //Main win event handlers
    void on_open_address_book(wxCommandEvent& event);
    void on_open_logs(wxCommandEvent& event);
    void on_open_preferences(wxCommandEvent& event);
    void on_exit(wxCommandEvent& event);
    void on_btn_start(wxCommandEvent& event);
    void on_btn_stop(wxCommandEvent& event);
    void on_btn_restart(wxCommandEvent& event);
    void on_btn_refresh(wxCommandEvent& event);

    //Main win controls
    wxButton* btn_start = nullptr;
    wxButton* btn_stop = nullptr;
    wxButton* btn_restart = nullptr;
    wxButton* btn_refresh = nullptr;
    wxStaticText* txt_status = nullptr;

    //misc
    void set_status();
    void start_srvc();
    void stop_srvc();

private:
    wxMenuBar* menu_bar = nullptr;
    wxMenu* menu_file = nullptr;

    std::string cmd = "systemctl";
    std::string arg_start = "start";
    std::string arg_stop = "stop";
    std::string arg_name = "downtime";
    std::vector<std::string> args;
    std::string pid = "0";
    bool running = false;
    bool just_started = true;

    wxDECLARE_EVENT_TABLE();
};

#endif //MAIN_WIN_H
