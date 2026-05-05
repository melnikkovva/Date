#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "CDate.h"
#include <sstream>

TEST_CASE("Default constructor")
{
    CDate d;
    REQUIRE(d.GetDay() == 1);
    REQUIRE(d.GetMonth() == Month::JANUARY);
    REQUIRE(d.GetYear() == 1970);
}

TEST_CASE("Constructor from timestamp")
{
    CDate d1(0);
    REQUIRE(d1.GetDay() == 1);
    REQUIRE(d1.GetMonth() == Month::JANUARY);
    REQUIRE(d1.GetYear() == 1970);

    CDate d2(31); 
    REQUIRE(d2.GetDay() == 1);
    REQUIRE(d2.GetMonth() == Month::FEBRUARY);
    REQUIRE(d2.GetYear() == 1970);

    CDate d3(365); 
    REQUIRE(d3.GetDay() == 1);
    REQUIRE(d3.GetMonth() == Month::JANUARY);
    REQUIRE(d3.GetYear() == 1971);
}

TEST_CASE("Valid and invalid dates")
{
    REQUIRE_NOTHROW(CDate(1, Month::JANUARY, 2000));
    
    REQUIRE_THROWS_AS(CDate(31, Month::FEBRUARY, 2020), std::invalid_argument);
    REQUIRE_THROWS_AS(CDate(0, Month::JANUARY, 2020), std::invalid_argument);
    REQUIRE_THROWS_AS(CDate(1, Month::JANUARY, 1969), std::invalid_argument);
    REQUIRE_THROWS_AS(CDate(1, Month::JANUARY, 10000), std::invalid_argument);
}

TEST_CASE("Leap year handling")
{
    // 29 февраля високосного
    REQUIRE_NOTHROW(CDate(29, Month::FEBRUARY, 2020));

    // 29 февраля не високосного
    REQUIRE_THROWS_AS(CDate(29, Month::FEBRUARY, 2021), std::invalid_argument);

    // 28 февраля вис + 1 = 29
    CDate d3(28, Month::FEBRUARY, 2020);
    ++d3;
    REQUIRE(d3.GetDay() == 29);
    REQUIRE(d3.GetMonth() == Month::FEBRUARY);

    // 29 февраля вис + 1 = 1
    ++d3;
    REQUIRE(d3.GetDay() == 1);
    REQUIRE(d3.GetMonth() == Month::MARCH);

    // 28 фев невыс +1 = 1
    CDate d4(28, Month::FEBRUARY, 2021);
    ++d4;
    REQUIRE(d4.GetDay() == 1);
    REQUIRE(d4.GetMonth() == Month::MARCH);
}

TEST_CASE("Add days")
{
    CDate d(28, Month::FEBRUARY, 2010);
    d += 3;

    REQUIRE(d.GetDay() == 3);
    REQUIRE(d.GetMonth() == Month::MARCH);
    REQUIRE(d.GetYear() == 2010);
}

TEST_CASE("Subtract days")
{
    CDate d(1, Month::JANUARY, 2010);
    d -= 2;

    REQUIRE(d.GetDay() == 30);
    REQUIRE(d.GetMonth() == Month::DECEMBER);
    REQUIRE(d.GetYear() == 2009);
}

TEST_CASE("Date difference")
{
    CDate d1(1, Month::JANUARY, 2010);
    CDate d2(30, Month::DECEMBER, 2009);

    REQUIRE((d1 - d2) == 2);
    REQUIRE((d2 - d1) == -2);
}

TEST_CASE("Addition")
{
    CDate d(1, Month::JANUARY, 1970);
    CDate d2 = d + 365;
    REQUIRE(d2.GetYear() == 1971);
    REQUIRE(d2.GetMonth() == Month::JANUARY);
    REQUIRE(d2.GetDay() == 1);

    CDate d3 = d + 365 * 10;
    REQUIRE(d3.GetYear() == 1979);
}

