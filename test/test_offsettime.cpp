#include <gtest/gtest.h>
#include <gob_datetime.hpp>
#include "helper.hpp"

using namespace goblib::datetime;

TEST(OffsetTime, Basic)
{
    {
        constexpr OffsetTime ot;
        ZoneOffset zo;
        EXPECT_TRUE(ot.valid());
        EXPECT_EQ(0,  ot.hour());
        EXPECT_EQ(12, ot.hour12());
        EXPECT_EQ(0,  ot.minute());
        EXPECT_EQ(0,  ot.second());
        EXPECT_TRUE(ot.isAM());
        EXPECT_FALSE(ot.isPM());
        EXPECT_EQ(zo, ot.offset());
        EXPECT_STREQ("00:00:00Z", ot.toString().c_str());
    }
    {
        constexpr LocalTime lt{12,34,56};
        constexpr ZoneOffset zo(-(3600 * 10 + 11 * 60 + 12));
        constexpr OffsetTime ot(lt, zo);
        constexpr auto hh = ot.hour();
        constexpr auto hh12 = ot.hour12();
        constexpr auto mm = ot.minute();
        constexpr auto ss = ot.second();
        constexpr auto am = ot.isAM();
        constexpr auto pm = ot.isPM();
        constexpr auto valid = ot.valid();
        EXPECT_TRUE(valid);
        EXPECT_EQ(12, hh);
        EXPECT_EQ(12, hh12);
        EXPECT_EQ(34, mm);
        EXPECT_EQ(56, ss);
        EXPECT_FALSE(am);
        EXPECT_TRUE(pm);
        EXPECT_EQ(zo, ot.offset());
        EXPECT_STREQ("12:34:56-10:11:12", ot.toString().c_str());
    }
    // Invalid 
    {
        struct LTZO { LocalTime lt; ZoneOffset zo; };
        LTZO tbl[] =
        {
            { { 12, 34, 56 }, ZoneOffset(-999999) },
            { { 45, 34, 56 }, ZoneOffset() },
            { { 45, 34, 56 }, ZoneOffset(999999) },
        };
        for(auto& e : tbl)
        {
            OffsetTime ot(e.lt, e.zo);
            EXPECT_FALSE(ot.valid()) << e.lt.toString().c_str() << " : " << e.zo.toString().c_str();
        }
    }
}

TEST(OffsetTime, InstanceMethods)
{
    // atDate
    {
        constexpr LocalTime lt{12,34,56};
        constexpr LocalDate ld(9, 8, 7);
        ZoneOffset zo = ZoneOffset::of(-10, -11, -12);
        const  OffsetTime ot(lt, zo);
        auto odt = ot.atDate(ld);
        EXPECT_EQ(9, odt.year());
        EXPECT_EQ(8, odt.month());
        EXPECT_EQ(7, odt.day());
        EXPECT_EQ(12, odt.hour());
        EXPECT_EQ(34, odt.minute());
        EXPECT_EQ(56, odt.second());
        EXPECT_EQ(zo, odt.offset());
    }
    // toString
    {
        struct LTZOS { LocalTime lt; int hh, mm, ss; const char* s; };
        LTZOS tbl[] =
        {
            { { 12, 34, 56 },  0,  0,  0, "12:34:56Z" },
            { {  9,  8,  7 },  9, 10, 11, "09:08:07+09:10:11" },
            { { 22, 33, 44 }, -1, -2, -3, "22:33:44-01:02:03" },
        };
        for(auto& e : tbl)
        {
            OffsetTime ot{e.lt, ZoneOffset::of(e.hh, e.mm, e.ss)};
            EXPECT_TRUE(ot.valid()) << e.s;
            EXPECT_STREQ(e.s, ot.toString().c_str()) << e.s;
        }
    }
    // withOffsetSameEpoch
    {
        {
            OffsetTime ot0{ {12, 34,56 }, ZoneOffset() }; // Z
            EXPECT_TRUE(ot0.valid()) << ot0.toString().c_str();

            ZoneOffset zoz;
            ZoneOffset zop = ZoneOffset::of( 3,  30,  45); // +03:30:45
            ZoneOffset zom = ZoneOffset::of(-3, -30, -45); // -03:30:45

            OffsetTime ot1 = ot0.withOffsetSameEpoch(zoz);
            EXPECT_EQ(ot0.toLocalTime(), ot1.toLocalTime());
            EXPECT_EQ(ot0.offset(), ot1.offset());

            OffsetTime ot2 = ot0.withOffsetSameEpoch(zop);
            EXPECT_EQ(LocalTime( 16, 5, 41), ot2.toLocalTime()) << ot2.toString().c_str();
            EXPECT_EQ(zop, ot2.offset());

            OffsetTime ot3 = ot0.withOffsetSameEpoch(zom);
            EXPECT_EQ(LocalTime( 9, 4, 11), ot3.toLocalTime()) << ot3.toString().c_str();
            EXPECT_EQ(zom, ot3.offset());
        }
        {
            ZoneOffset zo = ZoneOffset::of(9, 0, 0);
            OffsetTime ot0{ {12, 34, 56 }, zo };
            EXPECT_TRUE(ot0.valid()) << ot0.toString().c_str();

            ZoneOffset zoz;
            ZoneOffset zop = ZoneOffset::of( 3,  30,  45);
            ZoneOffset zom = ZoneOffset::of(-3, -30, -45);

            OffsetTime ot1 = ot0.withOffsetSameEpoch(zoz);
            EXPECT_EQ(LocalTime(3, 34, 56), ot1.toLocalTime());
            EXPECT_EQ(zoz, ot1.offset());
            EXPECT_STREQ("03:34:56Z", ot1.toString().c_str());

            OffsetTime ot2 = ot0.withOffsetSameEpoch(zop);
            EXPECT_EQ(LocalTime( 7, 5, 41), ot2.toLocalTime()) << ot2.toString().c_str();
            EXPECT_EQ(zop, ot2.offset());
            EXPECT_STREQ("07:05:41+03:30:45", ot2.toString().c_str());

            OffsetTime ot3 = ot0.withOffsetSameEpoch(zom);
            EXPECT_EQ(LocalTime( 0, 4, 11), ot3.toLocalTime()) << ot3.toString().c_str();
            EXPECT_EQ(zom, ot3.offset());
            EXPECT_STREQ("00:04:11-03:30:45", ot3.toString().c_str());
        }
    }
    // withOffsetSameLocal
    {
        LocalTime lt{12, 34, 56 };
        OffsetTime ot0{ lt,  ZoneOffset::of(-9, -30, 15) };
        auto ot1 = ot0.withOffsetSameLocal(ZoneOffset::MAX);
        EXPECT_EQ(lt, ot1.toLocalTime());
        EXPECT_EQ(ZoneOffset::MAX, ot1.offset());
        auto ot2 = ot0.withOffsetSameLocal(ZoneOffset::MIN);
        EXPECT_EQ(lt, ot2.toLocalTime());
        EXPECT_EQ(ZoneOffset::MIN, ot2.offset());
    }
}    

