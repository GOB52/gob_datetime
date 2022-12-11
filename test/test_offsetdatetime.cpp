#include <gtest/gtest.h>
#include <gob_datetime.hpp>
#include "helper.hpp"

using namespace goblib::datetime;

TEST(OffsetDateTime, Basic)
{
    {
        constexpr OffsetDateTime odt;
        constexpr auto y = odt.year();
        constexpr auto m = odt.month();
        constexpr auto d = odt.day();
        constexpr auto dofw = odt.dayOfWeek();
        constexpr auto ily = odt.isLeapYear();
        constexpr auto hh = odt.hour();
        constexpr auto hh12 = odt.hour12();
        constexpr auto mm = odt.minute();
        constexpr auto ss = odt.second();
        constexpr auto am = odt.isAM();
        constexpr auto pm = odt.isPM();
        constexpr auto zo = odt.offset();
        EXPECT_TRUE(odt.valid());
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
        EXPECT_STREQ("1970-01-01T00:00:00Z", odt.toString().c_str());
        EXPECT_EQ(ZoneOffset::UTC, zo);
    }
    {
        constexpr LocalDate ld(1999, 12, 31);
        constexpr LocalTime lt(23, 59, 59);
        ZoneOffset zo = ZoneOffset::of(-10,-20, -30);
        const OffsetDateTime odt{ld, lt, zo };
        EXPECT_TRUE(odt.valid());
        EXPECT_EQ(ld, odt.toLocalDate());
        EXPECT_EQ(lt, odt.toLocalTime());
        EXPECT_EQ(zo, odt.offset());
    }
    {
        struct tm tmp{};
        tmp.tm_year = 2009 - 1900;
        tmp.tm_mon = 8 - 1;
        tmp.tm_mday = 7;;
        tmp.tm_hour = 12;
        tmp.tm_min = 34;
        tmp.tm_sec = 56;
        ZoneOffset zo = ZoneOffset::of(10,20,30);
        OffsetDateTime odt{tmp, zo};
        EXPECT_TRUE(odt.valid());
        EXPECT_EQ(2009, odt.year());
        EXPECT_EQ(8, odt.month());
        EXPECT_EQ(7, odt.day());
        EXPECT_EQ(DayOfWeek::Fri, odt.dayOfWeek());
        EXPECT_FALSE(odt.isLeapYear());
        EXPECT_EQ(12, odt.hour());
        EXPECT_EQ(34, odt.minute());
        EXPECT_EQ(56, odt.second());
        EXPECT_EQ(zo, odt.offset());        
    }
    {
        const char* tbl[] =
        {
            "1987-06-05T04:03:21Z",
            "2001-02-03T04:56:07+09:00",
            "2222-11-22T22:33:44-10:30:40",
        };
        for(auto& e : tbl)
        {
            OffsetDateTime odt{e};
            EXPECT_TRUE(odt.valid()) << odt.toString().c_str();
            EXPECT_STREQ(e, odt.toString().c_str()) << odt.toString().c_str();
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
            ZoneOffset zoTbl[] = { ZoneOffset::of(9) /* OK */, ZoneOffset::of(10, -10, 10) /* NG */ };
            for(auto& zo : zoTbl)
            {
                OffsetDateTime odt(LocalDate(e.y, e.m, e.d), LocalTime(e.hh, e.mm, e.ss), zo);
                EXPECT_FALSE(odt.valid()) << odt.toString().c_str() << " : " << zo.toString().c_str();
            }
        }
        const char* stbl[] =
        {
            "1987-06-05:04:03:02-100:20",   // Invalid offset
            "2000-02-10T44:01:02Z",         // Invalid time
            "2000-02-10T44:01:02anv",       // Invalid time, offset
            "2100-02-29T12:34:56-07:00",    // Invalid date
            "2004-13-06T07:08:08-19:30:10", // Invalid date, offset
            "1999-02-31T45:00:12+01:30",    // Invalid date, time
            "1987-11-31T03:-1:34-03:BC:AA", // Invalid all
        };
        for(auto& e : stbl)
        {
            OffsetDateTime odt(e);
            EXPECT_FALSE(odt.valid()) << e;
        }
    }
}

