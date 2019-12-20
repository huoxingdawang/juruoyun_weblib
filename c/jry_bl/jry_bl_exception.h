/* Copyright (c) [2019] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JRY_BL_EXCEPTION_H
#define __JRY_BL_EXCEPTION_H
#include "jry_bl_exception_config.h"
#if JRY_BL_EXCEPTION_ENABLE==1
#include "jry_bl_ying.h"
#define JRY_BL_ERROR_NO_ERROR		1000000
#define JRY_BL_ERROR_NULL_POINTER	1000001
#define JRY_BL_ERROR_MEMORY_ERROR	1000002
#define JRY_BL_ERROR_TYPE_ERROR		1000003





#include <stdio.h>
#include <stdlib.h>
#define jry_bl_exception(x) __jry_bl_exception(__FILE__,__LINE__,x)
void __jry_bl_exception(const char * file,int line,int x);
#endif
#endif
