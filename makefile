#   Copyright (c) [2019] juruoyun developer team
#   Juruoyun basic lib is licensed under the Mulan PSL v1.
#   You can use this software according to the terms and conditions of the Mulan PSL v1.
#   You may obtain a copy of Mulan PSL v1 at:
#      http://license.coscl.org.cn$(H)MulanPSL
#   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER BITSPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
#   PURPOSE.
#   See the Mulan PSL v1 for more details.
CC = gcc
BITS =
EXLIB = $(JBL_EXLIB) $(JWL_EXLIB)
complain_re2c = jwl
ifeq ($(shell uname),Linux)
	system = linux
else
ifeq ($(shell uname),Darwin)
	system = macos
else	
	system = windows
endif
endif

ifeq ($(system),linux)
	H = /
	rm = rm -f
	pre = linux_
endif
ifeq ($(system),windows)
	H = \\
	rm = del
	pre = win_
endif
ifeq ($(system),macos)
	H = /
	rm = rm -f
	pre = macos_
endif
all:jbl jwl socket_client socket_server test2 webserver
clean:
	$(rm) tmp$(H)* /s /Q
	$(rm) exes$(H)* /s /Q
	$(rm) testfiles$(H)*.out /s
	$(rm) testfiles$(H)*.ans /s
	$(rm) testfiles$(H)*.html /s
init:
	mkdir tmp
	mkdir exes
ifeq ($(system),linux)
	sudo gcc tools/pause.c -o /bin/pause
endif
run:
	exes$(H)test    &&pause
	exes$(H)socket  &&pause

#examples
test2:
	$(CC) $(BITS) -c -o tmp$(H)$(pre)test2.o examples$(H)test2.c	
	$(CC) $(BITS) -o exes$(H)test2 tmp$(H)$(pre)test2.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
socket_server:
	$(CC) $(BITS) -c -o tmp$(H)$(pre)socket_server.o examples$(H)socket_server.c	
	$(CC) $(BITS) -o exes$(H)socket_server tmp$(H)$(pre)socket_server.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
socket_client:
	$(CC) $(BITS) -c -o tmp$(H)$(pre)socket_client.o examples$(H)socket_client.c	
	$(CC) $(BITS) -o exes$(H)socket_client tmp$(H)$(pre)socket_client.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
webserver:
	$(CC) $(BITS) -c -o tmp$(H)$(pre)webserver.o examples$(H)webserver.c	
	$(CC) $(BITS) -o exes$(H)webserver tmp$(H)$(pre)webserver.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
request:
	$(CC) $(BITS) -c -o tmp$(H)$(pre)request.o examples$(H)request.c	
	$(CC) $(BITS) -o exes$(H)request tmp$(H)$(pre)request.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
#   Copyright (c) [2020] juruoyun developer team
#   Juruoyun basic lib is licensed under the Mulan PSL v1.
#   You can use this software according to the terms and conditions of the Mulan PSL v1.
#   You may obtain a copy of Mulan PSL v1 at:
#      http://license.coscl.org.cn$(H)MulanPSL
#   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER BITSPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
#   PURPOSE.
#   See the Mulan PSL v1 for more details.
#jbl
ifeq ($(system),linux)
JBL_EXLIB = 
endif
ifeq ($(system),windows)
JBL_EXLIB = 
endif
jbl                       :jbl/jbl_aes jbl/jbl_array jbl/jbl_base64 jbl/jbl_bitset jbl/jbl_cmd jbl/jbl_exception jbl/jbl_file jbl/jbl_gc jbl/jbl_ht jbl/jbl_ll jbl/jbl_log jbl/jbl_malloc jbl/jbl_md5 jbl/jbl_rand jbl/jbl_sha1 jbl/jbl_stream jbl/jbl_string jbl/jbl_time jbl/jbl_var jbl/jbl_ying 
	ar  rc tmp$(H)$(pre)jbl.a tmp$(H)$(pre)jbl_*.o
jbl/jbl_aes            :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_aes.o         jbl$(H)jbl_aes.c         $(JBL_EXLIB)
jbl/jbl_array          :
	$(CC) $(BITS) -c       -o tmp$(H)$(pre)jbl_array.o       jbl$(H)jbl_array.c       $(JBL_EXLIB)
