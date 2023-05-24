#include "TaxCalculation.hpp"

#include <iostream>
#include <cmath>

 double TaxCalculation::calculateNet20Percent(double price)
{
    return price / 1.2;
}

 double TaxCalculation::calculateNet10Percent(double price)
{
    return price / 1.1;
}

 double TaxCalculation::calculateTax20Percent(double price)
{
    return price - price / 1.2;
}

 double TaxCalculation::calculateTax10Percent(double price)
{
    return price - price / 1.1;
}
