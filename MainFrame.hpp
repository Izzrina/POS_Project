#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include "SQLiteDB.hpp"
#include "Structs.hpp"
#include "TaxCalculation.hpp"

#include <wx/wx.h>
#include <wx/listctrl.h>

#include <vector>
#include <algorithm>
#include <fstream>

class MainFrame : public wxFrame  //MainFrame inherited from wxFrame
{

public:

    MainFrame(const wxString& title);

    TaxCalculation tax;


private:

    void onCategoryButton(wxCommandEvent& evt);
    void onProductButton(wxCommandEvent& evt);
    void onButtonIncrement(wxCommandEvent& evt);
    void onButtonDecrement(wxCommandEvent& evt);
    void onButtonDelete(wxCommandEvent& evt);
    void onButtonClear(wxCommandEvent& evt);
    void onButtonSale(wxCommandEvent& evt);
    void onButtonClosing(wxCommandEvent& evt);
    void onButtonProduct20(wxCommandEvent& evt);

    void addProductToDisplay(int id);
    void issueInvoice();
    std::string getCurrentDate();

    wxButton *productButton;
    wxFlexGridSizer* productSizer;
    wxListView *listView;
    wxTextCtrl *displaySum;
    wxTextCtrl *displayNumber;

    int _categoryId = 6000;

    std::vector<Category> _categories;
    std::vector<Product> _products;

    std::string _user = "Arbeiter 1";
    std::string _date = "";
    int _accountNumber = 0;
    Product _product;
    std::vector<Product> _account;
    double _sum;
    wxString _password = "secret";

};

#endif
