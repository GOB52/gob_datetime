#include <gtest/gtest.h>
#include <gob_datetime.hpp>
#include "helper.hpp"

using namespace goblib::datetime;

TEST(LocalTime, Basic)
{
    {
        constexpr LocalTime lt0;
        EXPECT_TRUE(lt0.valid());
        EXPECT_EQ(0,  lt0.hour());
        EXPECT_EQ(12, lt0.hour12());
        EXPECT_EQ(0,  lt0.minute());
        EXPECT_EQ(0,  lt0.second());
        EXPECT_TRUE(lt0.isAM());
        EXPECT_FALSE(lt0.isPM());
        EXPECT_STREQ("00:00:00", lt0.toString().c_str());
    }
    {
        constexpr LocalTime lt1{12,34,56};
        constexpr auto hh = lt1.hour();
        constexpr auto hh12 = lt1.hour12();
        constexpr auto mm = lt1.minute();
        constexpr auto ss = lt1.second();
        constexpr auto am = lt1.isAM();
        constexpr auto pm = lt1.isPM();
        constexpr auto valid = lt1.valid();
        constexpr auto sofd = lt1.toSecondOfDay();
        EXPECT_TRUE(valid);
        EXPECT_EQ(12, hh);
        EXPECT_EQ(12, hh12);
        EXPECT_EQ(34, mm);
        EXPECT_EQ(56, ss);
        EXPECT_FALSE(am);
        EXPECT_TRUE(pm);
        EXPECT_EQ(12 * 3600 + 34 * 60 + 56, sofd);
        EXPECT_STREQ("12:34:56", lt1.toString().c_str());
    }
    {
        LocalTime lt2("09:08:07");
        EXPECT_TRUE(lt2.valid());
        EXPECT_EQ(9, lt2.hour());
        EXPECT_EQ(9, lt2.hour12());
        EXPECT_EQ(8, lt2.minute());
        EXPECT_EQ(7, lt2.second());
        EXPECT_TRUE(lt2.isAM());
        EXPECT_FALSE(lt2.isPM());
        EXPECT_STREQ("09:08:07", lt2.toString().c_str());
    }
    {
        struct tm tmp{};
        tmp.tm_hour = 15; 
        tmp.tm_min = 16;
        tmp.tm_sec = 17;
        LocalTime lt{tmp};
        EXPECT_TRUE(lt.valid());
        EXPECT_EQ(15, lt.hour());
        EXPECT_EQ( 3, lt.hour12());
        EXPECT_EQ(16, lt.minute());
        EXPECT_EQ(17, lt.second());
        EXPECT_FALSE(lt.isAM());
        EXPECT_TRUE(lt.isPM());
        EXPECT_STREQ("15:16:17", lt.toString().c_str());
    }
    // hour12
    {
        int8_t h12Tbl[24] =
        {
            12, 1, 2, 3, 4 ,5, 6, 7, 8, 9, 10, 11,
            12, 1, 2, 3, 4 ,5, 6, 7, 8, 9, 10, 11
        };
        int8_t idx = 0;
        for(auto& e : h12Tbl)
        {
            LocalTime lt(idx, 0, 0);
            EXPECT_EQ(e, lt.hour12()) << lt.toString().c_str();
            EXPECT_EQ((idx/12) == 0, lt.isAM()) << lt.toString().c_str();
            EXPECT_EQ((idx/12) != 0, lt.isPM()) << lt.toString().c_str();
            ++idx;
        }
    }
    // invalid time
    {
        struct HMS { int h,m,s; };
        HMS tbl[] =
        {
            {  1,  2, -2 },
            {  2, 99,  4 },
            {  3, -8, 61 },
            { 77, 22, 33 },
            { 88, 33,-11 },
            { 99, 99, 10 },
            { 66, 66, -6 },
        };
        for(auto& e : tbl)
        {
            LocalTime lt(e.h, e.m, e.s);
            EXPECT_FALSE(lt.valid()) << lt.toString().c_str();
        }

        const char* stbl[] =
        {
            "01:02:-2",
            "02:99:04",
            "03:-8:61",
            "44:22:33",
            "88:33:-11",
            "99:99:10",
            "66:66:-6",
            "abcd",
        };
        for(auto& e : stbl)
        {
            LocalTime lt(e);
            EXPECT_FALSE(lt.valid()) << e << " : " << lt.toString().c_str();
        }
    }
}

TEST(LocalTime, InstanceMethods)
{
    // atDate
    {
        { // Valid
            LocalTime lt(12,23,34);
            LocalDate ld = LocalDate::now();
            LocalDateTime ldt = lt.atDate(ld);
            EXPECT_EQ(lt.hour(), ldt.hour());
            EXPECT_EQ(lt.minute(), ldt.minute());
            EXPECT_EQ(lt.second(), ldt.second());
        }
        { // Invalid
            LocalTime lt(-12,23,54); // Invalid time
            LocalDate ld = LocalDate::now();
            LocalDateTime ldt = lt.atDate(ld);
            EXPECT_FALSE(ldt.valid());
        }
        { // Invalid
            LocalTime lt(1,2,3);
            LocalDate ld(13456, 99, 99); // Invalid date
            LocalDateTime ldt = lt.atDate(ld);
            EXPECT_FALSE(ldt.valid());
        }
    }
    // toSecondOfDay
    {
        struct HMSD { int8_t h,m,s; int32_t sofd; };
        HMSD tbl[] =
        {
            {  0,  0,  0, 0 },
            {  0,  0,  1, 1 },
            {  0,  0, 10, 10 },
            {  1,  2,  3, 3723 },
            {  4,  5,  6, 14706 },
            {  7,  8,  9, 25689 },
            { 12,  0,  0, 43200 },
            { 23, 45, 60, 85560 },
            { 23, 59, 59, 86399 },
        };
        for(auto& e :tbl)
        {
            LocalTime lt(e.h, e.m, e.s);
            EXPECT_EQ(e.sofd, lt.toSecondOfDay()) << lt.toString().c_str();
        }
    }
    // toString
    {
        LocalTime lt(23, 34, 56);
        EXPECT_STREQ("23:34:56", lt.toString().c_str());
        EXPECT_STREQ("11:34:56", lt.toString("%I:%M:%S").c_str());
        EXPECT_STREQ("23:34", lt.toString("%R").c_str());
        EXPECT_STREQ("11:34:56 PM", lt.toString("%r").c_str());
    }
}

