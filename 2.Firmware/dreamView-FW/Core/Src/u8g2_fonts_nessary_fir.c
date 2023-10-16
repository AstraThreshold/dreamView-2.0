//
// Created by Fir on 2023/2/13.
// 所有需要的额外官方字体
/**
* NOTICE:
* + 中文字库需要自己制作 制作过程如下
*     + 在Other文件夹内有对应的工具 首先要编辑str.txt 修改成自己想要的
*     + 然后在当前目录下git bash键入
*         - echo '32-128,' > myfont.map
*         - cat str.txt | iconv -f utf-8 -t c99 | sed 's/\\u\([0-9a-f]\{4\}\)/\$\1,\n/g' | sort | uniq | sed '/^$/d' | tr '/a-f/' '/A-F/' >> myfont.map
*     + 出现myfont.map
*     + 进入bdfconv文件夹 双击build_myfont.bat
*     + 出现u8g2_font_myfont.c 在其首行include u8g2
*     + 复制编辑好的c文件到项目src文件夹 大功告成
* + 绘制中文要使用DrawUTF8函数，相应地，获取对应宽度也要使用GetUTF8Width函数
*/
#include "u8g2.h"
const uint8_t u8g2_font_chargen_92_tr[1658] U8G2_FONT_SECTION("u8g2_font_chargen_92_tr") =
  "_\0\4\3\4\4\4\5\5\14\22\0\376\16\376\16\0\2;\4V\6] \5\0\10\71!\10\342\15"
  "\71<\60\42\42\10F\253\71\210p\42#\34\332\11\271\11\211\10\211\220\34\34\210\220\10\211\10\211\220\34"
  "\34\210\220\10\211\10\211\1$\34\352\11\71\211\331\34DLH\224\210TH\35]I\224\210THL\34"
  "\304\230\11\1%#\354\10yLI\324L\250\21\221\20\31\251\30\32\221\34\234\24\31\232(\31\221\20\221"
  "\221\220\230\251\220\232\0&\32\352\11\271\214\25M\314\250\231\30*\33+\232\230\240\71\232\30\251\260\20!"
  "\2'\7B\255\71\34\4(\21\345\15\371\210L\214\210\214\310h\63#\263F\0)\21\345\12\71\310\254"
  "\221\231\321\311\210\310\304\210\14\0*\30\312\31\71)\21\251\220\230\70\210\261\42\243\262\71\210\230\220(Q"
  "$\4+\15\210:\371\210\251\71\70\220\21S\3,\10B\355\70PH\0-\7(j\71<\20.\6"
  "\42\15\71\20/\24\352\11\71\312\315\311\215\315\311\215\315\311\215\315\311\315\11\2\60\31\352\11\271\330\34D"
  "\14\225\21\331\234LP\214\334\30\221\25M\34\304\230\0\61\14\346\13\271\10\211\60\322\237\34\24\62\24\352"
  "\11\271\330\34D\14\225\11\312\215<\231\23Txp`\63\27\352\11\271\330\34D\14\225\11\312\15U\25"
  "\16\222\25M\34\304\230\0\64\31\352\11\71Q\25M\310\214\210\314\210\10\211\10\211\10\211\34\34\230\11j"
  "\2\65\26\352\11\71|\20(x\20r \70\250\220\254h\342 \306\4\0\66\31\352\11\271\330\34D\14"
  "\225\21\12\36\204\34HH\225\221\25M\34\304\230\0\67\16\352\11\71<\60\224\33\333NP\37\1\70\32"
  "\352\11\271\330\34D\14\225\221\25M\34\204\34D\14\225\221\25M\34\304\230\0\71\31\352\11\271\330\34D"
  "\14\225\221UI\34\210\34\4\12\222\25M\34\304\230\0:\7\242\15\71\20\23;\11\302\355\70\20SH"
  "\0<\14\327\33y\11\315\354Hj\257\4=\13hJ\71<\220\207\70\70\20>\15\327\32\71H\355\225"
  "\320\314\216\244\0\77\23\352\11\271\330\34D\14\225\11\312\215m'\17-(\4@\32\352\11\271\330\34D"
  "\14\225\321\234\34\204\210\220\210\220\34\304P\10\16\36\231\0A\23\352\11\71\311Q\331\214L\14\225\61;"
  "x\60\306\231\0B\30\352\11\71\34\204\34HH\225\261:\250\70\220\220*cuPq\20\2C\23\352"
  "\11\271\330\34D\14\225\21\352\263\242\211\203\30\23\0D\22\352\11\71\34\204\34HH\225\361\257\16*\16"
  "B\0E\22\352\11\71|\20\250\341A\310A\210\240\206\7\7\6F\20\352\11\71|\20\250\341A\310A"
  "\210\240\36\2G\25\352\11\271\330\34D\14\225\21\252y\306\254h\342 \306\4\0H\15\352\11\71\210\361"
  "\331\301\203\61>\23I\15\352\11\71<\60\22\324\77:\70\60J\17\352\11\71\71\23\324O\204D\326X"
  "\21\1K\34\352\11\71\210Q\25M\310\214\210\314H\14UUI\14\211\314\310\214\10MH\225\11L\13"
  "\352\11\71\10\352\177xp`M\22\352\11\71\210\221\25\235\34\34H\60\21!\343g\2N\24\352\11\71"
  "\210\61\253\62\272\241\30!\231\240\71\262*c&O\22\352\11\271\330\34D\14\225\361gE\23\7\61&"
  "\0P\24\352\11\71\34\204\34HH\225\261:\250\70\10\21\324C\0Q\27\352\11\271\330\34D\14\225\361"
  "\211\10\311\4\315A\314\310\201H\205\0R\33\352\11\71\34\204\34HH\225\261:\250\70\10\221\30\22\231"
  "\221\31\21\232\220*\23S\26\352\11\271\330\34D\14\225\21\16\36\35\16\222\25M\34\304\230\0T\14\352"
  "\11\71<\60\22\324\377\10\0U\16\352\11\71\210\361\77+\232\70\210\61\1V\20\352\11\71\210\361\317\212"
  "&Ff\254\350\204\0W\22\352\11\71\210\361'\42\24\24\7\7\42Ged\2X\25\352\11\71\210\61"
  "+\232\30\231\261\42\243\262\31\231\30*c&Y\21\352\11\71\210qV\64\61\62cE'\250G\0Z"
  "\17\352\11\71<\60\224\33\333\357\4\17\16\14[\12\346\13\71<\322\77:(\134\21\352\11\71\10\16\12"
  ".\24\134(\270PpP\0]\13\346\13\71\34\24\351\37\35\34^\13X\232\371H\321X\214\24\11_"
  "\7*\351\70<\60`\7\64\334\71\310(a\25\252\11\271\24\335\310\14\312\234\34D\14IHI\34L"
  "X\10b\23\352\11\71\10\352\242\344@b\250\214W\7\25\7!\0c\22\252\11\271\330\34D\14\225\21"
  "*+\232\70\210\61\1d\23\352\11\71\352\244B\342\240\250\214\263*\211\3\221\203\0e\23\252\11\271\330"
  "\34D\14\225\35<\30\34\232\70\210\61\1f\17\350\12\371\320\230h&db$\246\217\0g\27\312\351"
  "\270TH\34\24\225\221\25M\34\210T\10\26M\34\304\230\0h\20\352\11\71\10\352\204D\302\202\306\212"
  "\214\317\4i\17\350\12\371\210\311\3\325\224\351\233\203\3\1j\21\370\371\270\251\7\271\70\323\31Q\311\204"
  "\15\11\0k\30\352\11\71\10\352\214\252hBfDdF\202\346 \204dB\252L\0l\14\350\12\71"
  "\324\224\351\277\71\70\20m\31\252\11\71HHL\34XL\220\210\220\210\220\210\220\210\220\210\220\210\220("
  "n\16\252\11\71H\224\34H\14\225\361\63\1o\21\252\11\271\330\34D\14\225qV\64q\20c\2p"
  "\24\312\351\70H\224\34H\14\225\61+:\250\220(\21T\10q\23\312\351\270TH\34\24\225\61+\232"
  "\70\20\251\20\324\0r\17\252\11\71H\224\34H\320X\21\352!\0s\22\252\11\271\330\34D\14\31\36"
  "\35\32M\34\304\230\0t\17\350\12\371\210isp #\246gE\4u\16\252\11\71\210\361\263\242\211"
  "\3\221\12\1v\20\252\11\71\210\361\254hbd\306\212N\10\0w\27\252\11\71\210\61\21!\21!\21"
  "!\21\241\240\70\250\30\231\11\12\1x\23\252\11\71\210\25M\214\314X\221Q\331\214L\14\225\11y\24"
  "\312\351\70\210\361\254J\342@\344 Pn\342 \344\4\0z\14\252\11\71<\60\333g\7\7\6{\20"
  "\347\13\371\220\224Hi\63\64&\245U\15\1|\7\342\15\71\374\0}\20\347\12\71\320T\351lhF"
  "J\223\22\32\0~\15Ji\271\314HTXT\310\214\0\0\0\0\4\377\377\0";

