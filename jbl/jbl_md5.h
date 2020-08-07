/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_MD5_H
#define __JBL_MD5_H
#include "jbl_md5_config.h"
#if JBL_MD5_ENABLE==1
/*******************************************************************************************/
/*                            依赖jbl_ying jbl_exception jbl_string                        */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_ying.h"
#include "jbl_string.h"
/*******************************************************************************************/
/*                            以下函实现md5                                               */
/*******************************************************************************************/
jbl_string*	jbl_md5	(jbl_string* this,jbl_string* out);	//进行md5加密
#endif
#endif