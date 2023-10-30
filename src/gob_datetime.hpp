/*!
  @file gob_datetime.hpp
  @brief date-time classes like Java JSR-310

  @mainpage gob_datetime
  date-time classes like Java JSR-310 for C++11 or later.

  @sa https://docs.oracle.com/javase/jp/18/docs/api/java.base/java/time/package-summary.html

  @warning Not all methods are available in all java.time classes
  @warning Note that there are restrictions imposed by the time_t and time functions.
  
  @author GOB https://twitter.com/gob_52_gob

  @copyright 2022 GOB
  @copyright Licensed under the MIT license. See LICENSE file in the project root for full license information.

  @todo Add ZonedDateTime and TimeZone classes.
*/
#ifndef GOBLIB_DATETIME_HPP
#define GOBLIB_DATETIME_HPP

#include <ctime>
#if __cplusplus >= 202002L
# include <compare>  // ordering C++20
#else
# include <utility> // std::rel_ops C++11,14,17
#endif

#ifdef ARDUINO
# include <WString.h> // String
# ifdef _min
#   undef _min
# endif
# ifdef _max
#   undef _max
# endif
/*
  ESP-IDF
  defined _USE_LONG_TIME_T if time_t is 32bit.
  see also https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system_time.html#bit-time-t
 */
# include <sys/types.h>
#else
# include <string>
#endif

/*!
  @def GOBLIB_DATETIME_USE_TIME_T_GREATER_THAN_32BIT
  @brief Size of time_t is 32bit or greater?
  @note Define GOBLIB_DATETIME_USE_TIME_T_32BIT if time_t is 32bit.
  @note Define GOBLIB_DATETIME_USE_TIME_T_GREATER_THAN_32BIT if time_t is greater than 32bit.
  @sa https://en.wikipedia.org/wiki/Year_2038_problem
 */
#if (_TIME_BITS > 32) || !defined(_USE_LONG_TIME_T)
# define GOBLIB_DATETIME_USE_TIME_T_GREATER_THAN_32BIT
#else
# define GOBLIB_DATETIME_USE_TIME_T_32BIT
#endif

/*!
  @namespace goblib
  @brief Top level namespace of mine.
*/
namespace goblib {
/*!
  @@namespace datetime
  @brief For date-time
*/
namespace datetime {

/*!
  @typedef string_t
  @brief Type of string instance.
  @note String if defined(ARDUINO)
  @note std::string if not defined(ARDUINO)
 */
#ifdef ARDUINO
using string_t = String;
#else
using string_t = std::string;
#endif

/*!
  @typedef timediff_t
  @brief Type of the result of subtracting two time_t. 
  @note On POSIX systems, std::time_t is measured in seconds, and difftime is equivalent to arithmetic subtraction, but C and C++ allow fractional units for time_t. 
*/
using timediff_t = double; // Same as retuen value type of std::difftime.


/*!
  @brief Convert location string to POSIX TZ string.
  @param location Time-zone location string,  such as "Asia/Tokyo", "America/Los_Angeles", "Australia/Lord_Howe"
  @retval != nullptr POSIX TZ string, such as "JST-9", "PST8PDT,M3.2.0,M11.1.0", "<+1030>-10:30<+11>-11,M10.1.0,M4.1.0"
  @retval == nullptr There is no POSIX string corresponding to location string.
*/
const char* locationToPOSIX(const char* location);

///@name Interconversion of struct tm elements.(year, month)
///@{
constexpr int year2tm (const int year)  { return year - 1900; } //!< @brief from year to tm_year.
constexpr int month2tm(const int month) { return month - 1;   } //!< @brief from month to tm_mon.
constexpr int tm2year (const int year)  { return year + 1900; } //!< @brief from tm_year to year
constexpr int tm2month(const int month) { return month + 1;   } //!< @brief from tm_mon to month
///@}

class LocalTime;
class OffsetTime;
class LocalDateTime;
class OffsetDateTime;


/*!
  @enum Day of week
  @brief A day-of-week, such as Sun..
*/
enum class DayOfWeek : uint8_t
{
    Sun, Mon, Tue, Wed, Thu, Fri, Sat
};


/*!
  @class LocalDate
  @brief A date without a time-zone in the ISO-8601 calendar system, such as 2009-08-07.
 */
class LocalDate
{
  public:
    ///@name Constructors
    ///@{
    constexpr LocalDate() {}
    constexpr LocalDate(const int16_t y, const int8_t m, const int8_t d) : _year(y), _month(m), _day(d) {}
    constexpr explicit LocalDate(const struct tm& tm) : LocalDate(tm2year(tm.tm_year), tm2month(tm.tm_mon), tm.tm_mday) {}
    explicit  LocalDate(const char* dateString, const char* formatString = nullptr) : LocalDate() { *this = parse(dateString, formatString); }
    ///@}
    
