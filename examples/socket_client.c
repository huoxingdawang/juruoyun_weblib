#include "main.h"
#include "time.h"
jwl_socket *sock=NULL;
void stop()
{
	sock=jwl_socket_free(sock);
}
int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	jbl_exception_add_exit_function(stop);
	jbl_rand_srand(time(0));
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jbl_string *ip=jbl_string_add_chars(NULL,UC((argc>1)?argv[1]:"127.0.0.1"));
	jbl_string *port=jbl_string_add_chars(NULL,UC((argc>2)?argv[2]:"1217"));

	for(int i=0;i<1;++i)
	{
		jbl_string *send=jbl_rand_string(NULL,1024*1024,UC jbl_rand_dict_small jbl_rand_dict_big  jbl_rand_dict_number jbl_rand_dict_symbol);
		jbl_time *t1=NULL,*t2=NULL;
//		jbl_string_view(send);
		sock=jwl_socket_connect(sock,jwl_get_binary_ip(ip),jbl_string_get_uint(port));
		t1=jbl_time_now(t1);		
		jwl_socket_send_safe(sock,send);
		pchars("\nsend used time:");puint(jbl_time_minus((t2=jbl_time_now(t2)),t1));pchars("ms\n");		
		puint(jbl_string_get_length(send));pn();
		send=jbl_string_free(send);
		t1=jbl_time_now(t1);
		send=jwl_socket_receive_safe(sock,NULL);
		pchars("\nreceive used time:");puint(jbl_time_minus((t2=jbl_time_now(t2)),t1));pchars("ms\n");
		puint(jbl_string_get_length(send));pn();				
		
		
		t1=jbl_time_free(t1);t2=jbl_time_free(t2);
		sock=jwl_socket_free(sock);
		send=jbl_string_free(send);
	}

	ip=jbl_string_free(ip);
	port=jbl_string_free(port);
	
	jwl_stop();		
	jbl_stop();		
}
