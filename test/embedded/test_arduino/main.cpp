/*
  main for embedeed test (arduino)
*/
#include <gtest/gtest.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <gob_datetime.hpp>

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
const char* ntpURL[] =
{
    "ntp.nict.jp",
    "ntp.jst.mfeed.ad.jp",
    "time.cloudflare.com",
};

void test()
{
    M5.Lcd.fillRect( 0,0,32,240, RUN_ALL_TESTS() ? TFT_RED : TFT_GREEN );
}
//
}
void setup()
{
    M5.begin();

    printf("%s", "Connect to WiFi:");
    
    WiFi.begin(); // Connect to credential in hardware. (ESP32 saves the last Wifi connection)
    int tcount = 20;

    while(tcount-- > 0 && WiFi.status() != WL_CONNECTED)
    {
        putchar('.');
        delay(500);
    }
    putchar('\n');

    if(WiFi.status() != WL_CONNECTED)
    {
        M5.Lcd.fillScreen(TFT_LIGHTGREY);
        printf("*** Failed to connect WiFi ***\n");
    }
    else
    {
        M5.Lcd.fillScreen(TFT_ORANGE);
    }
    
    // Configurate local time.
    const char* tzstr = goblib::datetime::locationToPOSIX(TIMEZONE_LOCATION);
    configTzTime((tzstr ? tzstr : "UTC0"), ntpURL[0], ntpURL[1], ntpURL[2]);
    struct tm tmp{};
    getLocalTime(&tmp); // Waiting for time synchronization.
    
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    printf("TZ:%s\n", tzstr ? tzstr : "");
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
