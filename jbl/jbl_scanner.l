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
	unsigned char * YYMARKER;
	#define YYCTYPE		unsigned char
	/*!re2c
re2c:yyfill:enable = 0;
		[^]			{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_UNDEFINED;}	
		[\x00]		{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_END;}	
		"%d"		{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_INT;}	
		"%f"		{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_DOUBLE;}
		"%s"		{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_CHARS;}
		"%c"		{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_CHAR;}
		"%X"		{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_HEX;}
		"%errstr"	{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_ERRSTR;}
		"%v"		{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_VAR;}
		"%j"		{if(addr)*addr=YYCURSOR;return JBL_SCANNER_KEY_JSON;}
     */
	#undef YYCTYPE
	#undef YYFILL	
}



#endif