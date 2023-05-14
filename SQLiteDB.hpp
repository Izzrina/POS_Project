#ifndef SQLITEDB_HPP
#define SQLITEDB_HPP

#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <algorithm>


class SQLiteDB {

public:

    //constructor
    SQLiteDB(const char* filename) {
        rc = sqlite3_open(filename, &db);
        if(rc){
            std::cout << "Database opened successfully" << std::endl;
        }
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Could not open database");
        }
    }

    //destructor
    ~SQLiteDB() {
        sqlite3_close(db);
        std::cout << "Database closed successfully" << std::endl;
    }

    struct Category {
        int id;
        std::string name;
    };

    std::vector<Category> categories;

    struct Product {
        int count = 1;
        int id;
        std::string name;
        double price;
        double tax;
    };

    std::vector<Product> products;


    void getCategories();               //schreibt id und Name aller Kategorien in vector<Category> catgories

    void getProducts(int category);     //schreibt alle Produkte einer Kategorie in vector<Products> products

    void getProduct(int id);            // findet alle Spalten eines bestimmten Products in der Datenbank

    void addProductToAccount(int id);   // Fügt dem vector<Product> _account ein Produkt hinzu oder ändert die Anzahl des Produkts wenn es bereits im Vektor vorhanden ist

    void setAccountNumber();            //liest die letzte Rechnungsnummer aus der Datenbank und setzt die _accountNumber für die aktuelle Rechnung

    void clearAccount();                // Löscht den Vektor _account und somit alle Positionen der aktuellen Rechnung

    void deletePosition(int index);     // Nimmt als Parameter den Index und löscht das entsprechende Element aus dem Vektor

    void incrementAmountOfPosition(int index);  // Nimmt als Parameter den Index und erhöht die Anzahl des entsprechenden Elements im Vektor _account

    void decrementAmountOfPosition(int index);  // Nimmt als Parameter den Index und verringert die Anzahl des entsprechenden Elements im Vektor _account

    void writeInvoiceToDatabase();          //Speichert die aktuelle Rechnung in der Datenbank

    void writeSalesToDatabase();            //Speichert die Positionen der aktuellen Rechnung in der Datenbank;

    double getSumTax20();   // Liest die Verkäufe aus der Datenbank und addiert alle Werte mit 20% Mehrwertsteuer

    double getSumTax10();   //  Liest die Verkäufe aus der Datenbank und addiert alle Werte mit 10 % Mehrwertsteuer

    double getSum();    //  Liest die Verkäufe aus der Datenbank und berechnet den Gesamtumsatz

    /* -------------------Getter Methoden:---------------------*/

    int getAccountNumber();

    int getProductId();

    std::string getProductName();

    double getProductPrice();

    double getProductTax();

    std::string getUser();


private:

    sqlite3 *db;
    sqlite3_stmt* stmt;
    char *zErrMsg = 0;
    int rc;

    std::string _user = "Arbeiter 1";
    int _accountNumber = 0;
    std::vector<Product> _account;
    Product _product;

};


#endif
