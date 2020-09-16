#include "main.h"


int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	pchars("--------------------------------" __FILE__ "--------------------------------\n");


	jbl_string_free(jbl_string_view(jwl_get_string_ip(jwl_dns_get_ip_by_domin_chars(UC argv[1],JWB_DNS_QUERY_TYPE_A),NULL)));




	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_stop();		
	jbl_stop();		
}
