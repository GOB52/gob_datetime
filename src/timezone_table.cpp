
#ifdef ARDUINO
#include <Arduino.h>
#endif
#include "gob_datetime.hpp"
#include <map>
#include <cstring>

#ifndef PROGMEM
#define PROGMEM
#endif

namespace
{

PROGMEM const char loc_000[] = "Africa/Abidjan";
PROGMEM const char loc_001[] = "Africa/Accra";
PROGMEM const char loc_002[] = "Africa/Addis_Ababa";
PROGMEM const char loc_003[] = "Africa/Algiers";
PROGMEM const char loc_004[] = "Africa/Asmara";
PROGMEM const char loc_005[] = "Africa/Bamako";
PROGMEM const char loc_006[] = "Africa/Bangui";
PROGMEM const char loc_007[] = "Africa/Banjul";
PROGMEM const char loc_008[] = "Africa/Bissau";
PROGMEM const char loc_009[] = "Africa/Blantyre";
PROGMEM const char loc_010[] = "Africa/Brazzaville";
PROGMEM const char loc_011[] = "Africa/Bujumbura";
PROGMEM const char loc_012[] = "Africa/Cairo";
PROGMEM const char loc_013[] = "Africa/Casablanca";
PROGMEM const char loc_014[] = "Africa/Ceuta";
PROGMEM const char loc_015[] = "Africa/Conakry";
PROGMEM const char loc_016[] = "Africa/Dakar";
PROGMEM const char loc_017[] = "Africa/Dar_es_Salaam";
PROGMEM const char loc_018[] = "Africa/Djibouti";
PROGMEM const char loc_019[] = "Africa/Douala";
PROGMEM const char loc_020[] = "Africa/El_Aaiun";
PROGMEM const char loc_021[] = "Africa/Freetown";
PROGMEM const char loc_022[] = "Africa/Gaborone";
PROGMEM const char loc_023[] = "Africa/Harare";
PROGMEM const char loc_024[] = "Africa/Johannesburg";
PROGMEM const char loc_025[] = "Africa/Juba";
PROGMEM const char loc_026[] = "Africa/Kampala";
PROGMEM const char loc_027[] = "Africa/Khartoum";
PROGMEM const char loc_028[] = "Africa/Kigali";
PROGMEM const char loc_029[] = "Africa/Kinshasa";
PROGMEM const char loc_030[] = "Africa/Lagos";
PROGMEM const char loc_031[] = "Africa/Libreville";
PROGMEM const char loc_032[] = "Africa/Lome";
PROGMEM const char loc_033[] = "Africa/Luanda";
PROGMEM const char loc_034[] = "Africa/Lubumbashi";
PROGMEM const char loc_035[] = "Africa/Lusaka";
PROGMEM const char loc_036[] = "Africa/Malabo";
PROGMEM const char loc_037[] = "Africa/Maputo";
PROGMEM const char loc_038[] = "Africa/Maseru";
PROGMEM const char loc_039[] = "Africa/Mbabane";
PROGMEM const char loc_040[] = "Africa/Mogadishu";
PROGMEM const char loc_041[] = "Africa/Monrovia";
PROGMEM const char loc_042[] = "Africa/Nairobi";
PROGMEM const char loc_043[] = "Africa/Ndjamena";
PROGMEM const char loc_044[] = "Africa/Niamey";
PROGMEM const char loc_045[] = "Africa/Nouakchott";
PROGMEM const char loc_046[] = "Africa/Ouagadougou";
PROGMEM const char loc_047[] = "Africa/Porto-Novo";
PROGMEM const char loc_048[] = "Africa/Sao_Tome";
PROGMEM const char loc_049[] = "Africa/Tripoli";
PROGMEM const char loc_050[] = "Africa/Tunis";
PROGMEM const char loc_051[] = "Africa/Windhoek";
PROGMEM const char loc_052[] = "America/Adak";
PROGMEM const char loc_053[] = "America/Anchorage";
PROGMEM const char loc_054[] = "America/Anguilla";
PROGMEM const char loc_055[] = "America/Antigua";
PROGMEM const char loc_056[] = "America/Araguaina";
PROGMEM const char loc_057[] = "America/Argentina/Buenos_Aires";
PROGMEM const char loc_058[] = "America/Argentina/Catamarca";
PROGMEM const char loc_059[] = "America/Argentina/Cordoba";
PROGMEM const char loc_060[] = "America/Argentina/Jujuy";
PROGMEM const char loc_061[] = "America/Argentina/La_Rioja";
PROGMEM const char loc_062[] = "America/Argentina/Mendoza";
PROGMEM const char loc_063[] = "America/Argentina/Rio_Gallegos";
PROGMEM const char loc_064[] = "America/Argentina/Salta";
PROGMEM const char loc_065[] = "America/Argentina/San_Juan";
PROGMEM const char loc_066[] = "America/Argentina/San_Luis";
PROGMEM const char loc_067[] = "America/Argentina/Tucuman";
PROGMEM const char loc_068[] = "America/Argentina/Ushuaia";
PROGMEM const char loc_069[] = "America/Aruba";
PROGMEM const char loc_070[] = "America/Asuncion";
PROGMEM const char loc_071[] = "America/Atikokan";
PROGMEM const char loc_072[] = "America/Bahia";
PROGMEM const char loc_073[] = "America/Bahia_Banderas";
PROGMEM const char loc_074[] = "America/Barbados";
PROGMEM const char loc_075[] = "America/Belem";
PROGMEM const char loc_076[] = "America/Belize";
PROGMEM const char loc_077[] = "America/Blanc-Sablon";
PROGMEM const char loc_078[] = "America/Boa_Vista";
PROGMEM const char loc_079[] = "America/Bogota";
PROGMEM const char loc_080[] = "America/Boise";
PROGMEM const char loc_081[] = "America/Cambridge_Bay";
PROGMEM const char loc_082[] = "America/Campo_Grande";
PROGMEM const char loc_083[] = "America/Cancun";
PROGMEM const char loc_084[] = "America/Caracas";
PROGMEM const char loc_085[] = "America/Cayenne";
PROGMEM const char loc_086[] = "America/Cayman";
PROGMEM const char loc_087[] = "America/Chicago";
PROGMEM const char loc_088[] = "America/Chihuahua";
PROGMEM const char loc_089[] = "America/Costa_Rica";
PROGMEM const char loc_090[] = "America/Creston";
PROGMEM const char loc_091[] = "America/Cuiaba";
PROGMEM const char loc_092[] = "America/Curacao";
PROGMEM const char loc_093[] = "America/Danmarkshavn";
PROGMEM const char loc_094[] = "America/Dawson";
PROGMEM const char loc_095[] = "America/Dawson_Creek";
PROGMEM const char loc_096[] = "America/Denver";
PROGMEM const char loc_097[] = "America/Detroit";
PROGMEM const char loc_098[] = "America/Dominica";
PROGMEM const char loc_099[] = "America/Edmonton";
PROGMEM const char loc_100[] = "America/Eirunepe";
PROGMEM const char loc_101[] = "America/El_Salvador";
PROGMEM const char loc_102[] = "America/Fortaleza";
PROGMEM const char loc_103[] = "America/Fort_Nelson";
PROGMEM const char loc_104[] = "America/Glace_Bay";
PROGMEM const char loc_105[] = "America/Godthab";
PROGMEM const char loc_106[] = "America/Goose_Bay";
PROGMEM const char loc_107[] = "America/Grand_Turk";
PROGMEM const char loc_108[] = "America/Grenada";
PROGMEM const char loc_109[] = "America/Guadeloupe";
PROGMEM const char loc_110[] = "America/Guatemala";
PROGMEM const char loc_111[] = "America/Guayaquil";
PROGMEM const char loc_112[] = "America/Guyana";
PROGMEM const char loc_113[] = "America/Halifax";
PROGMEM const char loc_114[] = "America/Havana";
PROGMEM const char loc_115[] = "America/Hermosillo";
PROGMEM const char loc_116[] = "America/Indiana/Indianapolis";
PROGMEM const char loc_117[] = "America/Indiana/Knox";
PROGMEM const char loc_118[] = "America/Indiana/Marengo";
PROGMEM const char loc_119[] = "America/Indiana/Petersburg";
PROGMEM const char loc_120[] = "America/Indiana/Tell_City";
PROGMEM const char loc_121[] = "America/Indiana/Vevay";
PROGMEM const char loc_122[] = "America/Indiana/Vincennes";
PROGMEM const char loc_123[] = "America/Indiana/Winamac";
PROGMEM const char loc_124[] = "America/Inuvik";
PROGMEM const char loc_125[] = "America/Iqaluit";
PROGMEM const char loc_126[] = "America/Jamaica";
PROGMEM const char loc_127[] = "America/Juneau";
PROGMEM const char loc_128[] = "America/Kentucky/Louisville";
PROGMEM const char loc_129[] = "America/Kentucky/Monticello";
PROGMEM const char loc_130[] = "America/Kralendijk";
PROGMEM const char loc_131[] = "America/La_Paz";
PROGMEM const char loc_132[] = "America/Lima";
PROGMEM const char loc_133[] = "America/Los_Angeles";
PROGMEM const char loc_134[] = "America/Lower_Princes";
PROGMEM const char loc_135[] = "America/Maceio";
PROGMEM const char loc_136[] = "America/Managua";
PROGMEM const char loc_137[] = "America/Manaus";
PROGMEM const char loc_138[] = "America/Marigot";
PROGMEM const char loc_139[] = "America/Martinique";
PROGMEM const char loc_140[] = "America/Matamoros";
PROGMEM const char loc_141[] = "America/Mazatlan";
PROGMEM const char loc_142[] = "America/Menominee";
PROGMEM const char loc_143[] = "America/Merida";
PROGMEM const char loc_144[] = "America/Metlakatla";
PROGMEM const char loc_145[] = "America/Mexico_City";
PROGMEM const char loc_146[] = "America/Miquelon";
PROGMEM const char loc_147[] = "America/Moncton";
PROGMEM const char loc_148[] = "America/Monterrey";
PROGMEM const char loc_149[] = "America/Montevideo";
PROGMEM const char loc_150[] = "America/Montreal";
PROGMEM const char loc_151[] = "America/Montserrat";
PROGMEM const char loc_152[] = "America/Nassau";
PROGMEM const char loc_153[] = "America/New_York";
PROGMEM const char loc_154[] = "America/Nipigon";
PROGMEM const char loc_155[] = "America/Nome";
PROGMEM const char loc_156[] = "America/Noronha";
PROGMEM const char loc_157[] = "America/North_Dakota/Beulah";
PROGMEM const char loc_158[] = "America/North_Dakota/Center";
PROGMEM const char loc_159[] = "America/North_Dakota/New_Salem";
PROGMEM const char loc_160[] = "America/Nuuk";
PROGMEM const char loc_161[] = "America/Ojinaga";
PROGMEM const char loc_162[] = "America/Panama";
PROGMEM const char loc_163[] = "America/Pangnirtung";
PROGMEM const char loc_164[] = "America/Paramaribo";
PROGMEM const char loc_165[] = "America/Phoenix";
PROGMEM const char loc_166[] = "America/Port-au-Prince";
PROGMEM const char loc_167[] = "America/Port_of_Spain";
PROGMEM const char loc_168[] = "America/Porto_Velho";
PROGMEM const char loc_169[] = "America/Puerto_Rico";
PROGMEM const char loc_170[] = "America/Punta_Arenas";
PROGMEM const char loc_171[] = "America/Rainy_River";
PROGMEM const char loc_172[] = "America/Rankin_Inlet";
PROGMEM const char loc_173[] = "America/Recife";
PROGMEM const char loc_174[] = "America/Regina";
PROGMEM const char loc_175[] = "America/Resolute";
PROGMEM const char loc_176[] = "America/Rio_Branco";
PROGMEM const char loc_177[] = "America/Santarem";
PROGMEM const char loc_178[] = "America/Santiago";
PROGMEM const char loc_179[] = "America/Santo_Domingo";
PROGMEM const char loc_180[] = "America/Sao_Paulo";
PROGMEM const char loc_181[] = "America/Scoresbysund";
PROGMEM const char loc_182[] = "America/Sitka";
PROGMEM const char loc_183[] = "America/St_Barthelemy";
PROGMEM const char loc_184[] = "America/St_Johns";
PROGMEM const char loc_185[] = "America/St_Kitts";
PROGMEM const char loc_186[] = "America/St_Lucia";
PROGMEM const char loc_187[] = "America/St_Thomas";
PROGMEM const char loc_188[] = "America/St_Vincent";
PROGMEM const char loc_189[] = "America/Swift_Current";
PROGMEM const char loc_190[] = "America/Tegucigalpa";
PROGMEM const char loc_191[] = "America/Thule";
PROGMEM const char loc_192[] = "America/Thunder_Bay";
PROGMEM const char loc_193[] = "America/Tijuana";
PROGMEM const char loc_194[] = "America/Toronto";
PROGMEM const char loc_195[] = "America/Tortola";
PROGMEM const char loc_196[] = "America/Vancouver";
PROGMEM const char loc_197[] = "America/Whitehorse";
PROGMEM const char loc_198[] = "America/Winnipeg";
PROGMEM const char loc_199[] = "America/Yakutat";
PROGMEM const char loc_200[] = "America/Yellowknife";
PROGMEM const char loc_201[] = "Antarctica/Casey";
PROGMEM const char loc_202[] = "Antarctica/Davis";
PROGMEM const char loc_203[] = "Antarctica/DumontDUrville";
PROGMEM const char loc_204[] = "Antarctica/Macquarie";
PROGMEM const char loc_205[] = "Antarctica/Mawson";
PROGMEM const char loc_206[] = "Antarctica/McMurdo";
PROGMEM const char loc_207[] = "Antarctica/Palmer";
PROGMEM const char loc_208[] = "Antarctica/Rothera";
PROGMEM const char loc_209[] = "Antarctica/Syowa";
PROGMEM const char loc_210[] = "Antarctica/Troll";
PROGMEM const char loc_211[] = "Antarctica/Vostok";
PROGMEM const char loc_212[] = "Arctic/Longyearbyen";
PROGMEM const char loc_213[] = "Asia/Aden";
PROGMEM const char loc_214[] = "Asia/Almaty";
PROGMEM const char loc_215[] = "Asia/Amman";
PROGMEM const char loc_216[] = "Asia/Anadyr";
PROGMEM const char loc_217[] = "Asia/Aqtau";
PROGMEM const char loc_218[] = "Asia/Aqtobe";
PROGMEM const char loc_219[] = "Asia/Ashgabat";
PROGMEM const char loc_220[] = "Asia/Atyrau";
PROGMEM const char loc_221[] = "Asia/Baghdad";
PROGMEM const char loc_222[] = "Asia/Bahrain";
PROGMEM const char loc_223[] = "Asia/Baku";
PROGMEM const char loc_224[] = "Asia/Bangkok";
PROGMEM const char loc_225[] = "Asia/Barnaul";
PROGMEM const char loc_226[] = "Asia/Beirut";
PROGMEM const char loc_227[] = "Asia/Bishkek";
PROGMEM const char loc_228[] = "Asia/Brunei";
PROGMEM const char loc_229[] = "Asia/Chita";
PROGMEM const char loc_230[] = "Asia/Choibalsan";
PROGMEM const char loc_231[] = "Asia/Colombo";
PROGMEM const char loc_232[] = "Asia/Damascus";
PROGMEM const char loc_233[] = "Asia/Dhaka";
PROGMEM const char loc_234[] = "Asia/Dili";
PROGMEM const char loc_235[] = "Asia/Dubai";
PROGMEM const char loc_236[] = "Asia/Dushanbe";
PROGMEM const char loc_237[] = "Asia/Famagusta";
PROGMEM const char loc_238[] = "Asia/Gaza";
PROGMEM const char loc_239[] = "Asia/Hebron";
PROGMEM const char loc_240[] = "Asia/Ho_Chi_Minh";
PROGMEM const char loc_241[] = "Asia/Hong_Kong";
PROGMEM const char loc_242[] = "Asia/Hovd";
PROGMEM const char loc_243[] = "Asia/Irkutsk";
PROGMEM const char loc_244[] = "Asia/Jakarta";
PROGMEM const char loc_245[] = "Asia/Jayapura";
PROGMEM const char loc_246[] = "Asia/Jerusalem";
PROGMEM const char loc_247[] = "Asia/Kabul";
PROGMEM const char loc_248[] = "Asia/Kamchatka";
PROGMEM const char loc_249[] = "Asia/Karachi";
PROGMEM const char loc_250[] = "Asia/Kathmandu";
PROGMEM const char loc_251[] = "Asia/Khandyga";
PROGMEM const char loc_252[] = "Asia/Kolkata";
PROGMEM const char loc_253[] = "Asia/Krasnoyarsk";
PROGMEM const char loc_254[] = "Asia/Kuala_Lumpur";
PROGMEM const char loc_255[] = "Asia/Kuching";
PROGMEM const char loc_256[] = "Asia/Kuwait";
PROGMEM const char loc_257[] = "Asia/Macau";
PROGMEM const char loc_258[] = "Asia/Magadan";
PROGMEM const char loc_259[] = "Asia/Makassar";
PROGMEM const char loc_260[] = "Asia/Manila";
PROGMEM const char loc_261[] = "Asia/Muscat";
PROGMEM const char loc_262[] = "Asia/Nicosia";
PROGMEM const char loc_263[] = "Asia/Novokuznetsk";
PROGMEM const char loc_264[] = "Asia/Novosibirsk";
PROGMEM const char loc_265[] = "Asia/Omsk";
PROGMEM const char loc_266[] = "Asia/Oral";
PROGMEM const char loc_267[] = "Asia/Phnom_Penh";
PROGMEM const char loc_268[] = "Asia/Pontianak";
PROGMEM const char loc_269[] = "Asia/Pyongyang";
PROGMEM const char loc_270[] = "Asia/Qatar";
PROGMEM const char loc_271[] = "Asia/Qyzylorda";
PROGMEM const char loc_272[] = "Asia/Riyadh";
PROGMEM const char loc_273[] = "Asia/Sakhalin";
PROGMEM const char loc_274[] = "Asia/Samarkand";
PROGMEM const char loc_275[] = "Asia/Seoul";
PROGMEM const char loc_276[] = "Asia/Shanghai";
PROGMEM const char loc_277[] = "Asia/Singapore";
PROGMEM const char loc_278[] = "Asia/Srednekolymsk";
PROGMEM const char loc_279[] = "Asia/Taipei";
PROGMEM const char loc_280[] = "Asia/Tashkent";
PROGMEM const char loc_281[] = "Asia/Tbilisi";
PROGMEM const char loc_282[] = "Asia/Tehran";
PROGMEM const char loc_283[] = "Asia/Thimphu";
PROGMEM const char loc_284[] = "Asia/Tokyo";
PROGMEM const char loc_285[] = "Asia/Tomsk";
PROGMEM const char loc_286[] = "Asia/Ulaanbaatar";
PROGMEM const char loc_287[] = "Asia/Urumqi";
PROGMEM const char loc_288[] = "Asia/Ust-Nera";
PROGMEM const char loc_289[] = "Asia/Vientiane";
PROGMEM const char loc_290[] = "Asia/Vladivostok";
PROGMEM const char loc_291[] = "Asia/Yakutsk";
PROGMEM const char loc_292[] = "Asia/Yangon";
PROGMEM const char loc_293[] = "Asia/Yekaterinburg";
PROGMEM const char loc_294[] = "Asia/Yerevan";
PROGMEM const char loc_295[] = "Atlantic/Azores";
PROGMEM const char loc_296[] = "Atlantic/Bermuda";
PROGMEM const char loc_297[] = "Atlantic/Canary";
PROGMEM const char loc_298[] = "Atlantic/Cape_Verde";
PROGMEM const char loc_299[] = "Atlantic/Faroe";
PROGMEM const char loc_300[] = "Atlantic/Madeira";
PROGMEM const char loc_301[] = "Atlantic/Reykjavik";
PROGMEM const char loc_302[] = "Atlantic/South_Georgia";
PROGMEM const char loc_303[] = "Atlantic/Stanley";
PROGMEM const char loc_304[] = "Atlantic/St_Helena";
PROGMEM const char loc_305[] = "Australia/Adelaide";
PROGMEM const char loc_306[] = "Australia/Brisbane";
PROGMEM const char loc_307[] = "Australia/Broken_Hill";
PROGMEM const char loc_308[] = "Australia/Currie";
PROGMEM const char loc_309[] = "Australia/Darwin";
PROGMEM const char loc_310[] = "Australia/Eucla";
PROGMEM const char loc_311[] = "Australia/Hobart";
PROGMEM const char loc_312[] = "Australia/Lindeman";
PROGMEM const char loc_313[] = "Australia/Lord_Howe";
PROGMEM const char loc_314[] = "Australia/Melbourne";
PROGMEM const char loc_315[] = "Australia/Perth";
PROGMEM const char loc_316[] = "Australia/Sydney";
PROGMEM const char loc_317[] = "Europe/Amsterdam";
PROGMEM const char loc_318[] = "Europe/Andorra";
PROGMEM const char loc_319[] = "Europe/Astrakhan";
PROGMEM const char loc_320[] = "Europe/Athens";
PROGMEM const char loc_321[] = "Europe/Belgrade";
PROGMEM const char loc_322[] = "Europe/Berlin";
PROGMEM const char loc_323[] = "Europe/Bratislava";
PROGMEM const char loc_324[] = "Europe/Brussels";
PROGMEM const char loc_325[] = "Europe/Bucharest";
PROGMEM const char loc_326[] = "Europe/Budapest";
PROGMEM const char loc_327[] = "Europe/Busingen";
PROGMEM const char loc_328[] = "Europe/Chisinau";
PROGMEM const char loc_329[] = "Europe/Copenhagen";
PROGMEM const char loc_330[] = "Europe/Dublin";
PROGMEM const char loc_331[] = "Europe/Gibraltar";
PROGMEM const char loc_332[] = "Europe/Guernsey";
PROGMEM const char loc_333[] = "Europe/Helsinki";
PROGMEM const char loc_334[] = "Europe/Isle_of_Man";
PROGMEM const char loc_335[] = "Europe/Istanbul";
PROGMEM const char loc_336[] = "Europe/Jersey";
PROGMEM const char loc_337[] = "Europe/Kaliningrad";
PROGMEM const char loc_338[] = "Europe/Kiev";
PROGMEM const char loc_339[] = "Europe/Kirov";
PROGMEM const char loc_340[] = "Europe/Lisbon";
PROGMEM const char loc_341[] = "Europe/Ljubljana";
PROGMEM const char loc_342[] = "Europe/London";
PROGMEM const char loc_343[] = "Europe/Luxembourg";
PROGMEM const char loc_344[] = "Europe/Madrid";
PROGMEM const char loc_345[] = "Europe/Malta";
PROGMEM const char loc_346[] = "Europe/Mariehamn";
PROGMEM const char loc_347[] = "Europe/Minsk";
PROGMEM const char loc_348[] = "Europe/Monaco";
PROGMEM const char loc_349[] = "Europe/Moscow";
PROGMEM const char loc_350[] = "Europe/Oslo";
PROGMEM const char loc_351[] = "Europe/Paris";
PROGMEM const char loc_352[] = "Europe/Podgorica";
PROGMEM const char loc_353[] = "Europe/Prague";
PROGMEM const char loc_354[] = "Europe/Riga";
PROGMEM const char loc_355[] = "Europe/Rome";
PROGMEM const char loc_356[] = "Europe/Samara";
PROGMEM const char loc_357[] = "Europe/San_Marino";
PROGMEM const char loc_358[] = "Europe/Sarajevo";
PROGMEM const char loc_359[] = "Europe/Saratov";
PROGMEM const char loc_360[] = "Europe/Simferopol";
PROGMEM const char loc_361[] = "Europe/Skopje";
PROGMEM const char loc_362[] = "Europe/Sofia";
PROGMEM const char loc_363[] = "Europe/Stockholm";
PROGMEM const char loc_364[] = "Europe/Tallinn";
PROGMEM const char loc_365[] = "Europe/Tirane";
PROGMEM const char loc_366[] = "Europe/Ulyanovsk";
PROGMEM const char loc_367[] = "Europe/Uzhgorod";
PROGMEM const char loc_368[] = "Europe/Vaduz";
PROGMEM const char loc_369[] = "Europe/Vatican";
PROGMEM const char loc_370[] = "Europe/Vienna";
PROGMEM const char loc_371[] = "Europe/Vilnius";
PROGMEM const char loc_372[] = "Europe/Volgograd";
PROGMEM const char loc_373[] = "Europe/Warsaw";
PROGMEM const char loc_374[] = "Europe/Zagreb";
PROGMEM const char loc_375[] = "Europe/Zaporozhye";
PROGMEM const char loc_376[] = "Europe/Zurich";
PROGMEM const char loc_377[] = "Indian/Antananarivo";
PROGMEM const char loc_378[] = "Indian/Chagos";
PROGMEM const char loc_379[] = "Indian/Christmas";
PROGMEM const char loc_380[] = "Indian/Cocos";
PROGMEM const char loc_381[] = "Indian/Comoro";
PROGMEM const char loc_382[] = "Indian/Kerguelen";
PROGMEM const char loc_383[] = "Indian/Mahe";
PROGMEM const char loc_384[] = "Indian/Maldives";
PROGMEM const char loc_385[] = "Indian/Mauritius";
PROGMEM const char loc_386[] = "Indian/Mayotte";
PROGMEM const char loc_387[] = "Indian/Reunion";
PROGMEM const char loc_388[] = "Pacific/Apia";
PROGMEM const char loc_389[] = "Pacific/Auckland";
PROGMEM const char loc_390[] = "Pacific/Bougainville";
PROGMEM const char loc_391[] = "Pacific/Chatham";
PROGMEM const char loc_392[] = "Pacific/Chuuk";
PROGMEM const char loc_393[] = "Pacific/Easter";
PROGMEM const char loc_394[] = "Pacific/Efate";
PROGMEM const char loc_395[] = "Pacific/Enderbury";
PROGMEM const char loc_396[] = "Pacific/Fakaofo";
PROGMEM const char loc_397[] = "Pacific/Fiji";
PROGMEM const char loc_398[] = "Pacific/Funafuti";
PROGMEM const char loc_399[] = "Pacific/Galapagos";
PROGMEM const char loc_400[] = "Pacific/Gambier";
PROGMEM const char loc_401[] = "Pacific/Guadalcanal";
PROGMEM const char loc_402[] = "Pacific/Guam";
PROGMEM const char loc_403[] = "Pacific/Honolulu";
PROGMEM const char loc_404[] = "Pacific/Kiritimati";
PROGMEM const char loc_405[] = "Pacific/Kosrae";
PROGMEM const char loc_406[] = "Pacific/Kwajalein";
PROGMEM const char loc_407[] = "Pacific/Majuro";
PROGMEM const char loc_408[] = "Pacific/Marquesas";
PROGMEM const char loc_409[] = "Pacific/Midway";
PROGMEM const char loc_410[] = "Pacific/Nauru";
PROGMEM const char loc_411[] = "Pacific/Niue";
PROGMEM const char loc_412[] = "Pacific/Norfolk";
PROGMEM const char loc_413[] = "Pacific/Noumea";
PROGMEM const char loc_414[] = "Pacific/Pago_Pago";
PROGMEM const char loc_415[] = "Pacific/Palau";
PROGMEM const char loc_416[] = "Pacific/Pitcairn";
PROGMEM const char loc_417[] = "Pacific/Pohnpei";
PROGMEM const char loc_418[] = "Pacific/Port_Moresby";
PROGMEM const char loc_419[] = "Pacific/Rarotonga";
PROGMEM const char loc_420[] = "Pacific/Saipan";
PROGMEM const char loc_421[] = "Pacific/Tahiti";
PROGMEM const char loc_422[] = "Pacific/Tarawa";
PROGMEM const char loc_423[] = "Pacific/Tongatapu";
PROGMEM const char loc_424[] = "Pacific/Wake";
PROGMEM const char loc_425[] = "Pacific/Wallis";
PROGMEM const char loc_426[] = "Etc/GMT";
PROGMEM const char loc_427[] = "Etc/GMT-0";
PROGMEM const char loc_428[] = "Etc/GMT-1";
PROGMEM const char loc_429[] = "Etc/GMT-2";
PROGMEM const char loc_430[] = "Etc/GMT-3";
PROGMEM const char loc_431[] = "Etc/GMT-4";
PROGMEM const char loc_432[] = "Etc/GMT-5";
PROGMEM const char loc_433[] = "Etc/GMT-6";
PROGMEM const char loc_434[] = "Etc/GMT-7";
PROGMEM const char loc_435[] = "Etc/GMT-8";
PROGMEM const char loc_436[] = "Etc/GMT-9";
PROGMEM const char loc_437[] = "Etc/GMT-10";
PROGMEM const char loc_438[] = "Etc/GMT-11";
PROGMEM const char loc_439[] = "Etc/GMT-12";
PROGMEM const char loc_440[] = "Etc/GMT-13";
PROGMEM const char loc_441[] = "Etc/GMT-14";
PROGMEM const char loc_442[] = "Etc/GMT0";
PROGMEM const char loc_443[] = "Etc/GMT+0";
PROGMEM const char loc_444[] = "Etc/GMT+1";
PROGMEM const char loc_445[] = "Etc/GMT+2";
PROGMEM const char loc_446[] = "Etc/GMT+3";
PROGMEM const char loc_447[] = "Etc/GMT+4";
PROGMEM const char loc_448[] = "Etc/GMT+5";
PROGMEM const char loc_449[] = "Etc/GMT+6";
PROGMEM const char loc_450[] = "Etc/GMT+7";
PROGMEM const char loc_451[] = "Etc/GMT+8";
PROGMEM const char loc_452[] = "Etc/GMT+9";
PROGMEM const char loc_453[] = "Etc/GMT+10";
PROGMEM const char loc_454[] = "Etc/GMT+11";
PROGMEM const char loc_455[] = "Etc/GMT+12";
PROGMEM const char loc_456[] = "Etc/UCT";
PROGMEM const char loc_457[] = "Etc/UTC";
PROGMEM const char loc_458[] = "Etc/Greenwich";
PROGMEM const char loc_459[] = "Etc/Universal";
PROGMEM const char loc_460[] = "Etc/Zulu";

PROGMEM const char posix_000[] = "GMT0";
PROGMEM const char posix_001[] = "GMT0";
PROGMEM const char posix_002[] = "EAT-3";
PROGMEM const char posix_003[] = "CET-1";
PROGMEM const char posix_004[] = "EAT-3";
PROGMEM const char posix_005[] = "GMT0";
PROGMEM const char posix_006[] = "WAT-1";
PROGMEM const char posix_007[] = "GMT0";
PROGMEM const char posix_008[] = "GMT0";
PROGMEM const char posix_009[] = "CAT-2";
PROGMEM const char posix_010[] = "WAT-1";
PROGMEM const char posix_011[] = "CAT-2";
PROGMEM const char posix_012[] = "EET-2";
PROGMEM const char posix_013[] = "<+01>-1";
PROGMEM const char posix_014[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_015[] = "GMT0";
PROGMEM const char posix_016[] = "GMT0";
PROGMEM const char posix_017[] = "EAT-3";
PROGMEM const char posix_018[] = "EAT-3";
PROGMEM const char posix_019[] = "WAT-1";
PROGMEM const char posix_020[] = "<+01>-1";
PROGMEM const char posix_021[] = "GMT0";
PROGMEM const char posix_022[] = "CAT-2";
PROGMEM const char posix_023[] = "CAT-2";
PROGMEM const char posix_024[] = "SAST-2";
PROGMEM const char posix_025[] = "CAT-2";
PROGMEM const char posix_026[] = "EAT-3";
PROGMEM const char posix_027[] = "CAT-2";
PROGMEM const char posix_028[] = "CAT-2";
PROGMEM const char posix_029[] = "WAT-1";
PROGMEM const char posix_030[] = "WAT-1";
PROGMEM const char posix_031[] = "WAT-1";
PROGMEM const char posix_032[] = "GMT0";
PROGMEM const char posix_033[] = "WAT-1";
PROGMEM const char posix_034[] = "CAT-2";
PROGMEM const char posix_035[] = "CAT-2";
PROGMEM const char posix_036[] = "WAT-1";
PROGMEM const char posix_037[] = "CAT-2";
PROGMEM const char posix_038[] = "SAST-2";
PROGMEM const char posix_039[] = "SAST-2";
PROGMEM const char posix_040[] = "EAT-3";
PROGMEM const char posix_041[] = "GMT0";
PROGMEM const char posix_042[] = "EAT-3";
PROGMEM const char posix_043[] = "WAT-1";
PROGMEM const char posix_044[] = "WAT-1";
PROGMEM const char posix_045[] = "GMT0";
PROGMEM const char posix_046[] = "GMT0";
PROGMEM const char posix_047[] = "WAT-1";
PROGMEM const char posix_048[] = "GMT0";
PROGMEM const char posix_049[] = "EET-2";
PROGMEM const char posix_050[] = "CET-1";
PROGMEM const char posix_051[] = "CAT-2";
PROGMEM const char posix_052[] = "HST10HDT,M3.2.0,M11.1.0";
PROGMEM const char posix_053[] = "AKST9AKDT,M3.2.0,M11.1.0";
PROGMEM const char posix_054[] = "AST4";
PROGMEM const char posix_055[] = "AST4";
PROGMEM const char posix_056[] = "<-03>3";
PROGMEM const char posix_057[] = "<-03>3";
PROGMEM const char posix_058[] = "<-03>3";
PROGMEM const char posix_059[] = "<-03>3";
PROGMEM const char posix_060[] = "<-03>3";
PROGMEM const char posix_061[] = "<-03>3";
PROGMEM const char posix_062[] = "<-03>3";
PROGMEM const char posix_063[] = "<-03>3";
PROGMEM const char posix_064[] = "<-03>3";
PROGMEM const char posix_065[] = "<-03>3";
PROGMEM const char posix_066[] = "<-03>3";
PROGMEM const char posix_067[] = "<-03>3";
PROGMEM const char posix_068[] = "<-03>3";
PROGMEM const char posix_069[] = "AST4";
PROGMEM const char posix_070[] = "<-04>4<-03>,M10.1.0/0,M3.4.0/0";
PROGMEM const char posix_071[] = "EST5";
PROGMEM const char posix_072[] = "<-03>3";
PROGMEM const char posix_073[] = "CST6CDT,M4.1.0,M10.5.0";
PROGMEM const char posix_074[] = "AST4";
PROGMEM const char posix_075[] = "<-03>3";
PROGMEM const char posix_076[] = "CST6";
PROGMEM const char posix_077[] = "AST4";
PROGMEM const char posix_078[] = "<-04>4";
PROGMEM const char posix_079[] = "<-05>5";
PROGMEM const char posix_080[] = "MST7MDT,M3.2.0,M11.1.0";
PROGMEM const char posix_081[] = "MST7MDT,M3.2.0,M11.1.0";
PROGMEM const char posix_082[] = "<-04>4";
PROGMEM const char posix_083[] = "EST5";
PROGMEM const char posix_084[] = "<-04>4";
PROGMEM const char posix_085[] = "<-03>3";
PROGMEM const char posix_086[] = "EST5";
PROGMEM const char posix_087[] = "CST6CDT,M3.2.0,M11.1.0";
PROGMEM const char posix_088[] = "MST7MDT,M4.1.0,M10.5.0";
PROGMEM const char posix_089[] = "CST6";
PROGMEM const char posix_090[] = "MST7";
PROGMEM const char posix_091[] = "<-04>4";
PROGMEM const char posix_092[] = "AST4";
PROGMEM const char posix_093[] = "GMT0";
PROGMEM const char posix_094[] = "MST7";
PROGMEM const char posix_095[] = "MST7";
PROGMEM const char posix_096[] = "MST7MDT,M3.2.0,M11.1.0";
PROGMEM const char posix_097[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_098[] = "AST4";
PROGMEM const char posix_099[] = "MST7MDT,M3.2.0,M11.1.0";
PROGMEM const char posix_100[] = "<-05>5";
PROGMEM const char posix_101[] = "CST6";
PROGMEM const char posix_102[] = "<-03>3";
PROGMEM const char posix_103[] = "MST7";
PROGMEM const char posix_104[] = "AST4ADT,M3.2.0,M11.1.0";
PROGMEM const char posix_105[] = "<-03>3<-02>,M3.5.0/-2,M10.5.0/-1";
PROGMEM const char posix_106[] = "AST4ADT,M3.2.0,M11.1.0";
PROGMEM const char posix_107[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_108[] = "AST4";
PROGMEM const char posix_109[] = "AST4";
PROGMEM const char posix_110[] = "CST6";
PROGMEM const char posix_111[] = "<-05>5";
PROGMEM const char posix_112[] = "<-04>4";
PROGMEM const char posix_113[] = "AST4ADT,M3.2.0,M11.1.0";
PROGMEM const char posix_114[] = "CST5CDT,M3.2.0/0,M11.1.0/1";
PROGMEM const char posix_115[] = "MST7";
PROGMEM const char posix_116[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_117[] = "CST6CDT,M3.2.0,M11.1.0";
PROGMEM const char posix_118[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_119[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_120[] = "CST6CDT,M3.2.0,M11.1.0";
PROGMEM const char posix_121[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_122[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_123[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_124[] = "MST7MDT,M3.2.0,M11.1.0";
PROGMEM const char posix_125[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_126[] = "EST5";
PROGMEM const char posix_127[] = "AKST9AKDT,M3.2.0,M11.1.0";
PROGMEM const char posix_128[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_129[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_130[] = "AST4";
PROGMEM const char posix_131[] = "<-04>4";
PROGMEM const char posix_132[] = "<-05>5";
PROGMEM const char posix_133[] = "PST8PDT,M3.2.0,M11.1.0";
PROGMEM const char posix_134[] = "AST4";
PROGMEM const char posix_135[] = "<-03>3";
PROGMEM const char posix_136[] = "CST6";
PROGMEM const char posix_137[] = "<-04>4";
PROGMEM const char posix_138[] = "AST4";
PROGMEM const char posix_139[] = "AST4";
PROGMEM const char posix_140[] = "CST6CDT,M3.2.0,M11.1.0";
PROGMEM const char posix_141[] = "MST7MDT,M4.1.0,M10.5.0";
PROGMEM const char posix_142[] = "CST6CDT,M3.2.0,M11.1.0";
PROGMEM const char posix_143[] = "CST6CDT,M4.1.0,M10.5.0";
PROGMEM const char posix_144[] = "AKST9AKDT,M3.2.0,M11.1.0";
PROGMEM const char posix_145[] = "CST6CDT,M4.1.0,M10.5.0";
PROGMEM const char posix_146[] = "<-03>3<-02>,M3.2.0,M11.1.0";
PROGMEM const char posix_147[] = "AST4ADT,M3.2.0,M11.1.0";
PROGMEM const char posix_148[] = "CST6CDT,M4.1.0,M10.5.0";
PROGMEM const char posix_149[] = "<-03>3";
PROGMEM const char posix_150[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_151[] = "AST4";
PROGMEM const char posix_152[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_153[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_154[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_155[] = "AKST9AKDT,M3.2.0,M11.1.0";
PROGMEM const char posix_156[] = "<-02>2";
PROGMEM const char posix_157[] = "CST6CDT,M3.2.0,M11.1.0";
PROGMEM const char posix_158[] = "CST6CDT,M3.2.0,M11.1.0";
PROGMEM const char posix_159[] = "CST6CDT,M3.2.0,M11.1.0";
PROGMEM const char posix_160[] = "<-03>3<-02>,M3.5.0/-2,M10.5.0/-1";
PROGMEM const char posix_161[] = "MST7MDT,M3.2.0,M11.1.0";
PROGMEM const char posix_162[] = "EST5";
PROGMEM const char posix_163[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_164[] = "<-03>3";
PROGMEM const char posix_165[] = "MST7";
PROGMEM const char posix_166[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_167[] = "AST4";
PROGMEM const char posix_168[] = "<-04>4";
PROGMEM const char posix_169[] = "AST4";
PROGMEM const char posix_170[] = "<-03>3";
PROGMEM const char posix_171[] = "CST6CDT,M3.2.0,M11.1.0";
PROGMEM const char posix_172[] = "CST6CDT,M3.2.0,M11.1.0";
PROGMEM const char posix_173[] = "<-03>3";
PROGMEM const char posix_174[] = "CST6";
PROGMEM const char posix_175[] = "CST6CDT,M3.2.0,M11.1.0";
PROGMEM const char posix_176[] = "<-05>5";
PROGMEM const char posix_177[] = "<-03>3";
PROGMEM const char posix_178[] = "<-04>4<-03>,M9.1.6/24,M4.1.6/24";
PROGMEM const char posix_179[] = "AST4";
PROGMEM const char posix_180[] = "<-03>3";
PROGMEM const char posix_181[] = "<-01>1<+00>,M3.5.0/0,M10.5.0/1";
PROGMEM const char posix_182[] = "AKST9AKDT,M3.2.0,M11.1.0";
PROGMEM const char posix_183[] = "AST4";
PROGMEM const char posix_184[] = "NST3:30NDT,M3.2.0,M11.1.0";
PROGMEM const char posix_185[] = "AST4";
PROGMEM const char posix_186[] = "AST4";
PROGMEM const char posix_187[] = "AST4";
PROGMEM const char posix_188[] = "AST4";
PROGMEM const char posix_189[] = "CST6";
PROGMEM const char posix_190[] = "CST6";
PROGMEM const char posix_191[] = "AST4ADT,M3.2.0,M11.1.0";
PROGMEM const char posix_192[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_193[] = "PST8PDT,M3.2.0,M11.1.0";
PROGMEM const char posix_194[] = "EST5EDT,M3.2.0,M11.1.0";
PROGMEM const char posix_195[] = "AST4";
PROGMEM const char posix_196[] = "PST8PDT,M3.2.0,M11.1.0";
PROGMEM const char posix_197[] = "MST7";
PROGMEM const char posix_198[] = "CST6CDT,M3.2.0,M11.1.0";
PROGMEM const char posix_199[] = "AKST9AKDT,M3.2.0,M11.1.0";
PROGMEM const char posix_200[] = "MST7MDT,M3.2.0,M11.1.0";
PROGMEM const char posix_201[] = "<+11>-11";
PROGMEM const char posix_202[] = "<+07>-7";
PROGMEM const char posix_203[] = "<+10>-10";
PROGMEM const char posix_204[] = "AEST-10AEDT,M10.1.0,M4.1.0/3";
PROGMEM const char posix_205[] = "<+05>-5";
PROGMEM const char posix_206[] = "NZST-12NZDT,M9.5.0,M4.1.0/3";
PROGMEM const char posix_207[] = "<-03>3";
PROGMEM const char posix_208[] = "<-03>3";
PROGMEM const char posix_209[] = "<+03>-3";
PROGMEM const char posix_210[] = "<+00>0<+02>-2,M3.5.0/1,M10.5.0/3";
PROGMEM const char posix_211[] = "<+06>-6";
PROGMEM const char posix_212[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_213[] = "<+03>-3";
PROGMEM const char posix_214[] = "<+06>-6";
PROGMEM const char posix_215[] = "EET-2EEST,M2.5.4/24,M10.5.5/1";
PROGMEM const char posix_216[] = "<+12>-12";
PROGMEM const char posix_217[] = "<+05>-5";
PROGMEM const char posix_218[] = "<+05>-5";
PROGMEM const char posix_219[] = "<+05>-5";
PROGMEM const char posix_220[] = "<+05>-5";
PROGMEM const char posix_221[] = "<+03>-3";
PROGMEM const char posix_222[] = "<+03>-3";
PROGMEM const char posix_223[] = "<+04>-4";
PROGMEM const char posix_224[] = "<+07>-7";
PROGMEM const char posix_225[] = "<+07>-7";
PROGMEM const char posix_226[] = "EET-2EEST,M3.5.0/0,M10.5.0/0";
PROGMEM const char posix_227[] = "<+06>-6";
PROGMEM const char posix_228[] = "<+08>-8";
PROGMEM const char posix_229[] = "<+09>-9";
PROGMEM const char posix_230[] = "<+08>-8";
#ifdef ARDUINO
PROGMEM const char posix_231[] = "GMT-05:30";
#else
PROGMEM const char posix_231[] = "<+0530>-5:30";
#endif
PROGMEM const char posix_232[] = "EET-2EEST,M3.5.5/0,M10.5.5/0";
PROGMEM const char posix_233[] = "<+06>-6";
PROGMEM const char posix_234[] = "<+09>-9";
PROGMEM const char posix_235[] = "<+04>-4";
PROGMEM const char posix_236[] = "<+05>-5";
PROGMEM const char posix_237[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_238[] = "EET-2EEST,M3.4.4/48,M10.5.5/1";
PROGMEM const char posix_239[] = "EET-2EEST,M3.4.4/48,M10.5.5/1";
PROGMEM const char posix_240[] = "<+07>-7";
PROGMEM const char posix_241[] = "HKT-8";
PROGMEM const char posix_242[] = "<+07>-7";
PROGMEM const char posix_243[] = "<+08>-8";
PROGMEM const char posix_244[] = "WIB-7";
PROGMEM const char posix_245[] = "WIT-9";
PROGMEM const char posix_246[] = "IST-2IDT,M3.4.4/26,M10.5.0";
#ifdef ARDUINO
PROGMEM const char posix_247[] = "GMT-04:30";
#else
PROGMEM const char posix_247[] = "<+0430>-4:30";
#endif
PROGMEM const char posix_248[] = "<+12>-12";
PROGMEM const char posix_249[] = "PKT-5";
#ifdef ARDUINO
PROGMEM const char posix_250[] = "GMT-05:45";
#else
PROGMEM const char posix_250[] = "<+0545>-5:45";
#endif
PROGMEM const char posix_251[] = "<+09>-9";
PROGMEM const char posix_252[] = "IST-5:30";
PROGMEM const char posix_253[] = "<+07>-7";
PROGMEM const char posix_254[] = "<+08>-8";
PROGMEM const char posix_255[] = "<+08>-8";
PROGMEM const char posix_256[] = "<+03>-3";
PROGMEM const char posix_257[] = "CST-8";
PROGMEM const char posix_258[] = "<+11>-11";
PROGMEM const char posix_259[] = "WITA-8";
PROGMEM const char posix_260[] = "PST-8";
PROGMEM const char posix_261[] = "<+04>-4";
PROGMEM const char posix_262[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_263[] = "<+07>-7";
PROGMEM const char posix_264[] = "<+07>-7";
PROGMEM const char posix_265[] = "<+06>-6";
PROGMEM const char posix_266[] = "<+05>-5";
PROGMEM const char posix_267[] = "<+07>-7";
PROGMEM const char posix_268[] = "WIB-7";
PROGMEM const char posix_269[] = "KST-9";
PROGMEM const char posix_270[] = "<+03>-3";
PROGMEM const char posix_271[] = "<+05>-5";
PROGMEM const char posix_272[] = "<+03>-3";
PROGMEM const char posix_273[] = "<+11>-11";
PROGMEM const char posix_274[] = "<+05>-5";
PROGMEM const char posix_275[] = "KST-9";
PROGMEM const char posix_276[] = "CST-8";
PROGMEM const char posix_277[] = "<+08>-8";
PROGMEM const char posix_278[] = "<+11>-11";
PROGMEM const char posix_279[] = "CST-8";
PROGMEM const char posix_280[] = "<+05>-5";
PROGMEM const char posix_281[] = "<+04>-4";
#ifdef ARDUINO
PROGMEM const char posix_282[] = "GMT-03:30GDT-04:30,J79/24,J263/24";
#else
PROGMEM const char posix_282[] = "<+0330>-3:30<+0430>,J79/24,J263/24";
#endif
PROGMEM const char posix_283[] = "<+06>-6";
PROGMEM const char posix_284[] = "JST-9";
PROGMEM const char posix_285[] = "<+07>-7";
PROGMEM const char posix_286[] = "<+08>-8";
PROGMEM const char posix_287[] = "<+06>-6";
PROGMEM const char posix_288[] = "<+10>-10";
PROGMEM const char posix_289[] = "<+07>-7";
PROGMEM const char posix_290[] = "<+10>-10";
PROGMEM const char posix_291[] = "<+09>-9";
#ifdef ARDUINO
PROGMEM const char posix_292[] = "GMT-06:30";
#else
PROGMEM const char posix_292[] = "<+0630>-6:30";
#endif
PROGMEM const char posix_293[] = "<+05>-5";
PROGMEM const char posix_294[] = "<+04>-4";
PROGMEM const char posix_295[] = "<-01>1<+00>,M3.5.0/0,M10.5.0/1";
PROGMEM const char posix_296[] = "AST4ADT,M3.2.0,M11.1.0";
PROGMEM const char posix_297[] = "WET0WEST,M3.5.0/1,M10.5.0";
PROGMEM const char posix_298[] = "<-01>1";
PROGMEM const char posix_299[] = "WET0WEST,M3.5.0/1,M10.5.0";
PROGMEM const char posix_300[] = "WET0WEST,M3.5.0/1,M10.5.0";
PROGMEM const char posix_301[] = "GMT0";
PROGMEM const char posix_302[] = "<-02>2";
PROGMEM const char posix_303[] = "<-03>3";
PROGMEM const char posix_304[] = "GMT0";
PROGMEM const char posix_305[] = "ACST-9:30ACDT,M10.1.0,M4.1.0/3";
PROGMEM const char posix_306[] = "AEST-10";
PROGMEM const char posix_307[] = "ACST-9:30ACDT,M10.1.0,M4.1.0/3";
PROGMEM const char posix_308[] = "AEST-10AEDT,M10.1.0,M4.1.0/3";
PROGMEM const char posix_309[] = "ACST-9:30";
#ifdef ARDUINO
PROGMEM const char posix_310[] = "GMT-08:45";
#else
PROGMEM const char posix_310[] = "<+0845>-8:45";
#endif
PROGMEM const char posix_311[] = "AEST-10AEDT,M10.1.0,M4.1.0/3";
PROGMEM const char posix_312[] = "AEST-10";
#ifdef ARDUINO
PROGMEM const char posix_313[] = "LHST-10:30LHDT-11,M10.1.0,M4.1.0";
#else
PROGMEM const char posix_313[] = "<+1030>-10:30<+11>-11,M10.1.0,M4.1.0";
#endif
PROGMEM const char posix_314[] = "AEST-10AEDT,M10.1.0,M4.1.0/3";
PROGMEM const char posix_315[] = "AWST-8";
PROGMEM const char posix_316[] = "AEST-10AEDT,M10.1.0,M4.1.0/3";
PROGMEM const char posix_317[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_318[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_319[] = "<+04>-4";
PROGMEM const char posix_320[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_321[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_322[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_323[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_324[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_325[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_326[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_327[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_328[] = "EET-2EEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_329[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_330[] = "IST-1GMT0,M10.5.0,M3.5.0/1";
PROGMEM const char posix_331[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_332[] = "GMT0BST,M3.5.0/1,M10.5.0";
PROGMEM const char posix_333[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_334[] = "GMT0BST,M3.5.0/1,M10.5.0";
PROGMEM const char posix_335[] = "<+03>-3";
PROGMEM const char posix_336[] = "GMT0BST,M3.5.0/1,M10.5.0";
PROGMEM const char posix_337[] = "EET-2";
PROGMEM const char posix_338[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_339[] = "<+03>-3";
PROGMEM const char posix_340[] = "WET0WEST,M3.5.0/1,M10.5.0";
PROGMEM const char posix_341[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_342[] = "GMT0BST,M3.5.0/1,M10.5.0";
PROGMEM const char posix_343[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_344[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_345[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_346[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_347[] = "<+03>-3";
PROGMEM const char posix_348[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_349[] = "MSK-3";
PROGMEM const char posix_350[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_351[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_352[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_353[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_354[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_355[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_356[] = "<+04>-4";
PROGMEM const char posix_357[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_358[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_359[] = "<+04>-4";
PROGMEM const char posix_360[] = "MSK-3";
PROGMEM const char posix_361[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_362[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_363[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_364[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_365[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_366[] = "<+04>-4";
PROGMEM const char posix_367[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_368[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_369[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_370[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_371[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_372[] = "<+03>-3";
PROGMEM const char posix_373[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_374[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_375[] = "EET-2EEST,M3.5.0/3,M10.5.0/4";
PROGMEM const char posix_376[] = "CET-1CEST,M3.5.0,M10.5.0/3";
PROGMEM const char posix_377[] = "EAT-3";
PROGMEM const char posix_378[] = "<+06>-6";
PROGMEM const char posix_379[] = "<+07>-7";
#ifdef ARDUINO
PROGMEM const char posix_380[] = "GMT-06:30";
#else
PROGMEM const char posix_380[] = "<+0630>-6:30";
#endif
PROGMEM const char posix_381[] = "EAT-3";
PROGMEM const char posix_382[] = "<+05>-5";
PROGMEM const char posix_383[] = "<+04>-4";
PROGMEM const char posix_384[] = "<+05>-5";
PROGMEM const char posix_385[] = "<+04>-4";
PROGMEM const char posix_386[] = "EAT-3";
PROGMEM const char posix_387[] = "<+04>-4";
PROGMEM const char posix_388[] = "<+13>-13";
PROGMEM const char posix_389[] = "NZST-12NZDT,M9.5.0,M4.1.0/3";
PROGMEM const char posix_390[] = "<+11>-11";
#ifdef ARDUINO
PROGMEM const char posix_391[] = "CHAST-12:45CHADT-13:45,M9.5.0/2:45,M4.1.0/3:45";
#else
PROGMEM const char posix_391[] = "<+1245>-12:45<+1345>,M9.5.0/2:45,M4.1.0/3:45";
#endif
PROGMEM const char posix_392[] = "<+10>-10";
PROGMEM const char posix_393[] = "<-06>6<-05>,M9.1.6/22,M4.1.6/22";
PROGMEM const char posix_394[] = "<+11>-11";
PROGMEM const char posix_395[] = "<+13>-13";
PROGMEM const char posix_396[] = "<+13>-13";
PROGMEM const char posix_397[] = "<+12>-12<+13>,M11.2.0,M1.2.3/99";
PROGMEM const char posix_398[] = "<+12>-12";
PROGMEM const char posix_399[] = "<-06>6";
PROGMEM const char posix_400[] = "<-09>9";
PROGMEM const char posix_401[] = "<+11>-11";
PROGMEM const char posix_402[] = "ChST-10";
PROGMEM const char posix_403[] = "HST10";
PROGMEM const char posix_404[] = "<+14>-14";
PROGMEM const char posix_405[] = "<+11>-11";
PROGMEM const char posix_406[] = "<+12>-12";
PROGMEM const char posix_407[] = "<+12>-12";
#ifdef ARDUINO
PROGMEM const char posix_408[] = "GMT+09:30";
#else
PROGMEM const char posix_408[] = "<-0930>9:30";
#endif
PROGMEM const char posix_409[] = "SST11";
PROGMEM const char posix_410[] = "<+12>-12";
PROGMEM const char posix_411[] = "<-11>11";
PROGMEM const char posix_412[] = "<+11>-11<+12>,M10.1.0,M4.1.0/3";
PROGMEM const char posix_413[] = "<+11>-11";
PROGMEM const char posix_414[] = "SST11";
PROGMEM const char posix_415[] = "<+09>-9";
PROGMEM const char posix_416[] = "<-08>8";
PROGMEM const char posix_417[] = "<+11>-11";
PROGMEM const char posix_418[] = "<+10>-10";
PROGMEM const char posix_419[] = "<-10>10";
PROGMEM const char posix_420[] = "ChST-10";
PROGMEM const char posix_421[] = "<-10>10";
PROGMEM const char posix_422[] = "<+12>-12";
PROGMEM const char posix_423[] = "<+13>-13";
PROGMEM const char posix_424[] = "<+12>-12";
PROGMEM const char posix_425[] = "<+12>-12";
PROGMEM const char posix_426[] = "GMT0";
PROGMEM const char posix_427[] = "GMT0";
PROGMEM const char posix_428[] = "<+01>-1";
PROGMEM const char posix_429[] = "<+02>-2";
PROGMEM const char posix_430[] = "<+03>-3";
PROGMEM const char posix_431[] = "<+04>-4";
PROGMEM const char posix_432[] = "<+05>-5";
PROGMEM const char posix_433[] = "<+06>-6";
PROGMEM const char posix_434[] = "<+07>-7";
PROGMEM const char posix_435[] = "<+08>-8";
PROGMEM const char posix_436[] = "<+09>-9";
PROGMEM const char posix_437[] = "<+10>-10";
PROGMEM const char posix_438[] = "<+11>-11";
PROGMEM const char posix_439[] = "<+12>-12";
PROGMEM const char posix_440[] = "<+13>-13";
PROGMEM const char posix_441[] = "<+14>-14";
PROGMEM const char posix_442[] = "GMT0";
PROGMEM const char posix_443[] = "GMT0";
PROGMEM const char posix_444[] = "<-01>1";
PROGMEM const char posix_445[] = "<-02>2";
PROGMEM const char posix_446[] = "<-03>3";
PROGMEM const char posix_447[] = "<-04>4";
PROGMEM const char posix_448[] = "<-05>5";
PROGMEM const char posix_449[] = "<-06>6";
PROGMEM const char posix_450[] = "<-07>7";
PROGMEM const char posix_451[] = "<-08>8";
PROGMEM const char posix_452[] = "<-09>9";
PROGMEM const char posix_453[] = "<-10>10";
PROGMEM const char posix_454[] = "<-11>11";
PROGMEM const char posix_455[] = "<-12>12";
PROGMEM const char posix_456[] = "UTC0";
PROGMEM const char posix_457[] = "UTC0";
PROGMEM const char posix_458[] = "GMT0";
PROGMEM const char posix_459[] = "UTC0";
PROGMEM const char posix_460[] = "UTC0";

struct compare_cstr
{
    bool operator()(const char* a, const char* b) const
    {
        return std::strcmp(a,b) < 0;
    }
};

const std::map<const char*, const char*, compare_cstr> locationConversionTable =
{
    {loc_000, posix_000 },
    {loc_001, posix_001 },
    {loc_002, posix_002 },
    {loc_003, posix_003 },
    {loc_004, posix_004 },
    {loc_005, posix_005 },
    {loc_006, posix_006 },
    {loc_007, posix_007 },
    {loc_008, posix_008 },
    {loc_009, posix_009 },
    {loc_010, posix_010 },
    {loc_011, posix_011 },
    {loc_012, posix_012 },
    {loc_013, posix_013 },
    {loc_014, posix_014 },
    {loc_015, posix_015 },
    {loc_016, posix_016 },
    {loc_017, posix_017 },
    {loc_018, posix_018 },
    {loc_019, posix_019 },
    {loc_020, posix_020 },
    {loc_021, posix_021 },
    {loc_022, posix_022 },
    {loc_023, posix_023 },
    {loc_024, posix_024 },
    {loc_025, posix_025 },
    {loc_026, posix_026 },
    {loc_027, posix_027 },
    {loc_028, posix_028 },
    {loc_029, posix_029 },
    {loc_030, posix_030 },
    {loc_031, posix_031 },
    {loc_032, posix_032 },
    {loc_033, posix_033 },
    {loc_034, posix_034 },
    {loc_035, posix_035 },
    {loc_036, posix_036 },
    {loc_037, posix_037 },
    {loc_038, posix_038 },
    {loc_039, posix_039 },
    {loc_040, posix_040 },
    {loc_041, posix_041 },
    {loc_042, posix_042 },
    {loc_043, posix_043 },
    {loc_044, posix_044 },
    {loc_045, posix_045 },
    {loc_046, posix_046 },
    {loc_047, posix_047 },
    {loc_048, posix_048 },
    {loc_049, posix_049 },
    {loc_050, posix_050 },
    {loc_051, posix_051 },
    {loc_052, posix_052 },
    {loc_053, posix_053 },
    {loc_054, posix_054 },
    {loc_055, posix_055 },
    {loc_056, posix_056 },
    {loc_057, posix_057 },
    {loc_058, posix_058 },
    {loc_059, posix_059 },
    {loc_060, posix_060 },
    {loc_061, posix_061 },
    {loc_062, posix_062 },
    {loc_063, posix_063 },
    {loc_064, posix_064 },
    {loc_065, posix_065 },
    {loc_066, posix_066 },
    {loc_067, posix_067 },
    {loc_068, posix_068 },
    {loc_069, posix_069 },
    {loc_070, posix_070 },
    {loc_071, posix_071 },
    {loc_072, posix_072 },
    {loc_073, posix_073 },
    {loc_074, posix_074 },
    {loc_075, posix_075 },
    {loc_076, posix_076 },
    {loc_077, posix_077 },
    {loc_078, posix_078 },
    {loc_079, posix_079 },
    {loc_080, posix_080 },
    {loc_081, posix_081 },
    {loc_082, posix_082 },
    {loc_083, posix_083 },
    {loc_084, posix_084 },
    {loc_085, posix_085 },
    {loc_086, posix_086 },
    {loc_087, posix_087 },
    {loc_088, posix_088 },
    {loc_089, posix_089 },
    {loc_090, posix_090 },
    {loc_091, posix_091 },
    {loc_092, posix_092 },
    {loc_093, posix_093 },
    {loc_094, posix_094 },
    {loc_095, posix_095 },
    {loc_096, posix_096 },
    {loc_097, posix_097 },
    {loc_098, posix_098 },
    {loc_099, posix_099 },
    {loc_100, posix_100 },
    {loc_101, posix_101 },
    {loc_102, posix_102 },
    {loc_103, posix_103 },
    {loc_104, posix_104 },
    {loc_105, posix_105 },
    {loc_106, posix_106 },
    {loc_107, posix_107 },
    {loc_108, posix_108 },
    {loc_109, posix_109 },
    {loc_110, posix_110 },
    {loc_111, posix_111 },
    {loc_112, posix_112 },
    {loc_113, posix_113 },
    {loc_114, posix_114 },
    {loc_115, posix_115 },
    {loc_116, posix_116 },
    {loc_117, posix_117 },
    {loc_118, posix_118 },
    {loc_119, posix_119 },
    {loc_120, posix_120 },
    {loc_121, posix_121 },
    {loc_122, posix_122 },
    {loc_123, posix_123 },
    {loc_124, posix_124 },
    {loc_125, posix_125 },
    {loc_126, posix_126 },
    {loc_127, posix_127 },
    {loc_128, posix_128 },
    {loc_129, posix_129 },
    {loc_130, posix_130 },
    {loc_131, posix_131 },
    {loc_132, posix_132 },
    {loc_133, posix_133 },
    {loc_134, posix_134 },
    {loc_135, posix_135 },
    {loc_136, posix_136 },
    {loc_137, posix_137 },
    {loc_138, posix_138 },
    {loc_139, posix_139 },
    {loc_140, posix_140 },
    {loc_141, posix_141 },
    {loc_142, posix_142 },
    {loc_143, posix_143 },
    {loc_144, posix_144 },
    {loc_145, posix_145 },
    {loc_146, posix_146 },
    {loc_147, posix_147 },
    {loc_148, posix_148 },
    {loc_149, posix_149 },
    {loc_150, posix_150 },
    {loc_151, posix_151 },
    {loc_152, posix_152 },
    {loc_153, posix_153 },
    {loc_154, posix_154 },
    {loc_155, posix_155 },
    {loc_156, posix_156 },
    {loc_157, posix_157 },
    {loc_158, posix_158 },
    {loc_159, posix_159 },
    {loc_160, posix_160 },
    {loc_161, posix_161 },
    {loc_162, posix_162 },
    {loc_163, posix_163 },
    {loc_164, posix_164 },
    {loc_165, posix_165 },
    {loc_166, posix_166 },
    {loc_167, posix_167 },
    {loc_168, posix_168 },
    {loc_169, posix_169 },
    {loc_170, posix_170 },
    {loc_171, posix_171 },
    {loc_172, posix_172 },
    {loc_173, posix_173 },
    {loc_174, posix_174 },
    {loc_175, posix_175 },
    {loc_176, posix_176 },
    {loc_177, posix_177 },
    {loc_178, posix_178 },
    {loc_179, posix_179 },
    {loc_180, posix_180 },
    {loc_181, posix_181 },
    {loc_182, posix_182 },
    {loc_183, posix_183 },
    {loc_184, posix_184 },
    {loc_185, posix_185 },
    {loc_186, posix_186 },
    {loc_187, posix_187 },
    {loc_188, posix_188 },
    {loc_189, posix_189 },
    {loc_190, posix_190 },
    {loc_191, posix_191 },
    {loc_192, posix_192 },
    {loc_193, posix_193 },
    {loc_194, posix_194 },
    {loc_195, posix_195 },
    {loc_196, posix_196 },
    {loc_197, posix_197 },
    {loc_198, posix_198 },
    {loc_199, posix_199 },
    {loc_200, posix_200 },
    {loc_201, posix_201 },
    {loc_202, posix_202 },
    {loc_203, posix_203 },
    {loc_204, posix_204 },
    {loc_205, posix_205 },
    {loc_206, posix_206 },
    {loc_207, posix_207 },
    {loc_208, posix_208 },
    {loc_209, posix_209 },
    {loc_210, posix_210 },
    {loc_211, posix_211 },
    {loc_212, posix_212 },
    {loc_213, posix_213 },
    {loc_214, posix_214 },
    {loc_215, posix_215 },
    {loc_216, posix_216 },
    {loc_217, posix_217 },
    {loc_218, posix_218 },
    {loc_219, posix_219 },
    {loc_220, posix_220 },
    {loc_221, posix_221 },
    {loc_222, posix_222 },
    {loc_223, posix_223 },
    {loc_224, posix_224 },
    {loc_225, posix_225 },
    {loc_226, posix_226 },
    {loc_227, posix_227 },
    {loc_228, posix_228 },
    {loc_229, posix_229 },
    {loc_230, posix_230 },
    {loc_231, posix_231 },
    {loc_232, posix_232 },
    {loc_233, posix_233 },
    {loc_234, posix_234 },
    {loc_235, posix_235 },
    {loc_236, posix_236 },
    {loc_237, posix_237 },
    {loc_238, posix_238 },
    {loc_239, posix_239 },
    {loc_240, posix_240 },
    {loc_241, posix_241 },
    {loc_242, posix_242 },
    {loc_243, posix_243 },
    {loc_244, posix_244 },
    {loc_245, posix_245 },
    {loc_246, posix_246 },
    {loc_247, posix_247 },
    {loc_248, posix_248 },
    {loc_249, posix_249 },
    {loc_250, posix_250 },
    {loc_251, posix_251 },
    {loc_252, posix_252 },
    {loc_253, posix_253 },
    {loc_254, posix_254 },
    {loc_255, posix_255 },
    {loc_256, posix_256 },
    {loc_257, posix_257 },
    {loc_258, posix_258 },
    {loc_259, posix_259 },
    {loc_260, posix_260 },
    {loc_261, posix_261 },
    {loc_262, posix_262 },
    {loc_263, posix_263 },
    {loc_264, posix_264 },
    {loc_265, posix_265 },
    {loc_266, posix_266 },
    {loc_267, posix_267 },
    {loc_268, posix_268 },
    {loc_269, posix_269 },
    {loc_270, posix_270 },
    {loc_271, posix_271 },
    {loc_272, posix_272 },
    {loc_273, posix_273 },
    {loc_274, posix_274 },
    {loc_275, posix_275 },
    {loc_276, posix_276 },
    {loc_277, posix_277 },
    {loc_278, posix_278 },
    {loc_279, posix_279 },
    {loc_280, posix_280 },
    {loc_281, posix_281 },
    {loc_282, posix_282 },
    {loc_283, posix_283 },
    {loc_284, posix_284 },
    {loc_285, posix_285 },
    {loc_286, posix_286 },
    {loc_287, posix_287 },
    {loc_288, posix_288 },
    {loc_289, posix_289 },
    {loc_290, posix_290 },
    {loc_291, posix_291 },
    {loc_292, posix_292 },
    {loc_293, posix_293 },
    {loc_294, posix_294 },
    {loc_295, posix_295 },
    {loc_296, posix_296 },
    {loc_297, posix_297 },
    {loc_298, posix_298 },
    {loc_299, posix_299 },
    {loc_300, posix_300 },
    {loc_301, posix_301 },
    {loc_302, posix_302 },
    {loc_303, posix_303 },
    {loc_304, posix_304 },
    {loc_305, posix_305 },
    {loc_306, posix_306 },
    {loc_307, posix_307 },
    {loc_308, posix_308 },
    {loc_309, posix_309 },
    {loc_310, posix_310 },
    {loc_311, posix_311 },
    {loc_312, posix_312 },
    {loc_313, posix_313 },
    {loc_314, posix_314 },
    {loc_315, posix_315 },
    {loc_316, posix_316 },
    {loc_317, posix_317 },
    {loc_318, posix_318 },
    {loc_319, posix_319 },
    {loc_320, posix_320 },
    {loc_321, posix_321 },
    {loc_322, posix_322 },
    {loc_323, posix_323 },
    {loc_324, posix_324 },
    {loc_325, posix_325 },
    {loc_326, posix_326 },
    {loc_327, posix_327 },
    {loc_328, posix_328 },
    {loc_329, posix_329 },
    {loc_330, posix_330 },
    {loc_331, posix_331 },
    {loc_332, posix_332 },
    {loc_333, posix_333 },
    {loc_334, posix_334 },
    {loc_335, posix_335 },
    {loc_336, posix_336 },
    {loc_337, posix_337 },
    {loc_338, posix_338 },
    {loc_339, posix_339 },
    {loc_340, posix_340 },
    {loc_341, posix_341 },
    {loc_342, posix_342 },
    {loc_343, posix_343 },
    {loc_344, posix_344 },
    {loc_345, posix_345 },
    {loc_346, posix_346 },
    {loc_347, posix_347 },
    {loc_348, posix_348 },
    {loc_349, posix_349 },
    {loc_350, posix_350 },
    {loc_351, posix_351 },
    {loc_352, posix_352 },
    {loc_353, posix_353 },
    {loc_354, posix_354 },
    {loc_355, posix_355 },
    {loc_356, posix_356 },
    {loc_357, posix_357 },
    {loc_358, posix_358 },
    {loc_359, posix_359 },
    {loc_360, posix_360 },
    {loc_361, posix_361 },
    {loc_362, posix_362 },
    {loc_363, posix_363 },
    {loc_364, posix_364 },
    {loc_365, posix_365 },
    {loc_366, posix_366 },
    {loc_367, posix_367 },
    {loc_368, posix_368 },
    {loc_369, posix_369 },
    {loc_370, posix_370 },
    {loc_371, posix_371 },
    {loc_372, posix_372 },
    {loc_373, posix_373 },
    {loc_374, posix_374 },
    {loc_375, posix_375 },
    {loc_376, posix_376 },
    {loc_377, posix_377 },
    {loc_378, posix_378 },
    {loc_379, posix_379 },
    {loc_380, posix_380 },
    {loc_381, posix_381 },
    {loc_382, posix_382 },
    {loc_383, posix_383 },
    {loc_384, posix_384 },
    {loc_385, posix_385 },
    {loc_386, posix_386 },
    {loc_387, posix_387 },
    {loc_388, posix_388 },
    {loc_389, posix_389 },
    {loc_390, posix_390 },
    {loc_391, posix_391 },
    {loc_392, posix_392 },
    {loc_393, posix_393 },
    {loc_394, posix_394 },
    {loc_395, posix_395 },
    {loc_396, posix_396 },
    {loc_397, posix_397 },
    {loc_398, posix_398 },
    {loc_399, posix_399 },
    {loc_400, posix_400 },
    {loc_401, posix_401 },
    {loc_402, posix_402 },
    {loc_403, posix_403 },
    {loc_404, posix_404 },
    {loc_405, posix_405 },
    {loc_406, posix_406 },
    {loc_407, posix_407 },
    {loc_408, posix_408 },
    {loc_409, posix_409 },
    {loc_410, posix_410 },
    {loc_411, posix_411 },
    {loc_412, posix_412 },
    {loc_413, posix_413 },
    {loc_414, posix_414 },
    {loc_415, posix_415 },
    {loc_416, posix_416 },
    {loc_417, posix_417 },
    {loc_418, posix_418 },
    {loc_419, posix_419 },
    {loc_420, posix_420 },
    {loc_421, posix_421 },
    {loc_422, posix_422 },
    {loc_423, posix_423 },
    {loc_424, posix_424 },
    {loc_425, posix_425 },
    {loc_426, posix_426 },
    {loc_427, posix_427 },
    {loc_428, posix_428 },
    {loc_429, posix_429 },
    {loc_430, posix_430 },
    {loc_431, posix_431 },
    {loc_432, posix_432 },
    {loc_433, posix_433 },
    {loc_434, posix_434 },
    {loc_435, posix_435 },
    {loc_436, posix_436 },
    {loc_437, posix_437 },
    {loc_438, posix_438 },
    {loc_439, posix_439 },
    {loc_440, posix_440 },
    {loc_441, posix_441 },
    {loc_442, posix_442 },
    {loc_443, posix_443 },
    {loc_444, posix_444 },
    {loc_445, posix_445 },
    {loc_446, posix_446 },
    {loc_447, posix_447 },
    {loc_448, posix_448 },
    {loc_449, posix_449 },
    {loc_450, posix_450 },
    {loc_451, posix_451 },
    {loc_452, posix_452 },
    {loc_453, posix_453 },
    {loc_454, posix_454 },
    {loc_455, posix_455 },
    {loc_456, posix_456 },
    {loc_457, posix_457 },
    {loc_458, posix_458 },
    {loc_459, posix_459 },
    {loc_460, posix_460 },
};
//
}

namespace goblib { namespace datetime {

const char* locationToPOSIX(const char* location)
{
    //    string_t loc(location);
    return  (location && location[0] && locationConversionTable.count(location) == 1)
            ? locationConversionTable.at(location) : nullptr;
}

#if 0
void checkPOSIX()
{
    for(auto& e : locationConversionTable)
    {
        setenv("TZ", e.second, 1);
        tzset();

        auto tzinfo = __gettzinfo();
        auto& r0 = tzinfo->__tzrule[0];
        auto& r1 = tzinfo->__tzrule[1];

        printf("%32s:%48s:%8ld:%8ld [%c]:[%c]\n", e.first.c_str(), e.second, r0.offset, r1.offset,
               std::abs(r0.offset) >= 64800 ? 'X' : ' ',
               std::abs(r1.offset) >= 64800 ? 'X' : ' ');
    }
}
#endif
//
}};
