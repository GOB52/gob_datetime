/*!
  @file gob_datetime.cpp
  @brief date-time classes like Java JSR 310
*/
#include "gob_datetime.hpp"
#include <cstdio> // sscanf
#include <cmath> // abs, remainder

#ifndef NDEBUG
# define DT_LOG(format, ...) do { printf((format), ##__VA_ARGS__); putchar('\n');} while(0)
#else
# define DT_LOG(format, ...) /**/
#endif

namespace 
{
template <typename T> constexpr typename std::underlying_type<T>::type to_underlying(T e) noexcept
{
    return static_cast<typename std::underlying_type<T>::type>(e);
}

template <typename T> constexpr int sign(const T& v) noexcept { return (v > (T)0) - (v < (T)0); }

inline struct tm* toLocaltime(const time_t* t, struct tm* out)
{
#ifdef _MSC_VER
    localtime_s(out, t);
    return out;
#else
    return localtime_r(t, out);
#endif
}

inline struct tm* toGmtime(const time_t* t, struct tm* out)
{
#ifdef _MSC_VER
    gmtime_s(out, t);
    return out;

#else
    return gmtime_r(t, out);
#endif
}

goblib::datetime::string_t tm2str(const struct tm& tm, const char* fmt = nullptr)
{
    char buf[80];
    std::strftime(buf, sizeof(buf), fmt ? fmt : "%FT%T", &tm);
    buf[sizeof(buf)-1] = '\0';
    return goblib::datetime::string_t(buf);
}

time_t mkgmtime(const struct tm& tm)
{
    constexpr time_t month_day[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

    int month = tm.tm_mon % 12;
    int year = tm.tm_year + tm.tm_mon / 12;
    if (month < 0) {   // Negative values % 12 are still negative.
        month += 12;
        --year;
    }

    const int year_for_leap = (month > 1) ? year + 1 : year;

    time_t rt = tm.tm_sec                             // Seconds
            + 60 * (tm.tm_min                          // Minute = 60 seconds
            + 60 * (tm.tm_hour                         // Hour = 60 minutes
            + 24 * (month_day[month] + tm.tm_mday - 1  // Day = 24 hours
            + 365 * (year - 70)                         // Year = 365 days
            + (year_for_leap - 69) / 4                  // Every 4 years is     leap...
            - (year_for_leap - 1) / 100                 // Except centuries...
            + (year_for_leap + 299) / 400)));           // Except 400s.
    return rt < 0 ? -1 : rt;
}

#ifdef UNIT_TEST
goblib::datetime::MockClock _mockClock;
#endif

time_t getNow()
{
#if !defined(NDEBUG) && defined(UNIT_TEST)
    time_t t = _mockClock.now();
#else
    time_t t = std::time(&t);
#endif
    return t;
}
//
}

namespace goblib { namespace datetime {

#ifdef UNIT_TEST
// ----------------------------------------------------------------------
// class MockClock
time_t MockClock::now() const
{
    return (_epoch == -1) ? std::time(nullptr) : _epoch;
}

void injectMockClock(const MockClock& clock)
{
    _mockClock = clock;
}

void removeMockClock()
{
    _mockClock = MockClock();
}
#endif


// ----------------------------------------------------------------------
// class LocalDate
const LocalDate LocalDate::MIN(MIN_YEAR, MIN_MONTH, MIN_DAY);
#ifdef GOBLIB_DATETIME_USE_TIME_T_32BIT
const LocalDate LocalDate::MAX(MAX_YEAR, 1, 19); // 2038-01-19 ... The Year 2038 problem!
#else
const LocalDate LocalDate::MAX(MAX_YEAR, MAX_MONTH, 31);
#endif

// for GCC C++11,C++14
#if !defined(__clang__) && defined(__GNUG__) && __cplusplus < 201703L
constexpr int8_t  LocalDate::_dayOfWeekTable[12];
constexpr int8_t  LocalDate::_lengthOfMonthTable[2][12];
constexpr int16_t LocalDate::_daysOfMonthTable[2][12];
#endif

LocalDateTime LocalDate::atStartOfDay() const
{
    return atTime(0, 0, 0);
}

LocalDateTime LocalDate::atTime(const int hh, const int mm, const int ss) const
{
    return LocalDateTime::of(year(), month(), day(), hh, mm, ss);
}

LocalDateTime LocalDate::atTime(const LocalTime& lt) const 
{
    return LocalDateTime::of(*this, lt);
}

OffsetDateTime LocalDate::atTime(const OffsetTime& ot) const
{
    return atTime(ot.toLocalTime()).atOffset(ot.offset());
}

int32_t LocalDate::toEpochDay() const
{
    int32_t y = year();
    int32_t m = month();
    int32_t total = 0;
    total += 365 * y;
    if (y >= 0) { total += (y + 3) / 4 - (y + 99) / 100 + (y + 399) / 400; }
    else        { total -= y / -4 - y / -100 + y / -400; }

    total += ((367 * m - 362) / 12);
    total += day() - 1;
    if (m > 2)
    {
        total -= (1 + !isLeapYear());
        //        total--;
        //        if (!isLeapYear()) { total--; }
    }
    return total - 719528;
}

string_t LocalDate::toString(const char* fmt) const
{
    struct tm tmp{};
    tmp.tm_year = year2tm(year());
    tmp.tm_mon = month2tm(month());
    tmp.tm_mday = day();
    tmp.tm_wday = to_underlying(dayOfWeek());
    tmp.tm_yday = dayOfYear();
    return tm2str(tmp, fmt ? fmt : "%F");
}

#if 0
LocalDate LocalDate::ofEpochDay(int32_t eod)
{
    return LocalDate();
}

LocalDate LocalDate::ofYearDay(int16_t yy, int16_t doy)
{
    return LocalDate();    
}
#endif

/*! @warning There are limitations and impacts due to standard time functions. */
LocalDate LocalDate::now()
{
    return LocalDateTime::now().toLocalDate();
}

LocalDate LocalDate::parse(const char* s)
{
    struct tm tmp{};
    strptime(s, "%Y-%m-%d", &tmp);
    return LocalDate(tm2year(tmp.tm_year), tm2month(tmp.tm_mon), tmp.tm_mday);
    /*
    return (strptime(s, "%Y-%m-%d", &tmp) != nullptr) ?
            LocalDate(tm2year(tmp.tm_year), tm2month(tmp.tm_mon), tmp.tm_mday) : LocalDate();
    */
}


// ----------------------------------------------------------------------
// class LocalTime
const LocalTime LocalTime::MIN { MIN_HOUR, MIN_MINUTE, MIN_SECOND };
const LocalTime LocalTime::MAX { MAX_HOUR, MAX_MINUTE, 59 };

LocalDateTime LocalTime::atDate(const LocalDate& ld)
{
    return LocalDateTime::of(ld, *this);
}

string_t LocalTime::toString(const char* fmt) const
{
    struct tm tmp{};
    tmp.tm_hour = hour();
    tmp.tm_min = minute();
    tmp.tm_sec = second();
    return tm2str(tmp, fmt ? fmt : "%T");
}

/*! @warning There are limitations and impacts due to standard time functions. */
LocalTime LocalTime::now()
{
    return LocalDateTime::now().toLocalTime();
}

LocalTime LocalTime::ofSecondOfDay(const int32_t sod)
{
    int32_t v = sod;
    auto hh = v / SEC_PER_HOUR;
    v -= hh * SEC_PER_HOUR;
    auto mm = v / SEC_PER_MIN;
    v -= mm * SEC_PER_MIN;
    return LocalTime(hh, mm, v);
}

LocalTime LocalTime::parse(const char* s)
{
    struct tm tmp{};
    tmp.tm_hour = tmp.tm_min = tmp.tm_sec = -1;

    strptime(s, "%H:%M:%S", &tmp);
    return LocalTime(tmp.tm_hour, tmp.tm_min, tmp.tm_sec);
    /*
    return (strptime(s, "%H:%M:%S", &tmp) != nullptr) ? LocalTime(tmp.tm_hour, tmp.tm_min, tmp.tm_sec) : LocalTime();
    */
}

LocalTime operator+(const LocalTime& a, const int32_t& sec)
{
    auto sod = a.toSecondOfDay();
    int32_t sod2 = ((sec % LocalTime::SEC_PER_DAY) + sod + LocalTime::SEC_PER_DAY) % LocalTime::SEC_PER_DAY;
    return (sod != sod2) ? LocalTime::ofSecondOfDay(sod2) : LocalTime(a);
}


// ----------------------------------------------------------------------
// class ZoneOffset
const ZoneOffset ZoneOffset::UTC{0};
const ZoneOffset ZoneOffset::MIN{ MIN_SEC };
const ZoneOffset ZoneOffset::MAX{ MAX_SEC };
const ZoneOffset ZoneOffset::INVALID { 0xBADBEAF }; // 195935919  "+54426:38:39"

string_t ZoneOffset::toString() const
{
    if(_seconds == 0) { return string_t("Z"); }
    
    int sch = (sign(_seconds) >= 0) ? '+' : '-';
    int hh = std::abs(hour());
    int mm = std::abs(minute());
    int ss = std::abs(second());

    char buf[64];
    if(!ss) { snprintf(buf, sizeof(buf), "%c%02d:%02d", sch, hh, mm);  }
    else    { snprintf(buf, sizeof(buf), "%c%02d:%02d:%02d", sch, hh, mm, ss); }
    buf[sizeof(buf) - 1] = '\0';
    return string_t(buf);
}

ZoneOffset ZoneOffset::of(const char* s)
{
    if(!s || !s[0]) { return INVALID; }
    if(s[0] == 'Z' && !s[1]) { return UTC; }

    char sch = 0;
    int hh,mm,ss;
    hh = mm = ss = 0;
    if(sscanf(s, "%c%02d:%02d:%02d", &sch, &hh, &mm, &ss) != 4)
    {
        hh = mm = ss = 0;
        if(sscanf(s, "%c%02d:%02d", &sch, &hh, &mm) != 3)
        {
            return INVALID;
        }
    }
    if(sch != '+' && sch != '-') { return INVALID; }
    int sign = (sch == '-') ? -1 : 1;
    return of(sign * hh, sign * mm ,sign * ss);
}

ZoneOffset ZoneOffset::of(const int8_t hour, const int8_t minute, const int8_t second)
{
    bool valid = (sign(hour) == 1) ? (sign(minute) >= 0 && sign(second) >= 0) // +hour, +/z, +/z
            : (sign(hour) == -1)   ? (sign(minute) <= 0 && sign(second) <= 0) // -hour, -/z, -/z
            : (sign(minute) == 1)  ? sign(second) >= 0 // 0, +min, +/z
            : (sign(minute) == -1) ? sign(second) <= 0 // 0, -min, -/z
            : true; // 0,0, second

    return valid ? ofTotalSeconds(hour * SEC_PER_HOUR + minute * SEC_PER_MIN + second) : INVALID;
}


// ----------------------------------------------------------------------
// class OffsetTime
const OffsetTime OffsetTime::MIN = OffsetTime(LocalTime::MIN, ZoneOffset::MAX);
const OffsetTime OffsetTime::MAX = OffsetTime(LocalTime::MAX, ZoneOffset::MIN);

OffsetDateTime OffsetTime::atDate(const LocalDate& ld) const
{
    return OffsetDateTime::of(ld, _lt, _zoff);
}

OffsetTime OffsetTime::withOffsetSameEpoch(const ZoneOffset& zo)
{
    if(zo == _zoff) { return *this; }
    LocalTime tmp = _lt + (zo.totalSeconds() - _zoff.totalSeconds());
    return OffsetTime(tmp, zo);
}

/*! @warning There are limitations and impacts due to standard time functions. */
OffsetTime OffsetTime::now()
{
    time_t t = getNow();
    struct tm tml{};
    toLocaltime(&t, &tml);
    auto diff = std::difftime(mkgmtime(tml), std::mktime(&tml));
    //    DT_LOG("tml:%s t:%ld diff:%lf", tm2str(tml).c_str(), t, diff);
    return ofEpochSecond(t, ZoneOffset(diff));
}

OffsetTime OffsetTime::ofEpochSecond(const time_t& t, const ZoneOffset& zo)
{
    time_t sod = (t + zo.totalSeconds())  % SEC_PER_DAY;
    return OffsetTime(LocalTime::ofSecondOfDay(sod), zo);
}

OffsetTime OffsetTime::parse(const char* s)
{
    ZoneOffset zo;
    struct tm tmp{};
    tmp.tm_hour = tmp.tm_min = tmp.tm_sec = -1;

    auto p = strptime(s, "%H:%M:%S", &tmp);
    zo = ZoneOffset::of(p);
    //    DT_LOG("[%s]", p ? p : "NULL");
    return OffsetTime(LocalTime(tmp), zo);
}


// ----------------------------------------------------------------------
// class LocalDateTime
const LocalDateTime LocalDateTime::MIN = LocalDateTime(LocalDate::MIN, LocalTime::MIN);
#ifdef GOBLIB_DATETIME_USE_TIME_T_32BIT
const LocalDateTime LocalDateTime::MAX = LocalDateTime(LocalDate::MAX, LocalTime{3, 14, 7}); // 2038-01-19T03:14:07
#else
const LocalDateTime LocalDateTime::MAX = LocalDateTime(LocalDate::MAX, LocalTime::MAX);
#endif
OffsetDateTime LocalDateTime::atOffset(const ZoneOffset& zo) const
{
    return OffsetDateTime(*this, zo);
}

time_t LocalDateTime::toEpochSecond(const ZoneOffset& zo) const
{
    return _date.toEpochDay() * 86400LL + _time.toSecondOfDay() - zo.totalSeconds();
}

string_t LocalDateTime::toString(const char* fmt) const
{
    struct tm tmp = toTm();
    return tm2str(tmp, fmt);
}

struct tm LocalDateTime::toTm() const
{
    struct tm tmp{};
    tmp.tm_year = year2tm(year());
    tmp.tm_mon = month2tm(month());
    tmp.tm_mday = day();
    tmp.tm_wday = to_underlying(_date.dayOfWeek());
    tmp.tm_yday = _date.dayOfYear();
    tmp.tm_hour = hour();
    tmp.tm_min = minute();
    tmp.tm_sec = second();
    return tmp;
}
                   
/*! @warning There are limitations and impacts due to standard time functions. */
LocalDateTime LocalDateTime::now()
{
    time_t t = getNow();
    struct tm tmp{};
    toLocaltime(&t, &tmp);
    return LocalDateTime(tmp);
}

LocalDateTime LocalDateTime::ofEpochSecond(const time_t& epoch, const ZoneOffset& zo)
{
    struct tm tmp{};
    time_t t = epoch + zo.totalSeconds();
    return LocalDateTime(*toGmtime(&t, &tmp));
}

LocalDateTime LocalDateTime::parse(const char* s)
{
    struct tm tmp{};
    tmp.tm_hour = tmp.tm_min = tmp.tm_sec = -1;

    strptime(s, "%Y-%m-%dT%H:%M:%S", &tmp);
    return LocalDateTime(tmp);
    //    return (strptime(s, "%Y-%m-%dT%H:%M:%S", &tmp) != nullptr) ? LocalDateTime(tmp) : LocalDateTime();
}

// ----------------------------------------------------------------------
// class OffsetDateTime
const OffsetDateTime OffsetDateTime::MIN = OffsetDateTime(LocalDateTime::MIN, ZoneOffset::MAX);
#ifdef GOBLIB_DATETIME_USE_TIME_T_32BIT
const OffsetDateTime OffsetDateTime::MAX = OffsetDateTime(LocalDateTime::MAX, ZoneOffset::UTC); // 2038-01-19T03:14:07Z
#else
const OffsetDateTime OffsetDateTime::MAX = OffsetDateTime(LocalDateTime::MAX, ZoneOffset::MIN);
#endif

OffsetDateTime OffsetDateTime::withOffsetSameEpoch(const ZoneOffset& zo) const
{
    if(_zoff == zo) { return *this; }
#if 0
    auto diff = zo.totalSeconds() - _zoff.totalSeconds();
    auto epoch = _datetime.toEpochSecond() + diff;
    return OffsetDateTime(LocalDateTime::ofEpochSecond(epoch), zo);
#endif
    return OffsetDateTime(LocalDateTime::ofEpochSecond(_datetime.toEpochSecond(_zoff), zo), zo);
}

/*! @warning There are limitations and impacts due to standard time functions. */
OffsetDateTime OffsetDateTime::now()
{
    time_t t = getNow();
    struct tm tml{};
    toLocaltime(&t, &tml);
    auto diff = std::difftime(mkgmtime(tml), std::mktime(&tml));
    //    DT_LOG("tml:%s t:%ld diff:%lf", tm2str(tml).c_str(), t, diff);
    return OffsetDateTime(LocalDateTime(tml), ZoneOffset(diff));
}

OffsetDateTime OffsetDateTime::parse(const char* s)
{
    ZoneOffset zo;
    struct tm tmp{};
    tmp.tm_hour = tmp.tm_min = tmp.tm_sec = -1;
    auto p = strptime(s, "%Y-%m-%dT%H:%M:%S", &tmp);
    zo = ZoneOffset::of(p);
    return OffsetDateTime(LocalDateTime(tmp), zo);
}
//
}}
