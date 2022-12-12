#include <gtest/gtest.h>
#include <gob_datetime.hpp>
#include <limits>
#include "helper.hpp"

using namespace goblib::datetime;

TEST(LocalDateTime, Basic)
{
    {
        constexpr LocalDateTime ldt;
        constexpr auto y = ldt.year();
        constexpr auto m = ldt.month();
        constexpr auto d = ldt.day();
        constexpr auto dofw = ldt.dayOfWeek();
        constexpr auto ily = ldt.isLeapYear();
        constexpr auto hh = ldt.hour();
        constexpr auto hh12 = ldt.hour12();
        constexpr auto mm = ldt.minute();
        constexpr auto ss = ldt.second();
        constexpr auto am = ldt.isAM();
        constexpr auto pm = ldt.isPM();
        EXPECT_TRUE(ldt.valid());
        EXPECT_EQ(1970, y);
        EXPECT_EQ(1, m);
        EXPECT_EQ(1, d);
        EXPECT_EQ(dofw, DayOfWeek::Thu);
        EXPECT_FALSE(ily);
        EXPECT_EQ(0, hh);
        EXPECT_EQ(12, hh12);
        EXPECT_EQ(0, mm);
        EXPECT_EQ(0, ss);
        EXPECT_TRUE(am);
        EXPECT_FALSE(pm);
        EXPECT_STREQ("1970-01-01T00:00:00", ldt.toString().c_str());
    }
    {
        constexpr LocalDate ld(1999, 12, 31);
        constexpr LocalTime lt(23, 59, 59);
        constexpr LocalDateTime ldt{ld, lt};
        EXPECT_TRUE(ldt.valid());
        EXPECT_EQ(ld, ldt.toLocalDate());
        EXPECT_EQ(lt, ldt.toLocalTime());
    }
    {
        struct tm tmp{};
        tmp.tm_year = 2009 - 1900;
        tmp.tm_mon = 8 - 1;
        tmp.tm_mday = 7;;
        tmp.tm_hour = 12;
        tmp.tm_min = 34;
        tmp.tm_sec = 56;
        LocalDateTime ldt{tmp};
        EXPECT_TRUE(ldt.valid());
        EXPECT_EQ(2009, ldt.year());
        EXPECT_EQ(8, ldt.month());
        EXPECT_EQ(7, ldt.day());
        EXPECT_EQ(DayOfWeek::Fri, ldt.dayOfWeek());
        EXPECT_FALSE(ldt.isLeapYear());
        EXPECT_EQ(12, ldt.hour());
        EXPECT_EQ(34, ldt.minute());
        EXPECT_EQ(56, ldt.second());
    }
    {
        const char* tbl[] =
        {
            "1987-06-05T04:03:21",
            "2001-02-03T04:56:07",
            "2022-11-22T22:33:44",
        };
        for(auto& e : tbl)
        {
            LocalDateTime ldt{e};
            EXPECT_TRUE(ldt.valid()) << ldt.toString().c_str();
            EXPECT_STREQ(e, ldt.toString().c_str()) << ldt.toString().c_str();
        }
    }
    // Invalid date-time
    {
        struct YMDHMS { int y,m,d,hh,mm,ss;};
        YMDHMS tbl[] =
        {
            { 1987,  9,  8, 12, 34, 99 }, // Invalid time
            {  123, 11, 30,  7, 17, 27 }, // Invalid date  
            { 2000, 15, 01, 32, 03, 12 }, // Invalid both
        };
        for(auto& e : tbl)
        {
            LocalDateTime ldt(LocalDate(e.y, e.m, e.d), LocalTime(e.hh, e.mm, e.ss));
            EXPECT_FALSE(ldt.valid()) << ldt.toString().c_str();
        }
        const char* stbl[] =
        {
            "2100-02-29T12:34:56",
            "2000-02-10T44:01:02",
            "1987-11-31T03:-1:34",
        };
        for(auto& e : stbl)
        {
            LocalDateTime ldt(e);
            EXPECT_FALSE(ldt.valid()) << e  << " : " << ldt.toString().c_str();
        }
    }
}

