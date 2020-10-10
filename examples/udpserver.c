#include "main.h"
jwl_socket *host=NULL;
jbl_uint64 count=0;
jbl_uint8 buf[65536];
void stop()
{
	host=jwl_socket_free(host);
}

int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jbl_exception_add_exit_function(stop);
	host=jwl_socket_bind(host,jwl_get_binary_ip_chars(UC((argc>1)?argv[1]:"0.0.0.0")),jbl_getuint64(UC((argc>2)?argv[2]:"1217")),JWL_SOCKET_MODE_UDP);
	jwl_socket_view(host);pf();
	


	while(1)
	{
pl();
		jwl_socket * client=NULL;
		jwl_socket_receive_chars(host,buf,65536,&client);
		jbl_stream_push_chars(jbl_stream_stdout,buf);	
		jwl_socket_send_chars(client,UC"wswjldqfq\n",jbl_strlen(UC"wswjldqfq\n"));
		client=jwl_socket_free(client);
	}	




	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_stop();		
	jbl_stop();		
}
