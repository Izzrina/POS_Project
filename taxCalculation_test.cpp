#include "TaxCalculation.hpp"


#include "catch_amalgamated.hpp"


TEST_CASE("Net Amount is calculated correctly", "[taxCalucaltion]") {
  TaxCalculation tax;

  SECTION("caluclate net amount when tax is 20%", "[price]") {
    REQUIRE(tax.calculateNet20Percent(0.0) == 0.0);
    REQUIRE(tax.calculateNet20Percent(0.9) == 0.75);
    REQUIRE(tax.calculateNet20Percent(16.8) == Catch::Approx(14.0).epsilon(0.01));
    REQUIRE(tax.calculateNet20Percent(20.0) == Catch::Approx(16.67).epsilon(0.01));
    REQUIRE(tax.calculateNet20Percent(120.0) == 100.0);
  }

  SECTION("calculate net amount when tax is 10%", "[price]") {
    REQUIRE(tax.calculateNet10Percent(0.0) == 0.0);
    REQUIRE(tax.calculateNet10Percent(0.9) == Catch::Approx(0.82).epsilon(0.01));
    REQUIRE(tax.calculateNet10Percent(16.8) == Catch::Approx(15.27).epsilon(0.01));
    REQUIRE(tax.calculateNet10Percent(20.0) == Catch::Approx(18.18).epsilon(0.01));
    REQUIRE(tax.calculateNet10Percent(110.0) == Catch::Approx(100.0).epsilon(0.01));
  }


  SECTION("Calulate 20% tax", "[price]") {
    REQUIRE(tax.calculateTax20Percent(0.0) == 0.0);
    REQUIRE(tax.calculateTax20Percent(0.9) == Catch::Approx(0.15).epsilon(0.01));
    REQUIRE(tax.calculateTax20Percent(16.8) == Catch::Approx(2.8).epsilon(0.01));
    REQUIRE(tax.calculateTax20Percent(20.0) == Catch::Approx(3.33).epsilon(0.01));
    REQUIRE(tax.calculateTax20Percent(120.0) == Catch::Approx(20.0).epsilon(0.01));
  }

  SECTION("Calulate 10% tax", "[price]") {
    REQUIRE(tax.calculateTax10Percent(0.0) == 0.0);
    //REQUIRE(tax.calculateTax10Percent(0.9) == Catch::Approx(0.08).epsilon(0.01));
    REQUIRE(tax.calculateTax10Percent(16.8) == Catch::Approx(1.53).epsilon(0.01));
    REQUIRE(tax.calculateTax10Percent(20.0) == Catch::Approx(1.82).epsilon(0.01));
    REQUIRE(tax.calculateTax10Percent(110.0) == Catch::Approx(10.0).epsilon(0.01));
  }

}
