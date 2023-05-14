#include "SQLiteDB.hpp"

#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <algorithm>

/**********************************************************
*Nimmt als Parameter die id einer Kategorie und schreibt
* die entsprechenden Produkte in den Vektor products
***********************************************************/

    void SQLiteDB::getProducts(int category) {

    products.clear();

    const char* sql = "SELECT id, productname, price, tax FROM products WHERE categoryId = ?;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Error preparing SQL statement");
        }

        sqlite3_bind_int(stmt, 1, category);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
                Product product;
                product.id = sqlite3_column_int(stmt, 0);
                product.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                product.price = sqlite3_column_double(stmt, 2);
                product.tax = sqlite3_column_double(stmt, 3);

                products.push_back(product);
            }

        sqlite3_finalize(stmt);
    }

/*******************************************************
 *Nimmt als Parameter die id einer Kategorie und schreibt
 * die entsprechenden Produkte in den Vektor products
 *******************************************************/

    void SQLiteDB::getProduct (int id) {

    const char* sql = "SELECT id, productname, price, tax FROM products WHERE id = ?;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Error preparing SQL statement");
        }

        sqlite3_bind_int(stmt, 1, id);


        while (sqlite3_step(stmt) == SQLITE_ROW) {

                _product.id = sqlite3_column_int(stmt, 0);
                _product.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                _product.price = sqlite3_column_double(stmt, 2);
                _product.tax = sqlite3_column_double(stmt, 3);
            }
            std::cout << _product.name << "   " <<  _product.price << std::endl;

        sqlite3_finalize(stmt);
    }

/*******************************************************
 *Liest die Kategorienen aus der Datenbank in den
 * Vektor categories
 *******************************************************/

    void SQLiteDB::getCategories() {

    categories.clear();

    const char* sql = "SELECT id, categoryName FROM categories;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Error preparing SQL statement");
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
                Category category;
                category.id = sqlite3_column_int(stmt, 0);
                category.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                categories.push_back(category);
            }

        sqlite3_finalize(stmt);
    }

/*******************************************************
 *  Liest die letzte Rechnungsnummer aus der Datenbank und
 * setzt die aktuelle Rechnungsnummer +1
 *******************************************************/

    void SQLiteDB::setAccountNumber(){

    std::string sql = "SELECT MAX(id) FROM accounts";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
      std::cerr << "Fehler beim Vorbereiten der SQL-Abfrage: " << sqlite3_errmsg(db) << std::endl;
    }

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
      _accountNumber = sqlite3_column_int(stmt, 0) +1;
    }

    sqlite3_finalize(stmt);
    }


/***********************************************************
 *  Getter-Methoden
 ***********************************************************/

    int SQLiteDB::getProductId(){
        return _product.id;
    }

        int SQLiteDB::getAccountNumber(){
       return _accountNumber;
    }


    std::string SQLiteDB::getProductName(){
        return _product.name;
    }

    double SQLiteDB::getProductPrice() {
        return _product.price;
    }

    double SQLiteDB::getProductTax() {
        return _product.tax;
    }

    std::string SQLiteDB::getUser(){
        return _user;
    }

/********************************************************
 *Nimmt als Parameter die id eines Produkts und fügt dieses
 * dem Vektor _account hinzu wenn es noch nicht vorhanden ist.
 * Ansonsten wird dessen Anzahl um 1 erhöht.
 ********************************************************/

    void SQLiteDB::addProductToAccount(int id) {
 		auto iterator = std::find_if(_account.begin(), _account.end(), [&](const Product& p) {
 			return p.id == id;
 		});

 		if (iterator != _account.end()) {

 			iterator->count += 1;
 			std::cout << "One more" << std::endl;
 		} else {

 			_account.push_back(_product);
 			std::cout << "Added Product" << std::endl;
 		}
 	}
/**********************************************************
 *Löscht den Vektor _account, in dem die Positionen der aktuellen
 * Rechnung gespeichert sind
 **********************************************************/

    void SQLiteDB::clearAccount(){
        _account.clear();
    }

