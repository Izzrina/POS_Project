#ifndef SQLITEDB_HPP
#define SQLITEDB_HPP

#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Structs.hpp"

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

    std::vector<Category> getCategories();

    std::vector<Product> getProducts(int category);

    Product getProduct(int id);

    int getAccountNumber();

    void writeInvoiceToDatabase(int accountNumber, std::string user);

    void writeSalesToDatabase(std::vector<Product> account, int accountNumber);

    double getFinalSum();

    double getFinalSumTax10();

    double getFinalSumTax20();

    void deleteAllSalesData();

private:

    sqlite3 *db;
    sqlite3_stmt* stmt;
    char *zErrMsg = 0;
    int rc;

};

#endif