TEST(OffsetDateTime, InstanceMethods)
{
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
            auto odt = OffsetDateTime{ {e.y, e.m, e.d}, {e.hh, e.mm, e.ss}, e.zo};
            EXPECT_EQ(e.e, odt.toEpochSecond()) << odt.toString().c_str() << " : " << e.e;
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
            auto odt = OffsetDateTime::of(e.y, e.m, e.d, e.hh, e.mm, e.ss, e.zo);
            EXPECT_EQ(e.e, odt.toEpochSecond()) << odt.toString().c_str() << " : " << e.e;
        }
#endif
    }
    // toOffsetTime
    {
        OffsetDateTime odt0 { {1999,12,31}, {23,59,59}, ZoneOffset::MIN }; 
        auto ot = odt0.toOffsetTime();
        EXPECT_EQ(23, ot.hour());
        EXPECT_EQ(59, ot.minute());
        EXPECT_EQ(59, ot.second());
        EXPECT_EQ(ZoneOffset::MIN, ot.offset());
    }
    // toString
    {
        OffsetDateTime odt = OffsetDateTime::of(2345, 6, 7, 18, 9, 10, ZoneOffset::of(-12, -34, -56));
        EXPECT_STREQ("2345-06-07T18:09:10-12:34:56", odt.toString().c_str());
        EXPECT_STREQ("06/07/45-12:34:56", odt.toString("%D").c_str());
        //        EXPECT_STREQ("Thu Jun  7 18:09:10 2345-12:34:56", odt.toString("%c").c_str()); // depends on your locale.
        EXPECT_STREQ("06:09:10-12:34:56", odt.toString("%I:%M:%S").c_str());
        EXPECT_STREQ("18:09-12:34:56", odt.toString("%R").c_str());
        EXPECT_STREQ("06:09:10 PM-12:34:56", odt.toString("%r").c_str());
    }
    // withOffsetSameEpoch
    {
        {
            LocalDateTime ldt{ {2010, 9, 8}, {12, 34, 56 }};
            ZoneOffset zop = ZoneOffset::of( 2,  34,  56); // +02:34:56
            ZoneOffset zom = ZoneOffset::of(-2, -34, -56); // -02:34:56
            OffsetDateTime odt0(ldt, ZoneOffset::UTC);
        
            OffsetDateTime odt1 = odt0.withOffsetSameEpoch(ZoneOffset::UTC);
            EXPECT_EQ(odt0.toLocalDateTime(), odt1.toLocalDateTime());
            EXPECT_EQ(odt0.offset(), odt1.offset());
            EXPECT_EQ(odt0.toEpochSecond(), odt1.toEpochSecond());

            OffsetDateTime odt2 = odt0.withOffsetSameEpoch(zop);
            EXPECT_EQ(LocalDateTime({2010, 9, 8}, {15, 9, 52 }), odt2.toLocalDateTime()) << odt2.toString().c_str();
            EXPECT_EQ(zop, odt2.offset());
            EXPECT_EQ(odt0.toEpochSecond(), odt2.toEpochSecond());

            OffsetDateTime odt3 = odt0.withOffsetSameEpoch(zom);
            EXPECT_EQ(LocalDateTime({2010, 9, 8}, {10, 0, 0 }), odt3.toLocalDateTime()) << odt3.toString().c_str();
            EXPECT_EQ(zom, odt3.offset());
            EXPECT_EQ(odt0.toEpochSecond(), odt3.toEpochSecond());
        }
        {
            LocalDateTime ldt{ {2010, 9, 8}, {12, 34, 56 }};
            ZoneOffset zop = ZoneOffset::of( 13,  34,  56); // +13:34:56
            ZoneOffset zom = ZoneOffset::of(-13, -34, -56); // -13:34:56
            OffsetDateTime odt0(ldt, ZoneOffset::of(9));
        
            OffsetDateTime odt1 = odt0.withOffsetSameEpoch(ZoneOffset::of(9));
            EXPECT_EQ(odt0.toLocalDateTime(), odt1.toLocalDateTime());
            EXPECT_EQ(odt0.offset(), odt1.offset());
            EXPECT_EQ(odt0.toEpochSecond(), odt1.toEpochSecond());

            OffsetDateTime odt2 = odt0.withOffsetSameEpoch(zop);
            EXPECT_EQ(LocalDateTime({2010, 9, 8}, {17, 9, 52 }), odt2.toLocalDateTime()) << odt2.toString().c_str();
            EXPECT_EQ(zop, odt2.offset());
            EXPECT_EQ(odt0.toEpochSecond(), odt2.toEpochSecond());

            OffsetDateTime odt3 = odt0.withOffsetSameEpoch(zom);
            EXPECT_EQ(LocalDateTime({2010, 9, 7}, {14, 0, 0 }), odt3.toLocalDateTime()) << odt3.toString().c_str();
            EXPECT_EQ(zom, odt3.offset());
            EXPECT_EQ(odt0.toEpochSecond(), odt3.toEpochSecond());
        }
    }
    //  withOffsetSameLocal
    {
        LocalDateTime ldt{{1999,12,31}, {12, 34, 56 }};
        OffsetDateTime odt0{ ldt,  ZoneOffset::of(-9, -30, 15) };

        auto odt1 = odt0.withOffsetSameLocal(ZoneOffset::MAX);
        EXPECT_EQ(ldt, odt1.toLocalDateTime());
        EXPECT_EQ(ZoneOffset::MAX, odt1.offset());

        auto odt2 = odt0.withOffsetSameLocal(ZoneOffset::MIN);
        EXPECT_EQ(ldt, odt2.toLocalDateTime());
        EXPECT_EQ(ZoneOffset::MIN, odt2.offset());
    }
}

