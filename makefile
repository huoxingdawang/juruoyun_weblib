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
BITS = -g
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
all:jbl jwl test2 webserver ip2region request
clean:
	$(rm) tmp$(H)* /s /Q
	$(rm) exes$(H)* /s /Q
	$(rm) testfiles$(H)*.out /s
	$(rm) testfiles$(H)*.ans /s
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
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)test2.o             examples$(H)test2.c	
	$(CC) $(BITS) -o exes$(H)test2            tmp$(H)$(pre)test2.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
webserver:
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)webserver.o         examples$(H)webserver.c	
	$(CC) $(BITS) -o exes$(H)webserver        tmp$(H)$(pre)webserver.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
dns:
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)dns.o               examples$(H)dns.c	
	$(CC) $(BITS) -o exes$(H)dns              tmp$(H)$(pre)dns.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
request:
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)request.o           examples$(H)request.c	
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)request_proxy.o     examples$(H)request_proxy.c	
	$(CC) $(BITS) -o exes$(H)request           tmp$(H)$(pre)request.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
	$(CC) $(BITS) -o exes$(H)request_proxy     tmp$(H)$(pre)request_proxy.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
ip2region:
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)ip2region.o         examples$(H)ip2region.c	
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)ip2region_maker.o   examples$(H)ip2region_maker.c	
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)cip.o               examples$(H)cip.c	
	$(CC) $(BITS) -o exes$(H)ip2region         tmp$(H)$(pre)ip2region.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
	$(CC) $(BITS) -o exes$(H)ip2region_maker   tmp$(H)$(pre)ip2region_maker.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
	$(CC) $(BITS) -o exes$(H)cip               tmp$(H)$(pre)cip.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
websocket:
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)websocket_client.o  examples$(H)websocket_client.c	
	$(CC) $(BITS) -o exes$(H)websocket_client  tmp$(H)$(pre)websocket_client.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
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
jbl                       :jbl/jbl_aes jbl/jbl_array jbl/jbl_base64 jbl/jbl_bitset jbl/jbl_cmd jbl/jbl_exception jbl/jbl_endian jbl/jbl_file jbl/jbl_gc jbl/jbl_ht jbl/jbl_json jbl/jbl_ll jbl/jbl_log jbl/jbl_malloc jbl/jbl_md5 jbl/jbl_rand jbl/jbl_scanner jbl/jbl_sha1 jbl/jbl_stream jbl/jbl_string jbl/jbl_time jbl/jbl_var jbl/jbl_ying 
	ar  rc tmp$(H)$(pre)jbl.a tmp$(H)$(pre)jbl_*.o
jbl/jbl_aes            :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_aes.o         jbl$(H)jbl_aes.c         $(JBL_EXLIB)
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_aes_soft.o    jbl$(H)jbl_aes_soft.c    $(JBL_EXLIB)
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_aes_hard.o    jbl$(H)jbl_aes_hard.c    $(JBL_EXLIB) -maes
jbl/jbl_array          :
	$(CC) $(BITS) -c       -o tmp$(H)$(pre)jbl_array.o       jbl$(H)jbl_array.c       $(JBL_EXLIB)
jbl/jbl_base64         :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_base64.o      jbl$(H)jbl_base64.c      $(JBL_EXLIB)
jbl/jbl_bitset         :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_bitset.o      jbl$(H)jbl_bitset.c      $(JBL_EXLIB)
jbl/jbl_cmd            :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_cmd.o         jbl$(H)jbl_cmd.c         $(JBL_EXLIB)
jbl/jbl_endian         :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_endian.o      jbl$(H)jbl_endian.c      $(JBL_EXLIB)
jbl/jbl_exception      :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_exception.o   jbl$(H)jbl_exception.c   $(JBL_EXLIB)
jbl/jbl_file           :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_file.o        jbl$(H)jbl_file.c        $(JBL_EXLIB)
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_file_ct.o     jbl$(H)jbl_file_ct.c     $(JBL_EXLIB)
jbl/jbl_gc             :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_gc.o          jbl$(H)jbl_gc.c          $(JBL_EXLIB)
jbl/jbl_ht             :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_ht.o          jbl$(H)jbl_ht.c          $(JBL_EXLIB)
jbl/jbl_json           :
ifeq ($(system),linux)
ifeq ($(findstring jbl,$(complain_re2c)),jbl)
	re2c -f -c jbl$(H)jbl_json.l -o jbl$(H)jbl_json.c
endif
endif
ifeq ($(system),macos)
ifeq ($(findstring jbl,$(complain_re2c)),jbl)
	re2c -f -c jbl$(H)jbl_json.l -o jbl$(H)jbl_json.c
endif
endif
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_json.o        jbl$(H)jbl_json.c        $(JBL_EXLIB)
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
jbl/jbl_scanner           :
ifeq ($(system),linux)
ifeq ($(findstring jbl,$(complain_re2c)),jbl)
	re2c -o jbl$(H)jbl_scanner.c jbl$(H)jbl_scanner.l
endif
endif
ifeq ($(system),macos)
ifeq ($(findstring jbl,$(complain_re2c)),jbl)
	re2c -o jbl$(H)jbl_scanner.c jbl$(H)jbl_scanner.l
endif
endif
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_scanner.o     jbl$(H)jbl_scanner.c     $(JBL_EXLIB)  
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
ifeq ($(system),macos)
ifeq ($(findstring jbl,$(complain_re2c)),jbl)
	re2c -o jbl$(H)jbl_time.c jbl$(H)jbl_time.l
endif
endif
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jbl_time.o       jbl$(H)jbl_time.c        $(JBL_EXLIB)
jbl/jbl_var            :
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

jwl					:jwl/jwl_ying jwl/jwl_dns jwl/jwl_socket jwl/jwl_http jwl/jwl_websocket jwl/jwl_ip2region
	ar rc tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jwl_*.o
jwl/jwl_ying		:
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jwl_ying.o       jwl$(H)jwl_ying.c      $(JWL_EXLIB)
jwl/jwl_dns			:
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jwl_dns.o        jwl$(H)jwl_dns.c       $(JWL_EXLIB)
jwl/jwl_socket		:
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jwl_socket.o     jwl$(H)jwl_socket.c    $(JWL_EXLIB)
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jwl_socket_poll.o     jwl$(H)jwl_socket_poll.c    $(JWL_EXLIB)
jwl/jwl_http		:
ifeq ($(system),linux)
ifeq ($(findstring jwl,$(complain_re2c)),jwl)
	re2c -f -c jwl$(H)jwl_http.l -o jwl$(H)jwl_http.c
endif
endif
ifeq ($(system),macos)
ifeq ($(findstring jwl,$(complain_re2c)),jwl)
	re2c -f -c jwl$(H)jwl_http.l -o jwl$(H)jwl_http.c
endif
endif
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jwl_http.o       jwl$(H)jwl_http.c      $(JWL_EXLIB)
jwl/jwl_websocket   :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jwl_websocket.o  jwl$(H)jwl_websocket.c $(JWL_EXLIB)
jwl/jwl_ip2region   :
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jwl_ip2region.o  jwl$(H)jwl_ip2region.c $(JWL_EXLIB)
