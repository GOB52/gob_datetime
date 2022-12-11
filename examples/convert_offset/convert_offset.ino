// for M5Stack/M5StackCore2
#include <M5Unified.h>
#include <WiFi.h>
#include <cstdio>
#include <gob_datetime.hpp>
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
    WiFi.begin(); // Connect to credential in hardware. (ESP32 saves the last Wifi connection)
    int tcount = 20;
    printf("%s", "Connect to WiFi:");
    while(tcount-- > 0 && WiFi.status() != WL_CONNECTED)
    {
        putchar('.');
        delay(500);
    }
    putchar('\n');

    if(WiFi.status() != WL_CONNECTED)
    {
        printf("*** Failed to connect WiFi ***\n");
    }

    // Configurate local time.
    const char* tzstr = locationToPOSIX(TIMEZONE_LOCATION);
    configTzTime((tzstr ? tzstr : ""), ntpURL[0], ntpURL[1], ntpURL[2]);
    struct tm tmp{};
    getLocalTime(&tmp); // Waiting for time synchronization.
    
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    auto p = goblib::datetime::locationToPOSIX("America/Los_Angeles"); // => "PST8PDT,M3.2.0,M11.1.0"
    setenv("TZ", p ? p : "", 1);
    tzset(); // Set timezone by POSIX tz string.
}

const ZoneOffset zo = ZoneOffset::of(+9); // +09:00

void print_time()
{
    OffsetDateTime odt = OffsetDateTime::now(); // Get current time and offset.
    OffsetDateTime odt2 = odt.withOffsetSameEpoch(zo); // Convert zone offset to +9:00
    printf("[%s] => [%s]\n", odt.toString().c_str(), odt2.toString().c_str());
}


void loop()
{
    print_time();
    delay(10 * 1000);
}
