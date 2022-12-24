/*
  main for embedeed test (arduino)
*/
#include <gtest/gtest.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <gob_datetime.hpp>
#include <esp_random.h> // esp_random() is hardware RNG. (No random seed initialization is required)
#include <algorithm>

#if __has_include (<esp_idf_version.h>)
#include <esp_idf_version.h>
#else // esp_idf_version.h has been introduced in Arduino 1.0.5 (ESP-IDF3.3)
#define ESP_IDF_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))
#define ESP_IDF_VERSION ESP_IDF_VERSION_VAL(3,2,0)
#endif

#ifndef TIMEZONE_LOCATION
#define TIMEZONE_LOCATION "Asia/Tokyo"
#endif

namespace
{
PROGMEM const char ntp0[] = "ntp.nict.jp";
PROGMEM const char ntp1[] = "ntp.jst.mfeed.ad.jp";
PROGMEM const char ntp2[] = "time.cloudflare.com";
const char* ntpURLTable[] = { ntp0, ntp1, ntp2 }; // DON'T USE PROGMEM! (because it will be shuffled later)

struct ESP32RNG
{
    using result_type = uint32_t;
    static result_type min() { return 0; }
    static result_type max() { return sizeof(ntpURLTable) / sizeof(ntpURLTable[0]); }
    result_type  operator()() { return esp_random() % max(); }
};

void configTime(const char* tzstr)
{
    Serial.println("Connect WiFi");
    WiFi.begin(); // Connect to credential in Hardware. (ESP32 saves the last WiFi connection)
    int tcount = 20;
    while(tcount-- > 0 && WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
    if(WiFi.status() != WL_CONNECTED)
    {
        M5.Lcd.fillScreen(TFT_LIGHTGREY);
        Serial.println("Failed to connect");
        return;
    }
    M5.Lcd.fillScreen(TFT_ORANGE);

    std::shuffle(std::begin(ntpURLTable), std::end(ntpURLTable), ESP32RNG());
    auto ptz = goblib::datetime::locationToPOSIX(tzstr);

    Serial.println(ptz ? ptz : "NONE");
    Serial.println(ntpURLTable[0]);
    
    configTzTime(ptz ? ptz : "", ntpURLTable[0], ntpURLTable[1], ntpURLTable[2]);
    // waiting for time synchronization
    {
        std::tm discard{};
        getLocalTime(&discard, 10 * 1000);
    }
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

void test()
{
    M5.Lcd.fillRect( 0,0,32,240, RUN_ALL_TESTS() ? TFT_RED : TFT_GREEN );
}
//
}
void setup()
{
    M5.begin();

    // Configurate local time.
    configTime(TIMEZONE_LOCATION);

    printf("CPP %ld\n", __cplusplus);
    printf("ESP-IDF Version %d.%d.%d\n",
           (ESP_IDF_VERSION>>16) & 0xFF, (ESP_IDF_VERSION>>8)&0xFF, ESP_IDF_VERSION & 0xFF);

    ::testing::InitGoogleTest();

#ifdef GTEST_FILTER
    ::testing::GTEST_FLAG(filter) = GTEST_FILTER;
#endif
}
    
void loop()
{
    test();
    delay(10 * 1000);
}
