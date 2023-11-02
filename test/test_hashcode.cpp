/*
  Test for hash
  Note: It takes a long time (several minutes), so if you want to prioritize other tests, you can filter them in main.cpp.
 */
#include <gtest/gtest.h>
#include <gob_datetime.hpp>
#include "helper.hpp"
#include <algorithm>

using namespace goblib::datetime;

// Not enough memory on M5Stack.
#if !defined(ARDUINO)

TEST(Hash, LocalDate)
{
    std::vector<uint32_t> v;
    for(int y = 1970;y <= 2038; ++y)
    {
        for(int m=1;m<=12;++m)
        {
            for(int d=1;d<=31;++d)
            {
                v.push_back(LocalDate(y, m, d).hash());
            }
        }
    }
    auto it = std::unique(v.begin(), v.end());
    EXPECT_TRUE(it == v.end()); // Unique?
}

TEST(Hash, LocalTime)
{
    std::vector<uint32_t> v;
    for(int h = 0;h < 24; ++h)
    {
        for(int m=0;m<60;++m)
        {
            for(int s=0;s < 60; ++s)
            {
                v.push_back(LocalTime(h, m, s).hash());
            }
        }
    }
    auto it = std::unique(v.begin(), v.end());
    EXPECT_TRUE(it == v.end()); // Unique?
}

TEST(Hash, LocalDateTime)
{
    // hash
    std::vector<uint32_t> v;
    for(int y=1970;y <= 2038; ++y)
    {
        for(int m=1;m<=12;++m)
        {
            for(int d=1;d<=31;++d)
            {
                for(int hh=0;hh<24;++hh)
                {
                    for(int mm=0;mm<60;++mm)
                    {
                        for(int ss=0;ss<60;++ss)
                        {
                            v.push_back(LocalDateTime(y,m,d, hh, mm, ss).hash());
                        }
                    }
                }
            }
        }
    }
    auto it = std::unique(v.begin(), v.end());
    EXPECT_TRUE(it == v.end()); // Unique?
}    

// ZoneOffset hash() same as totalSeconds(), then skip test.
// OffsetTime hash() hash() make by LocalTime.hash() ^ ZoneOffset.hash(), then skip test.
// OffsetDateTime hash() make by LocalDatetTime().hash() ^ ZoneOffset.hash(), then skip test.

#endif
