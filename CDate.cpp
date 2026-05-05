#include "CDate.h"
#include <iomanip>
#include <sstream>

constexpr unsigned WEEK_LENGTH = 7;
constexpr unsigned MIN_YEAR = 1970;
constexpr unsigned MAX_YEAR = 9999;
constexpr unsigned SHIFT_FOR_COUNT_WEEKDAY = 4;
constexpr unsigned DAYS_IN_LEAP_YEAR = 366;
constexpr unsigned DAYS_IN_NOT_LEAP_YEAR = 365;
constexpr unsigned DAYS_IN_4_YEARS = 365 * 4 + 1;  
constexpr unsigned DAYS_IN_100_YEARS = 365 * 100 + 24; 
constexpr unsigned DAYS_IN_400_YEARS = 365 * 400 + 97; 
constexpr unsigned SHIFT_TO_1970 = 719468;
constexpr unsigned DAYS_IN_FIRST_FIVE_MONTHS = 153;
constexpr unsigned MONTHS_SHIFT = 3;
constexpr unsigned MONTHS_OFFSET = 9;

static constexpr unsigned monthLengths[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static constexpr unsigned monthSums[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

CDate::CDate() : m_days(0) {}

CDate::CDate(unsigned day, Month month, unsigned year)
    : m_days(MakeTimestamp(day, month, year))
{
}

CDate::CDate(unsigned timestamp)
{
    unsigned maxDays = GetMaxDays();
    if (timestamp > maxDays)
        throw std::out_of_range("Timestamp out of valid date range");
    m_days = timestamp;
}

unsigned CDate::GetDay() const
{
    unsigned d, y;
    Month m;
    MakeDateFormat(m_days, d, m, y);
    return d;
}

Month CDate::GetMonth() const
{
    unsigned d, y;
    Month m;
    MakeDateFormat(m_days, d, m, y);
    return m;
}

unsigned CDate::GetYear() const
{
    unsigned d, y;
    Month m;
    MakeDateFormat(m_days, d, m, y);
    return y;
}

WeekDay CDate::GetWeekDay() const
{
    return WeekDay((m_days + SHIFT_FOR_COUNT_WEEKDAY) % WEEK_LENGTH);
}

bool CDate::IsLeapYear(unsigned year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

unsigned CDate::CountDaysInMonth(Month m, unsigned year)
{
    unsigned monthNum = static_cast<unsigned>(m);
    if (monthNum == 2 && IsLeapYear(year)) return 29;
    return monthLengths[monthNum - 1];
}

unsigned CDate::MakeTimestamp(unsigned d, Month m, unsigned y)
{
    if (y < MIN_YEAR || y > MAX_YEAR)
        throw std::invalid_argument("Invalid date: year is out of range");
    
    unsigned monthNum = static_cast<unsigned>(m);
    if (monthNum < 1 || monthNum > 12)
        throw std::invalid_argument("Invalid date: month is out of range");
    
    if (d < 1 || d > CountDaysInMonth(m, y))
        throw std::invalid_argument("Invalid date: day is out of range");
    
    int shiftedYear = y - ( monthNum <= 2 ? 1 : 0);
    int shiftedMonth = (monthNum > 2) ? (monthNum - MONTHS_SHIFT) : (monthNum + MONTHS_OFFSET);

    int daysFromZeroYear = shiftedYear * DAYS_IN_NOT_LEAP_YEAR + shiftedYear / 4 
                              - shiftedYear / 100  + shiftedYear / 400
                              + (DAYS_IN_FIRST_FIVE_MONTHS * shiftedMonth + 2) / 5 
                              + d - 1;
    
    return daysFromZeroYear - SHIFT_TO_1970;
}

void CDate::MakeDateFormat(unsigned days, unsigned& d, Month& m, unsigned& y)
{
    int daysFromZeroYear = static_cast<int>(days) + SHIFT_TO_1970;

    int fourCenturies = daysFromZeroYear / DAYS_IN_400_YEARS;
    int remainingDays = daysFromZeroYear % DAYS_IN_400_YEARS;

    int centuries = (remainingDays * 4 + 3) / DAYS_IN_400_YEARS;
    remainingDays -= centuries * DAYS_IN_100_YEARS;
    
    int fourYears = remainingDays / DAYS_IN_4_YEARS;
    remainingDays -= fourYears * DAYS_IN_4_YEARS;
    
    int yearInCycle = remainingDays / DAYS_IN_NOT_LEAP_YEAR;
    if (yearInCycle == 4) yearInCycle = 3;  
    remainingDays -= yearInCycle * DAYS_IN_NOT_LEAP_YEAR;
    
    int shiftedYear = fourCenturies * 400 + centuries * 100 + fourYears * 4 + yearInCycle;
    
    int monthIndex = (5 * remainingDays + 2) / DAYS_IN_FIRST_FIVE_MONTHS;  
    int monthNum = monthIndex < 10 ? monthIndex + MONTHS_SHIFT : monthIndex - MONTHS_OFFSET;
    int dayOfYear = remainingDays - (DAYS_IN_FIRST_FIVE_MONTHS * monthIndex + 2) / 5 + 1;
    
    y = static_cast<unsigned>(shiftedYear + (monthNum <= 2 ? 1 : 0));
    m = static_cast<Month>(monthNum);
    d = static_cast<unsigned>(dayOfYear);
}

void CDate::AddDays(int delta)
{
    int newDays = static_cast<int>(m_days) + delta;
    unsigned maxDays = GetMaxDays();
    
    if (newDays < 0 || newDays > static_cast<int>(maxDays))
        throw std::out_of_range("Resulting date is out of valid range");
    
    m_days = static_cast<unsigned>(newDays);
}

unsigned CDate::GetMaxDays()
{
    return MakeTimestamp(31, Month::DECEMBER, MAX_YEAR);

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
    return static_cast<int>(a.GetTimestamp()) - static_cast<int>(b.GetTimestamp());
}

std::ostream& operator<<(std::ostream& os, const CDate& d)
{
    os << std::setw(2) << std::setfill('0') << d.GetDay() << "."
       << std::setw(2) << static_cast<int>(d.GetMonth()) << "."
       << d.GetYear();
    return os;
}

std::istream& operator>>(std::istream& is, CDate& d)
{
    std::string str;
    is >> str;
    
    unsigned day, monthNum, year;
    char dot1, dot2;
    
    std::stringstream ss(str);
    if (ss >> day >> dot1 >> monthNum >> dot2 >> year &&  dot1 == '.' && dot2 == '.' ) 
    {
        CDate temp(day, static_cast<Month>(monthNum), year);
        d = temp;
    }
    else
    {
        throw std::invalid_argument("Invalid date format");
    }
    
    return is;
}