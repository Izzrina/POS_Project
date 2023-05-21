#ifndef STRUCTS_HPP
#define STRUCTS_HPP

struct Category {
    int id;
    std::string name;
};


struct Product {
    int amount = 1;
    int id;
    std::string name;
    double price;
    double tax;
};


#endif
