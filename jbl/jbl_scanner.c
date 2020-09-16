/* Generated by re2c 1.3 on Wed Sep 16 15:42:06 2020 */
#line 1 "jbl/jbl_scanner.l"
/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/   
#include "jbl_scanner.h"
#if JBL_SCANNER_ENABLE==1
#if JBL_VAR_ENABLE == 0
#define JBL_SCANNER_KEY_VAR JBL_SCANNER_KEY_UNDEFINED
#define JBL_SCANNER_KEY_JSON JBL_SCANNER_KEY_UNDEFINED
#endif
jbl_scanner_key jbl_scanner(unsigned char * YYCURSOR,unsigned char **addr)
{
//	unsigned char * YYMARKER;
	#define YYCTYPE		unsigned char
	
#line 24 "jbl/jbl_scanner.c"
{
	YYCTYPE yych;
	yych = *YYCURSOR;
	switch (yych) {
	case '%':	goto yy4;
	default:	goto yy2;
	}
yy2:
	++YYCURSOR;
yy3:
#line 22 "jbl/jbl_scanner.l"
	{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_UNDEFINED;}
#line 37 "jbl/jbl_scanner.c"
yy4:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'X':	goto yy5;
	case 'c':	goto yy7;
	case 'd':	goto yy9;
	case 'f':	goto yy11;
	case 'j':	goto yy13;
	case 's':	goto yy15;
	case 'v':	goto yy17;
	default:	goto yy3;
	}
yy5:
	++YYCURSOR;
#line 28 "jbl/jbl_scanner.l"
	{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_HEX;}
#line 54 "jbl/jbl_scanner.c"
yy7:
	++YYCURSOR;
#line 27 "jbl/jbl_scanner.l"
	{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_CHAR;}
#line 59 "jbl/jbl_scanner.c"
yy9:
	++YYCURSOR;
#line 24 "jbl/jbl_scanner.l"
	{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_INT;}
#line 64 "jbl/jbl_scanner.c"
yy11:
	++YYCURSOR;
#line 25 "jbl/jbl_scanner.l"
	{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_DOUBLE;}
#line 69 "jbl/jbl_scanner.c"
yy13:
	++YYCURSOR;
#line 30 "jbl/jbl_scanner.l"
	{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_JSON;}
#line 74 "jbl/jbl_scanner.c"
yy15:
	++YYCURSOR;
#line 26 "jbl/jbl_scanner.l"
	{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_CHARS;}
#line 79 "jbl/jbl_scanner.c"
yy17:
	++YYCURSOR;
#line 29 "jbl/jbl_scanner.l"
	{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_VAR;}
#line 84 "jbl/jbl_scanner.c"
}
#line 31 "jbl/jbl_scanner.l"

	#undef YYCTYPE
	#undef YYFILL	
}



#endif