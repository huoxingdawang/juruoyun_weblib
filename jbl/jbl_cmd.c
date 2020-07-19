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
jbl_string * jbl_execute_cmd(const jbl_string *cmd,jbl_string *result)
{
	if(cmd==NULL)jbl_exception("NULL POINTER");
	char *buf=jbl_malloc(1024);
	FILE *ptr;
	if((ptr=popen((char*)jbl_string_get_chars(cmd),"r"))!=NULL)
	{
		while(fgets(buf,1024,ptr)!=NULL)
			result=jbl_string_add_chars_length(result,(jbl_uint8*)buf,jbl_strlen(buf));
		pclose(ptr);
	}
	jbl_free(buf);
	return result;
}
#endif
