#include "main.h"
int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	while(1)
	{
		jbl_string* ip=jbl_string_read(NULL,UC"input:");
		if(!ip->len)
		{
			ip=jbl_string_free(ip);
			break;
		}
		jbl_string * s1=jwl_ip2region(jwl_get_binary_ip(ip),NULL);
		jbl_string_view(s1);
		s1=jbl_string_free(s1);
		ip=jbl_string_free(ip);
	}
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_stop();		
	jbl_stop();		
}
