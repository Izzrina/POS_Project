#include "MainFrame.hpp"
#include <wx/wx.h>
#include <wx/listctrl.h>

#include <sqlite3.h>
#include "SQLiteDB.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>

using std::vector;

SQLiteDB database = SQLiteDB("register.db");

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{

	this->SetSizeHints( wxSize( 1300,730 ), wxDefaultSize );
	this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	this->SetBackgroundColour( wxColour( 48, 48, 48 ) );

	wxBoxSizer* sizerMain;	// enhält 3 Sizer
	sizerMain = new wxBoxSizer( wxHORIZONTAL );

	// -------------------------------------------------------------------------------
	// Linker Abschnitt mit Tagesabschluss und Logout

	wxBoxSizer* sizerLeft= new wxBoxSizer( wxVERTICAL );
	sizerLeft->SetMinSize(wxSize(100, -1)); // Minimale Größe
    //sizerLeft->SetMaxSize(wxSize(400, 400)); // Maximale Größe


	wxPanel *m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel1->SetBackgroundColour( wxColour( 48, 48, 48 ) );
	sizerLeft->Add( m_panel1, 1, wxEXPAND | wxALL, 5 );

	wxButton *buttonClosing = new wxButton( this, wxID_ANY, "A\nb\ns\nc\nh\nl\nu\ns\ns", wxDefaultPosition, wxDefaultSize, 0);
	buttonClosing->SetMinSize(wxSize(20, 100));
	buttonClosing->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	buttonClosing->SetForegroundColour( wxColour( 240, 240, 240 ) );
	buttonClosing->SetBackgroundColour( wxColour( 52, 100, 235 ) );

	buttonClosing->Bind(wxEVT_BUTTON, &MainFrame::onButtonClosing, this);

	sizerLeft->Add( buttonClosing, 1, wxALL|wxEXPAND, 20 );


	sizerMain->Add( sizerLeft, 1, wxEXPAND|wxTOP, 20 );

	/*********************************************************************************************************
		sizerMiddle contains:
		> the display for the choosen products, a display for the actual price to pay
		> buttons to enter numbers
		> a button to delete a product from the actual account and a button to close the bill and print it out
	*/

	wxBoxSizer* sizerMiddle;
	sizerMiddle = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* sizerDisplays;
	sizerDisplays = new wxBoxSizer( wxVERTICAL );

	database.setAccountNumber();
	wxString accountNumber = wxString::Format("Rechnungsnummer: %d", database.getAccountNumber());

	displayAccountNumber = new wxTextCtrl( this, wxID_ANY, accountNumber, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxBORDER_NONE |wxTE_READONLY );
	displayAccountNumber->Enable( false );
	displayAccountNumber->SetFont( wxFont( 15, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans") ) );
	displayAccountNumber->SetForegroundColour( wxColour( 240, 240, 240 ) );
	displayAccountNumber->SetBackgroundColour( wxColour( 48, 48, 48 ) );

	sizerDisplays->Add( displayAccountNumber, 2, wxALL|wxEXPAND, 5 );

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

	sizerDisplays->Add( listView, 10, wxALL|wxEXPAND, 5 );

	wxStaticText *textSum = new wxStaticText( this, wxID_ANY, wxT("Gesamt"), wxDefaultPosition, wxDefaultSize, 0 );
	textSum->Wrap( -1 );
	textSum->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans") ) );
	textSum->SetForegroundColour( wxColour( 240, 240, 240 ) );

	sizerDisplays->Add( textSum, 1, wxALL|wxEXPAND, 5 );

	displaySum = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT|wxTE_READONLY );
	displaySum->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans") ) );
	displaySum->Enable( false );

	sizerDisplays->Add( displaySum, 2, wxALL|wxEXPAND, 5 );

	sizerMiddle->Add( sizerDisplays, 2, wxALL|wxEXPAND, 5 );

	displaySum->SetValue(wxString::Format("%.2f", _sum));

	wxBoxSizer* sizerControls = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* sizerOptions = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* sizerAmount = new wxBoxSizer( wxHORIZONTAL );

	wxButton *buttonDelete = new wxButton( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonDelete->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	buttonDelete->SetForegroundColour( wxColour( 240, 240, 240 ) );
	buttonDelete->SetBackgroundColour( wxColour( 251, 17, 42 ) );
	buttonDelete->SetMinSize( wxSize( 20,20 ) );

	buttonDelete->Bind(wxEVT_BUTTON, &MainFrame::onButtonDelete, this);

	sizerAmount->Add( buttonDelete, 1, wxALL|wxEXPAND, 5 );

	wxButton *buttonDecrement = new wxButton( this, wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonDecrement->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	buttonDecrement->SetForegroundColour( wxColour( 240, 240, 240 ) );
	buttonDecrement->SetBackgroundColour( wxColour( 96, 96, 96 ) );
	buttonDecrement->SetMinSize( wxSize( 20,20 ) );

	buttonDecrement->Bind(wxEVT_BUTTON, &MainFrame::onButtonDecrement, this);

	sizerAmount->Add( buttonDecrement, 1, wxALL|wxEXPAND, 5 );

	wxButton *buttonIncrement = new wxButton( this, wxID_ANY, wxT("+"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonIncrement->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	buttonIncrement->SetForegroundColour( wxColour( 240, 240, 240 ) );
	buttonIncrement->SetBackgroundColour( wxColour( 96, 96, 96 ) );
	buttonIncrement->SetMinSize( wxSize( 20,20 ) );

	buttonIncrement->Bind(wxEVT_BUTTON, &MainFrame::onButtonIncrement, this);

	sizerAmount->Add( buttonIncrement, 1, wxALL|wxEXPAND, 5 );

	sizerOptions->Add( sizerAmount, 1, wxALL|wxEXPAND, 10 );

	wxPanel *m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel2->SetBackgroundColour( wxColour( 48, 48, 48 ) );

	sizerOptions->Add( m_panel2, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* sizerCommits = new wxBoxSizer( wxVERTICAL );

	wxButton *buttonClear = new wxButton( this, wxID_ANY, wxT("STORNO"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonClear->SetFont( wxFont( 16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	buttonClear->SetForegroundColour( wxColour( 240, 240, 240 ) );
	buttonClear->SetBackgroundColour( wxColour( 251, 17, 42 ) );

	buttonClear->Bind(wxEVT_BUTTON, &MainFrame::onButtonClear, this);

	sizerCommits->Add( buttonClear, 1, wxALL|wxEXPAND, 5 );

	wxButton *buttonSale = new wxButton( this, wxID_ANY, wxT("BAR"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSale->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ) );
	buttonSale->SetForegroundColour( wxColour( 240, 240, 240 ) );
	buttonSale->SetBackgroundColour( wxColour( 17, 170, 39 ) );

	buttonSale->Bind(wxEVT_BUTTON, &MainFrame::onButtonInvoice, this);

	sizerCommits->Add( buttonSale, 2, wxALL|wxEXPAND, 5 );

	sizerControls->Add( sizerOptions, 3, wxALL|wxEXPAND, 5 );
	sizerControls->Add( sizerCommits, 2, wxALL|wxEXPAND, 5 );


	sizerMiddle->Add( sizerControls, 1, wxBOTTOM|wxEXPAND|wxLEFT, 20 );

	sizerMain->Add( sizerMiddle, 6, wxEXPAND, 5 );

//*****************************************************************************************************************
// sizerRight contains the buttons for the categories and their products

    wxBoxSizer* sizerRight;
	sizerRight = new wxBoxSizer( wxVERTICAL );




	wxTextCtrl *displayUser = new wxTextCtrl( this, wxID_ANY, wxT("Kellner1"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT|wxBORDER_NONE |wxTE_READONLY );
	displayUser->Enable( false );
	displayUser->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans") ) );
	displayUser->SetForegroundColour( wxColour( 240, 240, 240 ) );
	displayUser->SetBackgroundColour( wxColour( 48, 48, 48 ) );

	sizerRight->Add( displayUser, 1, wxEXPAND|wxRIGHT|wxTOP, 20 );
	//---------------------------------------------------------------------------------
	// read the categories from the database and create a button for each one of it.
	// the id of the button corresponds to the id of the category in the database

	wxGridSizer* categorySizer = new wxGridSizer( 0, 3, 0, 0 );
	productSizer = new wxFlexGridSizer( 0, 4, 0, 0 );
	productSizer->SetFlexibleDirection(wxBOTH);
	productSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	productSizer->AddGrowableCol(0);
	productSizer->AddGrowableCol(1);
	productSizer->AddGrowableCol(2);
	productSizer->AddGrowableCol(3);

	database.getCategories();

	for (const auto& cat : database.categories) {
	wxButton *categoryButton = new wxButton( this, cat.id, cat.name, wxDefaultPosition, wxDefaultSize, 0);
	categoryButton->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans")));
	categoryButton->SetForegroundColour( wxColour( 240, 240, 240 ));
	categoryButton->SetBackgroundColour( wxColour( 59, 122, 87 ));

	categoryButton->Bind(wxEVT_BUTTON, &MainFrame::onCategoryButtonClicked, this);

	categorySizer->Add( categoryButton, 1, wxALL|wxEXPAND, 5 );

	//For each category get all the products from the database and create a button which is hidden by default

	database.getProducts(cat.id);
		for (const auto& prod : database.products) {
		productButton = new wxButton( this, prod.id, prod.name, wxDefaultPosition, wxDefaultSize, 0);
		productButton->SetMinSize(wxSize(0, 100));
		productButton->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans")));
		productButton->SetForegroundColour( wxColour( 240, 240, 240 ));
		productButton->SetBackgroundColour( wxColour( 59, 122, 87 ));
		productButton->Bind(wxEVT_BUTTON, &MainFrame::onProductButtonClicked, this);
		productButton->Hide();
		productSizer->Add( productButton, 1, wxALL|wxEXPAND, 5 );
		//productSizer->Hide(this->FindWindowById(prod.id));
		}
	}

	//---------------------------------------------------------------------------------------
	//get the products of the first category and show the buttons

	database.getProducts(_categoryId);
	for (const auto& prod : database.products){
		(this->FindWindowById(prod.id))->Show();
		//productSizer->Show(this->FindWindowById(prod.id));
	}

	sizerRight->Add( categorySizer, 5, wxALL|wxEXPAND, 20 );
	sizerRight->Add( productSizer, 6, wxALL|wxEXPAND, 20 );

	sizerMain->Add( sizerRight, 8, wxEXPAND, 5 );


	this->SetSizer( sizerMain );
	this->Layout();

	this->Centre( wxBOTH );

}

/***********************************************************************
 * Zeigt die Produkt-Buttons der gewählten Kategorie (button->Show())
 * ********************************************************************/

	void MainFrame::onCategoryButtonClicked(wxCommandEvent& evt){

	database.getProducts(_categoryId);

	for (const auto& prod : database.products){
		(this->FindWindowById(prod.id))->Hide();
		//productSizer->Hide(this->FindWindowById(prod.id));

	}
	_categoryId = evt.GetId();
	//wxMessageBox(wxString::Format("%d was clicked!",_categoryId));

	database.getProducts(_categoryId);
	for (const auto& prod : database.products){
		(this->FindWindowById(prod.id))->Show();
		//productSizer->Show(this->FindWindowById(prod.id));

	}
	Layout();
	}

/***********************************************************************
 * Aktualisiert die Anzeige der Produkte auf der aktuellen Rechnung im listView
 * Befindet sich das betreffende Produkt bereits auf der Liste wird die Anzahl erhöht
 * ansonsten wird das Produkt der Liste hinzugefügt
 * ********************************************************************/
	void MainFrame::addProductToDisplay(int id) {
 			auto it = std::find_if(_displayProducts.begin(), _displayProducts.end(), [&](const displayProduct& p) {
 				return p.id == id;
 			});

 			if (it != _displayProducts.end()) {
 				it->count += 1;
 				//std::cout << "Display, One more" << std::endl;
				auto index = std::distance(_displayProducts.begin(), it);
				//std::cout << "Index: " << index << std::endl;
				listView->SetItem(index, 0, wxString::Format("%d", it->count));
				listView->SetItem(index, 3, wxString::Format("%.2f", it->price * it->count));
				_sum += it->price;
				displaySum->SetValue(wxString::Format("%.2f", _sum));

 			} else {
 				_displayProducts.push_back(_displayProduct);

 				std::cout << "Display, Added Product" << std::endl;
 				long index = listView->InsertItem(listView->GetItemCount(), wxString::Format("%d", _displayProduct.count));
 				listView->SetItem(index, 1, _displayProduct.name);
				//double rounded = std::round(_displayProduct.price * 100.0) / 100.0;
 				listView->SetItem(index, 2, wxString::Format("%.2f", _displayProduct.price));
 				listView->SetItem(index, 3, wxString::Format("%.2f", _displayProduct.price * _displayProduct.count));

				_sum += _displayProduct.price;
				displaySum->SetValue(wxString::Format("%.2f", _sum));

 			}

			for(auto& prod : _displayProducts){
				std::cout << prod.count << "   " << prod.name << "   " << prod.price << "   " << std::endl;
			}
    }

/***********************************************************************
 * Fügt der aktuellen Rechnung ein Produkt hinzu oder erhöht die Anzahl
 * wenn das betreffende Produkt bereits auf der Rechnung ist
 * ********************************************************************/

	void MainFrame::onProductButtonClicked(wxCommandEvent& evt){
		int productId = evt.GetId();
		//wxMessageBox(wxString::Format("%d was clicked!",productId));
		database.getProduct(productId);
		_displayProduct.id = database.getProductId();
		_displayProduct.name = database.getProductName();
		_displayProduct.price = database.getProductPrice();
		_displayProduct.tax = database.getProductTax();
		addProductToDisplay(productId);
		database.addProductToAccount(productId);
	}

/***********************************************************************
 * Entfernt alle Positionen der aktuellen Rechnung
 * ********************************************************************/

	void MainFrame::onButtonClear(wxCommandEvent& evt){
		_displayProducts.clear();
		database.clearAccount();
		_sum = 0.0;
		displaySum->SetValue(wxString::Format("%.2f", _sum));
		listView->DeleteAllItems();
	}

/***********************************************************************
 * Erhöht die Anzahl eines Artikels auf der aktuellen Rechnung
 * ********************************************************************/

	void MainFrame::onButtonDecrement(wxCommandEvent& evt){
		int index = listView->GetFirstSelected();
		if (index != -1) {
			database.decrementAmountOfPosition(index);
			if(_displayProducts[index].count > 1) {
			_displayProducts[index].count -= 1;
			listView->SetItem(index, 0, wxString::Format("%d", _displayProducts[index].count));
			listView->SetItem(index, 3, wxString::Format("%.2f", _displayProducts[index].count * _displayProducts[index].price));
			_sum -= _displayProducts[index].price;
			displaySum->SetValue(wxString::Format("%.2f", _sum));
			}

		} else {
			// No item selected
			wxMessageBox("Kein Artikel markiert", "Warnung", wxOK | wxICON_WARNING);

			}
	}

/***********************************************************************
 * Erhöht die Anzahl eines Artikels auf der aktuellen Rechnung
 * ********************************************************************/

	void MainFrame::onButtonIncrement(wxCommandEvent& evt){
		int index = listView->GetFirstSelected();
		if (index != -1) {
			database.incrementAmountOfPosition(index);
			_displayProducts[index].count += 1;
			listView->SetItem(index, 0, wxString::Format("%d", _displayProducts[index].count));
			listView->SetItem(index, 3, wxString::Format("%.2f", _displayProducts[index].count * _displayProducts[index].price));
			_sum += _displayProducts[index].price;
			displaySum->SetValue(wxString::Format("%.2f", _sum));


		} else {
			// No item selected
			wxMessageBox("Kein Artikel markiert", "Warnung", wxOK | wxICON_WARNING);

			}
	}

/***********************************************************************
 * Löscht die ausgewählte Position von der aktuellen Rechnung
 * ********************************************************************/

	void MainFrame::onButtonDelete(wxCommandEvent& evt){
		int index = listView->GetFirstSelected();
		if (index != -1) {
			database.deletePosition(index);
			_sum -= _displayProducts[index].price * _displayProducts[index].count;
			_displayProducts.erase(_displayProducts.begin() + index);
			listView->DeleteItem(index);
			displaySum->SetValue(wxString::Format("%.2f", _sum));
			index = -1;
		} else {
			// No item selected
			wxMessageBox("Kein Artikel markiert", "Warnung", wxOK | wxICON_WARNING);

			}
	}

/***********************************************************************
 * Erstellt ein File als Rechnung
 * ********************************************************************/

	void MainFrame::issueInvoice(){

		std::string text1 = ".txt";
		std::string text2 = "Rechnung";
		std::string name = std::to_string(database.getAccountNumber());
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
		file << "Datum: " << name <<  std::endl <<  std::endl;


		file << "Anzahl	" << "Artikel				" << "Einzelpreis			" << "Gesamt" <<  std::endl;
		file << "_____________________________________________________________________" <<  std::endl;


		for (const auto& prod : _displayProducts){

            file << prod.count << "		" << prod.name << "				";
			file << std::fixed << std::setprecision(2) << prod.price << "				" << (prod.price * prod.count) <<std::endl;;

			if(prod.tax == 1.2){
				tax20 += prod.price * prod.count;
			}
			if(prod.tax == 1.1){
				tax10 += prod.price * prod.count;
			}
			sum += prod.price * prod.count;
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
		file << "	MwSt: ";
		file << std::fixed << std::setprecision(2) << tax10 - netto10;
		file << "	Brutto: ";
		file << std::fixed << std::setprecision(2) << tax10 <<std::endl <<std::endl <<std::endl;

		file << "Vielen Dank"  <<std::endl <<std::endl <<std::endl;

        file.close();

        std::cout << "File written successfully." << std::endl;
    } else {
        std::cerr << "Unable to open file." << std::endl;
    }

	}

/***********************************************************************
 * Erstellt eine Rechnung
 * ********************************************************************/

	void MainFrame::onButtonInvoice(wxCommandEvent& evt){
		issueInvoice();
		database.writeInvoiceToDatabase();
		database.writeSalesToDatabase();
		_displayProducts.clear();
		database.clearAccount();
		_sum = 0.0;
		displaySum->SetValue(wxString::Format("%.2f", _sum));
		listView->DeleteAllItems();
		database.setAccountNumber();
		displayAccountNumber->SetValue(wxString::Format("%d", database.getAccountNumber()));
		Layout();

	}

/***********************************************************************
 * Macht den Tagesabschluss
 * ********************************************************************/

	void MainFrame::onButtonClosing(wxCommandEvent& evt){

		double sum = database.getSum();
		double sum10 = database.getSumTax10();
		double sum20 = database.getSumTax20();
		std::cout << std::endl;
		std::cout << "****************************************" << std::endl << std::endl;
		std::cout << "Gesamtumsatz: ";
		std::cout << std::fixed << std::setprecision(2) << sum  << std::endl << std::endl;
		std::cout << "MwSt%: 20	";
		std::cout << "Brutto: ";
		std::cout << std::fixed << std::setprecision(2) << sum20;
		std::cout << "	Netto: ";
		std::cout << std::fixed << std::setprecision(2) << calculateTax20(sum20);
		std::cout << "	MwSt: ";
		std::cout << std::fixed << std::setprecision(2) << sum20 - calculateTax20(sum20)  << std::endl;
		std::cout << "MwSt%: 10	";
		std::cout << "Brutto: ";
		std::cout << std::fixed << std::setprecision(2) << sum10;
		std::cout << "	Netto: ";
		std::cout << std::fixed << std::setprecision(2) << calculateTax10(sum10);
		std::cout << "	MwSt: ";
		std::cout << std::fixed << std::setprecision(2) << sum10 - calculateTax10(sum10)  << std::endl;


		std::cout << std::endl;
		std::cout << "****************************************" << std::endl << std::endl;
// 		_displayProducts.clear();
// 		database.clearAccount();
// 		_sum = 0.0;
// 		displaySum->SetValue(wxString::Format("%.2f", _sum));
// 		listView->DeleteAllItems();
// 		database.setAccountNumber();
// 		displayAccountNumber->SetValue(wxString::Format("%d", database.getAccountNumber()));
// 		Layout();

	}

	double MainFrame::calculateTax20(double sum){

		double result = sum/1.2;

		return result;
	}

	double MainFrame::calculateTax10(double sum){

		double result = sum/1.1;

		return result;
	}