TEST(OffsetDateTime, StaticMethods)
{
    // now
    {
        // Asia/Tokyo +09:00 /+09:00 (No DST)
        {
            pushTimezone("Asia/Tokyo");
            // Standard only
            {
                injectMockClockDateTime(1971, 1, 19, 0, 0, 0);
                LocalDateTime ldt {{1971, 1, 19}, {0, 0, 0}};
                OffsetDateTime odt = OffsetDateTime::now();
                EXPECT_TRUE(odt.valid());
                EXPECT_EQ(ldt, odt.toLocalDateTime()) << odt.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(+9), odt.offset()) << odt.toString().c_str();
                resetMockClock();
            }
            popTimezone();
        }
        // America/Los_Angeles -08:00 / -07:00 (DST)
        {
            pushTimezone("America/Los_Angeles");
            // Standard
            {
                injectMockClockDateTime(1971, 1, 19, 0, 0, 0);
                LocalDateTime ldt {{1971, 1, 19}, {0, 0, 0}};
                OffsetDateTime odt = OffsetDateTime::now();
                EXPECT_TRUE(odt.valid());
                EXPECT_EQ(ldt, odt.toLocalDateTime()) << odt.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(-8), odt.offset()) << odt.toString().c_str();
                resetMockClock();
            }
            // DST 389561696
            {
                injectMockClockDateTime(1982, 5, 6, 12, 34, 56);
                LocalDateTime ldt {{1982, 5, 6}, {12, 34, 56}};
                OffsetDateTime odt = OffsetDateTime::now();
                EXPECT_TRUE(odt.valid());
                EXPECT_EQ(ldt, odt.toLocalDateTime()) << odt.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(-7), odt.offset()) << odt.toString().c_str();
                resetMockClock();
            }
            // Lost
            {
                injectMockClockDateTime(2020, 3, 8, 2, 30, 0);
                LocalDateTime ldt {{2020, 3, 8}, {3, 30, 0}}; // slip to 3:30
                OffsetDateTime odt = OffsetDateTime::now();
                EXPECT_TRUE(odt.valid());
                EXPECT_EQ(ldt, odt.toLocalDateTime()) << odt.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(-7), odt.offset()) << odt.toString().c_str();
                resetMockClock();
            }
            // Ambiguous
            {
                injectMockClockDateTime(2020, 11, 1, 1, 0, 0);
                LocalDateTime ldt {{2020, 11, 1}, {1, 0, 0}}; // DST
                OffsetDateTime odt = OffsetDateTime::now();
                EXPECT_TRUE(odt.valid());
                EXPECT_EQ(ldt, odt.toLocalDateTime()) << odt.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(-7), odt.offset()) << odt.toString().c_str(); // DST side
                resetMockClock();
            }
            popTimezone();
        }
        // Australia/Lord_Howe +10:30 / +11:00 (DST)
        {
            pushTimezone("Australia/Lord_Howe");
            // Standard
            {
                injectMockClockDateTime(1993, 8, 9, 23, 45, 56);
                LocalDateTime ldt {{1993, 8, 9}, {23, 45, 56}};
                OffsetDateTime odt = OffsetDateTime::now();
                EXPECT_TRUE(odt.valid());
                EXPECT_EQ(ldt, odt.toLocalDateTime()) << ldt.toString().c_str() << " : " << odt.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(10, 30), odt.offset()) << odt.toString().c_str();
                resetMockClock();
            }
            // DST 1100264399
            {
                injectMockClockDateTime(2004, 11, 12, 23, 59, 59);
                LocalDateTime ldt {{2004, 11, 12}, {23, 59, 59}};
                OffsetDateTime odt = OffsetDateTime::now();
                EXPECT_TRUE(odt.valid());
                EXPECT_EQ(ldt, odt.toLocalDateTime()) << odt.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(11), odt.offset()) << odt.toString().c_str();
                resetMockClock();
            }
            // Lost
            {
                injectMockClockDateTime(2022, 10, 2, 2, 15, 0);
                LocalDateTime ldt {{2022, 10, 2}, {2, 45, 0}}; // slip to 2:45
                OffsetDateTime odt = OffsetDateTime::now();
                EXPECT_TRUE(odt.valid());
                EXPECT_EQ(ldt, odt.toLocalDateTime()) << odt.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(11), odt.offset()) << odt.toString().c_str();
                resetMockClock();
            }
            // Ambiguous
            {
                injectMockClockDateTime(2022, 4, 3, 1, 45, 0);
                LocalDateTime ldt {{2022, 4, 3}, {1, 45, 0}};
                OffsetDateTime odt = OffsetDateTime::now();
                EXPECT_TRUE(odt.valid());
                EXPECT_EQ(ldt, odt.toLocalDateTime()) << odt.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(11), odt.offset()) << odt.toString().c_str(); // DST side
                resetMockClock();
            }
            popTimezone();
        }
    }
    // of -> constructor
    //  parse
    {
        // valid
        {
            const char* tbl[] = { "1977-10-07T12:34:56Z", "2011-02-20T01:02:03+04:05:06", "1999-01-07T23:23:23-09:00" };
            for(auto& e : tbl)
            {
                OffsetDateTime odt = OffsetDateTime::parse(e);
                EXPECT_TRUE(odt.valid());
                EXPECT_STREQ(e, odt.toString().c_str()) << e;
            }
        }
        // invalid
        {
            const char* tbl[] = { "1982-02-03T10:31:51h", "1982-02-03T10:3151+12:34:56", "1999-3-42T10:31:51-09:00" };
            for(auto& e : tbl)
            {
                OffsetDateTime odt = OffsetDateTime::parse(e);
                EXPECT_FALSE(odt.valid()) << e;
            }
        }
    }
}