TEST_CASE("Comparison operators")
{
    SECTION("Equal dates")
    {
        CDate d1(1, Month::JANUARY, 2010);
        CDate d2(1, Month::JANUARY, 2010);
        
        REQUIRE(d1 == d2);
        REQUIRE(d1 <= d2);
        REQUIRE(d1 >= d2);
        REQUIRE(!(d1 != d2));
        REQUIRE(!(d1 < d2));
        REQUIRE(!(d1 > d2));
    }
    
    SECTION("Different days same month")
    {
        CDate d1(1, Month::JANUARY, 2010);
        CDate d2(2, Month::JANUARY, 2010);
        CDate d3(31, Month::JANUARY, 2010);
        
        REQUIRE(d1 < d2);
        REQUIRE(d2 > d1);
        REQUIRE(d1 <= d2);
        REQUIRE(d2 >= d1);
        REQUIRE(d1 != d2);
        
        REQUIRE(d1 < d3);
        REQUIRE(d2 < d3);
        REQUIRE(d3 > d2);
        REQUIRE(d3 >= d2);
    }
    
    SECTION("Different months same year")
    {
        CDate d1(1, Month::JANUARY, 2010);
        CDate d2(1, Month::FEBRUARY, 2010);
        CDate d3(1, Month::DECEMBER, 2010);
        
        REQUIRE(d1 < d2);
        REQUIRE(d2 > d1);
        REQUIRE(d2 < d3);
        REQUIRE(d3 > d2);
        REQUIRE(d1 < d3);
        REQUIRE(d3 > d1);
        
        REQUIRE(d1 != d2);
        REQUIRE(d2 != d3);
    }
    
    SECTION("Different years")
    {
        CDate d1(1, Month::JANUARY, 1970);
        CDate d2(1, Month::JANUARY, 2000);
        CDate d3(31, Month::DECEMBER, 9999);
        
        REQUIRE(d1 < d2);
        REQUIRE(d2 > d1);
        REQUIRE(d2 < d3);
        REQUIRE(d3 > d2);
        REQUIRE(d1 < d3);
        
        REQUIRE(d1 != d2);
        REQUIRE(d2 != d3);
    }
    
    SECTION("Boundary dates")
    {
        CDate d1(1, Month::JANUARY, 1970); 
        CDate d2(31, Month::DECEMBER, 9999);
        
        REQUIRE(d1 < d2);
        REQUIRE(d2 > d1);
        REQUIRE(d1 <= d2);
        REQUIRE(d2 >= d1);
        REQUIRE(d1 != d2);
    }
    
    SECTION("Month and year boundaries")
    {
        CDate d1(31, Month::DECEMBER, 2009);
        CDate d2(1, Month::JANUARY, 2010);
        CDate d3(28, Month::FEBRUARY, 2010);
        CDate d4(1, Month::MARCH, 2010);
        
        REQUIRE(d1 < d2); 
        REQUIRE(d2 < d3);  
        REQUIRE(d3 < d4);  
        
        REQUIRE(d2 > d1);
        REQUIRE(d3 > d2);
        REQUIRE(d4 > d3);
        
        REQUIRE(d1 != d2);
        REQUIRE(d2 != d3);
        REQUIRE(d3 != d4);
    }
    
    SECTION("Leap years")
    {
        CDate d1(28, Month::FEBRUARY, 2000); 
        CDate d2(29, Month::FEBRUARY, 2000); 
        CDate d3(1, Month::MARCH, 2000);    
        
        REQUIRE(d1 < d2);
        REQUIRE(d2 < d3);
        REQUIRE(d1 < d3);
        
        REQUIRE(d2 > d1);
        REQUIRE(d3 > d2);
        
        REQUIRE(d1 != d2);
        REQUIRE(d2 != d3);
    }
}

TEST_CASE("Week day")
{
    CDate d(1, Month::JANUARY, 1970);
    REQUIRE(d.GetWeekDay() == WeekDay::THURSDAY);
}

TEST_CASE("Input operator")
{
    std::stringstream ss("03.03.2010");

    CDate d;
    ss >> d;

    REQUIRE(d.GetDay() == 3);
    REQUIRE(d.GetMonth() == Month::MARCH);
    REQUIRE(d.GetYear() == 2010);
}

TEST_CASE("Output operator")
{
    CDate d(3, Month::MARCH, 2010);

    std::stringstream ss;
    ss << d;

    REQUIRE(ss.str() == "03.03.2010");
}

TEST_CASE("Input operator invalid formats")
{
    CDate d;
    
    std::stringstream ss1("99.99.9999");
    REQUIRE_THROWS_AS(ss1 >> d, std::invalid_argument);

    std::stringstream ss2("01-01-2020");
    REQUIRE_THROWS_AS(ss2 >> d, std::invalid_argument);

    std::stringstream ss3("abc");
    REQUIRE_THROWS_AS(ss3 >> d, std::invalid_argument);
}

TEST_CASE("Postfix increment/decrement")
{
    CDate d(31, Month::DECEMBER, 1999);
    CDate old = d++;
    REQUIRE(old.GetDay() == 31);
    REQUIRE(old.GetYear() == 1999);
    REQUIRE(d.GetDay() == 1);
    REQUIRE(d.GetYear() == 2000);

    CDate d2(1, Month::JANUARY, 2000);
    CDate old2 = d2--;
    REQUIRE(old2.GetDay() == 1);
    REQUIRE(d2.GetDay() == 31);
    REQUIRE(d2.GetYear() == 1999);
}

TEST_CASE("Week day for various dates")
{
    CDate d1(1, Month::JANUARY, 1970);
    REQUIRE(d1.GetWeekDay() == WeekDay::THURSDAY);

    CDate d2(1, Month::JANUARY, 1971);
    REQUIRE(d2.GetWeekDay() == WeekDay::FRIDAY);

    CDate d3(31, Month::DECEMBER, 1999);
    REQUIRE(d3.GetWeekDay() == WeekDay::FRIDAY);
}

TEST_CASE("Operator + and - with boundaries")
{
    REQUIRE_NOTHROW(CDate(1, Month::JANUARY, 1970) + 100);
    REQUIRE_THROWS_AS(CDate(1, Month::JANUARY, 1970) - 1, std::out_of_range);
    REQUIRE_THROWS_AS(CDate(31, Month::DECEMBER, 9999) + 1, std::out_of_range);
}