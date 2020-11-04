//
// Address_book_win.h
//

#ifndef ADDRESS_BOOK_WIN_H
#define ADDRESS_BOOK_WIN_H

#include <vector>

#include <wx/datectrl.h>
#include <wx/gbsizer.h>
#include <wx/listctrl.h>
#include <wx/wx.h>

#include "../include/Utilities.h"


class Address_book_win : public wxFrame
{
public:
    Address_book_win(wxFrame* parent,wxString name,const wxSize& size);
    ~Address_book_win();

    //event handlers
    void on_column_click(wxListEvent& event);
    void on_add(wxCommandEvent& event);
    void on_remove(wxCommandEvent& event);
    void on_open(wxCommandEvent&event);
    void on_save(wxCommandEvent& event);
    void on_save_as(wxCommandEvent& event);

    //menubar
    wxMenuBar* menubar = nullptr;
    wxMenu* menu_ctrl = nullptr;

    //controls
    wxStaticText* txt_hostname = nullptr;
    wxTextCtrl* in_hostname = nullptr;
    wxStaticText* txt_ip_address = nullptr;
    wxTextCtrl* in_ip_address = nullptr;
    wxButton* btn_add = nullptr;
    wxListCtrl* lc_address_book = nullptr;
    wxButton* btn_remove = nullptr;

    //misc methods
    std::vector<Node> get_entries();
    void display_address_book(std::string fp = "");

private:

    std::string ab_path = dtUtil::DEFAULT_ADDRESS_BOOK_PATH;
    bool sort_hostname_descend = true;
    bool sort_ip_descend = true;

    wxDECLARE_EVENT_TABLE();

};


#endif //ADDRESS_BOOK_WIN_H
