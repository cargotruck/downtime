//
// Log_win.h
//

#ifndef LOG_WIN_H
#define LOG_WIN_H

#include <wx/datectrl.h>
#include <wx/listctrl.h>
#include <wx/wx.h>

#include "./Utilities.h"

class Log_win : public wxFrame
{
public:
    Log_win(wxFrame* parent,wxString name,const wxSize& size);
    ~Log_win();

    //event handlers
    void on_column_click(wxListEvent& event);
    void on_btn_date_select(wxCommandEvent& event);
    
    //menubar
    
    //controls
    wxDatePickerCtrl* dpc_start = nullptr;
    wxDatePickerCtrl* dpc_end = nullptr;
    wxButton* btn_date_select = nullptr;     
    wxListCtrl* lc_logs = nullptr;

    //misc methods
    std::vector<Log> get_entries();
    void clear_lc_logs();
    void update_lc_logs(std::vector<Log> l);
    void append_lc_logs(std::vector<Log> l);

private:
    std::string log_path = dtUtil::DEFAULT_LOG_PATH;
    bool sort_time_descend = true;
    bool sort_entry_descend = true;
    
    wxDECLARE_EVENT_TABLE();
};

#endif //LOG_WIN_H
