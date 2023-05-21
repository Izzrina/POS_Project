#include "App.hpp"
#include "MainFrame.hpp"
#include "SQLiteDB.hpp"
#include "Structs.hpp"

#include <wx/wx.h>
#include <iostream>
#include <iomanip>

SQLiteDB database = SQLiteDB("register.db");

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
    this->SetBackgroundColour( wxColour( 64, 64, 64 ) );


    wxBoxSizer* mainSizer = new wxBoxSizer( wxHORIZONTAL );

    /*************************LEFT SIZER****************************/

    wxBoxSizer* leftSizer = new wxBoxSizer( wxVERTICAL );

    wxButton *closingButton = new wxButton( this, wxID_ANY, "C\nL\nO\nS\nI\nN\nG", wxDefaultPosition, wxDefaultSize, 0 );
    closingButton->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
    closingButton->SetBackgroundColour( wxColour( 160, 160, 160 ) );
    closingButton->SetMinSize( wxSize( 20,20 ) );

    closingButton->Bind(wxEVT_BUTTON, &MainFrame::onButtonClosing, this);

    leftSizer->Add( closingButton, 3, wxALL|wxEXPAND, 5 );

    mainSizer->Add( leftSizer, 1, wxALL|wxEXPAND, 20 );

    /*************************MIDDLE SIZER****************************/

    wxBoxSizer* middleSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* displaySizer = new wxBoxSizer( wxVERTICAL );


    _accountNumber = database.getAccountNumber();
    wxString accountNumber = wxString::Format("Rechnungsnummer: %d", _accountNumber);

    displayNumber = new wxTextCtrl( this, wxID_ANY, accountNumber, wxDefaultPosition, wxDefaultSize, 0|wxBORDER_NONE );
    displayNumber->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
    displayNumber->SetForegroundColour( wxColour( 240, 240, 240 ) );
    displayNumber->SetBackgroundColour( wxColour( 64, 64, 64 ) );

    displaySizer->Add(displayNumber, 1, wxALL|wxEXPAND, 5 );

    listView = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    listView->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans") ) );
    listView->InsertColumn(0, "Anzahl");
    listView->InsertColumn(1, "Produkt");
    listView->InsertColumn(2, "Preis");
    listView->InsertColumn(3, "Gesamt");
    listView->SetColumnWidth(0, 100);
    listView->SetColumnWidth(1, 200);
    listView->SetColumnWidth(2, 100);
    listView->SetColumnWidth(3, 100);

    displaySizer->Add(listView, 5, wxALL | wxEXPAND, 5);

    wxStaticText *text = new wxStaticText( this, wxID_ANY, wxT("Gesamt"), wxDefaultPosition, wxDefaultSize, 0 );
    text->Wrap( -1 );
    text->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
    text->SetForegroundColour( wxColour( 240, 240, 240 ) );

    displaySizer->Add( text, 0, wxALL|wxEXPAND, 5 );

    displaySum = new wxTextCtrl( this, wxID_ANY, "0.00", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT | wxTE_READONLY );
    displaySum->SetFont( wxFont( 16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans") ) );
	displaySum->Enable( false );
    displaySizer->Add( displaySum, 1, wxALL|wxEXPAND, 5 );

    middleSizer->Add( displaySizer, 2, wxEXPAND, 5 );

    wxBoxSizer* controlSizer;
    controlSizer = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* leftControlSizer;
    leftControlSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* buttonSizer;
    buttonSizer = new wxBoxSizer( wxHORIZONTAL );

    wxButton *decrementButton = new wxButton( this, wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
    decrementButton->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
    decrementButton->SetBackgroundColour( wxColour( 176, 176, 176 ) );
    decrementButton->SetMinSize( wxSize( 20,20 ) );

    decrementButton->Bind(wxEVT_BUTTON, &MainFrame::onButtonDecrement, this);

    buttonSizer->Add( decrementButton, 1, wxALL|wxEXPAND, 10 );

    wxButton *incrementButton = new wxButton( this, wxID_ANY, wxT("+"), wxDefaultPosition, wxDefaultSize, 0 );
    incrementButton->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
    incrementButton->SetBackgroundColour( wxColour( 176, 176, 176 ) );
    incrementButton->SetMinSize( wxSize( 20,20 ) );

    incrementButton->Bind(wxEVT_BUTTON, &MainFrame::onButtonIncrement, this);

    buttonSizer->Add( incrementButton, 1, wxALL|wxEXPAND, 10 );

    leftControlSizer->Add( buttonSizer, 1, wxBOTTOM|wxEXPAND|wxTOP, 20 );

    wxButton *deleteButton = new wxButton( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
    deleteButton->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
    deleteButton->SetForegroundColour( wxColour( 240, 240, 240 ) );
    deleteButton->SetBackgroundColour( wxColour( 219, 41, 41 ) );
    deleteButton->SetMinSize( wxSize( 20,20 ) );

    deleteButton->Bind(wxEVT_BUTTON, &MainFrame::onButtonDelete, this);

    leftControlSizer->Add(deleteButton, 1, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 30 );

    controlSizer->Add( leftControlSizer, 1, wxEXPAND, 10 );

    wxBoxSizer* rightControlsizer;
    rightControlsizer = new wxBoxSizer( wxVERTICAL );

    wxButton *clearButton= new wxButton( this, wxID_ANY, wxT("CLEAR"), wxDefaultPosition, wxDefaultSize, 0 );
    clearButton->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
    clearButton->SetForegroundColour( wxColour( 240, 240, 240 ) );
    clearButton->SetBackgroundColour( wxColour( 218, 42, 42 ) );

    clearButton->Bind(wxEVT_BUTTON, &MainFrame::onButtonClear, this);

    rightControlsizer->Add(clearButton, 1, wxALL|wxEXPAND, 10 );

    wxButton *saleButton = new wxButton( this, wxID_ANY, wxT("CASH"), wxDefaultPosition, wxDefaultSize, 0 );
    saleButton->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
    saleButton->SetForegroundColour( wxColour( 240, 240, 240 ) );
    saleButton->SetBackgroundColour( wxColour( 21, 151, 57 ) );

    saleButton->Bind(wxEVT_BUTTON, &MainFrame::onButtonSale, this);

    rightControlsizer->Add( saleButton, 2, wxALL|wxEXPAND, 10 );

    controlSizer->Add( rightControlsizer, 1, wxEXPAND, 5 );

    middleSizer->Add( controlSizer, 1, wxEXPAND, 5 );

    mainSizer->Add( middleSizer, 4, wxALL|wxEXPAND, 20 );

    /*************************RIGHT SIZER****************************/

    wxBoxSizer* rightSizer = new wxBoxSizer( wxVERTICAL );

    _date = getCurrentDate();
    wxString date(_date);

    wxTextCtrl *displayDate = new wxTextCtrl( this, wxID_ANY, date, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT|wxBORDER_NONE );
    displayDate->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
    displayDate->SetForegroundColour( wxColour( 240, 240, 240 ) );
    displayDate->SetBackgroundColour( wxColour( 64, 64, 64 ) );

    rightSizer->Add( displayDate, 1, wxALL|wxEXPAND, 10 );

    wxGridSizer* categorySizer = new wxGridSizer( 0, 3, 0, 0 );

    productSizer = new wxFlexGridSizer( 0, 4, 0, 0 );
    productSizer->SetFlexibleDirection(wxHORIZONTAL);
    productSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    productSizer->AddGrowableCol(0);
    productSizer->AddGrowableCol(1);
    productSizer->AddGrowableCol(2);
    productSizer->AddGrowableCol(3);

    std::vector<Category> categories = database.getCategories();

    for (const auto& cat : categories)
    {
        wxButton *categoryButton = new wxButton( this, cat.id, cat.name, wxDefaultPosition, wxDefaultSize, 0);
        categoryButton->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans")));
        categoryButton->SetForegroundColour( wxColour( 240, 240, 240 ));
        categoryButton->SetBackgroundColour( wxColour( 59, 122, 87 ));

        categoryButton->Bind(wxEVT_BUTTON, &MainFrame::onCategoryButton, this);

        categorySizer->Add( categoryButton, 1, wxALL|wxEXPAND, 5 );

        //For each category get all the products from the database and create a button which is hidden by default
        _products = database.getProducts(cat.id);
        for (const auto& prod : _products)
        {
            productButton = new wxButton( this, prod.id, prod.name, wxDefaultPosition, wxDefaultSize, 0);
            productButton->SetMinSize(wxSize(0, 100));
            productButton->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans")));
            productButton->SetForegroundColour( wxColour( 240, 240, 240 ));
            productButton->SetBackgroundColour( wxColour( 59, 122, 87 ));
            productButton->Bind(wxEVT_BUTTON, &MainFrame::onProductButton, this);
            productButton->Hide();
            productSizer->Add( productButton, 1, wxALL|wxEXPAND, 5 );
            //productSizer->Hide(this->FindWindowById(prod.id));
        }
    }

    _products = database.getProducts(_categoryId);
    for (const auto& prod : _products)
    {
        //(this->FindWindowById(prod.id))->Show();
        productSizer->Show(this->FindWindowById(prod.id));
    }

    rightSizer->Add( categorySizer, 3, wxALL|wxEXPAND, 10 );

    rightSizer->Add( productSizer, 5, wxALL|wxEXPAND, 10 );

    mainSizer->Add( rightSizer, 6, wxALL|wxEXPAND, 20 );

    this->SetSizer( mainSizer );
    this->Layout();
}


std::string MainFrame::getCurrentDate() {
    // Aktuelles Datum abrufen
    std::time_t now = std::time(nullptr);
    std::tm* date = std::localtime(&now);

    // Datum im Format "TT.MM.JJJJ" erzeugen
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << date->tm_mday << '.'
    << std::setfill('0') << std::setw(2) << (date->tm_mon + 1) << '.'
    << (date->tm_year + 1900);

    return oss.str();
}

/**
 * Event handler for category buttons.
 * Hides the products currently displayed and shows the products of the selected category.
 * @param evt The wxCommandEvent representing the button click event.
 */
void MainFrame::onCategoryButton(wxCommandEvent& evt)
{

    // Hide the products currently displayed
    for (const auto& prod : _products)
    {
        (this->FindWindowById(prod.id))->Hide();
        //productSizer->Hide(this->FindWindowById(prod.id));
    }
    _categoryId = evt.GetId();
    // Get the products of the selected category from the database
    _products = database.getProducts(_categoryId);

    // Show the products of the selected category
    for (const auto& prod : _products)
    {
        (this->FindWindowById(prod.id))->Show();
        //productSizer->Show(this->FindWindowById(prod.id));
    }
    Layout(); // Update the layout to reflect the changes
}

/**
 * Adds a product to the display.
 * If the product already exists in the account, increments the quantity and updates the display accordingly.
 * If the product is new, adds it to the account and creates a new entry in the display.
 * @param id The ID of the product to add.
 */
void MainFrame::addProductToDisplay(int id)
{

    // Check if the product already exists in the account
    auto it = std::find_if(_account.begin(), _account.end(), [&](const Product& p)
    {
        return p.id == id;
    });

    if (it != _account.end())
    {
        // Increment the quantity and update the display
        it->amount += 1;
        auto index = std::distance(_account.begin(), it);
        //std::cout << "Index: " << index << std::endl;
        listView->SetItem(index, 0, wxString::Format("%d", it->amount));
        listView->SetItem(index, 3, wxString::Format("%.2f", it->price * it->amount));
        _account[index].amount = it->amount;
        _sum += it->price;
        displaySum->SetValue(wxString::Format("%.2f", _sum));

    } else
    {
        // Add the product to the account and create a new entry in the display
        _account.push_back(_product);

        //std::cout << "Display, Added Product" << std::endl;
        long index = listView->InsertItem(listView->GetItemCount(), wxString::Format("%d", _product.amount));
        listView->SetItem(index, 1, _product.name);
        //double rounded = std::round(_displayProduct.price * 100.0) / 100.0;
        listView->SetItem(index, 2, wxString::Format("%.2f", _product.price));
        listView->SetItem(index, 3, wxString::Format("%.2f", _product.price * _product.amount));


        _sum += _product.price;
        displaySum->SetValue(wxString::Format("%.2f", _sum));
    }

    for(auto& prod : _account){
        std::cout << prod.amount << "   " << prod.name << "   " << prod.price << " | ";
    }
    std::cout << std::endl;
}

/**
 * Handles the click event of a product button.
 * Retrieves the product information based on the button's ID from the database,
 * and adds the product to the display.
 */
void MainFrame::onProductButton(wxCommandEvent& evt)
{

    int productId = evt.GetId();

    _product = database.getProduct(productId); // Retrieve the product information from the database

    addProductToDisplay(productId); // Add the product to the display
}

/**
 * Increments the quantity of the selected item in the account by 1 and updates the total sum and list view accordingly.
 * If no item is selected, it displays a warning message.
 */
void MainFrame::onButtonIncrement(wxCommandEvent& evt)
{
    int index = listView->GetFirstSelected();
    if (index != -1) {

        _account[index].amount += 1; // Increase the quantity of the selected item by 1

        // Update the quantity and total price displayed in the list view
        listView->SetItem(index, 0, wxString::Format("%d", _account[index].amount));
        listView->SetItem(index, 3, wxString::Format("%.2f", _account[index].amount * _account[index].price));

        // Update the total sum by adding the price of one item
        _sum += _account[index].price;
        displaySum->SetValue(wxString::Format("%.2f", _sum));

    } else {
        // No item selected
        wxMessageBox("Kein Artikel markiert", "Warnung", wxOK | wxICON_WARNING);
    }
}

/**
 * Decrements the quantity of the selected item in the account by 1 and updates the total sum and list view accordingly.
 * If no item is selected or the quantity is already 1, it displays a warning message.
 */
void MainFrame::onButtonDecrement(wxCommandEvent& evt)
{
    int index = listView->GetFirstSelected();
    if (index != -1) {
        if(_account[index].amount > 1) {

            // Decrease the quantity of the selected item by 1
            _account[index].amount -= 1;

            // Update the quantity and total price displayed in the list view
            listView->SetItem(index, 0, wxString::Format("%d", _account[index].amount));
            listView->SetItem(index, 3, wxString::Format("%.2f", _account[index].amount * _account[index].price));

            // Update the total sum by subtracting the price of one item
            _sum -= _account[index].price;
            displaySum->SetValue(wxString::Format("%.2f", _sum));
        }

    } else {
        // No item selected
        wxMessageBox("Kein Artikel markiert", "Warnung", wxOK | wxICON_WARNING);
    }
}

/**
 * Deletes the selected item from the account and updates the total sum and list view accordingly.
 * If no item is selected, it displays a warning message.
 */
void MainFrame::onButtonDelete(wxCommandEvent& evt)
{

    int index = listView->GetFirstSelected();
    if (index != -1) {

        // Calculate the price without and update the total sum
        _sum -= _account[index].price * _account[index].amount;

        // Remove the selected item from the account and the list view
        _account.erase(_account.begin() + index);
        listView->DeleteItem(index);

        // Update the displayed total sum
        displaySum->SetValue(wxString::Format("%.2f", _sum));

        index = -1; // Reset the index

    } else {
        // No item selected
        wxMessageBox("Kein Artikel markiert", "Warnung", wxOK | wxICON_WARNING);

    }
}

/**
 * Clears the current account, resets the total sum to zero, and clears the items displayed in the list view.
 */

void MainFrame::onButtonClear(wxCommandEvent& evt)
{
    // Clear the current account and reset the total sum
    _account.clear();
    _sum = 0.0;
    // Update the displayed total sum
    displaySum->SetValue(wxString::Format("%.2f", _sum));
    // Clear the items displayed in the list view
    listView->DeleteAllItems();
}

/**
 * Generates and saves an invoice as a text file.
 * The file is named using the account number and contains the invoice details and calculated amounts.
 * The invoice includes the company name, address, invoice number, date, item details, and total amount.
 * It also calculates and includes the tax amounts and the net and gross amounts for each tax rate.
 * Finally, it includes a closing message.
 */
void MainFrame::issueInvoice()
{

    std::string text1 = ".txt";
    std::string text2 = "Rechnung";
    std::string name = std::to_string(_accountNumber);
    std::string filename = text2 + name + text1;

    double tax20 = 0.0;
    double tax10 = 0.0;
    double sum = 0.0;

    std::ofstream file(filename);

    if (file.is_open()) {
        file << "\n\n\n" << "Firmenname" << std::endl;
        file << "Strasse 00 " <<  std::endl;
        file << "O000 Ortschaft " <<  std::endl <<  std::endl <<  std::endl;
        file << "Rechnungsnummer: " << name <<  std::endl;
        file << "Datum: " << _date <<  std::endl <<  std::endl;


        file << "Anzahl	" << "Artikel				" << "Einzelpreis			" << "Gesamt" <<  std::endl;
        file << "_____________________________________________________________________" <<  std::endl;


        for (const auto& prod : _account)
        {

            file << prod.amount << "		" << prod.name << "				";
            file << std::fixed << std::setprecision(2) << prod.price << "				" << (prod.price * prod.amount) <<std::endl;;

            if(prod.tax == 1.2){
                tax20 += prod.price * prod.amount;
            }
            if(prod.tax == 1.1){
                tax10 += prod.price * prod.amount;
            }
            sum += prod.price * prod.amount;
        }
        file << "_____________________________________________________________________" <<  std::endl;
        file << "									Gesamt: EUR ";
        file << std::fixed << std::setprecision(2) << sum <<  std::endl <<  std::endl;

        double netto20 = tax20/1.2;
        double netto10 = tax10/1.1;

        file << "MwSt. 20%:	" << "Netto: ";
        file << std::fixed << std::setprecision(2) << netto20 ;
        file << "		MwSt: ";
        file << std::fixed << std::setprecision(2) << tax20 - netto20 ;
        file << "	Brutto: ";
        file << std::fixed << std::setprecision(2) << tax20  << std::endl;
        file << "MwSt. 10%:	";
        file << "Netto: ";
        file << std::fixed << std::setprecision(2) << netto10 ;
        file << "		MwSt: ";
        file << std::fixed << std::setprecision(2) << tax10 - netto10;
        file << "	Brutto: ";
        file << std::fixed << std::setprecision(2) << tax10 <<std::endl <<std::endl <<std::endl;

        file << "Vielen Dank"  <<std::endl <<std::endl <<std::endl;

        file.close();

        std::cout << "File written successfully." << std::endl;
    } else
    {
        std::cerr << "Unable to open file." << std::endl;
    }
}

/*
 * Event handler for the "Sale" button click.
 * Performs various actions related to completing a sale transaction, including issuing an invoice,
 * writing account and sales data to the database, clearing the account and sum variables,
 * updating the displayed sum and account number, and updating the layout.
 */
void MainFrame::onButtonSale(wxCommandEvent& evt)
{
    issueInvoice();
    database.writeInvoiceToDatabase(_accountNumber, _user);
    database.writeSalesToDatabase(_account, _accountNumber);
    _account.clear();
    _sum = 0.0;
    displaySum->SetValue(wxString::Format("%.2f", _sum));
    listView->DeleteAllItems();
    _accountNumber = database.getAccountNumber();
    displayNumber->SetValue(wxString::Format("%d", _accountNumber));
    Layout();
}

/**
 * Event handler for the "Closing" button.
 * Generates a closing report, clears the account data, and deletes all sales records in the database.
 * Updates the display and layout accordingly.
 */

void MainFrame::onButtonClosing(wxCommandEvent& evt)
{
    wxMessageDialog dialog(this, "Accomplished daily closing?", "Confirmation", wxYES_NO | wxICON_QUESTION);

    int result = dialog.ShowModal();

    if (result != wxID_YES)
    {
        return;
    }
    else
    {
    double sum = database.getFinalSum();
    double sum10 = database.getFinalSumTax10();
    double sum20 = database.getFinalSumTax20();

    std::string text1 = ".txt";
    std::string text2 = "Tagesabschluss";

    std::string date = _date;
    size_t dotPosition = date.find('.');
    while (dotPosition != std::string::npos)
    {
        date.erase(dotPosition, 1);
        dotPosition = date.find('.');
    }

    std::string filename = text2 + date + text1;

    std::ofstream file(filename);

    if (file.is_open())
    {
        file << "\n\n\n" << "Firmenname" << std::endl;
        file << "Strasse 00 " <<  std::endl;
        file << "O000 Ortschaft " <<  std::endl <<  std::endl;

        file << "Datum: " << _date <<  std::endl <<  std::endl;

        file << "Arbeiter: " << _user <<  std::endl <<  std::endl;


        file << std::endl;
        file << "****************************************" << std::endl << std::endl;
        file << "Gesamtumsatz: ";
        file << std::fixed << std::setprecision(2) << sum  << std::endl << std::endl;
        file << "MwSt%: 20	";
        file << "Brutto: ";
        file << std::fixed << std::setprecision(2) << sum20;
        file << "       Netto: ";
        file << std::fixed << std::setprecision(2) << sum20 / 1.2;
        file << "       MwSt: ";
        file << std::fixed << std::setprecision(2) << sum20 - (sum20 / 1.2)  << std::endl;
        file << "MwSt%: 10	";
        file << "Brutto: ";
        file << std::fixed << std::setprecision(2) << sum10;
        file << "       Netto: ";
        file << std::fixed << std::setprecision(2) << sum10 / 1.1;
        file << "       MwSt: ";
        file << std::fixed << std::setprecision(2) << sum10 - (sum10 / 1.1)  << std::endl;

        file << std::endl;
        file << "****************************************" << std::endl << std::endl;
        file.close();

        std::cout << "File written successfully." << std::endl;
    } else
    {
        std::cerr << "Unable to open file." << std::endl;
    }

    _account.clear();

    _sum = 0.0;
    displaySum->SetValue(wxString::Format("%.2f", _sum));
    listView->DeleteAllItems();
    _accountNumber = database.getAccountNumber();
    displayNumber->SetValue(wxString::Format("%d", _accountNumber));

    database.deleteAllSalesData();

    Layout();
    }
}

