/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_RAND_H
#define __JBL_RAND_H
#include "jbl_rand_config.h"
#if JBL_RAND_ENABLE==1
/*******************************************************************************************/
/*                            依赖jbl_ying jbl_exception                                   */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_ying.h"
/*******************************************************************************************/
/*                            联动jbl_string                                               */
/*******************************************************************************************/
#include "jbl_string.h"
/*******************************************************************************************/
/*                            以下函数完成随机数组件启动                                 */
/*******************************************************************************************/
void jbl_rand_start();
/*******************************************************************************************/
/*                            以下函数实现随机数                                         */
/*******************************************************************************************/
void			jbl_rand_srand			(jbl_uint32 seed);												//设置随机种子
jbl_uint32		jbl_rand				();																//产生一个随机数
jbl_uint32 		jbl_rand_between		(jbl_uint32 a,jbl_uint32 b);									//产生一个属于[a,b]的随机数
#if JBL_STRING_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现随机字符串                                     */
/*******************************************************************************************/
jbl_string *	jbl_rand_string			(jbl_string *this,jbl_string_size_type len,unsigned char* s);	//产生一个以s为字典,长度为len的随机字符串
#define			jbl_rand_dict_small		"abcdefghijklmnopqrstuvwxyz"									//小写字母字典
#define			jbl_rand_dict_big		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"									//大写字母字典
#define			jbl_rand_dict_number	"0123456789"													//数字字典
#define			jbl_rand_dict_symbol	"~!@#$%^&*()_+=-`{}|[]\\:\";'<>?,./"							//特殊字符字典
#endif

#else
#define			jbl_rand_start()
#endif
#endif
