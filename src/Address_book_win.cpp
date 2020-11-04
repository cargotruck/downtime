//
// Address_book_win.cpp
//

#include "../include/Address_book_win.h"
#include "../include/Node.h"

#include <algorithm>
#include <iostream>

enum{
    ID_btn_add = 11001,
    ID_btn_remove,
    ID_open,
    ID_save,
    ID_save_as,
    ID_close,
    ID_lc_address_book,
};

//-------------------------------------------------------------------------

Address_book_win::Address_book_win(wxFrame* parent,wxString name,const wxSize& size)
    : wxFrame(parent,wxID_ANY,name,wxDefaultPosition,size)
{
    
    //setup the menubar
    menubar = new wxMenuBar;
    menu_ctrl = new wxMenu;

    menu_ctrl->Append(wxID_ANY,"&New");
    menu_ctrl->Append(ID_open,"&Open");
    menu_ctrl->Append(ID_save,"&Save");
    menu_ctrl->Append(ID_save_as,"Save &As");

    menubar->Append(menu_ctrl,"&Controls");

    SetMenuBar(menubar);

    //address book controls
    wxBoxSizer* sizer_v1 = new wxBoxSizer(wxVERTICAL);
    wxGridBagSizer* sizer_h1 = new wxGridBagSizer();
    wxGridBagSizer* sizer_h2 = new wxGridBagSizer();

    txt_hostname = new wxStaticText(this,wxID_ANY,"Hostname:");
    in_hostname = new wxTextCtrl(this,wxID_ANY,"",wxDefaultPosition,wxSize(200,30));
    txt_ip_address = new wxStaticText(this,wxID_ANY,"IP Address:");
    in_ip_address = new wxTextCtrl(this,wxID_ANY,"",wxDefaultPosition,wxSize(200,30));
    btn_add = new wxButton(this,ID_btn_add,"ADD",wxDefaultPosition,wxSize(70,30));
    lc_address_book = new wxListCtrl(this,ID_lc_address_book,wxDefaultPosition,wxSize(500,500),wxLC_REPORT);
    btn_remove = new wxButton(this,ID_btn_remove,"REMOVE",wxDefaultPosition,wxSize(70,30));

    //set the list control's defaults
    lc_address_book->AppendColumn("Hostname");
    lc_address_book->AppendColumn("IP Address");
    lc_address_book->SetColumnWidth(0,200);
    lc_address_book->SetColumnWidth(1,200);

    sizer_h1->Add(txt_hostname,wxGBPosition(1,2),wxDefaultSpan);
    sizer_h1->Add(in_hostname,wxGBPosition(1,4),wxDefaultSpan,wxEXPAND | wxRIGHT);
    sizer_h1->Add(txt_ip_address,wxGBPosition(1,6),wxDefaultSpan);
    sizer_h1->Add(in_ip_address,wxGBPosition(1,8));
    sizer_h1->Add(btn_add,wxGBPosition(1,10));

    sizer_h2->Add(lc_address_book,wxGBPosition(1,4),wxDefaultSpan,wxEXPAND | wxALL);
    sizer_h2->Add(btn_remove,wxGBPosition(1,6),wxDefaultSpan,wxALIGN_CENTER);

    sizer_v1->Add(sizer_h1);
    sizer_v1->Add(sizer_h2);

    this->SetSizer(sizer_v1);
}

//-------------------------------------------------------------------------

Address_book_win::~Address_book_win() {};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(Address_book_win,wxFrame)
    EVT_BUTTON(ID_btn_add,Address_book_win::on_add)
    EVT_BUTTON(ID_btn_remove,Address_book_win::on_remove)
    EVT_MENU(ID_open,Address_book_win::on_open)
    EVT_MENU(ID_save,Address_book_win::on_save)
    EVT_MENU(ID_save_as,Address_book_win::on_save_as)
    EVT_LIST_COL_CLICK(ID_lc_address_book,Address_book_win::on_column_click)
wxEND_EVENT_TABLE()

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

