/* Copyright (c) [2019] juruoyun developer team
   Juruoyun web lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jry_wl_exception.h"
#if JRY_WL_EXCEPTION_ENABLE==1
void __jry_wl_exception(const char * file,int line,int x)
{
	fprintf(stderr,"-------------------------------------------------------\n");
	jry_bl_print_colorful(stderr,"ERROR!",JRY_BL_PRINT_COLORFUL_BLACK,JRY_BL_PRINT_COLORFUL_RED);
	fprintf(stderr,"\nAT\tfile:");
	jry_bl_print_colorful(stderr,file,JRY_BL_PRINT_COLORFUL_BLACK,JRY_BL_PRINT_COLORFUL_YELLOW);
	fprintf(stderr,"\n  \tline:");
	char ss[40];
	sprintf(ss,"%d",line);jry_bl_print_colorful(stderr,ss,JRY_BL_PRINT_COLORFUL_BLACK,JRY_BL_PRINT_COLORFUL_YELLOW);
	fprintf(stderr,"\nBecause:");
	sprintf(ss,"%d",x);jry_bl_print_colorful(stderr,ss,JRY_BL_PRINT_COLORFUL_BLACK,JRY_BL_PRINT_COLORFUL_YELLOW);	
	switch(x)
	{
		case JRY_WL_ERROR_NO_ERROR				:fprintf(stderr,":JRY_WL_ERROR_NO_ERROR")				;break;
		case JRY_WL_ERROR_SOCKET_INIT_FAILED	:fprintf(stderr,":JRY_WL_ERROR_SOCKET_INIT_FAILED")		;break;
		case JRY_WL_ERROR_SOCKET_CONNECT_FAILED	:fprintf(stderr,":JRY_WL_ERROR_SOCKET_CONNECT_FAILED")	;break;
		case JRY_WL_ERROR_SOCKET_BIND_FAILED	:fprintf(stderr,":JRY_WL_ERROR_SOCKET_BIND_FAILED")		;break;
		case JRY_WL_ERROR_SOCKET_LISTEN_FAILED	:fprintf(stderr,":JRY_WL_ERROR_SOCKET_LISTEN_FAILED")	;break;
		case JRY_WL_ERROR_SOCKET_DLL_UNLOAD		:fprintf(stderr,":JRY_WL_ERROR_SOCKET_DLL_UNLOAD")		;break;
		case JRY_WL_ERROR_SOCKET_ACCEPT_FAILED	:fprintf(stderr,":JRY_WL_ERROR_SOCKET_ACCEPT_FAILED")	;break;
	}
	exit(0);
}
#endif