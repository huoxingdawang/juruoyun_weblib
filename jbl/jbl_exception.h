/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_EXCEPTION_H
#define __JBL_EXCEPTION_H
#include "jbl_exception_config.h"
#if JBL_EXCEPTION_ENABLE==1
#include "jbl_ying.h"
void	jbl_exception_add_exit_function		(void (*func)(void));	//注册一个异常处理函数
extern	jbl_uint8							jbl_exception_on_error;


#define	jbl_exception(x)					__jbl_exception(__FUNCTION__,__FILE__,__LINE__,x)	//抛出一个异常
void	__jbl_exception						(const char * function,const char * file,int line,char * x);
void	jbl_exit							(int x);


#else
#include<stdlib.h>
#define	jbl_exception_add_exit_function(x)
#define	jbl_exception(x)					exit(0)
#define	jbl_exit(x)							exit(0)
#endif
#endif