TEST(LocalTime, StaticMethods)
{
    // now
    {
        struct HMS { int8_t h,m,s; };
        HMS tbl[] =
                {
                    {  0,  0,  0 },
                    {  0, 11, 22 },
                    {  3, 44, 55 },
                    { 12, 34, 56 },
                    { 23, 45, 56 },
                    { 23, 59, 59 },
                };
        for(auto& e : tbl)
        {
            injectMockClockTime(e.h, e.m, e.s);
            LocalTime lt = LocalTime::now(); // LocalTime::now call LocalDateTime::now inside.
            EXPECT_EQ(e.h, lt.hour());
            EXPECT_EQ(e.m, lt.minute());
            EXPECT_EQ(e.s, lt.second());
        }
        resetMockClock();
    }
    // ofSecondOfDay
    {
        struct HMSD { int8_t h,m,s; int32_t sofd; };
        HMSD tbl[] =
        {
            {  0,  0,  0, 0 },
            {  0,  0,  1, 1 },
            {  0,  0, 10, 10 },
            {  1,  2,  3, 3723 },
            {  4,  5,  6, 14706 },
            {  7,  8,  9, 25689 },
            { 12,  0,  0, 43200 },
            { 23, 59, 59, 86399 },
        };
        for(auto& e :tbl)
        {
            LocalTime lt = LocalTime::ofSecondOfDay(e.sofd);
            EXPECT_EQ(e.h, lt.hour())   << e.sofd << " : " << lt.toString().c_str();
            EXPECT_EQ(e.m, lt.minute()) << e.sofd << " : " << lt.toString().c_str();
            EXPECT_EQ(e.s, lt.second()) << e.sofd << " : " << lt.toString().c_str();
        }
    }
    // of -> constructor(h, m, s)
    // parse -> constructor(str)
}

TEST(LocalTime, Compare)
{
    LocalTime lt_b0(12, 34, 56);
    LocalTime lt_b1(12, 34 ,56);

    EXPECT_EQ(lt_b0, lt_b1);
    EXPECT_LE(lt_b0, lt_b1);
    EXPECT_GE(lt_b0, lt_b1);
    EXPECT_FALSE(lt_b0 < lt_b1);
    EXPECT_FALSE(lt_b0 > lt_b1);
    
    LocalTime lesser[] =
    {
        { 12, 34, 55 },
        LocalTime(12,  33,  59),
        LocalTime(11,  45,  58),
        LocalTime::MIN
    };
    for(auto& e : lesser)
    {
        EXPECT_NE(e, lt_b0) << e.toString().c_str() << " :cmp: " << lt_b0.toString().c_str();
        EXPECT_LT(e, lt_b0) << e.toString().c_str() << " :cmp: " << lt_b0.toString().c_str();
        EXPECT_LE(e, lt_b0) << e.toString().c_str() << " :cmp: " << lt_b0.toString().c_str();
    }
      
    LocalTime greater[] =
    {
        LocalTime(12, 34,  57),
        LocalTime(12, 35,  0),
        { 13, 12, 23 },
        LocalTime::MAX
    };
    for(auto& e : greater)
    {
        EXPECT_NE(e, lt_b0) << e.toString().c_str() << " :cmp: " << lt_b0.toString().c_str();
        EXPECT_GT(e, lt_b0) << e.toString().c_str() << " :cmp: " << lt_b0.toString().c_str(); 
        EXPECT_GE(e, lt_b0)  << e.toString().c_str() << " :cmp: " << lt_b0.toString().c_str();
    }
}

TEST(LocalTime, Arithmetic)
{
    {
        LocalTime lt(12, 34, 56);
        auto lt2 = lt + 41103;
        EXPECT_TRUE(lt2.valid());
        EXPECT_EQ(23, lt2.hour());
        EXPECT_EQ(59, lt2.minute());
        EXPECT_EQ(59, lt2.second());
        EXPECT_EQ(lt2, LocalTime::MAX);

        lt2 = lt + -45296;
        EXPECT_TRUE(lt2.valid());
        EXPECT_EQ(0, lt2.hour());
        EXPECT_EQ(0, lt2.minute());
        EXPECT_EQ(0, lt2.second());
        EXPECT_EQ(lt2, LocalTime::MIN);

        lt2 = lt + 41104;
        EXPECT_EQ(lt2, LocalTime::MIN);
        lt2 = lt + -45297;
        EXPECT_EQ(lt2, LocalTime::MAX);
    }
}
