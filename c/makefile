#   Copyright (c) [2019] juruoyun developer team
#   Juruoyun basic lib is licensed under the Mulan PSL v1.
#   You can use this software according to the terms and conditions of the Mulan PSL v1.
#   You may obtain a copy of Mulan PSL v1 at:
#      http://license.coscl.org.cn$(H)MulanPSL
#   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
#   PURPOSE.
#   See the Mulan PSL v1 for more details.
CC = gcc
EX = -m64
EXLIB = $(JRY_BL_EXLIB) $(JRY_WL_EXLIB)
ifeq ($(shell uname),Linux)
	H = /
	rm = rm -f
else
	H = \\
	rm = del
endif
all:jry_bl jry_wl test socket_client socket_server webserver
clean:
	$(rm) tmp$(H)*.o /s
	$(rm) tmp$(H)*.a /s
	$(rm) exes$(H)* /s /Q
	$(rm) testfiles$(H)*.out /s
	$(rm) testfiles$(H)*.ans /s
	$(rm) testfiles$(H)*.html /s
init:
	mkdir tmp
	mkdir exes
run:
	exes$(H)test
	exes$(H)socket

#examples
test:jry_bl jry_wl
	$(CC) $(EX) -c -std=c99 -o tmp$(H)test.o examples$(H)test.c	
	$(CC) -o exes$(H)test tmp$(H)test.o tmp$(H)jry_wl.a tmp$(H)jry_bl.a $(EXLIB)
socket_server:jry_bl jry_wl
	$(CC) $(EX) -c -std=c99 -o tmp$(H)socket_server.o examples$(H)socket_server.c	
	$(CC) -o exes$(H)socket_server tmp$(H)socket_server.o tmp$(H)jry_wl.a tmp$(H)jry_bl.a $(EXLIB)
socket_client:jry_bl jry_wl
	$(CC) $(EX) -c -std=c99 -o tmp$(H)socket_client.o examples$(H)socket_client.c	
	$(CC) -o exes$(H)socket_client tmp$(H)socket_client.o tmp$(H)jry_wl.a tmp$(H)jry_bl.a $(EXLIB)
webserver:jry_bl jry_wl
	$(CC) $(EX) -c -std=c99 -o tmp$(H)webserver.o examples$(H)webserver.c	
	$(CC) -o exes$(H)webserver tmp$(H)webserver.o tmp$(H)jry_wl.a tmp$(H)jry_bl.a $(EXLIB)
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
#   Copyright (c) [2019] juruoyun developer team
#   Juruoyun web lib is licensed under the Mulan PSL v1.
#   You can use this software according to the terms and conditions of the Mulan PSL v1.
#   You may obtain a copy of Mulan PSL v1 at:
#      http://license.coscl.org.cn$(H)MulanPSL
#   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
#   PURPOSE.
#   See the Mulan PSL v1 for more details.
#jry_wl
ifeq ($(shell uname),Linux)
JRY_WL_EXLIB =
else
JRY_WL_EXLIB = -lwsock32
endif

jry_wl					:jry_bl jry_wl/jry_wl_ying jry_wl/jry_wl_socket jry_wl/jry_wl_exception jry_wl/jry_wl_http
	ar rc tmp$(H)jry_wl.a tmp$(H)jry_wl_*.o
jry_wl/jry_wl_ying		:
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_wl_ying.o jry_wl$(H)jry_wl_ying.c
jry_wl/jry_wl_socket		:
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_wl_socket.o jry_wl$(H)jry_wl_socket.c
jry_wl/jry_wl_exception		:
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_wl_exception.o jry_wl$(H)jry_wl_exception.c
jry_wl/jry_wl_http		:
	$(CC) $(EX) -c -std=c99 -o tmp$(H)jry_wl_http.o jry_wl$(H)jry_wl_http.c
