#include "main.h"
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
	pchars("--------------------------------" __FILE__ "--------------------------------\n");pf();
	jbl_string *ip=jbl_string_add_chars(NULL,UC((argc>1)?argv[1]:"0.0.0.0"));
	jbl_string *port=jbl_string_add_chars(NULL,UC((argc>2)?argv[2]:"1217"));
	sock=jwl_socket_bind(sock,jwl_get_binary_ip(ip),jbl_string_get_uint(port));
	ip=jbl_string_free(ip);
	port=jbl_string_free(port);	
	jwl_socket_view(sock);pf();


	jbl_uint32 count=0;
	while(1)
	{
		jbl_string *get=NULL;
		jbl_time *t1=NULL,*t2=NULL;	
		jwl_socket *client=jwl_socket_accept(sock);
		jwl_socket_view(client);
		puint(++count);
		t1=jbl_time_now(t1);
		get=jwl_socket_receive_safe(client,NULL);
		pchars("\nreceive used time:");puint(jbl_time_minus((t2=jbl_time_now(t2)),t1));pchars("ms\n");
		puint(jbl_string_get_length(get));pn();		

		t1=jbl_time_now(t1);		
		jwl_socket_send_safe(client,get);
		pchars("\nsend used time:");puint(jbl_time_minus((t2=jbl_time_now(t2)),t1));pchars("ms\n");		
		puint(jbl_string_get_length(get));pn();
				
		
		
		
		t1=jbl_time_free(t1);t2=jbl_time_free(t2);	
		get=jbl_string_free(get);
		client=jwl_socket_free(client);
		pf();
	}
	
	
	sock=jwl_socket_free(sock);
	jwl_stop();
	jbl_stop();		
}
