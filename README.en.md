# gob_datetime
[In Japanese](README.md)  

date-time classes like Java JSR-310 for C++11 or later.  
It can be used in native and embedded (M5Stack) environments.


## What is the JSR-310?
[JSR-310](https://jcp.org/en/jsr/detail?id=310) is a package for handling date time time zone implemented in [java.time](https://docs.oracle.com/en/java/javase/19/docs/api/java.base/java/time/package-summary.html) since Java 8.  
It is a very sophisticated design, so I tried to imitate it.

## Restrictions
Not all java.time related classes are covered yet. Not all methods are implemented either.

Classes currently implemented
- LocalDate
- LocalTime
- OffsetTime
- ZoneOffset
- LocalDateTime
- OffsetDateTime

Limited by the size of **time_t** in the environment in which it is used. If time_t is 32-bit, [the year 2038 problem](https://en.wikipedia.org/wiki/Year_2038_problem) occurs.  
I Will implement other classes of java.time, such as ZonedDateTime.

## How to install
git clone or extract zip file to your library folder.  
If you are using PlatformIO, you can write the following to download automatically.

```ini
lib_deps = https://github.com/GOB52/gob_datetime
```

## How to use
Please include **gob_datetime.hpp**.  
namespace is **goblib::datetime**.

```cpp
#include <cstdio>
#include <gob_datetime.hpp>
using goblib::datetime::LocalDate;
using goblib::datetime::LocalTime;
using goblib::datetime::LocalDateTime;
using goblib::datetime::OffsetDateTime;
using goblib::datetime::ZoneOffset;

int main()
{
	auto p = goblib::datetime::locationToPOSIX("America/Los_Angeles"); // => "PST8PDT,M3.2.0,M11.1.0"
	setenv("TZ", p ? p : "", 1);
	tzset(); // Set timezone by POSIX tz string.

	OffsetDateTime odt = OffsetDateTime::parse("2010-09-08T12:34:56+09:00");
	LocalDate ld = odt.toLocalDate();
	LocalTime lt = odt.toLocalTime();
	ZoneOffset zo = odt.offset();
	printf("%s %s %s\n", ld.toString().c_str(), lt.toString().c_str(), zo.toString().c_str()); // 2010-09-08 12:34:56 +09:00

	LocalDateTime ldt = LocalDateTime::now();
	printf("%d/%d/%d\n", ldt.year(), ldt.month(), ldt.day()); // e.g. 2022/12/12

	return 0;
}
```

## Documents
You can make documents by [Doxygen](https://www.doxygen.nl/index.html). Use my [Doxyfile](doc/Doxyfile).  
[Sample sources](examples) will also help you understand.


## UnitTest
You can run [GoogleTest](https://google.github.io/googletest/) using [platformio.ini](platformio.ini).