TEST(LocalDateTime, InstanceMethods)
{
    // atOffset
    {
        struct YMDHMSO { int16_t y; int8_t m,d,hh,mm,ss; ZoneOffset zo; };
        YMDHMSO tbl[] =
                {
                    { 1987,  6,  5,  4,  32, 11,  ZoneOffset( 59400) }, // +16:30
                    { 1987,  6,  5,  4,  32, 11,  ZoneOffset(-28800) }, // -08:00
                    { 1987,  6,  5,  4,  32, 11,  ZoneOffset(     0) }, // 00:00
                };
        for(auto& e : tbl)
        {
            auto odt = LocalDateTime{e.y, e.m, e.d, e.hh, e.mm, e.ss}.atOffset(e.zo);
            EXPECT_EQ(e.y, odt.year()) << odt.toString().c_str();
            EXPECT_EQ(e.m, odt.month()) << odt.toString().c_str();
            EXPECT_EQ(e.d, odt.day()) << odt.toString().c_str();
            EXPECT_EQ(e.hh, odt.hour()) << odt.toString().c_str();
            EXPECT_EQ(e.mm, odt.minute()) << odt.toString().c_str();
            EXPECT_EQ(e.ss, odt.second()) << odt.toString().c_str();
            EXPECT_EQ(e.zo, odt.offset()) << odt.toString().c_str();
        }
    }
    // toEpochDay call LocalDate::toEpochDay inside.
    // toEpochSecond
    {
        struct YMDHMSOE { int16_t y; int8_t m,d,hh,mm,ss; ZoneOffset zo; time_t e;};
        YMDHMSOE tbl[] =
        {
            { 1987,  6,  5,  4,  32, 11, ZoneOffset( 59400), 549806531 },  // +16:30
            { 1987,  6,  5,  4,  32, 11, ZoneOffset(     0), 549865931 },  // -08:00
            { 1987,  6,  5,  4,  32, 11, ZoneOffset(-28800), 549894731 },  // -08:00
            { 2037, 12, 23, 11,  22, 33, ZoneOffset(     0), 2145180153 }, // +00:00
            { 2037, 12, 23, 12,  22, 33, ZoneOffset(  3600), 2145180153 }, // +01:00
            { 2037, 12, 23, 10,  22, 33, ZoneOffset( -3600), 2145180153 }, // -01:00
        };
        for(auto& e :tbl)
        {
            auto ldt = LocalDateTime(e.y, e.m, e.d, e.hh, e.mm, e.ss);
            EXPECT_EQ(e.e, ldt.toEpochSecond(e.zo)) << ldt.toString().c_str() << " : " << e.zo.toString().c_str();
        }
#ifdef GOBLIB_DATETIME_USE_TIME_T_GREATER_THAN_32BIT
        YMDHMSOE tbl2[] =
        {
            { 2038,  1, 19,  3,  14,  6, ZoneOffset::UTC, 2147483646 },
            { 2038,  1, 19,  3,  14,  7, ZoneOffset::UTC, 2147483647 }, // maxumum value of 32bit time_t
            { 2038,  1, 19,  3,  14,  8, ZoneOffset::UTC, 2147483648 },
            { 2100, 12, 23, 11,  22, 33, ZoneOffset::UTC, 4133244153 },
        };
        for(auto& e :tbl2)
        {
            auto ldt = LocalDateTime::of(e.y, e.m, e.d, e.hh, e.mm, e.ss);
            EXPECT_EQ(e.e, ldt.toEpochSecond(e.zo)) << ldt.toString().c_str() << " : " << e.zo.toString().c_str();
        }
#endif
    }
    // toSecondOfDay call LocalTime::toSecondOfDay
    // toString
    {
        LocalDateTime ldt = LocalDateTime::of(2345, 6, 7, 18, 9, 10);
        EXPECT_STREQ("2345-06-07T18:09:10", ldt.toString().c_str());
        EXPECT_STREQ("06/07/45", ldt.toString("%D").c_str());
        //        EXPECT_STREQ("Thu Jun  7 18:09:10 2345", ldt.toString("%c").c_str()); // depends on your locale.
        EXPECT_STREQ("06:09:10", ldt.toString("%I:%M:%S").c_str());
        EXPECT_STREQ("18:09", ldt.toString("%R").c_str());
        EXPECT_STREQ("06:09:10 PM", ldt.toString("%r").c_str());
    }
    // toTm
    {
        struct YMDHMS { int y,m,d,hh,mm,ss; };
        YMDHMS tbl[] =
        {
            { 1971,  2,  3,  0,  0,  0 },
            { 1982,  5,  6, 12, 34, 56 },
            { 1993,  8,  9, 23, 45, 56 },
            { 2004, 11, 12, 23, 59, 59 },
        };
        for(auto& e : tbl)
        {
            LocalDateTime ldt = LocalDateTime::of(e.y, e.m, e.d, e.hh, e.mm, e.ss);
            auto tmp = ldt.toTm();
            EXPECT_EQ(e.y - 1900, tmp.tm_year) << ldt.toString().c_str();
            EXPECT_EQ(e.m - 1, tmp.tm_mon) << ldt.toString().c_str();
            EXPECT_EQ(e.d, tmp.tm_mday) << ldt.toString().c_str();
            EXPECT_EQ(e.hh, tmp.tm_hour) << ldt.toString().c_str();
            EXPECT_EQ(e.mm, tmp.tm_min) << ldt.toString().c_str();
            EXPECT_EQ(e.ss, tmp.tm_sec) << ldt.toString().c_str();
        }
    }
}

