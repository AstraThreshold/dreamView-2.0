#include "oled.h"
/*
  Fontname: -wenquanyi-wenquanyi bitmap song-medium-r-normal--12-120-75-75-P-119-ISO10646-1
  Copyright: (null)
  Glyphs: 151/30503
  BBX Build Mode: 0
*/
const uint8_t u8g2_font_myfont[2533] U8G2_FONT_SECTION("u8g2_font_myfont") =
  "\227\0\3\2\4\4\3\4\5\13\15\0\376\10\376\12\377\1a\2\316\4\27 \5\0L\13!\7\221F"
  "\213S\0\42\7\64}\213\310\24#\16\226\304\233\250eX\242^\206%j\1$\17\245<\253l\251("
  "\231\250%Je\213\0%\20\226<\233(\351\242%a\232DI\27-\1&\16\205D\253,\211\222\254"
  "\62%Q\244\4'\6\61\376\212\1(\13\263=\253$J\242nQ\26)\14\263=\213,\312\242.Q"
  "\22\1*\14uD\253J\345\240,M\21\0+\13wD\274\270\66\14Y\134\3,\7\62>\13E\1"
  "-\6\25d\213A.\6!\306\12\1/\14\304<\273\246\254\224\225\262\32\0\60\12\205D\233%\363-"
  "Y\0\61\11\205D\213\261O\203\0\62\13\205D\233%\13km\203\0\63\14\205D\213!\15\223\65\14"
  "\7\5\64\16\206\304;-\211\272d\311\60\246\11\0\65\15\205D\213!\11\303!\15\303A\1\66\15\205"
  "D\233%\14\207$\263%\13\0\67\13\205D\213A\254\205YX\2\70\15\205D\233%\323\222%\263%"
  "\13\0\71\15\205D\233%\263%C\250%\13\0:\6aD\11I;\11\202>\213!V\24\0<\10"
  "\225D\313\254k\7=\10\65\134\213A\35\4>\11\225D\213\264[G\0\77\15\225D\233%\323\302H"
  "\313\241\60\2@\24\247<\254\255\222HZ\42)JE\251H\211\222e\23\0A\16\207D\274\70M\302"
  "$+\15J\252\6B\15\206\304\213A\11\305a\11\215\303\2C\14\206\304\233!\11\325\216\311\220\0D"
  "\15\207D\214A\12\223\324c\62H\0E\12\205D\213c\70\214\305AF\13\205D\213c\70$a#"
  "\0G\15\206\304\233!\11\325\332(&\203\0H\13\206\304\213\320\70\14\242c\0I\10\203D\212%\352"
  "eJ\10\243\64\252\376\264\0K\15\205D\213LJJ\232\226D\225,L\11\205D\213\260\217\203\0M"
  "\20\207D\14m\310\226\212R\221\42)R\325\0N\15\206\304\213pS\42)\321Fc\0O\14\207D"
  "\254\255\222\272&Y\66\1P\14\205D\213!\311l\203\22\26\1Q\15\227<\254\255\222\272&Y\266'"
  "\0R\20\206\304\213!\312\222,\311\222!jK\302\0S\14\205D\233AL\327\60\34\24\0T\11\207"
  "D\214C\26\367\15U\12\206\304\213\320\217\311\220\0V\17\207D\214TM\262(\253\204I\32g\0W"
  "\21\211D\215,\323\62\255\322)iJ\332\212Y\4X\15\206\304\213PL\242L\213ZB\61Y\13\207"
  "D\214\64\311*i\334\15Z\12\207D\214C\332\347a\10[\11\263>\213!\352O\3\134\14\245<\213"
  "\60\15\323\60\15\323\60]\11\263=\213\251\77\15\1^\10\65t\253,\251\5_\6\25<\213A`\7"
  "\62\375\212$\12a\14eD\233%K\6MK\206\0b\14\205D\213\60\34\222\314mP\0c\10d"
  "\304\232!k\34d\13\205D\313\312\240\271%C\0e\13eD\233%\33\206\60\35\2f\11\203D\232"
  "i\210:\1g\14\205\64\233AsK\206\60Y\0h\13\205D\213\60\34\222\314[\0i\7\201D\211"
  "d\30j\10\242\264\231,\351ek\15\205D\213\260\224\224\264$\252d\1l\7\201D\211\203\0m\16"
  "gD\214E\211\42)\222\42)\222\12n\11eD\213!\311\274\5o\12eD\233%sK\26\0p"
  "\14\205\64\213!\311\334\6%\14\1q\13\205\64\233AsK\206\260\0r\10cD\212!\352\4s\14"
  "eD\233%K\324$K\26\0t\12\203D\212(\32\242\66\1u\11eD\213\314[\62\4v\14e"
  "D\213LKJI\26F\0w\16gD\214(\222\42\245\242t\213\262\4x\13eD\213,\251Uj"
  "Z\0y\15\205\64\213LKJI\26fa\6z\12eD\213A\314\332\6\1{\13\243<\252$\252"
  "dQ[\0|\7\261\276\212\7\1}\13\243<\212,\252%QK\4~\7&\334\33\311\2\200\17\225"
  "D+\251\222\15Q\66Da\24)\0\0\0\0\4\377\377N\12\27\273<\336\34\313\261\34\313\261A\316"
  "\261\34\313\261\34K\207\203\0N:\31\271=\236(Nr$\34\216Y(eQ\222EI\224&Y\244"
  "*\0N\216\27\273<\236\341\234c\71\226\16\7\65\307r,\307r,GV\0N\256\25\273<\336t"
  "\70h\345A\307\207w(\33\344,\15\243yO\21\31\273<\276r\226\206\351p\21\263$\33\263\61J"
  "jIT\322\322\60\3QI\27\273<\336\70k\252\247\303A\213\362(\217\262R\26e\331:\4Qs"
  "\25\273<\256\264\26\16\347\34K\207C\234CI\236\245%]R\66\35\273<\276\70)&\311\260$R"
  "\230\14\203R\13\223dX:%\235\262\244\242\25\7S\315\32\273<\376m\220s,\307\206AK\262\60"
  "J\322,\315\222\60\312\242D\25S\321\31\273<\256\306(\213\262x\70\204\71\66\310Y\232T\263\64K"
  "\244H\24Vh \273<\236!\31\242J\324\22EC\62\304Q\64\34\264\332\240\14J\224D-Q\64"
  "$C\2X\360\31\273<\336t\70\250\371\60\350\370\60hQ\226\15\203\226C\71\224c\0\134O\30\273"
  "<\256\341\224G\303)\11\263\250\70\234\32\207S[\224E\305\10]\356\30\273<\276\60\216\262\341\240\306"
  "\303\65\36\16Z\16%\203\26V\206\3^U\34\273<\216\7\255:\14a\32\16C\230d\321p\320\222"
  "(\32\16Rc\224H\0^\246\31\273<\336x\70$Qq\70\65FC\230c\311\260E\265PL\246"
  "\5_\17\31\273<\356$G\242<\34\16r:$y\224gq\26eZ\62\304\2_&\32\273<\216"
  "!\312\263xx\253FI\70DC\230fqT\251\14J-\14_:\35\273<\216i\320\242\254\64("
  "c\226\15\203\26EC\22\265\14[\30\25\207i\210\2_\353\32\273<\256,\316beP:%\265("
  "I\206S\26gjTL\302L\16bl\37\273<\256h\320\322h\220\322,N\206!\222\222\212\226$"
  "J\224D\225\250-\33\242H\1b\315\33\273<\256\64\254\15\312 e\245\244\26I\203\242eJV\312"
  "J\331\60h\5c\11\34\273<\256\60-\15\7\251\22U\242T\31\26-JJR\26\212Y\22-\232"
  "\0dD\36\273<\256d\30\242,J\6e\310JY\222\14\231TR\206A\351\251Vi\31\264(e"
  "> \273<\256\60\316\262a\31\242,\312\242\244\66%Y\22%Y\222\225\42-J\242$J\302\0e"
  "p \273<\216()&R\66,Cd\211\22))%-\303RK\242$R\262\64K\222[\0e"
  "\271\27\273<\316\34L\207\203\230c\71\66\250a\32V\263\70\12\67\0f\227\34\273<\376lxJ\262"
  "$\252D\303S\36%\203\62$Y\22%\203\234\305\203\2f\335\34\273<\316aY\302\244\62,\215\311"
  "\360\222%Q\62<%\275,iRM\244\0g,\27\273<\336\34K\207\203\270'\345\244\32\325Z\242"
  "AJs,\5g:\34\273<\256\34\213\206\60*\15S\30\325\226(S\244J\324\22\25\223,\211\304"
  "\1g\177\35\273<\256T\213\266a\217\362h\220\226,R\246\244\224\210I\26&Q\222)Z\0j!"
  "\37\273<\256,\11\223\341 %a\64hJ\26)\303$eIe\30\242\60\315\222\60\321\4l`\36"
  "\273<\236\264\224da\22&\225\61Y\262D\251EIEK\246,\211\222,L\302AmK\34\273<"
  "\216d\320\22-)%-J/J\247\244S\322\62eI\307D\253\304\3u(\30\272<\236\341\222U"
  "\262\312p\311*Ye\270d\225,\23\63\61\22u\65\25\272=\316\34J\207KV\31.Y%\253\14"
  "\327\260u\20v\370\35\273<\276h\320\242,\31\266\322\240EY\264dIe\30\242Z)+\15Z\224"
  "\5w \36\273<\336\341\220\204R\62\34\222\250%j\31\16I\324\22EC\22%R\42\251a\0|"
  "\373\25\271=\356\341\234E\303\234g\311p\314\242\244\26U\42-~\337\36\273<\256\60M\206!\11\323"
  "\244\226\34\206(K\32\223h\210\222\34iR\242d\312\6\177n\32\273<\236\341T\211\242\341\234\16\7"
  ")\15\207!L\303a\10\323h\70\10\210L\32\273<\256h\220r(\207s(\31\206D\215\222\64L"
  "\303\64L\303l\2\210h\30\273<\336x\70\347\303\220\247\303AL\242\60JB-SJ\251(\211\346"
  "\36\273<\236\70\33\262\250\30\15\312\260t\31\226\212R\31\16Y\224\324\22\245\66D\312\22\213\241\27\273"
  "<\236\264\230c\371p\10\323\60\15\323$J\265\64\314\261\14\213\270\32\273<\236,\217rlP\243l"
  "L\223a\210\302\64\211R-\15s,\3\213\276\31\273<\236l\10\243z\24g\303\216%\303\26e-"
  "\241\226fI\272\11\213\325\34\273<\236\64\311\302(\33\6=[\206\64J\322(I\243J\24)\245Q"
  "\312\3\213\357\33\273<\236l\320\242,\316\342A\331\261h\320\302\64\31\206HJ\302(K\13\221\312\37"
  "\273<\276aH\266$JJ\341\224D\303\222HZ\250\14K\61K\222a\210\302\64\314\0\225.\37\273"
  "<\236d\312\262aQ\262DK\206\203\224\324\206S-I\206%\312\62E\312\222h\20\225\350\26\272="
  "\236h\230b\35\321\21\35\321\21\35\321\21\35\321\21y\227\363\31\273<\336x\70fy\22\16\7\35\36"
  "\206\60\15\207!L\303a\210\0\236\321\31\273<\236\341\224\225\222N\321\24\15\347x\70\247\303A)U"
  "\262\250\0\0";
