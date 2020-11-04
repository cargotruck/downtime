//
// Preferences_win.cpp
//

#include "../include/Preferences_win.h"

enum{
    ID_email_host = 13001,
    ID_sender,
    ID_recipient,
    ID_interval,
    ID_max_fail,
    ID_log_dir,
    ID_btn_browse,
    ID_btn_okay,
    ID_btn_cancel,
    ID_btn_apply,
};

Pref_win::Pref_win(wxFrame* parent,wxString name,const wxSize& size)
    : wxFrame(parent,wxID_ANY,name,wxDefaultPosition,size)
{
    //setup menubar

    //Pref win sizers
    wxBoxSizer* sizer_v1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_h2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_h3 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_h4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_h5 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_h6 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_h7 = new wxBoxSizer(wxHORIZONTAL);

    //Pref win controls
    txt_email_host = new wxStaticText(this,wxID_ANY,"Email Host:");
    in_email_host = new wxTextCtrl(this,ID_email_host,wxEmptyString,wxDefaultPosition,wxSize(200,30));
    txt_sender = new wxStaticText(this,wxID_ANY,"Send As Email:");
    in_sender = new wxTextCtrl(this,ID_sender,wxEmptyString,wxDefaultPosition,wxSize(200,30));
    txt_recipient = new wxStaticText(this,wxID_ANY,"Recipient Email:");
    in_recipient = new wxTextCtrl(this,ID_recipient,wxEmptyString,wxDefaultPosition,wxSize(200,30));
    txt_interval = new wxStaticText(this,wxID_ANY,"Test Interval (sec):");
    in_interval = new wxTextCtrl(this,
        ID_interval,
        wxEmptyString,
        wxDefaultPosition,
        wxSize(70,30),
        0,
        wxTextValidator(wxFILTER_DIGITS));
    txt_max_fail = new wxStaticText(this,wxID_ANY,"Allowed Failures:");
    in_max_fail = new wxTextCtrl(this,
        ID_max_fail,wxEmptyString,
        wxDefaultPosition,
        wxSize(70,30),
        0,
        wxTextValidator(wxFILTER_DIGITS));
    txt_log_dir = new wxStaticText(this,wxID_ANY,"Log Directory:");
    in_log_dir = new wxTextCtrl(this,ID_log_dir,wxEmptyString,wxDefaultPosition,wxSize(200,30)); 
    btn_browse = new wxButton(this,ID_btn_browse,"Browse",wxDefaultPosition,wxSize(70,30));
    btn_okay = new wxButton(this,ID_btn_okay,"Okay",wxDefaultPosition,wxSize(70,30));
    btn_cancel = new wxButton(this,ID_btn_cancel,"Cancel",wxDefaultPosition,wxSize(70,30));
    btn_apply = new wxButton(this,ID_btn_apply,"Apply",wxDefaultPosition,wxSize(70,30));

    sizer_h1->Add(txt_email_host,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h1->Add(in_email_host,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h2->Add(txt_sender,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h2->Add(in_sender,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h3->Add(txt_recipient,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h3->Add(in_recipient,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h4->Add(txt_interval,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h4->Add(in_interval,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h5->Add(txt_max_fail,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h5->Add(in_max_fail,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h6->Add(txt_log_dir,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h6->Add(in_log_dir,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h6->Add(btn_browse,0,wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT,10);
    sizer_h7->Add(btn_okay,0,wxTOP | wxALIGN_RIGHT,5);
    sizer_h7->Add(btn_cancel,0,wxTOP | wxALIGN_RIGHT,5);
    sizer_h7->Add(btn_apply,0,wxTOP | wxALIGN_RIGHT,5);

    sizer_v1->Add(sizer_h1,0,wxEXPAND | wxALL | wxALIGN_LEFT,10);
    sizer_v1->Add(sizer_h2,0,wxEXPAND | wxALL | wxALIGN_LEFT,10);
    sizer_v1->Add(sizer_h3,0,wxEXPAND | wxALL| wxALIGN_LEFT,10);
    sizer_v1->Add(sizer_h4,0,wxEXPAND | wxALL| wxALIGN_LEFT,10);
    sizer_v1->Add(sizer_h5,0,wxEXPAND | wxALL| wxALIGN_LEFT,10);
    sizer_v1->Add(sizer_h6,0,wxEXPAND | wxALL| wxALIGN_LEFT,10);
    sizer_v1->Add(sizer_h7,0,wxRIGHT | wxTOP | wxALIGN_RIGHT,10);

    this->SetSizer(sizer_v1);

    //set initial values
    Configuration config{dtUtil::read_config_file()};

    in_email_host->SetValue(config.get_host());
    in_sender->SetValue(config.get_sender());
    in_recipient->SetValue(config.get_recipient());
    in_interval->SetValue(std::to_string(config.get_interval()));
    in_max_fail->SetValue(std::to_string(config.get_max_fail()));
    in_log_dir->SetValue(config.get_log_loc());
}

//-------------------------------------------------------------------------

Pref_win::~Pref_win() {};

//-------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(Pref_win,wxFrame)
    EVT_BUTTON(ID_btn_browse,Pref_win::on_btn_browse)
    EVT_BUTTON(ID_btn_okay,Pref_win::on_btn_okay)
    EVT_BUTTON(ID_btn_cancel,Pref_win::on_btn_cancel)
    EVT_BUTTON(ID_btn_apply,Pref_win::on_btn_apply)
wxEND_EVENT_TABLE()

//-------------------------------------------------------------------------

void Pref_win::on_btn_browse(wxCommandEvent& event)
{
    wxFileDialog dlg_choose_dir(this,"Select Log Directory","","","*-downtime.log",wxFD_OPEN);

    if(dlg_choose_dir.ShowModal() == wxID_CANCEL) return;

    in_log_dir->SetValue(dlg_choose_dir.GetDirectory());
}

//-------------------------------------------------------------------------

void Pref_win::on_btn_okay(wxCommandEvent& event)
{
    Configuration new_config{get_values()}; 
    dtUtil::write_config_file(new_config);
    Close(true);
}

//-------------------------------------------------------------------------

void Pref_win::on_btn_cancel(wxCommandEvent& event)
{
    Close(true);
}

//-------------------------------------------------------------------------

void Pref_win::on_btn_apply(wxCommandEvent& event)
{
    Configuration new_config{get_values()}; 
    dtUtil::write_config_file(new_config);
}

//-------------------------------------------------------------------------

Configuration Pref_win::get_values()
{
    Configuration new_config{
        in_email_host->GetValue().ToStdString(),
        in_sender->GetValue().ToStdString(),
        in_recipient->GetValue().ToStdString(),
        wxAtoi(in_interval->GetValue()),
        wxAtoi(in_max_fail->GetValue()),
        in_log_dir->GetValue().ToStdString()};

    return new_config; 
}