    ///@name Properties
    ///@{
    constexpr int16_t   year()      const     { return _year;  } //!< @brief Gets the year.
    constexpr int8_t    month()     const     { return _month; } //!< @brief Gets the month.
    constexpr int8_t    day()       const     { return _day;   } //!< @brief Gets the day of month.
    constexpr DayOfWeek dayOfWeek() const
    {
        return static_cast<DayOfWeek>( ((year() - (month() < 3)) + (year() - (month() < 3)) / 4 - (year() - (month() < 3)) / 100 + (year() - (month() < 3)) / 400 + _dayOfWeekTable[month() - 1] + day()) % 7 );
    } //!< @brief Gets the day of week, which is an enum DayOfWeek.
    constexpr int16_t   dayOfYear() const     { return _daysOfMonthTable[ isLeapYear() ][ month() - 1] + day() - 1; } //!< @brief Gets the day of year. (same as struct tm.tm_yday)
    constexpr bool      isLeapYear() const    { return (year() % 4 == 0) && ((year() % 100) != 0 || (year() % 400) == 0); } //!< @brief Checks if the year is a leap year, according to the ISO proleptic calendar system rules.
    constexpr int8_t    lengthOfMonth() const {  return _lengthOfMonthTable[ isLeapYear() ][month() - 1]; } //!< @brief Returns the length of the month represented by this date.
    constexpr int16_t   lengthOfYear() const  { return isLeapYear() ? 366 : 365; } //!<  @brief Returns the length of the year represented by this date.
    constexpr uint32_t  hash() const          { return ((uint32_t)_year << 11) + ((uint32_t)_month << 6) + (uint32_t)_day; } //!< @brief A hash code for this. */
    ///@}

    /*! @brief Is valid instance? */
    constexpr bool valid() const
    {
        return (_year < MIN_YEAR || _year > MAX_YEAR || _month < MIN_MONTH || _month > MAX_MONTH || _day < MIN_DAY || _day > lengthOfMonth() ) ? false :
                (_year != MAX_YEAR) ? true :
                (_month <= MONTH_OF_MAX_DATE && _day <= DAY_OF_MAX_DATE);
    }
    /*! @brief Combines this date with the time of midnight to create a LocalDateTime at the start of this date. */
    LocalDateTime atStartOfDay() const;
    /*! @brief Combines this date with a time to create a LocalDateTime. */
    LocalDateTime atTime(const int hh, const int mm, const int ss) const;
    /*! @brief Combines this date with a time to create a LocalDateTime. */
    LocalDateTime atTime(const LocalTime& lt) const;
    /*! @brief Combines this date-time with an offset to create an OffsetDateTime. */
    OffsetDateTime atTime(const OffsetTime& offset) const;
    /*! @brief Converts this date to the Epoch Day. */
    int32_t toEpochDay() const;
    /*!
      @brief Outputs this date as a String, such as 2009-08-07
      @param fmt Format specifier similar to std::strftime.
     */
    string_t toString(const char* fmt = nullptr) const;

    /*! @brief Obtains the current date from the system clock in the default time-zone.*/
    static LocalDate now();
    /*! @brief Obtains an instance of LocalDate from a year, month and day. */
    static constexpr LocalDate of(const int16_t y, const int8_t m = 1, const int8_t d = 1) { return LocalDate(y, m, d); }
#if 0
    /*! @brief Obtains an instance of LocalDate from the epoch day count. */
    static LocalDate ofEpochDay(const int32_t eod);
    /*! @brief Obtains an instance of LocalDate from a year and day-of-year. */
    static LocalDate ofYearDay(const int16_t yy, int16_t doy);
#endif
    /*! @brief Obtains an instance of LocalDate from a text string such as 2009-08-07.
      @param s string for parse
      @param fmt format string for parse if exists. (default as DEFAULT_PARSE_FORMAT)
     */
    static LocalDate parse(const char* dateString, const char* formatString = nullptr);

#if __cplusplus < 202002L
    friend inline bool operator==(const LocalDate& a, const LocalDate& b) { return a.toEpochDay() == b.toEpochDay(); }
    friend inline bool operator< (const LocalDate& a, const LocalDate& b) { return a.toEpochDay() <  b.toEpochDay(); }
    friend inline bool operator!=(const LocalDate& a, const LocalDate& b) { return std::rel_ops::operator!=(a,b); }
    friend inline bool operator> (const LocalDate& a, const LocalDate& b) { return std::rel_ops::operator> (a,b); }
    friend inline bool operator<=(const LocalDate& a, const LocalDate& b) { return std::rel_ops::operator<=(a,b); }
    friend inline bool operator>=(const LocalDate& a, const LocalDate& b) { return std::rel_ops::operator>=(a,b); }
#else
    auto operator <=>(const LocalDate&) const = default;
#endif
   
