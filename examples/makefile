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
udp:
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)udpserver.o         examples$(H)udpserver.c	
	$(CC) $(BITS) -o exes$(H)udpserver         tmp$(H)$(pre)udpserver.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)udpclient.o         examples$(H)udpclient.c	
	$(CC) $(BITS) -o exes$(H)udpclient         tmp$(H)$(pre)udpclient.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
