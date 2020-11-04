//
// Main_win.cpp
//

#include <wx/gbsizer.h>
#include "../include/Main_win.h"

//TODO: something that polls the system and auto refreshes would be better than a button

enum
{
    ID_open_address_book = 10001,
    ID_open_logs,
    ID_open_preferences,
    ID_btn_start,
    ID_btn_stop,
    ID_btn_restart,
    ID_btn_refresh,
    ID_txt_status,
};

Main_win::Main_win(const wxString& title,const wxPoint& pos,const wxSize& size)
    :wxFrame(nullptr,wxID_ANY,title,pos,size)
{
    //setup the menu bar
    menu_bar = new wxMenuBar;
    menu_file = new wxMenu;

    menu_file->Append(ID_open_address_book,"Open &Address Book");
    menu_file->Append(ID_open_logs,"Open &Logs");
    menu_file->Append(ID_open_preferences,"&Preferences");
    menu_file->AppendSeparator();
    menu_file->Append(wxID_EXIT);

    menu_bar->Append(menu_file,"&File");

    SetMenuBar(menu_bar);  

    //Main win controls
    wxBoxSizer* sizer_master = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_v1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_v2 = new wxBoxSizer(wxVERTICAL);

    btn_start = new wxButton(this,ID_btn_start,"START",wxDefaultPosition,wxSize(80,30));
    btn_stop = new wxButton(this,ID_btn_stop,"STOP",wxDefaultPosition,wxSize(80,30));
    btn_restart = new wxButton(this,ID_btn_restart,"RESTART",wxDefaultPosition,wxSize(80,30));
    btn_refresh = new wxButton(this,ID_btn_refresh,"REFRESH",wxDefaultPosition,wxSize(80,30));
    txt_status = new wxStaticText(this,ID_txt_status,"Initialization failed",wxDefaultPosition,wxSize(100,100));

    sizer_v1->Add(btn_start,0,wxEXPAND | wxALIGN_CENTER,5);
    sizer_v1->Add(btn_stop,0,wxEXPAND | wxALIGN_CENTER,5);
    sizer_v1->Add(btn_restart,0,wxEXPAND | wxALIGN_CENTER,5);
    sizer_v1->Add(btn_refresh,0,wxEXPAND | wxALIGN_CENTER,5);
    sizer_v2->Add(txt_status,1,wxALL | wxALIGN_RIGHT | wxALIGN_CENTER,5);
    sizer_master->Add(sizer_v1,0,wxALL | wxALIGN_LEFT,5);
    sizer_master->Add(sizer_v2,1,wxALL | wxALIGN_RIGHT | wxALIGN_CENTER,5);
    
    this->SetSizer(sizer_master);

    //set control defaults
    set_status();
    just_started = false;
}

//-------------------------------------------------------------------------

Main_win::~Main_win() {};

//-------------------------------------------------------------------------
//event handlers

wxBEGIN_EVENT_TABLE(Main_win,wxFrame)
    EVT_MENU(ID_open_address_book,Main_win::on_open_address_book)
    EVT_MENU(ID_open_logs,Main_win::on_open_logs)
    EVT_MENU(ID_open_preferences,Main_win::on_open_preferences)
    EVT_MENU(wxID_EXIT,Main_win::on_exit)
    EVT_BUTTON(ID_btn_start,Main_win::on_btn_start)
    EVT_BUTTON(ID_btn_stop,Main_win::on_btn_stop)
    EVT_BUTTON(ID_btn_restart,Main_win::on_btn_restart)
    EVT_BUTTON(ID_btn_refresh,Main_win::on_btn_refresh)
wxEND_EVENT_TABLE()

//-------------------------------------------------------------------------

void Main_win::on_open_address_book(wxCommandEvent& event)
{
    Address_book_win* abw = new Address_book_win(this,"Address Book",wxSize(675,675));
    abw->Show();
    event.Skip();
}

//-------------------------------------------------------------------------

void Main_win::on_open_logs(wxCommandEvent& event)
{
    Log_win* lw = new Log_win(this,"Logs",wxSize(1000,800));
    lw->Show();
    event.Skip();
}

//-------------------------------------------------------------------------

void Main_win::on_open_preferences(wxCommandEvent& event)
{
    Pref_win* pw = new Pref_win(this,"Preferences",wxSize(500,400));
    pw->SetMinSize(wxSize(500,400));
    pw->SetMaxSize(wxSize(500,400));
    pw->Show();
    event.Skip();
}

//-------------------------------------------------------------------------

void Main_win::on_exit(wxCommandEvent& event)
{
    Close(true);
}

//-------------------------------------------------------------------------

void Main_win::on_btn_start(wxCommandEvent& event)
{
    if(!running) start_srvc();    
}

//-------------------------------------------------------------------------

void Main_win::on_btn_stop(wxCommandEvent& event)
{
    if(running) stop_srvc();
}

void Main_win::on_btn_restart(wxCommandEvent& event)
{
    if(running){
        stop_srvc();
        start_srvc();
    }else{
        start_srvc();
    }
}

void Main_win::on_btn_refresh(wxCommandEvent& event)
{
    set_status();
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

void Main_win::set_status()
{
    std::stringstream ss;

    if(!just_started){
        dtUtil::sleep(5);
    }

    if(!dtUtil::is_running(pid)){
        ss << "Service not running" << std::endl;
        running = false;
    }else{
        ss << "Service is running" << std::endl << "PID: " << pid << std::endl;
        running = true;
    }
    
    txt_status->SetLabel(ss.str());
}

//-------------------------------------------------------------------------

void Main_win::start_srvc()
{
    std::vector<std::string> args{arg_start,arg_name};

    Poco::ProcessHandle ph = Poco::Process::launch(cmd,args);
    set_status();
    
    args.clear();
}

void Main_win::stop_srvc()
{
    std::vector<std::string> args{arg_stop,arg_name};

    Poco::ProcessHandle ph = Poco::Process::launch(cmd,args);
    set_status();
    
    args.clear();
}