TEST(LocalDateTime, StaticMethods)
{
    // now
    {
        struct YMDHMS { int y,m,d,hh,mm,ss; };
        YMDHMS tbl[] =
        {
            { 1971,  2,  3,  0,  0,  0 },
            { 1982,  5,  6, 12, 34, 56 },
            { 1993,  8,  9, 23, 45, 56 },
            { 2004, 11, 12, 23, 59, 59 },
        };
        for(auto& e : tbl)
        {
            injectMockClockDateTime(e.y, e.m, e.d, e.hh, e.mm, e.ss);
            LocalDateTime ldt = LocalDateTime::now();
            EXPECT_EQ(e.y, ldt.year()) << ldt.toString().c_str();
            EXPECT_EQ(e.m, ldt.month()) << ldt.toString().c_str();
            EXPECT_EQ(e.d, ldt.day()) << ldt.toString().c_str();
            EXPECT_EQ(e.hh, ldt.hour()) << ldt.toString().c_str();
            EXPECT_EQ(e.mm, ldt.minute()) << ldt.toString().c_str();
            EXPECT_EQ(e.ss, ldt.second()) << ldt.toString().c_str();
        }
        resetMockClock();
    }
    // of -> constructor
    // ofEpochSecond
    {
        struct YMDHMSOE { LocalDate ld; LocalTime lt; ZoneOffset zo; time_t e;};
        YMDHMSOE tbl[] =
        {
            { {1987,  6,  5}, { 4,  32, 11}, ZoneOffset( 59400), 549806531 },  // +16:30
            { {1987,  6,  5}, { 4,  32, 11}, ZoneOffset(     0), 549865931 },  // -08:00
            { {1987,  6,  5}, { 4,  32, 11}, ZoneOffset(-28800), 549894731 },  // -08:00
            { {2037, 12, 23}, {11,  22, 33}, ZoneOffset(     0), 2145180153 }, // +00:00
            { {2037, 12, 23}, {12,  22, 33}, ZoneOffset(  3600), 2145180153 }, // +01:00
            { {2037, 12, 23}, {10,  22, 33}, ZoneOffset( -3600), 2145180153 }, // -01:00
        };
        for(auto& e :tbl)
        {
            auto ldt = LocalDateTime::ofEpochSecond(e.e, e.zo);
            EXPECT_EQ(e.ld, ldt.toLocalDate()) << e.e << " : " << e.zo.toString().c_str();
            EXPECT_EQ(e.lt, ldt.toLocalTime()) << e.e << " : " << e.zo.toString().c_str();
        }
#ifdef GOBLIB_DATETIME_USE_TIME_T_GREATER_THAN_32BIT
        YMDHMSOE tbl2[] =
        {
            { {2038,  1, 19}, { 3,  14,  6}, ZoneOffset::UTC, 2147483646 },
            { {2038,  1, 19}, { 3,  14,  7}, ZoneOffset::UTC, 2147483647 }, // maxumum value of 32bit time_t
            { {2038,  1, 19}, { 3,  14,  8}, ZoneOffset::UTC, 2147483648 },
            { {2100, 12, 23}, {11,  22, 33}, ZoneOffset::UTC, 4133244153 },
        };
        for(auto& e :tbl2)
        {
            auto ldt = LocalDateTime::ofEpochSecond(e.e, e.zo);
            EXPECT_EQ(e.ld, ldt.toLocalDate()) << e.e << " : " << e.zo.toString().c_str();
            EXPECT_EQ(e.lt, ldt.toLocalTime()) << e.e << " : " << e.zo.toString().c_str();
        }
#endif
    }
    // parse
    {
        // valid
        {
            const char* tbl[] = { "1977-10-07T12:34:56", "2011-02-20T01:02:03", "1999-01-07T23:23:23" };
            for(auto& e : tbl)
            {
                LocalDateTime ldt = LocalDateTime::parse(e);
                EXPECT_TRUE(ldt.valid());
                EXPECT_STREQ(e, ldt.toString().c_str()) << e;
            }
        }
        // invalid
        {
            const char* tbl[] = { "1982-02-03T10:31:99", "1982-02-31T10:31:56", "1999-3-42T26:31:51" };
            for(auto& e : tbl)
            {
                LocalDateTime ldt = LocalDateTime::parse(e);
                EXPECT_FALSE(ldt.valid()) << e;
            }
        }
    }
}