TEST(OffsetTime, StaticMethods)
{
    // now
    {
        // Asia/Tokyo +09:00 /+09:00 (No DST)
        {
            pushTimezone("Asia/Tokyo");
            // Standard only
            {
                injectMockClockDateTime(1971, 1, 19, 0, 0, 0);
                OffsetTime ot = OffsetTime::now();
                EXPECT_TRUE(ot.valid());
                EXPECT_EQ(LocalTime(0, 0, 0), ot.toLocalTime()) << ot.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(+9), ot.offset()) << ot.toString().c_str();
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
                OffsetTime ot = OffsetTime::now();
                EXPECT_TRUE(ot.valid());
                EXPECT_EQ(LocalTime(0, 0, 0), ot.toLocalTime()) << ot.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(-8), ot.offset()) << ot.toString().c_str();
                resetMockClock();
            }
            // DST 389561696
            {
                injectMockClockDateTime(1982, 5, 6, 12, 34, 56);
                OffsetTime ot = OffsetTime::now();
                EXPECT_TRUE(ot.valid());
                EXPECT_EQ(LocalTime(12, 34, 56), ot.toLocalTime()) << ot.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(-7), ot.offset()) << ot.toString().c_str();
                resetMockClock();
            }
            // Lost
            {
                injectMockClockDateTime(2020, 3, 8, 2, 30, 0);
                OffsetTime ot = OffsetTime::now();
                EXPECT_TRUE(ot.valid());
                EXPECT_EQ(LocalTime(3, 30, 0), ot.toLocalTime()) << ot.toString().c_str(); // slip to 3:30
                EXPECT_EQ(ZoneOffset::of(-7), ot.offset()) << ot.toString().c_str();
                resetMockClock();
            }
            // Ambiguous            
            {
                injectMockClockDateTime(2020, 11, 1, 1, 0, 0);
                OffsetTime ot = OffsetTime::now();
                EXPECT_TRUE(ot.valid());
                EXPECT_EQ(LocalTime(1, 0, 0), ot.toLocalTime()) << ot.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(-7), ot.offset()) << ot.toString().c_str(); // DST side
                resetMockClock();
            }    
            // 
            popTimezone();
        }
        // Australia/Lord_Howe +10:30 / +11:00 (DST)
        {
            pushTimezone("Australia/Lord_Howe");
            // Standard
            {
                injectMockClockDateTime(1993, 8, 9, 23, 45, 56);
                OffsetTime ot = OffsetTime::now();
                EXPECT_TRUE(ot.valid());
                EXPECT_EQ(LocalTime(23, 45, 56), ot.toLocalTime()) << ot.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(10, 30), ot.offset()) << ot.toString().c_str();
                resetMockClock();
            }
            // DST 1100264399
            {
                injectMockClockDateTime(2004, 11, 12, 23, 59, 59);
                OffsetTime ot = OffsetTime::now();
                EXPECT_TRUE(ot.valid());
                EXPECT_EQ(LocalTime(23, 59, 59), ot.toLocalTime()) << ot.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(11), ot.offset()) << ot.toString().c_str();
                resetMockClock();
            }
            // Lost
            {
                injectMockClockDateTime(2022, 10, 2, 2, 15, 0);
                OffsetTime ot = OffsetTime::now();
                EXPECT_TRUE(ot.valid());
                EXPECT_EQ(LocalTime(2, 45, 0), ot.toLocalTime()) << ot.toString().c_str(); // slip to 2:45
                EXPECT_EQ(ZoneOffset::of(11), ot.offset()) << ot.toString().c_str();
                resetMockClock();
            }
            // Ambiguous
            {
                injectMockClockDateTime(2022, 4, 3, 1, 45, 0);
                OffsetTime ot = OffsetTime::now();
                EXPECT_TRUE(ot.valid());
                EXPECT_EQ(LocalTime(1, 45, 0), ot.toLocalTime()) << ot.toString().c_str();
                EXPECT_EQ(ZoneOffset::of(11), ot.offset()) << ot.toString().c_str();  // DST side
                resetMockClock();
            }
            popTimezone();
        }
    }
    // of -> constructor
    // parse
    {
        // valid
        {
            const char* tbl[] = { "12:34:56Z", "01:02:03+04:05:06", "23:23:23-09:00" };
            for(auto& e : tbl)
            {
                OffsetTime ot = OffsetTime::parse(e);
                EXPECT_TRUE(ot.valid());
                EXPECT_STREQ(e, ot.toString().c_str()) << e;
            }
        }
        // invalid
        {
            const char* tbl[] = { "12:-4:56Z", "01:02:03+04:aa:06", "23:a3:23-09:00dk" };
            for(auto& e : tbl)
            {
                OffsetTime ot = OffsetTime::parse(e);
                EXPECT_FALSE(ot.valid()) << e;
            }
        }
    }
}