/**************************************************************
 *Nimmt als Parameter einen Index und löscht die entsprechende
 * Position im Vektor _account, in dem die Positionen der aktuellen
 * Rechnung gespeichert sind
 **************************************************************/

    void SQLiteDB::deletePosition(int index){
        std::cout << "Account size: " << _account.size() << std::endl;
        _account.erase(_account.begin() + index);
        std::cout << "Product deletet at position " << index << std::endl;
        std::cout << "Account size: " << _account.size() << std::endl;
    }

/**************************************************************
 *Nimmt als Parameter einen Index und erhöht an der entsprechenden
 * Position im Vektor _account die Anzahl des Produkts
 **************************************************************/

    void SQLiteDB::incrementAmountOfPosition(int index){
        _account[index].count += 1;

    }

/**********************************************************
 *Nimmt als Parameter einen Index und verringert an der entsprechenden
 * Position im Vektor _account die Anzahl des Produkts,
 * allerdings nur solange die höher ist als 1
 **********************************************************/

    void SQLiteDB::decrementAmountOfPosition(int index){
        if(_account[index].count > 1){
        _account[index].count -= 1;
        }
    }

/**********************************************************
 *Schreibt die aktuelle Rechnung (Rechnungsnummer, Benutzer
 * und Datum) in die Datenbank
 **********************************************************/

    void SQLiteDB::writeInvoiceToDatabase()
    {
        time_t now = time(0);
        char* date = ctime(&now);

        std::string sql = "INSERT INTO accounts VALUES ("
                        + std::to_string(_accountNumber) + ", '"
                        + _user + "', '"
                        + date + "');";


        char* errorMessage;
        rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errorMessage);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to insert record: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
    }

/**********************************************************
 * Schreibt die Positionen der aktuellen Rechnung in die Datenbank
 **********************************************************/

    void SQLiteDB::writeSalesToDatabase()
    {

        for (auto& prod : _account){
        std::string sql = "INSERT INTO sales VALUES ("
                        + std::to_string(prod.id) + ", "
                        + std::to_string(_accountNumber) + ", "
                        + std::to_string(prod.count) + ", "
                        + std::to_string(prod.price) + ", "
                        + std::to_string(prod.tax) + ");";


        char* errorMessage;
        rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errorMessage);
            if (rc != SQLITE_OK) {
                std::cerr << "Failed to insert record: " << errorMessage << std::endl;
                sqlite3_free(errorMessage);
            }

        }
    }

/**********************************************************
 *Liest alle Verkäufe aus der Datenbank und berechnet den Gesamtumsatz
 **********************************************************/
    double SQLiteDB::getSum()
{
    double finalSum = 0.0;

    const char* sql = "SELECT productCount, price FROM sales;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Error preparing SQL statement");
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
                int amount = sqlite3_column_int(stmt, 0);
                double price = sqlite3_column_double(stmt, 1);
                finalSum += amount * price;
            }

        sqlite3_finalize(stmt);

        return finalSum;
}

/**********************************************************
 *Berechnet die Summe aller Verkäufe mit 10% MwSt
 **********************************************************/
    double SQLiteDB::getSumTax10()
{
    double sum = 0.0;

    const char* sql = "SELECT productCount, price FROM sales WHERE tax = 1.1;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Error preparing SQL statement");
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
                int amount = sqlite3_column_int(stmt, 0);
                double price = sqlite3_column_double(stmt, 1);
                sum += amount * price;
            }

        sqlite3_finalize(stmt);

        return sum;
}

/**********************************************************
 *Berechnet die Summe aller Verkäufe mit 20% MwSt
 **********************************************************/
    double SQLiteDB::getSumTax20()
{
    double sum = 0.0;

    const char* sql = "SELECT productCount, price FROM sales WHERE tax = 1.2;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Error preparing SQL statement");
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
                int amount = sqlite3_column_int(stmt, 0);
                double price = sqlite3_column_double(stmt, 1);
                sum += amount * price;
            }

        sqlite3_finalize(stmt);

        return sum;
}