jbl/jbl_base64         :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_base64.o      jbl$(H)jbl_base64.c      $(JBL_EXLIB)
jbl/jbl_bitset         :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_bitset.o      jbl$(H)jbl_bitset.c      $(JBL_EXLIB)
jbl/jbl_cmd            :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_cmd.o         jbl$(H)jbl_cmd.c         $(JBL_EXLIB)
jbl/jbl_exception      :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_exception.o   jbl$(H)jbl_exception.c   $(JBL_EXLIB)
jbl/jbl_file           :
	$(CC) $(BITS) -c       -o tmp$(H)$(pre)jbl_file.o        jbl$(H)jbl_file.c        $(JBL_EXLIB)
jbl/jbl_gc             :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_gc.o          jbl$(H)jbl_gc.c          $(JBL_EXLIB)
jbl/jbl_ht             :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_ht.o          jbl$(H)jbl_ht.c          $(JBL_EXLIB)
jbl/jbl_ll             :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_ll.o          jbl$(H)jbl_ll.c          $(JBL_EXLIB)
jbl/jbl_log             :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_log.o         jbl$(H)jbl_log.c         $(JBL_EXLIB)
jbl/jbl_malloc         :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_malloc.o      jbl$(H)jbl_malloc.c      $(JBL_EXLIB)
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_malloc_fast.o jbl$(H)jbl_malloc_fast.c $(JBL_EXLIB)
jbl/jbl_md5            :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_md5.o         jbl$(H)jbl_md5.c         $(JBL_EXLIB)
jbl/jbl_rand           :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_rand.o        jbl$(H)jbl_rand.c        $(JBL_EXLIB)
jbl/jbl_sha1           :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_sha1.o        jbl$(H)jbl_sha1.c        $(JBL_EXLIB)    
jbl/jbl_stream         :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_stream.o      jbl$(H)jbl_stream.c      $(JBL_EXLIB)
jbl/jbl_string         :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_string.o      jbl$(H)jbl_string.c      $(JBL_EXLIB)
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_string_cc.o   jbl$(H)jbl_string_cc.c   $(JBL_EXLIB)
jbl/jbl_time           :
ifeq ($(system),linux)
ifeq ($(findstring jbl,$(complain_re2c)),jbl)
	re2c -o jbl$(H)jbl_time.c jbl$(H)jbl_time.l
endif
endif
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_time.o       jbl$(H)jbl_time.c        $(JBL_EXLIB)
jbl/jbl_var            :
ifeq ($(system),linux)
ifeq ($(findstring jbl,$(complain_re2c)),jbl)
	re2c -o jbl$(H)jbl_var.c jbl$(H)jbl_var.l
endif
endif
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_var.o        jbl$(H)jbl_var.c         $(JBL_EXLIB)
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_var_data.o   jbl$(H)jbl_var_data.c    $(JBL_EXLIB)
jbl/jbl_ying           :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_ying.o       jbl$(H)jbl_ying.c        $(JBL_EXLIB)
#   Copyright (c) [2019] juruoyun developer team
#   Juruoyun web lib is licensed under the Mulan PSL v1.
#   You can use this software according to the terms and conditions of the Mulan PSL v1.
#   You may obtain a copy of Mulan PSL v1 at:
#      http://license.coscl.org.cn$(H)MulanPSL
#   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
#   PURPOSE.
#   See the Mulan PSL v1 for more details.
#jwl
ifeq ($(system),linux)
JWL_EXLIB =
endif
ifeq ($(system),macos)
JWL_EXLIB =
endif
ifeq ($(system),windows)
JWL_EXLIB = -lwsock32
endif

jwl					:jwl/jwl_ying jwl/jwl_socket jwl/jwl_http
	ar rc tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jwl_*.o
jwl/jwl_ying		:
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jwl_ying.o    jwl$(H)jwl_ying.c   $(JWL_EXLIB)
jwl/jwl_socket		:
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jwl_socket.o  jwl$(H)jwl_socket.c $(JWL_EXLIB)
jwl/jwl_http		:
ifeq ($(system),linux)
ifeq ($(findstring jwl,$(complain_re2c)),jwl)
	re2c -o jwl$(H)jwl_http.c jwl$(H)jwl_http.l
endif
endif
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jwl_http.o    jwl$(H)jwl_http.c   $(JWL_EXLIB)
