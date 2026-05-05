#pragma once
#include <iostream>
#include <compare>

enum class Month
{
    JANUARY = 1, FEBRUARY, MARCH, APRIL,
    MAY, JUNE, JULY, AUGUST, SEPTEMBER,
    OCTOBER, NOVEMBER, DECEMBER
};

enum class WeekDay
{
    SUNDAY = 0, MONDAY, TUESDAY, WEDNESDAY,
    THURSDAY, FRIDAY, SATURDAY
};

class CDate
{
public:
    CDate();
    CDate(unsigned day, Month month, unsigned year);
    explicit CDate(unsigned timestamp);

    unsigned GetDay() const;
    Month GetMonth() const;
    unsigned GetYear() const;
    WeekDay GetWeekDay() const;
    unsigned GetTimestamp() const { return m_days; }

    CDate& operator++();
    CDate operator++(int);
    CDate& operator--();
    CDate operator--(int);
    CDate& operator+=(int days);
    CDate& operator-=(int days);
    
    auto operator<=>(const CDate&) const = default;
    
private:
    static bool IsLeapYear(unsigned year);
    static unsigned CountDaysInMonth(Month m, unsigned year);
    static unsigned MakeTimestamp(unsigned d, Month m, unsigned y);
    static void MakeDateFormat(unsigned days, unsigned& d, Month& m, unsigned& y);
    void AddDays(int delta);
    static unsigned GetMaxDays();

    unsigned m_days{0};
};

CDate operator+(const CDate& d, int days);
CDate operator+(int days, const CDate& d);
CDate operator-(const CDate& d, int days);
int operator-(const CDate& a, const CDate& b);
std::ostream& operator<<(std::ostream& os, const CDate& d);
std::istream& operator>>(std::istream& is, CDate& d);