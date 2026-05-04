#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "CDate.h"
#include <sstream>

TEST_CASE("Default constructor")
{
    CDate d;
    REQUIRE(d.IsValid());
    REQUIRE(d.GetDay() == 1);
    REQUIRE(d.GetMonth() == Month::JANUARY);
    REQUIRE(d.GetYear() == 1970);
}

TEST_CASE("Constructor from timestamp")
{
    CDate d1(0);
    REQUIRE(d1.IsValid());
    REQUIRE(d1.GetDay() == 1);
    REQUIRE(d1.GetMonth() == Month::JANUARY);
    REQUIRE(d1.GetYear() == 1970);

    CDate d2(31); 
    REQUIRE(d2.IsValid());
    REQUIRE(d2.GetDay() == 1);
    REQUIRE(d2.GetMonth() == Month::FEBRUARY);
    REQUIRE(d2.GetYear() == 1970);

    CDate d3(365); 
    REQUIRE(d3.IsValid());
    REQUIRE(d3.GetDay() == 1);
    REQUIRE(d3.GetMonth() == Month::JANUARY);
    REQUIRE(d3.GetYear() == 1971);
}

TEST_CASE("Valid and invalid dates")
{
    CDate valid(1, Month::JANUARY, 2000);
    REQUIRE(valid.IsValid());

    CDate invalid(31, Month::FEBRUARY, 2020);
    REQUIRE_FALSE(invalid.IsValid());
}

TEST_CASE("Leap year handling")
{
    // 29 февраля високосного
    CDate d1(29, Month::FEBRUARY, 2020);
    REQUIRE(d1.IsValid());

    // 29 февраля не високосного
    CDate d2(29, Month::FEBRUARY, 2021);
    REQUIRE_FALSE(d2.IsValid());

    // 28 февраля вис + 1 = 29
    CDate d3(28, Month::FEBRUARY, 2020);
    ++d3;
    REQUIRE(d3.GetDay() == 29);
    REQUIRE(d3.GetMonth() == Month::FEBRUARY);

    // 29 февраля невис + 1 = 1 
    ++d3;
    REQUIRE(d3.GetDay() == 1);
    REQUIRE(d3.GetMonth() == Month::MARCH);

    //в невысокосный норм перешло
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

TEST_CASE("Comparison operators")
{
    CDate d1(1, Month::JANUARY, 2010);
    CDate d2(2, Month::JANUARY, 2010);

    REQUIRE(d1 < d2);
    REQUIRE(d2 > d1);
    REQUIRE(d1 <= d2);
    REQUIRE(d2 >= d1);
    REQUIRE(d1 != d2);
}

TEST_CASE("Increment and decrement")
{
    CDate d(31, Month::DECEMBER, 2009);

    ++d;
    REQUIRE(d.GetDay() == 1);
    REQUIRE(d.GetMonth() == Month::JANUARY);
    REQUIRE(d.GetYear() == 2010);

    --d;
    REQUIRE(d.GetDay() == 31);
    REQUIRE(d.GetMonth() == Month::DECEMBER);
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

    REQUIRE(d.IsValid());
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

TEST_CASE("Invalid output")
{
    CDate d(31, Month::FEBRUARY, 2020);

    std::stringstream ss;
    ss << d;

    REQUIRE(ss.str() == "INVALID");
}

TEST_CASE("Out of range becomes invalid")
{
    CDate d(1, Month::JANUARY, 1970);
    d -= 1;

    REQUIRE_FALSE(d.IsValid());
}

TEST_CASE("Date difference large and negative")
{
    CDate d1(1, Month::JANUARY, 1970);
    CDate d2(31, Month::DECEMBER, 9999);
    REQUIRE((d2 - d1) > 0);

    CDate d3(2, Month::JANUARY, 1970);
    CDate d4(1, Month::JANUARY, 1970);
    REQUIRE((d3 - d4) == 1);
    REQUIRE((d4 - d3) == -1);
}

TEST_CASE("Addition with large days")
{
    CDate d(1, Month::JANUARY, 1970);
    CDate d2 = d + 365;
    REQUIRE(d2.GetYear() == 1971);
    REQUIRE(d2.GetMonth() == Month::JANUARY);
    REQUIRE(d2.GetDay() == 1);

    CDate d3 = d + 365 * 10;
    REQUIRE(d3.GetYear() == 1980);
}

TEST_CASE("Input operator invalid formats")
{
    CDate d;
    std::stringstream ss1("99.99.9999");
    ss1 >> d;
    REQUIRE_FALSE(d.IsValid());

    CDate d2;
    std::stringstream ss2("01-01-2020");
    ss2 >> d2;
    REQUIRE_FALSE(d2.IsValid());

    CDate d3;
    std::stringstream ss3("abc");
    ss3 >> d3;
    REQUIRE_FALSE(d3.IsValid());

    CDate d4;
    std::stringstream ss4("INVALID");
    ss4 >> d4;
    REQUIRE_FALSE(d4.IsValid());
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

TEST_CASE("Operations on invalid date")
{
    CDate invalid(31, Month::FEBRUARY, 2020);
    REQUIRE_FALSE(invalid.IsValid());

    invalid += 10;
    REQUIRE_FALSE(invalid.IsValid()); 

    invalid -= 5;
    REQUIRE_FALSE(invalid.IsValid());

    ++invalid;
    REQUIRE_FALSE(invalid.IsValid());

    CDate other(1, Month::JANUARY, 1970);
    
    auto sum1 = invalid + 5;
    REQUIRE_FALSE(sum1.IsValid());
    
    auto sum2 = 5 + invalid;
    REQUIRE_FALSE(sum2.IsValid());
    
    auto diff1 = invalid - 3;
    REQUIRE_FALSE(diff1.IsValid());
    
    int diff2 = invalid - other;
    REQUIRE(diff2 == -1); 

    REQUIRE_FALSE(invalid == other);
    REQUIRE(invalid != other);
    REQUIRE_FALSE(invalid < other);
    REQUIRE_FALSE(invalid > other);
}

TEST_CASE("Max valid date - boundary testing")
{
    CDate max(31, Month::DECEMBER, 9999);
    REQUIRE(max.IsValid());

    CDate over1(31, Month::DECEMBER, 9999);
    over1 += 1;
    REQUIRE_FALSE(over1.IsValid());

    CDate maxDate(31, Month::DECEMBER, 9999);
    CDate maxTimestamp(maxDate.GetDay(), maxDate.GetMonth(), maxDate.GetYear()); 
    
    CDate nearMax(30, Month::DECEMBER, 9999);
    nearMax += 2; 
    REQUIRE_FALSE(nearMax.IsValid());
}

TEST_CASE("Min valid date - boundary testing")
{
    CDate min(1, Month::JANUARY, 1970);
    REQUIRE(min.IsValid());

    CDate under = min;
    under -= 1;
    REQUIRE_FALSE(under.IsValid());

    CDate min2(1, Month::JANUARY, 1970);
    --min2;
    REQUIRE_FALSE(min2.IsValid());
}