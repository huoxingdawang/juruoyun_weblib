/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_SCANNER_H
#define __JBL_SCANNER_H
#include "jbl_var_config.h"
#if JBL_SCANNER_ENABLE==1
#include "jbl_ying.h"
#include "jbl_var.h"	
typedef enum
{
	JBL_SCANNER_KEY_UNDEFINED	,
	JBL_SCANNER_KEY_END			,
	JBL_SCANNER_KEY_INT			,
	JBL_SCANNER_KEY_UINT		,
	JBL_SCANNER_KEY_DOUBLE		,
	JBL_SCANNER_KEY_CHAR		,
	JBL_SCANNER_KEY_CHARS		,
	JBL_SCANNER_KEY_HEX			,
#if JBL_VAR_ENABLE == 1
	JBL_SCANNER_KEY_VAR			,
	JBL_SCANNER_KEY_JSON		,
#endif
}jbl_scanner_key;
jbl_scanner_key jbl_scanner(unsigned char * YYCURSOR,unsigned char **addr);


#endif
#endif