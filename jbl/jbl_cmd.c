/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#include "jbl_cmd.h"
#if JBL_CMD_ENABLE==1
#include <stdio.h>
#include "jbl_string.h"
jbl_string * jbl_execute_cmd(jbl_string *cmd,jbl_string *result)
{
	if(cmd==NULL)jbl_exception("NULL POINTER");
	FILE *ptr;
	if((ptr=popen((char*)jbl_string_get_chars(cmd),"r"))!=NULL)
	{
		jbl_string *thi;result=jbl_string_extend_to(result,1024,1,&thi);
		while(fgets((char *)thi->s+thi->len,1024,ptr)!=NULL)
			thi->len+=jbl_strlen(thi->s+thi->len),
			result=jbl_string_extend_to(result,1024,1,&thi);
		pclose(ptr);
	}
	return result;
}
jbl_string * jbl_execute_cmd_chars(unsigned char *cmd,jbl_string *result)
{
	if(cmd==NULL)jbl_exception("NULL POINTER");
	FILE *ptr;
	if((ptr=popen((char*)cmd,"r"))!=NULL)
	{
		jbl_string *thi;result=jbl_string_extend_to(result,1024,1,&thi);
		while(fgets((char *)thi->s+thi->len,1024,ptr)!=NULL)
			thi->len+=jbl_strlen(thi->s+thi->len),
			result=jbl_string_extend_to(result,1024,1,&thi);
		pclose(ptr);
	}
	return result;
}
#endif
