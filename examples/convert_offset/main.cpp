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
    M5_LOGE("Connect to WiFi:");
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

    auto p = goblib::datetime::locationToPOSIX("America/Los_Angeles"); // => "PST8PDT,M3.2.0,M11.1.0"
    setenv("TZ", p ? p : "", 1);
    tzset(); // Set timezone by POSIX tz string.
}

const ZoneOffset zo = ZoneOffset::of(+9); // +09:00

void print_time()
{
    OffsetDateTime odt = OffsetDateTime::now(); // Get current time and offset.
    OffsetDateTime odt2 = odt.withOffsetSameEpoch(zo); // Convert zone offset to +9:00
    M5_LOGI("[%s] <%s>=> [%s] <%s>",
           odt.toString().c_str(), odt.toLocalDateTime().toString().c_str(),
           odt2.toString().c_str(), odt2.toLocalDateTime().toString().c_str());
}

void loop()
{
    print_time();
    delay(10 * 1000);
}
