/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_exception.h"
#if JBL_EXCEPTION_ENABLE==1
#if JBL_STREAM_ENABLE ==1
#include "jbl_stream.h"
#endif
#include <stdio.h>
#include <stdlib.h>
jbl_uint32	jbl_exception_exit_functions_cnt;
jbl_uint8	jbl_exception_on_error;
void(*jbl_exception_exit_functions[JBL_EXCEPTION_EXIT_FUNCTIONS_LENGTH])(void);
void jbl_exception_add_exit_function(void (*func)(void))
{
	jbl_exception_exit_functions[jbl_exception_exit_functions_cnt++]=func;
}
void jbl_exit(int x)
{
#if JBL_STREAM_ENABLE ==1
	if(jbl_stream_stdout)
	{
		jbl_stream_push_chars(jbl_stream_stdout,UC"\n\n-------------------------------------------------------\n");
		jbl_stream_push_chars(jbl_stream_stdout,UC"EXIT!\n");
		jbl_stream_do(jbl_stream_stdout,jbl_stream_force);
	}
#endif
	while(jbl_exception_exit_functions_cnt--)jbl_exception_exit_functions[jbl_exception_exit_functions_cnt]();
	exit(x);
}
void __jbl_exception(const char * function,const char * file,int line,char * x)
{
	if(jbl_exception_on_error)
		printf("\n\n\nError occured ,when handling error\nFile%s\nLine %d\nFunction %s\n",file,line,function),exit(0);
	jbl_exception_on_error=1;
#if JBL_STREAM_ENABLE ==1
	if((!jbl_stream_stdout)&&x)
#endif
		printf("\n\n\nError occured ,but stream can't work\nFile%s\nLine %d\nFunction %s\n",file,line,function);
#if JBL_STREAM_ENABLE ==1
	if(jbl_stream_stdout)
	{
		jbl_stream_push_chars(jbl_stream_stdout,UC"\n\n-------------------------------------------------------\n");
		if(x)
			jbl_stream_push_chars(jbl_stream_stdout,UC"ERROR!\n");
		else
			jbl_stream_push_chars(jbl_stream_stdout,UC"EXIT!\n");
		jbl_stream_push_chars(jbl_stream_stdout,UC"At file:");
		jbl_stream_push_chars(jbl_stream_stdout,UC file);
		jbl_stream_push_chars(jbl_stream_stdout,UC"\nOn line:");
		jbl_stream_push_uint (jbl_stream_stdout,line);
		jbl_stream_push_chars(jbl_stream_stdout,UC"\nIn function:");
		jbl_stream_push_chars(jbl_stream_stdout,UC function);
		if(x)
		{
			jbl_stream_push_chars(jbl_stream_stdout,UC"\nMeeage:");
			jbl_stream_push_chars(jbl_stream_stdout,UC x);
		}
		jbl_stream_push_chars(jbl_stream_stdout,UC"\n");
		jbl_stream_do(jbl_stream_stdout,jbl_stream_force);
	}
#endif
	while(jbl_exception_exit_functions_cnt--)jbl_exception_exit_functions[jbl_exception_exit_functions_cnt]();
	exit(0);
}
#endif