TEST(OffsetTime, Compare)
{
    ZoneOffset zo9p = ZoneOffset::of(+9);
    ZoneOffset zo9n = ZoneOffset::of(-9);
    LocalTime lt0{12,12,12};
    LocalTime lt1{21,12,12};
    LocalTime lt2{ 3,12,12};
    
    OffsetTime ot_se0 = OffsetTime(lt0, ZoneOffset::UTC); // 12:12:12:Z
    OffsetTime ot_se1 = OffsetTime(lt1, zo9p);            // 21:12:12:+09:00
    OffsetTime ot_se2 = OffsetTime(lt2, zo9n);            // 03:12:12:-09:00

    EXPECT_EQ(ot_se0, ot_se1);
    EXPECT_LE(ot_se0, ot_se1);
    EXPECT_GE(ot_se0, ot_se1);
    EXPECT_FALSE(ot_se0 < ot_se1);
    EXPECT_FALSE(ot_se0 > ot_se1);

    EXPECT_EQ(ot_se1, ot_se2);
    EXPECT_LE(ot_se1, ot_se2);
    EXPECT_GE(ot_se1, ot_se2);
    EXPECT_FALSE(ot_se1 < ot_se2);
    EXPECT_FALSE(ot_se1 > ot_se2);
    
    EXPECT_EQ(ot_se2, ot_se0);
    EXPECT_LE(ot_se2, ot_se0);
    EXPECT_GE(ot_se2, ot_se0);
    EXPECT_FALSE(ot_se2 < ot_se0);
    EXPECT_FALSE(ot_se2 > ot_se0);

    OffsetTime lesser[] =
    {
        { { 21, 12, 12 }, ZoneOffset::of(+9,0,1) },
        { { 12, 12, 11 }, ZoneOffset::UTC },
        OffsetTime::MIN
    };
    for(auto& e : lesser)
    {
        EXPECT_NE(e, ot_se0) << e.toString().c_str() << " :cmp: " << ot_se0.toString().c_str();
        EXPECT_LT(e, ot_se1) << e.toString().c_str() << " :cmp: " << ot_se1.toString().c_str();
        EXPECT_LE(e, ot_se2) << e.toString().c_str() << " :cmp: " << ot_se2.toString().c_str();
    }
    OffsetTime greater[] =
    {
        { { 21, 12, 12 }, ZoneOffset::of(+8,59,59) },
        { { 12, 12, 13 }, ZoneOffset::UTC },
        OffsetTime::MAX
    };
    for(auto& e : greater)
    {
        EXPECT_NE(e, ot_se0) << e.toString().c_str() << " :cmp: " << ot_se0.toString().c_str();
        EXPECT_GT(e, ot_se1) << e.toString().c_str() << " :cmp: " << ot_se1.toString().c_str();
        EXPECT_GE(e, ot_se2) << e.toString().c_str() << " :cmp: " << ot_se2.toString().c_str();
    }
}
