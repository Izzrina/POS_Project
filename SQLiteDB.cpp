#include "SQLiteDB.hpp"
#include "Structs.hpp"

#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <algorithm>


/**
 * Retrieves the categories from the database.
 * @return A vector of Category objects representing the categories.
 */
std::vector<Category> SQLiteDB::getCategories()
{

    std::vector<Category> cat;

    const char* sql = "SELECT id, name FROM categories;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        throw std::runtime_error("Error preparing SQL statement");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Category category;
        category.id = sqlite3_column_int(stmt, 0);
        category.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        cat.push_back(category);
    }

    sqlite3_finalize(stmt);

    return cat;
}

/**
 * Retrieves the products of a specific category from the database.
 * @param category The ID of the category.
 * @return A vector of Product objects representing the products.
 */
std::vector<Product> SQLiteDB::getProducts(int category)
{

    std::vector<Product> prod;

    const char* sql = "SELECT id, name, price, tax FROM products WHERE categoryId = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        throw std::runtime_error("Error preparing SQL statement");
    }

    sqlite3_bind_int(stmt, 1, category);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Product product;
        product.id = sqlite3_column_int(stmt, 0);
        product.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        product.price = sqlite3_column_double(stmt, 2);
        product.tax = sqlite3_column_double(stmt, 3);

        prod.push_back(product);
    }

    sqlite3_finalize(stmt);

    return prod;
}

/**
 * Retrieves a product from the database based on its ID.
 * @param id The ID of the product.
 * @return A Product object representing the product.
 */
Product SQLiteDB::getProduct (int id)
{

    Product product;

    const char* sql = "SELECT id, name, price, tax FROM products WHERE id = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        throw std::runtime_error("Error preparing SQL statement");
    }

    sqlite3_bind_int(stmt, 1, id);


    while (sqlite3_step(stmt) == SQLITE_ROW)
    {

        product.id = sqlite3_column_int(stmt, 0);
        product.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        product.price = sqlite3_column_double(stmt, 2);
        product.tax = sqlite3_column_double(stmt, 3);
    }

    sqlite3_finalize(stmt);

    return product;
}

/**
 * Retrieves the next available account number.
 * @return The next available account number.
 */
int SQLiteDB::getAccountNumber()
{

    int number = 0;

    std::string sql = "SELECT MAX(accountNumber) FROM accounts";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Fehler beim Vorbereiten der SQL-Abfrage: " << sqlite3_errmsg(db) << std::endl;
    }

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {
        number = sqlite3_column_int(stmt, 0) +1;
    }

    sqlite3_finalize(stmt);

    return number;
}

/**
 * Writes an invoice record to the database.
 * @param accountNumber The account number associated with the invoice.
 * @param user The user associated with the invoice.
 */
void SQLiteDB::writeInvoiceToDatabase(int accountNumber, std::string user)
{
    time_t now = time(0);
    char* date = ctime(&now);

    std::string sql = "INSERT INTO accounts VALUES ("
    + std::to_string(accountNumber) + ", '"
    + user + "', '"
    + date + "');";

    char* errorMessage;
    rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errorMessage);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to insert record: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}

/**
 * Writes sales records to the database.
 * @param account A vector of products representing the sales.
 * @param accountNumber The account number associated with the sales.
 */
void SQLiteDB::writeSalesToDatabase(std::vector<Product> account, int accountNumber)
{
    for (auto& prod : account){
        std::string sql = "INSERT INTO sales VALUES ("
        + std::to_string(prod.id) + ", "
        + std::to_string(accountNumber) + ", "
        + std::to_string(prod.amount) + ", "
        + std::to_string(prod.price) + ", "
        + std::to_string(prod.tax) + ");";

        char* errorMessage;
        rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errorMessage);
        if (rc != SQLITE_OK)
        {
            std::cerr << "Failed to insert record: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
      }
}

/**
 * Calculates the total sum of all sales.
 * @return The total sum of all sales.
 */
double SQLiteDB::getFinalSum()
{
    double finalSum = 0.0;

    const char* sql = "SELECT amount, price FROM sales;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        throw std::runtime_error("Error preparing SQL statement");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int amount = sqlite3_column_int(stmt, 0);
        double price = sqlite3_column_double(stmt, 1);
        finalSum += amount * price;
    }

    sqlite3_finalize(stmt);

    return finalSum;
}

/**
 * Calculates the total sum of sales with a tax rate of 10%.
 * @return The total sum of sales with a tax rate of 10%.
 */
double SQLiteDB::getFinalSumTax10()
{
    double sum = 0.0;

    const char* sql = "SELECT amount, price FROM sales WHERE tax = 1.1;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        throw std::runtime_error("Error preparing SQL statement");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int amount = sqlite3_column_int(stmt, 0);
        double price = sqlite3_column_double(stmt, 1);
        sum += amount * price;
    }

    sqlite3_finalize(stmt);

    return sum;
}

/**
 * Calculates the total sum of sales with a tax rate of 20%.
 * @return The total sum of sales with a tax rate of 20%.
 */
double SQLiteDB::getFinalSumTax20()
{
    double sum = 0.0;

    const char* sql = "SELECT amount, price FROM sales WHERE tax = 1.2;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        throw std::runtime_error("Error preparing SQL statement");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int amount = sqlite3_column_int(stmt, 0);
        double price = sqlite3_column_double(stmt, 1);
        sum += amount * price;
    }

    sqlite3_finalize(stmt);

    return sum;
}

/**
 * Deletes all records in the "sales" table of an SQLite database.
 */
void SQLiteDB::deleteAllSalesData() {

    const char* sql = "DELETE FROM sales;";
    char* errorMessage;
    rc = sqlite3_exec(db, sql, NULL, NULL, &errorMessage);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to delete data: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
  }