  public:
    static const LocalDate MIN; //!< @brief The minimum supported date.
    static const LocalDate MAX; //!< @brief The maximum supported date.

    static constexpr int8_t  MIN_MONTH = 1;
    static constexpr int8_t  MAX_MONTH = 12;;
    static constexpr int8_t  MIN_DAY = 1; // Max is referenced from _lengthOfMonthTable.
    static constexpr int16_t MIN_YEAR = 1970;
#if defined(GOBLIB_DATETIME_USE_TIME_T_32BIT)
    // 2038-01-19 ... The Year 2038 problem!
    static constexpr int16_t MAX_YEAR = 2038;
    static constexpr int8_t MONTH_OF_MAX_DATE = 1;
    static constexpr int8_t DAY_OF_MAX_DATE = 19;
#else
    static constexpr int16_t MAX_YEAR = 32767;
    static constexpr int8_t MONTH_OF_MAX_DATE{12};
    static constexpr int8_t DAY_OF_MAX_DATE{31};
#endif

  private:
    int16_t _year  { MIN_YEAR };
    int8_t  _month { MIN_MONTH };
    int8_t  _day   { MIN_DAY };

    static constexpr int8_t  _dayOfWeekTable[]{ 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    static constexpr int8_t  _lengthOfMonthTable[2][12]
    {
        { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, // standard
        { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, // leap year
    };
    static constexpr int16_t _daysOfMonthTable[2][12]
    {
        { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 }, // standard
        { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 }, // leap year
    };

    static const char DEFAULT_PARSE_FORMAT[];
};


/*!
  @class LocalTime
  @brief A time without a time-zone in the ISO-8601 calendar system, such as 12:34:56.
  @warning Not support ms/us/ns.
 */
class LocalTime
{
  public:
    ///@name Constructors
    ///@{
    constexpr LocalTime() : _pad(0) {}
    constexpr LocalTime(const int8_t hour, const int8_t minute, const int8_t second)
            : _hour(hour), _minute(minute), _second(second) {}
    constexpr explicit LocalTime(const struct tm& tm) : LocalTime(tm.tm_hour, tm.tm_min, tm.tm_sec) {}
    explicit  LocalTime(const char* s) : LocalTime() { *this = parse(s); }
    ///@}
    
    ///@name Properties
    ///@{
    constexpr int8_t   hour()   const { return _hour;   } //!< @brief Gets the hour.
    constexpr int8_t   hour12() const { return (_hour % 12) ? (_hour % 12) : 12; } //!< @brief Gets the 12 hour clock [1-12]
    constexpr int8_t   minute() const { return _minute; } //!< @brief Gets the minute.
    constexpr int8_t   second() const { return _second; } //!< @brief Gets the second.
    constexpr bool     isAM()   const { return _hour < 12; } //!< @brief Is ante meridian?
    constexpr bool     isPM()   const { return !isAM();    } //!< @brief Is post meridian?
    constexpr uint32_t hash() const { return ((uint32_t)_hour << 12) + ((uint32_t)_minute << 6) + (uint32_t)_second; } //!< @brief A hash code for this.
    ///@}

    /*! @brief Is valid instance? */
    constexpr bool valid() const { return _hour >= MIN_HOUR && _hour <= MAX_HOUR && _minute >= MIN_MINUTE && _minute <= MAX_MINUTE && _second >= MIN_SECOND && _second <= MAX_SECOND; }
    /*! @brief Combines this time with a date to create a LocalDateTime. */
    LocalDateTime atDate(const LocalDate& ld);
    /*! @brief Extracts the time as seconds of day, from 0 to 24 * 60 * 60 - 1. */
    constexpr int32_t toSecondOfDay() const { return SEC_PER_HOUR * _hour + SEC_PER_MIN * _minute + _second; }
    /*!
      @brief Outputs this time as a String, such as 12:03.
      @param fmt Format specifier similar to std::strftime.
    */
    string_t toString(const char* fmt = nullptr) const;

    /*! @brief Obtains the current time from the system clock in the default time-zone. */
    static LocalTime now();
    /*! @brief Obtains an instance of LocalTime from an hour, minute and second. */
    static constexpr LocalTime of(const int8_t hour, const int8_t minute = 0, const int8_t second = 0) { return LocalTime(hour, minute, second); }
    /*! @brief Obtains an instance of LocalTime from a second-of-day value. */
    static LocalTime ofSecondOfDay(const int32_t secondOfDay);
    /*! @brief Obtains an instance of LocalTime from a text string such as 12:34:56 */
    static LocalTime parse(const char* s);

#if __cplusplus < 202002L
    friend inline bool operator==(const LocalTime& a, const LocalTime& b) { return a.toSecondOfDay() == b.toSecondOfDay(); }
    friend inline bool operator< (const LocalTime& a, const LocalTime& b) { return a.toSecondOfDay() <  b.toSecondOfDay(); }
    friend inline bool operator!=(const LocalTime& a, const LocalTime& b) { return std::rel_ops::operator!=(a,b); }
    friend inline bool operator> (const LocalTime& a, const LocalTime& b) { return std::rel_ops::operator> (a,b); }
    friend inline bool operator<=(const LocalTime& a, const LocalTime& b) { return std::rel_ops::operator<=(a,b); }
    friend inline bool operator>=(const LocalTime& a, const LocalTime& b) { return std::rel_ops::operator>=(a,b); }
#else
    auto operator <=>(const LocalTime&) const = default;
#endif

    //! @warning Recommended not to use as it is for internal use.
    friend LocalTime operator+(const LocalTime& a, const int32_t& sec);
    
  public:
    static const LocalTime MIN; //!< @brief The minimum supported time.
    static const LocalTime MAX; //!< @brief The maximum supported date.

  private:
    int8_t _hour   { MIN_HOUR };
    int8_t _minute { MIN_MINUTE };
    int8_t _second { MIN_SECOND };
    int8_t _pad{0}; // padding

    static constexpr int8_t  MIN_HOUR = 0;
    static constexpr int8_t  MAX_HOUR = 23;
    static constexpr int8_t  MIN_MINUTE = 0;
    static constexpr int8_t  MAX_MINUTE = 59;
    static constexpr int8_t  MIN_SECOND = 0;
    static constexpr int8_t  MAX_SECOND = 60; // sama as struct tm.tm_sec
    static constexpr int32_t SEC_PER_MIN = 60;
    static constexpr int32_t SEC_PER_HOUR = 60 * SEC_PER_MIN;
    static constexpr int32_t SEC_PER_DAY = 24 * SEC_PER_HOUR;
};


/*!
  @class ZoneOffset
  @brief A time-zone offset from Greenwich/UTC, such as +09:00.
*/
class ZoneOffset
{
  public:
    ///@name Constructors
    ///@{
    constexpr ZoneOffset() {}
    constexpr explicit ZoneOffset(const int32_t sec) : _seconds(sec) {}
    ///@}

    ///@name Properties
    ///@{
    constexpr float   totalHours()   const { return _seconds / (float)SEC_PER_HOUR; } //!< @brief Gets the total hours.
    constexpr float   totalMinutes() const { return _seconds / (float)SEC_PER_MIN;  } //!< @brief Gets the total minutes.
    constexpr int32_t totalSeconds() const { return _seconds; } //!< @brief Gets the total seconds.
    constexpr int32_t hour()         const { return _seconds / SEC_PER_HOUR; } //!< @brief Gets the hour part.
    constexpr int32_t minute()       const { return (_seconds / SEC_PER_MIN) % SEC_PER_MIN; } //!< @brief Gets the minute part.
    constexpr int32_t second()       const { return _seconds % SEC_PER_MIN; } //!< @brief Gets the second part.
    constexpr uint32_t hash()        const { return _seconds; } //!< @brief A hash code for this.
    ///@}

    /*! @brief Is valid instance? */
    constexpr bool valid() const { return _seconds >= MIN_SEC && _seconds <= MAX_SEC; }
    /*!  @brief Outputs normalized offset as a String, such as +09:00 */
    string_t toString() const;

    /*! @brief Obtains an instance of ZoneOffset from a text string such as +09:00 or Z (means UTC). */
    static ZoneOffset of(const char* s);
    /*!
      @brief Obtains an instance of ZoneOffset using an offset in hours, minutes and seconds.
      @warning The sign of the hours, minutes and seconds components must match.
     */
    static ZoneOffset of(const int8_t hour, const int8_t minute = 0, const int8_t second = 0);
    /*! @brief Obtains an instance of ZoneOffset specifying the total offset in seconds.  */
    static constexpr ZoneOffset ofTotalSeconds(const int32_t sec) { return ZoneOffset(sec); }

#if __cplusplus < 202002L    
    friend inline bool operator==(const ZoneOffset& a, const ZoneOffset& b) { return a._seconds == b._seconds; }
    friend inline bool operator< (const ZoneOffset& a, const ZoneOffset& b) { return a._seconds <  b._seconds; }
    friend inline bool operator!=(const ZoneOffset& a, const ZoneOffset& b) { return std::rel_ops::operator!=(a,b); }
    friend inline bool operator> (const ZoneOffset& a, const ZoneOffset& b) { return std::rel_ops::operator> (a,b); }
    friend inline bool operator<=(const ZoneOffset& a, const ZoneOffset& b) { return std::rel_ops::operator<=(a,b); }
    friend inline bool operator>=(const ZoneOffset& a, const ZoneOffset& b) { return std::rel_ops::operator>=(a,b); }
#else
    auto operator <=>(const ZoneOffset&) const = default;
#endif    

  public:
    static const ZoneOffset UTC; //!< @brief The time-zone offset for UTC.
    static const ZoneOffset MIN; //!< @brief The minimum supported offset;
    static const ZoneOffset MAX; //!< @brief The maximum supported offset;
    
  private:
    int32_t _seconds{0};
    static constexpr int32_t SEC_PER_MIN = 60;
    static constexpr int32_t SEC_PER_HOUR = 60 * SEC_PER_MIN;
    static constexpr int32_t MIN_SEC = -18 * SEC_PER_HOUR;
    static constexpr int32_t MAX_SEC = +18 * SEC_PER_HOUR;
    static const ZoneOffset INVALID;
};


/*!
  @class OffsetTime
  @brief A time with an offset from UTC/Greenwich in the ISO-8601 calendar system, such as 12:34:56+0700
 */
class OffsetTime
{
  public:
    ///@name Constructors
    ///@{
    constexpr OffsetTime() {}
    constexpr OffsetTime(const LocalTime& lt, const ZoneOffset& zoff) : _lt(lt), _zoff(zoff) {}
    ///@}

    ///@name Properties
    ///@{
    constexpr int8_t     hour()   const { return _lt.hour();   } //!< @brief Gets the hour.
    constexpr int8_t     hour12() const { return _lt.hour12(); } //!< @brief Gets the 12 hour clock [1-12]
    constexpr int8_t     minute() const { return _lt.minute(); } //!< @brief Gets the minute.
    constexpr int8_t     second() const { return _lt.second(); } //!< @brief Gets the second.
    constexpr bool       isAM()   const { return _lt.isAM();   } //!< @brief Is ante meridian?
    constexpr bool       isPM()   const { return _lt.isPM();   } //!< @brief Is post meridian?
    constexpr ZoneOffset offset() const { return _zoff;        } //!< @brief Gets the ZoneOffset.
    constexpr uint32_t   hash()   const { return _lt.hash() ^ _zoff.hash(); } //!< @brief A hash code for this.
    ///@}

    /*! @brief Is valid instance? */
    constexpr bool valid() const { return _lt.valid() && _zoff.valid(); }
    /*! @brief Combines this time with a date to create an OffsetDateTime. */
    OffsetDateTime atDate(const LocalDate& ld) const;
    /*! @brief Gets the LocalTime part of this date-time. */
    constexpr LocalTime toLocalTime() const { return _lt; }
    /*! @brief Outputs this time as a String, such as 12:34:56+07:00. */
    string_t toString() const { return _lt.toString() += _zoff.toString(); }
    /*! @brief Returns a copy of this OffsetTime with the specified offset ensuring that the result is at the same instant on an implied day.*/
    OffsetTime withOffsetSameEpoch(const ZoneOffset& zo);
    /*! @brief Returns a copy of this OffsetTime with the specified offset ensuring that the result has the same local time. */
    OffsetTime withOffsetSameLocal(const ZoneOffset& zo) { return (zo != _zoff) ? OffsetTime(_lt, zo) : *this; }

    /*! @brief Obtains the current time from the system clock in the default time-zone. */
    static OffsetTime now();
    /*! @brief Obtains an instance of OffsetTime from an hour, minute, and second. */
    static constexpr OffsetTime of(const int8_t hour, const int8_t minute, const int8_t second, const ZoneOffset& zoff) { return OffsetTime(LocalTime(hour, minute, second), zoff); }
    /*! @brief Obtains an instance of OffsetTime from a local time and an offset. */
    static constexpr OffsetTime of(const LocalTime& lt, const ZoneOffset& zoff) { return OffsetTime(lt, zoff); }

    static OffsetTime ofEpochSecond(const time_t& epoch, const ZoneOffset& zo);

    /*! @brief Obtains an instance of OffsetTime from a text string such as 12:34:56+07:00. */
    static OffsetTime parse(const char* s);

    // Compare using by total seconds because 00:00:00Z and 09:00:00+09:00 are same epoch.

    friend inline bool operator==(const OffsetTime& a, const OffsetTime& b) { return a.toEpochSecond() == b.toEpochSecond(); }
#if __cplusplus < 202002L
    friend inline bool operator< (const OffsetTime& a, const OffsetTime& b) { return a.toEpochSecond() <  b.toEpochSecond(); }
    friend inline bool operator!=(const OffsetTime& a, const OffsetTime& b) { return std::rel_ops::operator!=(a,b); }
    friend inline bool operator> (const OffsetTime& a, const OffsetTime& b) { return std::rel_ops::operator> (a,b); }
    friend inline bool operator<=(const OffsetTime& a, const OffsetTime& b) { return std::rel_ops::operator<=(a,b); }
    friend inline bool operator>=(const OffsetTime& a, const OffsetTime& b) { return std::rel_ops::operator>=(a,b); }
#else
    auto operator <=>(const OffsetTime& ot) const -> std::weak_ordering
    {
        return toEpochSecond() <=> ot.toEpochSecond();
    }
#endif

  private:
    inline int32_t toEpochSecond() const { return _lt.toSecondOfDay() - _zoff.totalSeconds(); }
    
  public:
    static const OffsetTime MIN; //!< //!< @brief The minimum supported offsettime.
    static const OffsetTime MAX; //!< //!< @brief The maximum supported offsettime.
    
  private:
    LocalTime _lt{};
    ZoneOffset _zoff{};

    static constexpr int32_t SEC_PER_MIN = 60;
    static constexpr int32_t SEC_PER_HOUR = 60 * SEC_PER_MIN;
    static constexpr int32_t SEC_PER_DAY = 24 * SEC_PER_HOUR;
};


/*!
  @class LocalDateTime
  @brief A date-time without a time-zone in the ISO-8601 calendar system, such as 2009-08-07T12:34:56. 
*/
class LocalDateTime
{
  public:
    ///@name Constructors
    //@{
    constexpr LocalDateTime() {}
    constexpr LocalDateTime(const int16_t y, const int8_t m, const int8_t d, const int8_t hh, const int8_t mm, const int8_t ss)
            : LocalDateTime({y, m, d}, {hh, mm, ss }) {}
    constexpr LocalDateTime(const LocalDate& ld, const LocalTime& lt) : _date(ld), _time(lt) {}
    constexpr explicit LocalDateTime(const struct tm& tm) : _date(tm), _time(tm) {}
    explicit  LocalDateTime(const char* s) { *this = parse(s); }
    //@}
    
    ///@name Properties
    ///@{
    constexpr int16_t   year()       const { return _date.year(); } //!< @brief Gets the year.
    constexpr int8_t    month()      const { return _date.month();  } //!< @brief Gets the month.
    constexpr int8_t    day()        const { return _date.day(); } //!< @brief Gets the day.
    constexpr DayOfWeek dayOfWeek()  const { return _date.dayOfWeek(); } //!< @brief Gets the day of week, which is an enum DayOfWeek.
    constexpr bool      isLeapYear() const { return _date.isLeapYear(); } //!< @brief Is leap year?
    constexpr int8_t    hour()       const { return _time.hour(); } //!< @brief Gets the hour.
    constexpr int8_t    hour12()     const { return _time.hour12(); } //!< @brief Gets the 12 hour clock [1-12]
    constexpr int8_t    minute()     const { return _time.minute(); } //!< @brief Gets the minute.
    constexpr int8_t    second()     const { return _time.second(); } //!< @brief Gets the second.
    constexpr bool      isAM()       const { return _time.isAM(); } //!< @brief Is ante meridian?
    constexpr bool      isPM()       const { return _time.isPM(); } //!< @brief Is post meridian?
    constexpr uint32_t  hash()       const { return _date.hash() ^ _time.hash(); } //!< @brief A hash code for this.
    ///@}

    /*! @brief Is valid instance? */
    constexpr bool valid() const { return _date.valid() && _time.valid(); }
    /*! @brief Combines with an offset to create an OffsetDateTime. */
    OffsetDateTime atOffset(const ZoneOffset& zo) const;
    /*! @brief Converts this date to the Epoch Day. */
    inline int32_t toEpochDay() const { return _date.toEpochDay(); }
    /*! @brief Converts this date-time to the number of seconds from the epoch of 1970-01-01T00:00:00Z. */
    time_t toEpochSecond(const ZoneOffset& zo) const;
    /*! @brief Gets the LocalDate part of this date-time. */
    constexpr LocalDate toLocalDate() const { return _date; }
    /*! @brief Gets the LocalTime part of this date-time. */
    constexpr LocalTime toLocalTime() const { return _time; }
    /*! @brief Extracts the time as seconds of day, from 0 to 24 * 60 * 60 - 1. */
    constexpr int32_t toSecondOfDay() const { return _time.toSecondOfDay(); }
    /*!
      @brief Outputs this date-time as a String, such as 2009-08-07T12:34:56
      @param fmt Format specifier similar to std::strftime.
     */
    string_t toString(const char* fmt = nullptr) const;
    /*! @brief Converts this date-time to the struct tm. */
    struct tm toTm() const;

    /*! @brief Obtains the current date-time from the system clock in the default time-zone. */
    static LocalDateTime now();
    /*! @brief Obtains an instance of LocalDateTime from year, month, day, hour, minute and second. */
    static constexpr LocalDateTime of(const int16_t year, const int8_t month, const int8_t day, const int8_t hour = 0, const int8_t minute = 0, const int8_t second = 0) { return LocalDateTime{{year, month, day}, {hour, minute, second}}; }
    /*! @brief Obtains an instance of LocalDateTime from a date and time. */
    static constexpr LocalDateTime of(const LocalDate& ld, const LocalTime& lt) { return LocalDateTime(ld, lt); }
    /*! @brief Obtains an instance of LocalDateTime using epoch and offset. */
    static LocalDateTime ofEpochSecond(const time_t& epoch, const ZoneOffset& zo);
    /*! @brief Obtains an instance of LocalDateTime from a text string.
      @param s string. such as "2009-08-07T12:34:56"
    */
    static LocalDateTime parse(const char* s);

#if __cplusplus < 202002L    
    friend inline bool operator==(const LocalDateTime& a, const LocalDateTime& b) { return a.toEpochSecond(ZoneOffset::UTC) == b.toEpochSecond(ZoneOffset::UTC); }
    friend inline bool operator< (const LocalDateTime& a, const LocalDateTime& b) { return a.toEpochSecond(ZoneOffset::UTC) <  b.toEpochSecond(ZoneOffset::UTC); }
    friend inline bool operator!=(const LocalDateTime& a, const LocalDateTime& b) { return std::rel_ops::operator!=(a,b); }
    friend inline bool operator> (const LocalDateTime& a, const LocalDateTime& b) { return std::rel_ops::operator> (a,b); }
    friend inline bool operator<=(const LocalDateTime& a, const LocalDateTime& b) { return std::rel_ops::operator<=(a,b); }
    friend inline bool operator>=(const LocalDateTime& a, const LocalDateTime& b) { return std::rel_ops::operator>=(a,b); }
#else
    auto operator <=>(const LocalDateTime&) const = default;
#endif    

  public:
    static const LocalDateTime MIN; //!< @brief The minimum supported localdatetime.
    static const LocalDateTime MAX; //!< @brief The maximum supported localdatetime.

  private:
    LocalDate _date{};
    LocalTime _time{};
};

/*!
  @class OffsetDateTime
  @brief A date-time with an offset from UTC/Greenwich in the ISO-8601 calendar system, such as 2010-09-08T12:34:56+07:00.
*/
class OffsetDateTime
{
  public:
    ///@name Constructors
    ///@{
    constexpr OffsetDateTime() {}
    constexpr OffsetDateTime(const LocalDate& ld, const LocalTime& lt, const ZoneOffset& zo) : OffsetDateTime({ld,lt}, zo) {}
    constexpr OffsetDateTime(const LocalDateTime& ldt, const ZoneOffset& zo) : _datetime(ldt), _zoff(zo) {}
    constexpr OffsetDateTime(const struct tm& tm, const ZoneOffset& zo) : OffsetDateTime(LocalDateTime(tm), zo) {}
    explicit  OffsetDateTime(const char* s) { *this = parse(s); }
    ///@}

    ///@name Properties
    ///@{
    constexpr int16_t    year()       const { return _datetime.year(); } //!< @brief Gets the year.
    constexpr int8_t     month()      const { return _datetime.month();  } //!< @brief Gets the month.
    constexpr int8_t     day()        const { return _datetime.day(); } //!< @brief Gets the day.
    constexpr DayOfWeek  dayOfWeek()  const { return _datetime.dayOfWeek(); } //!< @brief Gets the day of week, which is an enum DayOfWeek.
    constexpr bool       isLeapYear() const { return _datetime.isLeapYear(); } //!< @brief Is leap year?
    constexpr int8_t     hour()       const { return _datetime.hour(); } //!< @brief Gets the hour.
    constexpr int8_t     hour12()     const { return _datetime.hour12(); } //!< @brief Gets the 12 hour clock [1-12]
    constexpr int8_t     minute()     const { return _datetime.minute(); } //!< @brief Gets the minute.
    constexpr int8_t     second()     const { return _datetime.second(); } //!< @brief Gets the second.
    constexpr bool       isAM()       const { return _datetime.isAM(); } //!< @brief Is ante meridian?
    constexpr bool       isPM()       const { return _datetime.isPM(); } //!< @brief Is post meridian?
    constexpr ZoneOffset offset()     const { return _zoff; } //!< @brief Gets the offset.
    constexpr uint32_t   hash()       const { return _datetime.hash() ^ _zoff.hash(); } //!< @brief A hash code for this.
    ///@}

    /*! @brief Is valid instance? */
    constexpr bool valid() const { return _datetime.valid() && _zoff.valid(); }
    /*! @brief Converts this date-time to the number of seconds from the epoch of 1970-01-01T00:00:00Z. */
    inline time_t toEpochSecond() const { return _datetime.toEpochSecond(_zoff); }
    /*! @brief Gets the LocalDate part of this date-time. */
    constexpr LocalDate toLocalDate() const { return _datetime.toLocalDate(); }
    /*! @brief Gets the LocalDateTime part of this date-time. */
    constexpr LocalDateTime toLocalDateTime() const { return _datetime; }
    /*! @brief Gets the LocalTime part of this date-time. */ 
    constexpr LocalTime toLocalTime() const { return _datetime.toLocalTime(); }
    /*! @brief Converts this date-time to an {@code OffsetTime}. */
    OffsetTime toOffsetTime() const { return OffsetTime::of(_datetime.toLocalTime(), _zoff); }
    /*!
      @brief Outputs this date-time as a String, such as 2010-09-08T12:34:56+07:00
      @param fmt Format specifier similar to std::strftime.
     */
    string_t toString(const char* fmt = nullptr) const { return _datetime.toString(fmt) += _zoff.toString(); }
    /*! @brief Returns a copy of this OffsetDateTime with the specified offset ensuring that the result is at the same epoch.*/
    OffsetDateTime withOffsetSameEpoch(const ZoneOffset& zo) const;
    /*! @brief Returns a copy of this OffsetDateTime with the specified offset ensuring that the result has the same local date-time.*/
    constexpr OffsetDateTime withOffsetSameLocal(const ZoneOffset& zo) const { return OffsetDateTime(_datetime, zo); }

    /*! @brief Obtains the current date-time from the system clock in the default time-zone. */
    static OffsetDateTime now();
    /*! @brief Obtains an instance of OffsetDateTime from a date, time and offset. */
    static constexpr OffsetDateTime of(const LocalDate& ld, const LocalTime& lt, const ZoneOffset& zo) { return OffsetDateTime(ld, lt, zo); }
    /*! @brief Obtains an instance of OffsetDateTime from a date-time and offset. */
    static constexpr OffsetDateTime of(const LocalDateTime& ldt, const ZoneOffset& zo) { return OffsetDateTime(ldt, zo); }
    /*! @brief Obtains an instance of OffsetDateTime from struct tm */
    static constexpr OffsetDateTime of(const struct tm& tm, const ZoneOffset& zo) { return OffsetDateTime(tm, zo); }
    /*! @brief Obtains an instance of OffsetDateTime from year, month, day, hour, minute, second and offset. */
    static constexpr OffsetDateTime of(const int16_t year, const int8_t month, const int8_t day, const int8_t hour, const int8_t minute, const int8_t second, const ZoneOffset& zo) { return OffsetDateTime(LocalDateTime::of(year, month, day, hour, minute, second), zo); }
    /*! @brief Obtains an instance of OffsetDateTime from year, month, day and offset. */
    static constexpr OffsetDateTime of(const int16_t year, const int8_t month, const int8_t dayOfMonth, const ZoneOffset& zo) { return of(year, month, dayOfMonth, 0, 0, 0, zo); }

    /*! @brief Obtains an instance of OffsetDateTime from a text string such as 2009-08-07T12:34:56+07:00 */
    static OffsetDateTime parse(const char* s);


    friend inline bool operator==(const OffsetDateTime& a, const OffsetDateTime& b) { return a.toEpochSecond() == b.toEpochSecond(); }
#if __cplusplus < 202002L
    friend inline bool operator< (const OffsetDateTime& a, const OffsetDateTime& b) { return a.toEpochSecond() <  b.toEpochSecond(); }
    friend inline bool operator!=(const OffsetDateTime& a, const OffsetDateTime& b) { return std::rel_ops::operator!=(a,b); }
    friend inline bool operator> (const OffsetDateTime& a, const OffsetDateTime& b) { return std::rel_ops::operator> (a,b); }
    friend inline bool operator<=(const OffsetDateTime& a, const OffsetDateTime& b) { return std::rel_ops::operator<=(a,b); }
    friend inline bool operator>=(const OffsetDateTime& a, const OffsetDateTime& b) { return std::rel_ops::operator>=(a,b); }
#else
    auto operator <=>(const OffsetDateTime& odt) const -> std::weak_ordering
    {
        return toEpochSecond() <=> odt.toEpochSecond();
    }
#endif    

  public:
    static const OffsetDateTime MIN; //!< @brief The minimum supported offsetdatetime.
    static const OffsetDateTime MAX; //!< @brief The maximum supported offsetdatetime.
    
  private:
    LocalDateTime _datetime{};
    ZoneOffset _zoff;
};


#ifdef UNIT_TEST
// dummy clock for now()
class MockClock
{
  public:
    MockClock() : _epoch(-1) {} // using std::time()
    explicit MockClock(time_t t) : _epoch(t) {} // using fixed time_t
    time_t now() const;
  private:
    time_t _epoch;
};

void injectMockClock(const MockClock& clock); // Use MockClock::now in static now();
void removeMockClock(); // Remove MockClok;
#endif
//
}}
#endif
