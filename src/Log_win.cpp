//
// Log_win.cpp
//

//TODO: allow users to search listed logs

#include <algorithm>
#include <sstream>

#include "../include/Log_win.h"

enum{
    ID_dpc_start = 12001,
    ID_dpc_end,
    ID_btn_date_select,
    ID_lc_logs,
};

Log_win::Log_win(wxFrame* parent,wxString name,const wxSize& size)
    : wxFrame(parent,wxID_ANY,name,wxDefaultPosition,size)
{
    //setup menubar

    //log win controls
    wxBoxSizer* sizer_v1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_v2 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_h1 = new wxBoxSizer(wxHORIZONTAL);

    dpc_start = new wxDatePickerCtrl(this,ID_dpc_start,wxDefaultDateTime,wxDefaultPosition,wxSize(150,30));
    dpc_end = new wxDatePickerCtrl(this,ID_dpc_end,wxDefaultDateTime,wxDefaultPosition,wxSize(150,30));
    btn_date_select = new wxButton(this,ID_btn_date_select,"SELECT",wxDefaultPosition,wxSize(80,30));
    lc_logs = new wxListCtrl(this,ID_lc_logs,wxDefaultPosition,wxSize(600,800),wxLC_REPORT);

    //set the list control's defaults
    lc_logs->AppendColumn("Date");
    lc_logs->AppendColumn("Entry");
    lc_logs->SetColumnWidth(0,200);
    lc_logs->SetColumnWidth(1,600);

    sizer_v1->Add(dpc_start,0,wxEXPAND | wxALL | wxALIGN_CENTER,10);
    sizer_v1->Add(dpc_end,0,wxEXPAND | wxALL | wxALIGN_CENTER,10);
    sizer_v1->Add(btn_date_select,0,wxALIGN_CENTER);
    sizer_v2->Add(lc_logs,0,wxEXPAND | wxALIGN_CENTER);
    sizer_h1->Add(sizer_v1,0,wxEXPAND | wxALIGN_LEFT);
    sizer_h1->Add(sizer_v2,1,wxEXPAND | wxALIGN_RIGHT);

    this->SetSizer(sizer_h1);
}

//-------------------------------------------------------------------------

Log_win::~Log_win() {};

//-------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(Log_win,wxFrame)
    EVT_BUTTON(ID_btn_date_select,Log_win::on_btn_date_select)
    EVT_LIST_COL_CLICK(ID_lc_logs,Log_win::on_column_click)
wxEND_EVENT_TABLE()

//-------------------------------------------------------------------------

void Log_win::on_column_click(wxListEvent& event)
{
    int column = event.GetColumn();
    std::vector<Log> logs = get_entries();

    if(column < 0) return;

    if(column == 0){
        if(sort_time_descend){
            sort_time_descend = false;
            std::sort(logs.begin(),logs.end(),dtUtil::log_time_lssr);
        }else{
            sort_time_descend = true;
            std::sort(logs.begin(),logs.end(),dtUtil::log_time_grtr);
        }
    }

    if(column == 1){
        if(sort_entry_descend){
            sort_entry_descend = false;
            std::sort(logs.begin(),logs.end(),dtUtil::log_entry_lssr);
        }else{
            sort_entry_descend = true;
            std::sort(logs.begin(),logs.end(),dtUtil::log_entry_grtr);
        }
    }

    update_lc_logs(logs);
}

//-------------------------------------------------------------------------

void Log_win::on_btn_date_select(wxCommandEvent& event)
{
    int log_count = 0;
    std::string start_date = std::string(dpc_start->GetValue().Format("%Y%m%d"));
    std::string end_date = std::string(dpc_end->GetValue().Format("%Y%m%d"));

    if(start_date > end_date){
        wxMessageDialog msg(this,"Start Date greater than End Date","ERROR",wxOK | wxICON_ERROR | wxCENTER);
        msg.ShowModal(); 
        return; 
    }

    clear_lc_logs();

    for(wxDateTime s = dpc_start->GetValue();s <= dpc_end->GetValue();s.Add(wxDateSpan::Days(1))){
        std::stringstream log_file; 
        log_file << log_path << s.Format("%Y%m%d") << "-" << "downtime.log";
        std::fstream ifs{log_file.str()};

        if(ifs){
            ++log_count;
            std::vector<Log> logs = dtUtil::read_log(log_file.str());
            append_lc_logs(logs);
        }
    }

    if(log_count < 1){
        wxMessageDialog msg(this,"No log files found.","INFO",wxOK | wxICON_INFORMATION | wxCENTER);
        msg.ShowModal();
    }
}
            
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

std::vector<Log> Log_win::get_entries()
{
    std::vector<Log> logs;

    for(int i = 0;i < lc_logs->GetItemCount();++i){

        wxListItem row_info;
        std::string time;
        std::string entry;

        row_info.m_mask = wxLIST_MASK_TEXT;
        row_info.m_itemId = i;
        row_info.m_col = 0;
        lc_logs->GetItem(row_info);
        time = row_info.m_text;
        row_info.m_col = 1;
        lc_logs->GetItem(row_info);
        entry = row_info.m_text;

        Log l{time,entry};
        logs.push_back(l);
    }

    return logs;
}

//-------------------------------------------------------------------------

void Log_win::clear_lc_logs()
{
    lc_logs->DeleteAllItems();
}

//-------------------------------------------------------------------------

void Log_win::update_lc_logs(std::vector<Log> l)
{
    clear_lc_logs();
    
    for(int i = 0;i < l.size();++i){
        lc_logs->InsertItem(0,l[i].time);
        lc_logs->SetItem(0,1,l[i].entry);
    }

    lc_logs->SetColumnWidth(1,wxLIST_AUTOSIZE);
}

//-------------------------------------------------------------------------

void Log_win::append_lc_logs(std::vector<Log> l)
{
    for(int i = 0;i < l.size();++i){
        lc_logs->InsertItem(0,l[i].time);
        lc_logs->SetItem(0,1,l[i].entry);
    }

    lc_logs->SetColumnWidth(1,wxLIST_AUTOSIZE);
}
