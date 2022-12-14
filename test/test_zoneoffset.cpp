#include <gtest/gtest.h>
#include <gob_datetime.hpp>
#include "helper.hpp"

using namespace goblib::datetime;

TEST(ZoneOffset, Basic)
{
    {
        constexpr ZoneOffset zo;
        constexpr auto th = zo.totalHours();
        constexpr auto tm = zo.totalMinutes();
        constexpr auto ts = zo.totalSeconds();
        constexpr auto hh = zo.hour();
        constexpr auto mm = zo.minute();
        constexpr auto ss = zo.second();
        constexpr auto valid = zo.valid();
        EXPECT_TRUE(valid);
        EXPECT_FLOAT_EQ(0.0f, th);
        EXPECT_FLOAT_EQ(0.0f, tm);
        EXPECT_EQ(0, ts);
        EXPECT_EQ(0, hh);
        EXPECT_EQ(0, mm);
        EXPECT_EQ(0, ss);
    }
    {
        constexpr ZoneOffset zo( 9 * 3600 + 30 * 60 + 12);
        constexpr auto th = zo.totalHours();
        constexpr auto tm = zo.totalMinutes();
        constexpr auto ts = zo.totalSeconds();
        constexpr auto hh = zo.hour();
        constexpr auto mm = zo.minute();
        constexpr auto ss = zo.second();
        constexpr auto valid = zo.valid();
        EXPECT_TRUE(valid);
        EXPECT_FLOAT_EQ(9.503333f, th);
        EXPECT_FLOAT_EQ(570.2f, tm);
        EXPECT_EQ(34212, ts);
        EXPECT_EQ(9, hh);
        EXPECT_EQ(30, mm);
        EXPECT_EQ(12, ss);
    }
    {
        constexpr ZoneOffset zo{ -(12 * 3600 + 3 * 60 + 45) };
        constexpr auto th = zo.totalHours();
        constexpr auto tm = zo.totalMinutes();
        constexpr auto ts = zo.totalSeconds();
        constexpr auto hh = zo.hour();
        constexpr auto mm = zo.minute();
        constexpr auto ss = zo.second();
        constexpr auto valid = zo.valid();
        EXPECT_TRUE(valid);
        EXPECT_FLOAT_EQ(-12.0625f, th);
        EXPECT_FLOAT_EQ(-723.75f, tm);
        EXPECT_EQ(-43425, ts);
        EXPECT_EQ(-12, hh);
        EXPECT_EQ(-3, mm);
        EXPECT_EQ(-45, ss);
    }
    // Invalid ZoneOffset
    {
        ZoneOffset ng0(ZoneOffset::MIN.totalSeconds() -1);
        EXPECT_FALSE(ng0.valid());
        ZoneOffset ng1(ZoneOffset::MAX.totalSeconds() +1);
        EXPECT_FALSE(ng1.valid());
    }
}

TEST(ZoneOffset, InstanceMethods)
{
    // toString
    {
        struct HMSS{ int h,m,s; const char* str; };
        HMSS tbl[] =
        {
            {   0,   0,   0, "Z" },
            //
            {   1,  30,  50, "+01:30:50" },

            {   0,  45,   1, "+00:45:01"},
            {   0,  30,   0, "+00:30"},
            {   0,   0,  45, "+00:00:45" },
            //
            { -16, -54, -32, "-16:54:32" },
            { -16, -54,   0, "-16:54" },
            {  0, -54,  -32, "-00:54:32" },
            {  0, -54,    0, "-00:54" },
            {  0,   0, -45,  "-00:00:45" },
        };
        for(auto& e : tbl)
        {
            ZoneOffset zo = ZoneOffset::of(e.h, e.m, e.s);
            EXPECT_TRUE(zo.valid()) << e.str;
            EXPECT_STREQ(e.str, zo.toString().c_str()) << e.str;
        }
    }
}

