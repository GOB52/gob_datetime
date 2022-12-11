#include "helper.hpp"
#include <gob_datetime.hpp>
#include <stack>

#include <cstdio>

using goblib::datetime::string_t;

namespace
{
void setTZ(const char* POSIX)
{
# ifdef _MSC_VER
    _putenv("TZ", POSIX);
# else
    setenv("TZ", POSIX, 1);
# endif
    tzset();
}

std::stack<string_t> _tzStack;
//
}

// Injection MockClock to LocalDateTime
void injectMockClockDate(const int y, const int m,  const int d)
{
    injectMockClockDateTime(y, m, d, 0, 0, 0);
}

void injectMockClockTime(const int hh, const int mm, const int ss)
{
    injectMockClockDateTime(2001, 1, 1, hh, mm, ss);
}

void injectMockClockDateTime(const int y, const int m, const int d, const int hh, const int mm, const int ss)
{
    struct tm tmp{};
    tmp.tm_year = y - 1900;
    tmp.tm_mon = m - 1;
    tmp.tm_mday = d;
    tmp.tm_hour = hh;
    tmp.tm_min = mm;
    tmp.tm_sec = ss;

    struct tm tmp2;
    tmp2 = tmp;
    tmp.tm_isdst = -1;

    time_t t = std::mktime(&tmp); // epoch of current timezone.
    if(tmp.tm_isdst <= 0)
    {
        tmp2.tm_isdst = 1;
        time_t tt  = std::mktime(&tmp2);
        if(tmp2.tm_isdst == 1) { t = tt; } // Exists valid DST
    }
    //    printf("%s:[%d]:%ld\n", __func__, tmp.tm_isdst, t);

    goblib::datetime::MockClock mc(t);
    goblib::datetime::injectMockClock(mc);
}

// Remove MockClock.
void resetMockClock()
{
    goblib::datetime::removeMockClock();
}

// Push the specified time-zone
void pushTimezone(const char* location)
{
    //        printf("%s:[%s]\n", __func__, goblib::datetime::locationToPOSIX(location));
    auto p = getenv("TZ");
    string_t ctz = p ? p : "";
    _tzStack.push(ctz);
    setTZ(goblib::datetime::locationToPOSIX(location));
}

// Pop time-zone
void popTimezone()
{
    if(_tzStack.empty()) { return; }
    auto tzs = _tzStack.top();
    _tzStack.pop();
    //        printf("%s:[%s]\n", __func__, tzs.c_str());
    setTZ(tzs.c_str());
}
