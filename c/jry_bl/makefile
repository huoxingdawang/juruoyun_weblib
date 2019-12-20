#   Copyright (c) [2019] juruoyun developer team
#   Juruoyun basic lib is licensed under the Mulan PSL v1.
#   You can use this software according to the terms and conditions of the Mulan PSL v1.
#   You may obtain a copy of Mulan PSL v1 at:
#      http://license.coscl.org.cn$(H)MulanPSL
#   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
#   PURPOSE.
#   See the Mulan PSL v1 for more details.
#jry_bl
ifeq ($(shell uname),Linux)
JRY_BL_EXLIB = 
else
JRY_BL_EXLIB = 
endif
jry_bl					:jry_bl/jry_bl_aes jry_bl/jry_bl_array jry_bl/jry_bl_base64 jry_bl/jry_bl_base64 jry_bl/jry_bl_link_list jry_bl/jry_bl_malloc jry_bl/jry_bl_md5 jry_bl/jry_bl_string jry_bl/jry_bl_sha1 jry_bl/jry_bl_var jry_bl/jry_bl_ying
	ar rc tmp$(H)jry_bl.a tmp$(H)jry_bl_*.o
jry_bl/jry_bl_aes		:jry_bl/jry_bl_ying jry_bl/jry_bl_exception jry_bl/jry_bl_malloc jry_bl/jry_bl_string
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_bl_aes.o jry_bl$(H)jry_bl_aes.c $(JRY_BL_EXLIB)
jry_bl/jry_bl_array		:jry_bl/jry_bl_ying jry_bl/jry_bl_exception jry_bl/jry_bl_malloc
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_bl_array.o jry_bl$(H)jry_bl_array.c $(JRY_BL_EXLIB)
jry_bl/jry_bl_base64	:jry_bl/jry_bl_ying jry_bl/jry_bl_exception jry_bl/jry_bl_malloc jry_bl/jry_bl_string
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_bl_base64.o jry_bl$(H)jry_bl_base64.c	$(JRY_BL_EXLIB)
jry_bl/jry_bl_exception	:jry_bl/jry_bl_ying 
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_bl_exception.o jry_bl$(H)jry_bl_exception.c $(JRY_BL_EXLIB)
jry_bl/jry_bl_link_list :jry_bl/jry_bl_ying jry_bl/jry_bl_exception jry_bl/jry_bl_malloc jry_bl/jry_bl_var
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_bl_link_list.o jry_bl$(H)jry_bl_link_list.c $(JRY_BL_EXLIB)
jry_bl/jry_bl_malloc	:jry_bl/jry_bl_ying jry_bl/jry_bl_exception
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_bl_malloc.o jry_bl$(H)jry_bl_malloc.c $(JRY_BL_EXLIB)
jry_bl/jry_bl_md5		:jry_bl/jry_bl_ying jry_bl/jry_bl_exception jry_bl/jry_bl_malloc jry_bl/jry_bl_string
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_bl_md5.o jry_bl$(H)jry_bl_md5.c $(JRY_BL_EXLIB)
jry_bl/jry_bl_sha1		:jry_bl/jry_bl_ying jry_bl/jry_bl_exception jry_bl/jry_bl_malloc jry_bl/jry_bl_string
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_bl_sha1.o jry_bl$(H)jry_bl_sha1.c $(JRY_BL_EXLIB)	
jry_bl/jry_bl_string	:jry_bl/jry_bl_ying jry_bl/jry_bl_exception jry_bl/jry_bl_malloc
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_bl_string.o jry_bl$(H)jry_bl_string.c $(JRY_BL_EXLIB)
jry_bl/jry_bl_var		:jry_bl/jry_bl_ying jry_bl/jry_bl_exception jry_bl/jry_bl_malloc jry_bl/jry_bl_string
	$(CC) $(EX) -c -w -std=c99 -o tmp$(H)jry_bl_var.o jry_bl$(H)jry_bl_var.c $(JRY_BL_EXLIB)
jry_bl/jry_bl_ying		:
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_bl_ying.o jry_bl$(H)jry_bl_ying.c $(JRY_BL_EXLIB)
