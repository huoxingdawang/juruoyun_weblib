#include "main.h"
int main(int argc,char** argv)
{
	jbl_start();
	jwl_start();
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_http_resh *resh1=jwl_http_resh_new();
	
	resh1=jwl_http_resh_free(resh1);
	jwl_stop();		
	jbl_stop();		
}
