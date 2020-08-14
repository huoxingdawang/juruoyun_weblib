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
	re2c -c jwl$(H)jwl_http.l -o jwl$(H)jwl_http.c
endif
endif
	$(CC) $(BITS) -c -Wall -o tmp$(H)$(pre)jwl_http.o    jwl$(H)jwl_http.c   $(JWL_EXLIB)