TEST(ZoneOffset, StaticMethods)
{
    // of
    {
        struct HMSS{ int h,m,s; const char* str; };
        HMSS tbl[] = // valid
        {
            {   0,   0,   0, "Z" },
            //
            {   1,  30,  50, "+01:30:50" },
            {   1,  30,   0, "+01:30" },
            {   0,  45,   1, "+00:45:01"},
            {   0,  30,   0, "+00:30"},
            {   0,   0,  45, "+00:00:45" },
            //
            { -16, -54, -32, "-16:54:32" },
            { -16, -54,   0, "-16:54" },
            {  0, -54,  -32, "-00:54:32" },
            {  0, -54,    0, "-00:54" },
            {  0,   0, -45,  "-00:00:45" },
        };
        for(auto& e : tbl)
        {
            ZoneOffset zo0 = ZoneOffset::of(e.str);
            EXPECT_TRUE(zo0.valid()) << e.str;
            EXPECT_EQ(e.h, zo0.hour()) << e.str;
            EXPECT_EQ(e.m, zo0.minute()) << e.str;
            EXPECT_EQ(e.s, zo0.second()) << e.str;

            ZoneOffset zo1 = ZoneOffset::of(e.h, e.m, e.s);
            EXPECT_TRUE(zo1.valid()) << e.str;
            EXPECT_STREQ(e.str, zo1.toString().c_str());
        }
        HMSS tbl2[] = // Invalid
        {
            {  18,   0,   1, "+18::0001" },
            { -18,   0,  -1, "-18::0001" },
            {  12,  34, -56, "+12:34:-56" },
            {  12, -34,  56, "+12:-34:56" },
            {  12, -34, -56, "+12:-34:-56" },
            { -12,  34,  56, "abcdefg" },
            { -12,  34, -56, "-12:34:-56" },
            { -12, -34,  56, "-12:-34:56" },
        };
        for(auto& e : tbl2)
        {
            ZoneOffset zo0 = ZoneOffset::of(e.str);
            EXPECT_FALSE(zo0.valid()) << e.str;
            ZoneOffset zo1 = ZoneOffset::of(e.h, e.m, e.s);
            EXPECT_FALSE(zo1.valid()) << e.str;
        }
    }
    // ofTotalSeconds
    {
        int32_t tbl[] = { 0, 1, -1, 1234, -1234, 23456, -23456, 64800, -64800 };
        for(auto& e : tbl)
        {
            ZoneOffset zo = ZoneOffset::ofTotalSeconds(e);
            EXPECT_EQ(e, zo.totalSeconds()) << e;
        }
    }
}

TEST(ZoneOffset, Compare)
{
    ZoneOffset zo_b0(12345);
    ZoneOffset zo_b1(12345);

    EXPECT_EQ(zo_b0, zo_b1);
    EXPECT_LE(zo_b0, zo_b1);
    EXPECT_GE(zo_b0, zo_b1);
    EXPECT_FALSE(zo_b0 < zo_b1);
    EXPECT_FALSE(zo_b0 > zo_b1);
    
    ZoneOffset lesser[] =
    {
        ZoneOffset{12344},
        ZoneOffset(0),
        ZoneOffset{-3456},
        ZoneOffset::MIN,
    };
    for(auto& e : lesser)
    {
        EXPECT_NE(e, zo_b0) << e.toString().c_str() << " :cmp: " << zo_b0.toString().c_str();
        EXPECT_LT(e, zo_b0) << e.toString().c_str() << " :cmp: " << zo_b0.toString().c_str();
        EXPECT_LE(e, zo_b0) << e.toString().c_str() << " :cmp: " << zo_b0.toString().c_str();
    }
      
    ZoneOffset greater[] =
    {
        ZoneOffset{12346},
        ZoneOffset(23456),
        ZoneOffset::MAX,
    };
    for(auto& e : greater)
    {
        EXPECT_NE(e, zo_b0) << e.toString().c_str() << " :cmp: " << zo_b0.toString().c_str();
        EXPECT_GT(e, zo_b0) << e.toString().c_str() << " :cmp: " << zo_b0.toString().c_str(); 
        EXPECT_GE(e, zo_b0) << e.toString().c_str() << " :cmp: " << zo_b0.toString().c_str();
    }
}
