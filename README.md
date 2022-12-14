# gob_datetime
[In English](README.en.md)  

C++11 以降向け Java JSR-310 のような日付時間クラス  
通常の C++ 環境や M5Stack にて使用できます。

## Java JSR-310 とは
Java の パッケージ [java.time](https://docs.oracle.com/javase/jp/18/docs/api/java.base/java/time/package-summary.html) は Java 8 より [JSR-310](https://jcp.org/en/jsr/detail?id=310) として追加となりました。  
日付、時刻、タイムゾーン等を非常に上手く扱っている洗練された設計になっている印象です。  
C++20 にて std::chrono に タイムゾーン関連が追加されますが、 JSR-310 の Local / Zoned / Offset DateTime 的な物がありません。  
そこで憚りながら自作を試みてみた次第です。

## 制限
まだ全ての java.time 関連クラスが網羅されていません。  
またクラスのメソッドも全てが実装されているわけではありません。詳細はドキュメントを生成の上確認してください。  

現時点では以下のクラスが用意されています。  
- LocalDate
- LocalTime
- OffsetTime
- ZoneOffset
- LocalDateTime
- OffsetDateTime

お使いの環境の **time_t** のサイズによる制限をうけます。当然ながら 32 bit なら [2038 年問題](https://ja.wikipedia.org/wiki/2038%E5%B9%B4%E5%95%8F%E9%A1%8C)が発生します。  
まだタイムゾーン関連がないので大した事はできませんが、順次追加していく予定です。  
但しタイムゾーンデータベースを参照できない環境下を想定しているので POSIX 形式タイムゾーンの制限を受ける事になると思います。(一つの設定が全ての時に適用される)


## インストール
それぞれの環境のライブラリの位置に git clone するか、 ダウンロードしたものを解凍してください。  
platformio.ini であれば以下の様に書けば自動でダウンロードしてくれます。

```ini
lib_deps = git@github.com:GOB52/gob_datetime.git
```

## 使い方
**gob_datetime.hpp** をインクルードしてください。  
namespace は **goblib::datetime** です。

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

## ドキュメント
[Doxygen](https://www.doxygen.nl/index.html) によるドキュメント生成が可能です。 [Doxyfile](doc/Doxyfile) を使用してください。  

[サンプルソース](examples)も理解の手助けになるかもしれません。


## ユニットテスト
[GoogleTest](https://google.github.io/googletest/) により実装されたユニットテストを [platformio.ini](platformio.ini) で実行することができます。(Native / Embedded)  