void Address_book_win::on_add(wxCommandEvent& event)
{
    int index = lc_address_book->GetItemCount();
    lc_address_book->InsertItem(index,in_hostname->GetValue());
    lc_address_book->SetItem(index,1,in_ip_address->GetValue());
}

//-------------------------------------------------------------------------

void Address_book_win::on_remove(wxCommandEvent& event)
{
    long item = -1;
    for(int i = 0;i < lc_address_book->GetItemCount();++i){
        item = lc_address_book->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);

        if(item == -1) break;

        lc_address_book->DeleteItem(item);
    }
}

//-------------------------------------------------------------------------

//TODO: test with none address book files... consider making an address book extension
void Address_book_win::on_open(wxCommandEvent& event)
{
    wxFileDialog dlg_open(this,"Open Address Book","","","",wxFD_OPEN | wxFD_FILE_MUST_EXIST); 

    if(dlg_open.ShowModal() == wxID_CANCEL) return;

    ab_path = std::string(dlg_open.GetPath());

    display_address_book(ab_path);
}

//-------------------------------------------------------------------------

void Address_book_win::on_save(wxCommandEvent& event)
{
    std::vector<Node> nodes = get_entries();
   
    dtUtil::write_address_book(nodes,ab_path);
}

//-------------------------------------------------------------------------

void Address_book_win::on_save_as(wxCommandEvent& event)
{
    wxFileDialog dlg_save_as(
        this,
        "Save Address Book",
        "",
        "",
        "",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if(dlg_save_as.ShowModal() == wxID_CANCEL) return;

    ab_path = std::string(dlg_save_as.GetPath());
    std::vector<Node> nodes = get_entries();

    dtUtil::write_address_book(nodes,ab_path);
}

//-------------------------------------------------------------------------

void Address_book_win::on_column_click(wxListEvent& event)
{
    int column = event.GetColumn();
    std::vector<Node> ab = get_entries();

    if(column < 0) return;

    if(column == 0){
        if(sort_hostname_descend){
            sort_hostname_descend = false;
            std::sort(ab.begin(),ab.end(),dtUtil::node_name_lssr);
        }else{
            sort_hostname_descend = true;
            std::sort(ab.begin(),ab.end(),dtUtil::node_name_grtr);
        }
    }

    if(column == 1){
        if(sort_ip_descend){
            sort_ip_descend = false;
            std::sort(ab.begin(),ab.end(),dtUtil::node_address_lssr);
        }else{
            sort_ip_descend = true;
            std::sort(ab.begin(),ab.end(),dtUtil::node_address_grtr);
        }
    }
 
    lc_address_book->DeleteAllItems();

    for(int i = 0;i < ab.size();++i){
        lc_address_book->InsertItem(0,ab[i].name);
        lc_address_book->SetItem(0,1,ab[i].address); 
    }
   
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

std::vector<Node> Address_book_win::get_entries()
{
    std::vector<Node> nodes;

    for(int i = 0;i < lc_address_book->GetItemCount();++i){

        wxListItem row_info;
        std::string hostname;
        std::string ip;

        row_info.m_mask = wxLIST_MASK_TEXT;
        row_info.m_itemId = i;
        row_info.m_col = 0;
        lc_address_book->GetItem(row_info);
        hostname = row_info.m_text;
        row_info.m_col = 1;
        lc_address_book->GetItem(row_info);
        ip = row_info.m_text;
        

        Node n{hostname,ip};
        nodes.push_back(n);
    }

    return nodes;
}

//-------------------------------------------------------------------------

void Address_book_win::display_address_book(std::string fp)
{
    std::vector<Node> ab;
    if(fp.empty()){
        ab = dtUtil::read_address_book();
    }else{
        ab = dtUtil::read_address_book(fp);
    }

    lc_address_book->DeleteAllItems();

    for(int i = 0;i < ab.size();++i){
        lc_address_book->InsertItem(0,ab[i].name);
        lc_address_book->SetItem(0,1,ab[i].address); 
    }
}
