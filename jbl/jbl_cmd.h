/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_CMD_H
#define __JBL_CMD_H
#include "jbl_cmd_config.h"
#if JBL_CMD_ENABLE==1
#include "jbl_string.h"
jbl_string *	jbl_execute_cmd		(jbl_string *cmd,jbl_string *result);	//执行一段cmd命令
jbl_string *	jbl_execute_cmd_chars(unsigned char *cmd,jbl_string *result);





#endif
#endif
