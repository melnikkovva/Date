#include "CDate.h"
#include <iomanip>
#include <sstream>

constexpr unsigned NORMAL_YEAR_LENGTH = 365;
constexpr unsigned MONTHS_COUNT = 12;
constexpr unsigned WEEK_LENGTH = 7;
constexpr unsigned MIN_YEAR = 1970;
constexpr unsigned MAX_YEAR = 9999;
constexpr unsigned SHIFT_BETWEEN_1970_2000 = 719468;
constexpr unsigned FOUR_CENTURIES_CYCLE = 146097;
constexpr unsigned FOUR_YEARS_CYCLE = 1461;
constexpr unsigned CENTURY_CYCLE = 36524;
constexpr unsigned CENTURY_LENGTH = 400;
constexpr unsigned FIVE_MONTHS_DAYS = 153;

static constexpr unsigned monthLengths[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

CDate::CDate() : m_days(0), m_isValid(true) {}

CDate::CDate(unsigned day, Month month, unsigned year)
{
    m_isValid = MakeTimestamp(day, month, year, m_days);
}

CDate::CDate(unsigned timestamp) : m_days(timestamp), m_isValid(timestamp <= GetMaxDays()) {}

unsigned CDate::GetDay() const
{
    unsigned d, y;
    Month m;
    FromDays(m_days, d, m, y);
    return d;
}

Month CDate::GetMonth() const
{
    unsigned d, y;
    Month m;
    FromDays(m_days, d, m, y);
    return m;
}

unsigned CDate::GetYear() const
{
    unsigned d, y;
    Month m;
    FromDays(m_days, d, m, y);
    return y;
}

WeekDay CDate::GetWeekDay() const
{
    return WeekDay((m_days + 4) % WEEK_LENGTH);
}

bool CDate::IsValid() const
{
    return m_isValid;
}

bool CDate::IsLeapYear(unsigned year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

unsigned CDate::DaysInMonth(Month m, unsigned year)
{
    unsigned monthNum = static_cast<unsigned>(m);
    if (monthNum == 2 && IsLeapYear(year)) return 29;
    return monthLengths[monthNum - 1];
}

bool CDate::MakeTimestamp(unsigned d, Month m, unsigned y, unsigned& out)
{
    if (y < MIN_YEAR || y > MAX_YEAR) return false;
    unsigned monthNum = static_cast<unsigned>(m);
    if (monthNum < 1 || monthNum > MONTHS_COUNT) return false;
    if (d < 1 || d > DaysInMonth(m, y)) return false;
    
    signed adjustedYear = y - (monthNum <= 2 ? 1 : 0);
    int epochGroup = (adjustedYear >= 0 ? adjustedYear : adjustedYear - (CENTURY_LENGTH - 1)) / CENTURY_LENGTH;
    unsigned yearInGroup = adjustedYear - epochGroup * CENTURY_LENGTH;
    unsigned adjustedMonth = (monthNum > 2) ? (monthNum - 3) : (monthNum + 9);
    unsigned dayInGroup = yearInGroup * NORMAL_YEAR_LENGTH + yearInGroup / 4 - yearInGroup / 100 
                          + (FIVE_MONTHS_DAYS * adjustedMonth + 2) / 5 + d - 1;
    
    out = epochGroup * FOUR_CENTURIES_CYCLE + dayInGroup - SHIFT_BETWEEN_1970_2000;
    return true;
}

void CDate::FromDays(unsigned days, unsigned& d, Month& m, unsigned& y)
{
    long daysFromZero = static_cast<long>(days) + SHIFT_BETWEEN_1970_2000;
    long epochGroup = (daysFromZero >= 0 ? daysFromZero : daysFromZero - FOUR_CENTURIES_CYCLE + 1) / FOUR_CENTURIES_CYCLE;
    unsigned dayInGroup = daysFromZero - epochGroup * FOUR_CENTURIES_CYCLE;
    unsigned yearInGroup = (dayInGroup - dayInGroup / FOUR_YEARS_CYCLE + dayInGroup / CENTURY_CYCLE 
                            - dayInGroup / (FOUR_CENTURIES_CYCLE - 1)) / NORMAL_YEAR_LENGTH;
    int year = static_cast<int>(yearInGroup) + epochGroup * CENTURY_LENGTH;
    unsigned dayInYear = dayInGroup - (NORMAL_YEAR_LENGTH * yearInGroup + yearInGroup / 4 - yearInGroup / 100);
    unsigned shiftedMonth = (5 * dayInYear + 2) / FIVE_MONTHS_DAYS;
    
    unsigned monthNum = shiftedMonth < 10 ? shiftedMonth + 3 : shiftedMonth - 9;
    m = static_cast<Month>(monthNum);
    y = static_cast<unsigned>(year + (monthNum <= 2 ? 1 : 0));
    d = dayInYear - (FIVE_MONTHS_DAYS * shiftedMonth + 2) / 5 + 1;
}

void CDate::AddDays(int delta)
{
    if (!m_isValid) return;
    
    int newDays = static_cast<int>(m_days) + delta;
    
    if (newDays < 0 || newDays > static_cast<int>(GetMaxDays()))
        m_isValid = false;
    else
        m_days = static_cast<unsigned>(newDays);
}

unsigned CDate::GetMaxDays()
{
    unsigned max;
    MakeTimestamp(31, Month::DECEMBER, MAX_YEAR, max);
    return max;
}

CDate& CDate::operator++()
{
    AddDays(1);
    return *this;
}

CDate CDate::operator++(int)
{
    CDate tmp = *this;
    ++(*this);
    return tmp;
}

CDate& CDate::operator--()
{
    AddDays(-1);
    return *this;
}

CDate CDate::operator--(int)
{
    CDate tmp = *this;
    --(*this);
    return tmp;
}

CDate& CDate::operator+=(int days)
{
    AddDays(days);
    return *this;
}

CDate& CDate::operator-=(int days)
{
    AddDays(-days);
    return *this;
}

CDate operator+(const CDate& d, int days)
{
    CDate res = d;
    res += days;
    return res;
}

CDate operator+(int days, const CDate& d)
{
    return d + days;
}

CDate operator-(const CDate& d, int days)
{
    CDate res = d;
    res -= days;
    return res;
}

int operator-(const CDate& a, const CDate& b)
{
    return static_cast<int>(a.m_days) - static_cast<int>(b.m_days);
}

std::ostream& operator<<(std::ostream& os, const CDate& d)
{
    if (!d.m_isValid)
    {
        os << "INVALID";
        return os;
    }
    
    os << std::setw(2) << std::setfill('0') << d.GetDay() << "."
       << std::setw(2) << static_cast<int>(d.GetMonth()) << "."
       << d.GetYear();
    
    return os;
}

std::istream& operator>>(std::istream& is, CDate& d)
{
    std::string str;
    is >> str;
    
    if (str == "INVALID")
    {
        d.m_isValid = false;
        return is;
    }
    
    unsigned day, monthNum, year;
    char dot1, dot2;
    
    std::stringstream ss(str);
    if (ss >> day >> dot1 >> monthNum >> dot2 >> year && 
        dot1 == '.' && dot2 == '.' && monthNum >= 1 && monthNum <= 12)
    {
        d.m_isValid = CDate::MakeTimestamp(day, static_cast<Month>(monthNum), year, d.m_days);
    }
    else
    {
        d.m_isValid = false;
    }
    
    return is;
}