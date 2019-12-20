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
