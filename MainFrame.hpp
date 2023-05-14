#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include <wx/wx.h>
#include <wx/listctrl.h>

#include <vector>
#include <algorithm>
#include <fstream>

#include "SQLiteDB.hpp"

class MainFrame : public wxFrame  //MainFrame inherited from wxFrame
{

public:

MainFrame(const wxString& title);


    void onCategoryButtonClicked(wxCommandEvent& evt);  // Zeigt die Buttons der Produkte der gewählten Kategorie
    void onProductButtonClicked(wxCommandEvent& evt);   // Fügt der aktuellen Rechnung ein Produkt hinzu oder erhöht dessen Anzahl wenn es schon auf der        Rechnung ist
    void onButtonClear(wxCommandEvent& evt);            // Löscht alle Positionen der aktuellen Rechnung
    void onButtonDelete(wxCommandEvent& evt);           // Entfernt eine ausgewählte Position von der aktuellen Rechnung

    void onButtonDecrement(wxCommandEvent& evt);    // Erhöht die Anzahl eines Produkts auf der aktuellen Rechnung um 1
    void onButtonIncrement(wxCommandEvent& evt);    // Verringert die Anzahl eines Produkts auf der aktuellen Rechnung um 1

    void onButtonInvoice(wxCommandEvent& evt);      // Stellt die aktuelle Rechnung aus und schreibt die Verkäufe in die Datenbank

    void onButtonClosing(wxCommandEvent& evt);      // Macht den Tagesabschluss

    void issueInvoice();    // Schreibt die Rechnung in ein File

    void addProductToDisplay(int id);   // Fügt Produkte anhand deren Id der Anzeige der aktuellen Rechnung hinzu

    double calculateTax20(double sum);
    double calculateTax10(double sum);

    struct displayProduct {
        int count = 1;
        int id = 0;
        std::string name = "";
        double price = 0.0;
        double tax = 0.0;
    };

    wxButton *productButton;
    wxFlexGridSizer* productSizer;
    wxListView* listView;
    wxTextCtrl *displaySum;
    wxTextCtrl *displayAccountNumber;


private:

    int _categoryId = 1;
    int _productId = 0;
    int _index = -1;
    double _sum = 0;
    displayProduct _displayProduct;                 // speichert Daten des gewählten Produkts
    std::vector<displayProduct> _displayProducts;   // Speichert die Produkte die sich auf der aktuellen Rechnung befinden

};

#endif
