#include "main.h"
jbl_uint8 buf[65536];
int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_socket * socket=jwl_socket_connect(NULL,jwl_get_binary_ip_chars(UC((argc>1)?argv[1]:"127.0.0.1")),jbl_getuint64(UC((argc>2)?argv[2]:"1217")),JWL_SOCKET_MODE_UDP);
	jwl_socket_view(socket);pf();
	jwl_socket_send_chars(socket,UC"wjlswdhez\n",jbl_strlen(UC"wjlswdhez\n"));
	jwl_socket_receive_chars(socket,buf,65536,NULL);
	jbl_stream_push_chars(jbl_stream_stdout,buf);	
	socket=jwl_socket_free(socket);


	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_stop();		
	jbl_stop();		
}
