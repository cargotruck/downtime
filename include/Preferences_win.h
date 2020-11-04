//
// Preferences_win.h
//

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <string>
#include <wx/wx.h>
#include "./Configuration.h"
#include "./Utilities.h"

class Pref_win : public wxFrame
{
public:
    Pref_win(wxFrame* parent,wxString name,const wxSize& size);
    ~Pref_win();

    //event handlers
    void on_btn_browse(wxCommandEvent& event);
    void on_btn_okay(wxCommandEvent& event);
    void on_btn_cancel(wxCommandEvent& event);
    void on_btn_apply(wxCommandEvent& event);
    
    //menubar

    //controls
    wxStaticText* txt_email_host = nullptr;
    wxTextCtrl* in_email_host = nullptr;
    //TODO: button to test email
    wxStaticText* txt_sender = nullptr;
    wxTextCtrl* in_sender = nullptr;
    wxStaticText* txt_recipient = nullptr;
    wxTextCtrl* in_recipient = nullptr;
    wxStaticText* txt_interval = nullptr;
    wxTextCtrl* in_interval = nullptr;
    wxStaticText* txt_max_fail = nullptr;
    wxTextCtrl* in_max_fail = nullptr;
    wxStaticText* txt_log_dir = nullptr;
    wxTextCtrl* in_log_dir = nullptr;
    wxButton* btn_browse = nullptr;
    wxButton* btn_okay = nullptr;
    wxButton* btn_cancel = nullptr;
    wxButton* btn_apply = nullptr;

    //misc methods
    Configuration get_values();

private:
    wxDECLARE_EVENT_TABLE();
    
};

#endif //PREFERENCES_H
