#include <gtest/gtest.h>
#include <gob_datetime.hpp>
#include "helper.hpp"

using namespace goblib::datetime;

namespace
{
void test_dayOfMonth(int year, bool leap)
{
    constexpr static int8_t lomTable[2][12] =
    {
        { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, // standard
        { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, // leap year
    };
    for(int i=1; i<=12; ++i)
    {
        LocalDate ld(year, i, 1);
        EXPECT_EQ(lomTable[leap][i-1], ld.lengthOfMonth()) << year << ":" << i << ":" << leap;
    }
}
//
}

TEST(LocalDate, Basic)
{
    {
        constexpr LocalDate ld0;
        constexpr auto valid = ld0.valid();
        constexpr auto leapyear = ld0.isLeapYear();
        constexpr auto y = ld0.year();
        constexpr auto m = ld0.month();
        constexpr auto d = ld0.day();
        constexpr auto dofw = ld0.dayOfWeek();
        constexpr auto lofy = ld0.lengthOfYear();
        constexpr auto lofm = ld0.lengthOfMonth();
        EXPECT_TRUE(valid);
        EXPECT_EQ(1970, y);
        EXPECT_EQ(1, m);
        EXPECT_EQ(1, d);
        EXPECT_EQ(dofw, DayOfWeek::Thu);
        EXPECT_FALSE(leapyear);
        EXPECT_EQ(365, lofy);
        EXPECT_EQ(31, lofm);
        EXPECT_STREQ("1970-01-01", ld0.toString().c_str());
    }
    {
        constexpr LocalDate ld1(2100, 2, 28);
        EXPECT_TRUE(ld1.valid());
        EXPECT_EQ(ld1.year(), 2100);
        EXPECT_EQ(ld1.month(), 2);
        EXPECT_EQ(ld1.day(), 28);
        EXPECT_EQ(ld1.dayOfWeek(), DayOfWeek::Sun);
        EXPECT_FALSE(ld1.isLeapYear());
        EXPECT_EQ(365, ld1.lengthOfYear());
        EXPECT_EQ(28, ld1.lengthOfMonth());
        EXPECT_STREQ("2100-02-28", ld1.toString().c_str());
    }
    {
        LocalDate ld2("2009-08-07");;
        EXPECT_EQ(ld2.year(), 2009);
        EXPECT_EQ(ld2.month(), 8);
        EXPECT_EQ(ld2.day(), 7);
        EXPECT_EQ(ld2.dayOfWeek(), DayOfWeek::Fri);
        EXPECT_FALSE(ld2.isLeapYear());
        EXPECT_EQ(365, ld2.lengthOfYear());
        EXPECT_EQ(31, ld2.lengthOfMonth());
        EXPECT_STREQ("2009-08-07", ld2.toString().c_str());
    }
    {
        struct tm tmp{};
        tmp.tm_year = 2009 -1900;
        tmp.tm_mon = 8 - 1;
        tmp.tm_mday = 7;
        LocalDate ld3{tmp};
        EXPECT_EQ(ld3.year(), 2009);
        EXPECT_EQ(ld3.month(), 8);
        EXPECT_EQ(ld3.day(), 7);
        EXPECT_EQ(ld3.dayOfWeek(), DayOfWeek::Fri);
        EXPECT_FALSE(ld3.isLeapYear());
        EXPECT_EQ(365, ld3.lengthOfYear());
        EXPECT_EQ(31, ld3.lengthOfMonth());
        EXPECT_STREQ("2009-08-07", ld3.toString().c_str());
    }  
    // leap
    {
        struct LY { int y; bool l; };
        LY lyTable[] =
                {
                    { 1987, false },
                    { 1988, true },
                    { 2000, true },
                    { 2100, false },
                    { 2200, false },
                    { 2300, false },
                    { 2400, true },
                };
        for(auto& e : lyTable)
        {
            auto ld = LocalDate::of(e.y);
            EXPECT_EQ(ld.isLeapYear(), e.l) << ld.toString().c_str();
        }
    }
    // dayOfWeek
    {
        struct YMDW { int y,m,d; DayOfWeek w; };
        YMDW tbl[] =
        {
            { 1999, 12, 31, DayOfWeek::Fri },
            { 2000,  1,  1, DayOfWeek::Sat },
            { 2000,  2, 28, DayOfWeek::Mon },
            { 2000,  2, 29, DayOfWeek::Tue },
            { 2000,  3,  1, DayOfWeek::Wed },
            { 2001,  2, 28, DayOfWeek::Wed },
            { 2001,  3,  1, DayOfWeek::Thu },
        };
        for(auto& e : tbl)
        {
            LocalDate ld(e.y, e.m, e.d);
            EXPECT_EQ(e.w, ld.dayOfWeek()) << ld.toString().c_str();
        }
    }    
    // dayOfMonth
    {
        test_dayOfMonth(1987, false);
        test_dayOfMonth(1988, true);
        test_dayOfMonth(2000, true);
        test_dayOfMonth(2100, false);
    }    
    // day of year
    // std::mktime gmttime_r
    {
        time_t t = std::time(nullptr);
        struct tm tmp{};
        toLocaltime(&t, &tmp);
        LocalDate ld(tmp);
        EXPECT_EQ(tmp.tm_year + 1900, ld.year());
        EXPECT_EQ(tmp.tm_mon + 1, ld.month());
        EXPECT_EQ(tmp.tm_mday, ld.day());
        EXPECT_EQ(tmp.tm_wday, (int)ld.dayOfWeek());
        EXPECT_EQ(tmp.tm_yday, ld.dayOfYear()); // same as struct tm.tm_yday

        struct YMDDOY { int y, m, d, doy; };
        YMDDOY tbl[] =
        {
            { 2022,  1,  1, 0   },
            { 2022,  2, 10, 40  },
            { 2022,  3, 12, 70  },
            { 2022,  4, 11, 100 },
            { 2022, 10, 28, 300 },
            { 2022, 12, 31, 364 },
            // leapyear
            { 2000,  1,  1, 0   },
            { 2000,  2, 10, 40  },
            { 2000,  3, 12, 71  },
            { 2000,  4, 11, 101 },
            { 2000, 10, 28, 301 },
            { 2000, 12, 31, 365 },
        };
        for(auto& e : tbl)
        {
            LocalDate ld(e.y, e.m, e.d);
            EXPECT_EQ(e.doy, ld.dayOfYear()) << ld.toString().c_str();
        }
    }
    // Invalid date
    {
        struct YMD { int y,m,d; };
        {
            YMD tbl[] =
            {
                { 2100,  2, 28 },
                { 2000,  2, 29 },
                { 1987, 11, 30 }
            };
            for(auto& e : tbl)
            {
                LocalDate ld(e.y, e.m, e.d);
                EXPECT_TRUE(ld.valid()) << ld.toString().c_str();
            }
        }

        YMD tbl[] =
        {
            { 2100, 2, 29 },  // 2100 is not leap year, so 02-29 is not exists.
            { 2000, 2, 30 },  // 2000 is leap year.
            { 1987, 11, 31 }, // days of Nov is 30.
            { 2009,  1, 40 },
            { 2010, 13, 20 },
            { 2010, 16, -7 },
            {  123,  8,  9 },
            { 1192,  8,  0 },
            { 1600, -7,  9 },
            { 1889, 33, 33 },
        };
        for(auto& e : tbl)
        {
            LocalDate ld(e.y, e.m, e.d);
            EXPECT_FALSE(ld.valid()) << ld.toString().c_str();
        }

        const char* stbl[] =
        {
            "2100-02-29",
            "2000-02-30",
            "1987-11-31",
            "2009-01-40",
            "2010-13-20",
            "2010-16-67",
            "123-08-09",
            "1192-08-99",
            "1600-67-09",
            "1889-33-33",
            "abcdefg",
        };
        for(auto& e : stbl)
        {
            LocalDate ld(e);
            EXPECT_FALSE(ld.valid()) << ld.toString().c_str();
        }
    }
}

TEST(LocalDate, InstanceMethods)
{
    LocalDate ld{2009,8,7};
    int hh = 12;
    int mm = 34;
    int ss = 56;
    
    // atTime (atStartOfDay() calls atTime() inside)
    {
        auto ldt = ld.atTime(hh,mm,ss);
        EXPECT_EQ(ld.year(),  ldt.year());
        EXPECT_EQ(ld.month(), ldt.month());
        EXPECT_EQ(ld.day(),   ldt.day());
        EXPECT_EQ(hh, ldt.hour());
        EXPECT_EQ(mm, ldt.minute());
        EXPECT_EQ(ss, ldt.second());
    }
    {
        LocalTime lt(hh,mm,ss);
        auto ldt = ld.atTime(lt);
        EXPECT_EQ(ld.year(),  ldt.year());
        EXPECT_EQ(ld.month(), ldt.month());
        EXPECT_EQ(ld.day(),   ldt.day());
        EXPECT_EQ(hh, ldt.hour());
        EXPECT_EQ(mm, ldt.minute());
        EXPECT_EQ(ss, ldt.second());
    }
    {
        auto odt = ld.atTime(OffsetTime::MAX);
        auto ld2 = odt.toLocalDateTime().toLocalDate();
        EXPECT_EQ(ld.year(), ld2.year());
        EXPECT_EQ(ld.month(), ld2.month());
        EXPECT_EQ(ld.day(), ld2.day());
    }
    // lengthOfYear
    {
        EXPECT_EQ(365, LocalDate(1987,1,1).lengthOfYear());
        EXPECT_EQ(366, LocalDate(1988,1,1).lengthOfYear());
        EXPECT_EQ(366, LocalDate(2000,1,1).lengthOfYear());
        EXPECT_EQ(365, LocalDate(2100,1,1).lengthOfYear());
    }
    
    // lengthOfMonth
    {
        test_dayOfMonth(1987, false);
        test_dayOfMonth(1988, true);
        test_dayOfMonth(2000, true);
        test_dayOfMonth(2100, false);
    }

    // toEpochDay
    {
        struct YMDE { int y,m,d,e; };
        YMDE eodTable[] = {
            { 1970,  1,  1, 0 },
            { 1970,  1,  2, 1 },
            { 1970,  1, 11, 10 },
            { 1970,  4, 11, 100 },
            { 1972,  9, 27, 1000 },
            { 1997,  5, 19, 10000 },
            { 2243, 10, 17, 100000 }
        };
        for(auto& e : eodTable)
        {
            LocalDate ld(e.y, e.m, e.d);
            EXPECT_EQ(e.e, ld.toEpochDay()) << ld.toString().c_str();
        }
    }
    // toString
    {
        LocalDate ld{2009, 8, 7};
        EXPECT_STREQ("2009-08-07", ld.toString().c_str());
        EXPECT_STREQ("08/07/09", ld.toString("%D").c_str());
        //        EXPECT_STREQ("Fri Aug  7 00:00:00 2009", ld.toString("%c").c_str()); // depends on your locale.
    }
}

TEST(LocalDate, StaticMethods)
{
    // now
    struct YMD { int y,m,d; };
    YMD tbl[] =
    {
        { 1971,  2,  3 },
        { 1984,  5,  6 },
        { 1997,  8,  9 },
        { 2010, 11, 12 },
    };
    for(auto& e : tbl)
    {
        injectMockClockDate(e.y, e.m, e.d);
        LocalDate ld = LocalDate::now(); // LocalDate::now call LocalDateTime::now inside.
        EXPECT_EQ(e.y, ld.year());
        EXPECT_EQ(e.m, ld.month());
        EXPECT_EQ(e.d, ld.day());
        
    }
    resetMockClock();

    // of -> constructor(y, m, d)
    // parse -> construct(str)
}

TEST(LocalDate, Compare)
{
    LocalDate ld_b0(1987,  6,  5);
    LocalDate ld_b1(1987,  6,  5);

    EXPECT_EQ(ld_b0, ld_b1);
    EXPECT_LE(ld_b0, ld_b1);
    EXPECT_GE(ld_b0, ld_b1);
    EXPECT_FALSE(ld_b0 < ld_b1);
    EXPECT_FALSE(ld_b0 > ld_b1);
    
    LocalDate lesser[] =
    {
        { 1987, 6, 4 },
        LocalDate(1987,  5,  5),
        LocalDate(1986,  6,  5),
        LocalDate::MIN
    };
    for(auto& e : lesser)
    {
        EXPECT_NE(e, ld_b0) << e.toString().c_str() << " :cmp: " << ld_b0.toString().c_str();
        EXPECT_LT(e, ld_b0) << e.toString().c_str() << " :cmp: " << ld_b0.toString().c_str();
        EXPECT_LE(e, ld_b0) << e.toString().c_str() << " :cmp: " << ld_b0.toString().c_str();
    }
      
    LocalDate greater[] =
    {
        LocalDate(1987,  6,  6),
        LocalDate(1987,  7,  6),
        { 1988,  6,  5 },
        LocalDate::MAX
    };
    for(auto& e : greater)
    {
        EXPECT_NE(e, ld_b0) << e.toString().c_str() << " :cmp: " << ld_b0.toString().c_str();
        EXPECT_GT(e, ld_b0) << e.toString().c_str() << " :cmp: " << ld_b0.toString().c_str(); 
        EXPECT_GE(e, ld_b0)  << e.toString().c_str() << " :cmp: " << ld_b0.toString().c_str();
    }
}
