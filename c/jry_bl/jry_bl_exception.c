/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jry_bl_exception.h"
#if JRY_BL_EXCEPTION_ENABLE==1
void __jry_bl_exception(const char * file,int line,int x)
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
		case JRY_BL_ERROR_NO_ERROR		:fprintf(stderr,":JRY_BL_ERROR_NO_ERROR")		;break;
		case JRY_BL_ERROR_NULL_POINTER	:fprintf(stderr,":JRY_BL_ERROR_NULL_POINTER")	;break;
		case JRY_BL_ERROR_MEMORY_ERROR	:fprintf(stderr,":JRY_BL_ERROR_MEMORY_ERROR")	;break;
		case JRY_BL_ERROR_TYPE_ERROR	:fprintf(stderr,":JRY_BL_ERROR_TYPE_ERROR")		;break;
		
	}
	exit(0);
}
#endif