TEST(LocalDateTime, Compare)
{
    LocalDateTime ldt_b0(2010, 9, 8, 12, 34, 56);
    LocalDateTime ldt_b1(2010, 9, 8, 12, 34 ,56);

    EXPECT_EQ(ldt_b0, ldt_b1);
    EXPECT_LE(ldt_b0, ldt_b1);
    EXPECT_GE(ldt_b0, ldt_b1);
    EXPECT_FALSE(ldt_b0 < ldt_b1);
    EXPECT_FALSE(ldt_b0 > ldt_b1);
    
    LocalDateTime lesser[] =
    {
        { {2010, 9, 8},{12, 34, 55 }},
        { {2010, 9, 8},{12, 33, 56 }},
        { {2010, 9, 8},{11, 33, 56 }},
        { {2010, 9, 7},{12, 34, 56 }},
        { {2010, 8, 7},{12, 34, 56 }},
        { {2009, 8, 7},{12, 34, 56 }},
        LocalDateTime::MIN
    };
    for(auto& e : lesser)
    {
        EXPECT_NE(e, ldt_b0) << e.toString().c_str() << " :cmp: " << ldt_b0.toString().c_str();
        EXPECT_LT(e, ldt_b0) << e.toString().c_str() << " :cmp: " << ldt_b0.toString().c_str();
        EXPECT_LE(e, ldt_b0) << e.toString().c_str() << " :cmp: " << ldt_b0.toString().c_str();
    }
      
    LocalDateTime greater[] =
    {
        {{2010,  9,  8}, {12, 34,  57}},
        {{2010,  9,  8}, {12, 35,  56}},
        {{2010,  9,  8}, {13, 34,  56}},
        {{2010,  9,  9}, {12, 34,  56}},
        {{2010, 10,  8}, {12, 34,  56}},
        {{2011,  9,  8}, {12, 34,  56}},
        LocalDateTime::MAX
    };
    int idx = 0;
    for(auto& e : greater)
    {
        EXPECT_NE(e, ldt_b0) << idx << ":" << e.toString().c_str() << " :cmp: " << ldt_b0.toString().c_str();
        EXPECT_GT(e, ldt_b0) << idx << ":"<< e.toString().c_str() << " :cmp: " << ldt_b0.toString().c_str(); 
        EXPECT_GE(e, ldt_b0) << idx << ":"<< e.toString().c_str() << " :cmp: " << ldt_b0.toString().c_str();
        ++idx;
    }
}
