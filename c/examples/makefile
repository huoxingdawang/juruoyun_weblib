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