const uint8_t u8g2_font_luBIS14_tn[376] U8G2_FONT_SECTION("u8g2_font_luBIS14_tn") =
  "\22\0\4\3\4\5\4\4\5\17\21\0\375\16\374\16\374\0\0\0\0\1[ \5\0\60/*\21x\366"
  "\363\314\204\244\20\61\31!\231\230\211\210\1+\21\274\24\375\305\206\212F\35<\230\12\25\215M\7,\13"
  "u\262np\61#\62\61\2-\7%\324-\34\14.\10\65\22o\34X\0/\24\37\261\70\317.\26"
  "\236\35V<\254xvXxv\61\0\60\34\353\26}Q\335\314\210\14M\14M\14Y\31M\14M\14"
  "M\314\214\314\314\134Q\1\61\17\347\32}\34\230\320\360\204\206'\64\64\0\62\26\355\24=]\35\10E"
  "\221\256\134HGH\70Ix@r@\2\63\31\354\26\375\134\35\210\10QN\12N\225\35DR\256\220"
  "\242\70\30\71\10\2\64\31\354\22}\16\322\321E\214\205L\305\14\211\320\310\314\34<\20#\234\134\3\65"
  "\30\355\24\375\34\314\34\314\210\216\312\332\35D\226n\42Er\60s\20\5\66\36\355\24\275\35\35\310\20"
  "\211\20R\36\310\34\220\20Q\214M\214M\214\215\14\315\34\210U\1\67\22\353\30}\34\274[\70\267\241"
  "\340\334\340\334\340 \0\70\37\354\24}U\35\310\10\215\210\211\14\315\34DU\35\210\314P\214I\214I"
  "L\215\34\10\31\1\71\36\355\24}\225\35\310\14\215\214M\214M\214M\20\221\34\320\34H\22\222\10\321"
  "\34\10\235\1:\14\266\24\257\260\240GBAB\2\0\0\0\4\377\377\0";