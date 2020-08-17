/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_SHA1_H
#define __JBL_SHA1_H
#include "jbl_sha1_config.h"
#if JBL_SHA1_ENABLE==1
/*******************************************************************************************/
/*                            依赖jbl_exception jbl_ying jbl_string                        */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_ying.h"
#include "jbl_string.h"
/*******************************************************************************************/
/*                            以下函数实现字符串的sha1加密操作                           */
/*******************************************************************************************/
jbl_string*	jbl_sha1	(jbl_string* this,jbl_string* out,jbl_uint8 raw);//进行sha1加密
#endif
#endif