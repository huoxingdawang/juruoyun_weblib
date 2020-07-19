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
ifeq ($(shell uname),Linux)
	H = /
	rm = rm -f
	pre = linux_
else
	H = \\
	rm = del
	pre = win_
endif
all:jbl jwl test socket_client socket_server
clean:
	$(rm) tmp$(H)* /s /Q
	$(rm) exes$(H)* /s /Q
	$(rm) testfiles$(H)*.out /s
	$(rm) testfiles$(H)*.ans /s
	$(rm) testfiles$(H)*.html /s
init:
	mkdir tmp
	mkdir exes
ifeq ($(shell uname),Linux)
	sudo gcc tools/pause.c -o /bin/pause
endif
run:
	exes$(H)test    &&pause
	exes$(H)socket  &&pause

#examples
test:
	$(CC) $(BITS) -c -std=gnu99 -o tmp$(H)$(pre)test.o examples$(H)test.c	
	$(CC) $(BITS) -o exes$(H)test tmp$(H)$(pre)test.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
socket_server:
	$(CC) $(BITS) -c -std=gnu99 -o tmp$(H)$(pre)socket_server.o examples$(H)socket_server.c	
	$(CC) $(BITS) -o exes$(H)socket_server tmp$(H)$(pre)socket_server.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
socket_client:
	$(CC) $(BITS) -c -std=gnu99 -o tmp$(H)$(pre)socket_client.o examples$(H)socket_client.c	
	$(CC) $(BITS) -o exes$(H)socket_client tmp$(H)$(pre)socket_client.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
webserver:
	$(CC) $(BITS) -c -std=gnu99 -o tmp$(H)$(pre)webserver.o examples$(H)webserver.c	
	$(CC) $(BITS) -o exes$(H)webserver tmp$(H)$(pre)webserver.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
request:
	$(CC) $(BITS) -c -std=gnu99 -o tmp$(H)$(pre)request.o examples$(H)request.c	
	$(CC) $(BITS) -o exes$(H)request tmp$(H)$(pre)request.o tmp$(H)$(pre)jwl.a tmp$(H)$(pre)jbl.a $(EXLIB)
