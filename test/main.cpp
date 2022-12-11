/*
  main for native test.
*/
#include <gtest/gtest.h>
#include <gob_datetime.hpp>
#include "helper.hpp"

// C++ version
#if   __cplusplus >= 202002L
# pragma message "C++20 or later"
#elif __cplusplus >= 201703L
# pragma message "C++17 or later"
#elif __cplusplus >= 201402L
# pragma message "C++14 or later"
#elif __cplusplus >= 201103L
# pragma message "C++11 or later"
#else
# error "Need C++11 or later"
# endif
// Compiler
#if defined(__clang__)
# pragma message "Clang"
#elif defined(_MSC_VER)
# pragma message "MSVC"
#elif defined(__BORLANDC__)
# pragma message "BORANDC"
#elif defined(__MINGW32__) || defined(__MINGW64__)
# pragma message "MINGW"
#elif defined(__INTEL_COMPILER)
# pragma message "ICC"
#elif defined(__GNUG__)
# pragma message "GCC"
#else
# pragma message "Unknown compiler"
#endif
// tume_t
#ifdef GOBLIB_DATETIME_USE_TIME_T_32BIT
# pragma message "time_t is 32bit"
#else
# pragma message "time_t is GREATER than 32bit"
#endif

#ifndef TIMEZONE_LOCATION
#define TIMEZONE_LOCATION "Asia/Tokyo"
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result" 

__attribute__((weak)) int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    #ifdef GTEST_FILTER
    ::testing::GTEST_FLAG(filter) = GTEST_FILTER;
    #endif

    const char* tzstr = goblib::datetime::locationToPOSIX(TIMEZONE_LOCATION);
    setenv("TZ", tzstr ? tzstr : "", 1);
    tzset();
    
    RUN_ALL_TESTS();
    // Always return zero-code and allow PlatformIO to parse results
    return 0;
}

#pragma GCC diagnostic pop
