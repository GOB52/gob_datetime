/* Helper functions for unit test*/
#ifndef HELPER_HPP
#define HELPER_HPP

#include <ctime>

// Injection MockClock to LocalDateTime
void injectMockClockDate(const int y, const int m = 1,  const int d = 1);
void injectMockClockTime(const int hh, const int mm = 0, const int ss = 0);
void injectMockClockDateTime(const int y, const int m, const int d, const int hh = 0, const int mm = 0, const int ss = 0);

// Remove MockClock.
void resetMockClock();

// Push the specified time-zone
void pushTimezone(const char* location);
// Pop time-zone
void popTimezone();

// Commonization of localtime_r/_s
inline struct tm* toLocaltime(const time_t* t, struct tm* out)
{
#ifdef _MSC_VER
    localtime_s(out, t);
    return out;
#else
    return localtime_r(t, out);
#endif
}

// Commonization of gmtime_r/_s
inline struct tm* toGmtime(const time_t* t, struct tm* out)
{
#ifdef _MSC_VER
    gmtime_s(out, t);
    return out;
#else
    return gmtime_r(t, out);
#endif
}

#endif
