// for M5Stack/M5StackCore2
#include <M5Unified.h>
#include <WiFi.h>
#include <cstdio>
#include <gob_datetime.hpp>
#include <esp_sntp.h>

using namespace goblib::datetime;

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
//
}
void setup()
{
    M5.begin();
    M5.Display.clear(TFT_DARKGREY);
    WiFi.begin(); // Connect to credential in hardware. (ESP32 saves the last Wifi connection)

    int tcount = 20;
    M5_LOGI("Connect to WiFi:");
    while(tcount-- > 0 && WiFi.status() != WL_CONNECTED)
    {
        M5_LOGI(".");
        delay(500);
    }
    M5.Display.clear(TFT_DARKGREEN);
    if(WiFi.status() != WL_CONNECTED)
    {
        M5_LOGI("*** Failed to connect WiFi ***\n");
        M5.Display.clear(TFT_RED);
    }

    // Configurate local time.
    const char* tzstr = locationToPOSIX(TIMEZONE_LOCATION);
    configTzTime((tzstr ? tzstr : ""), ntpURL[0], ntpURL[1], ntpURL[2]);

    // Waiting for time synchronization.
    int32_t retry{10};    
    sntp_sync_status_t st;
    while( ((st = sntp_get_sync_status()) == SNTP_SYNC_STATUS_RESET) && --retry >= 0)
    {
        M5_LOGI("   ...sync in progress");
        delay(1000);
    }
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    struct tm tmp{};
    if(!getLocalTime(&tmp))
    {
        M5_LOGE("Failed to configTime");
        delay(1000);
        abort();
    }

    //
    auto p = goblib::datetime::locationToPOSIX("America/Los_Angeles"); // => "PST8PDT,M3.2.0,M11.1.0"
    setenv("TZ", p ? p : "", 1);
    tzset(); // Set timezone by POSIX tz string.

    OffsetDateTime odt = OffsetDateTime::parse("2010-09-08T12:34:56+09:00");
    LocalDate ld = odt.toLocalDate();
    LocalTime lt = odt.toLocalTime();
    ZoneOffset zo = odt.offset();
    M5_LOGI("=> %s %s %s", ld.toString().c_str(), lt.toString().c_str(), zo.toString().c_str()); // 2010-09-08 12:34:56 +09:00

    LocalDateTime ldt = LocalDateTime::now();
    M5_LOGI("-> %d/%d/%d", ldt.year(), ldt.month(), ldt.day()); // e.g. 2022/12/12
}
    
void loop()
{
    delay(10 * 1000);
}
