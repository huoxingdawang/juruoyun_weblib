#include "main.h"
#define ip "192.168.1.2"
int main()
{
	printf("--------------------------------" __FILE__ "--------------------------------\n");
	jry_bl_start();
	jry_wl_start();
	jry_bl_string a;jry_bl_string_inits(1,&a);
	jry_bl_string_equal_char_pointer(&a,ip);
	jry_bl_uint64 b;
	printf("%d\n",(inet_addr(ip))==(b=jry_wl_get_binary_ip(&a)));
	jry_bl_string_clear(&a);
	jry_wl_get_string_ip(b,&a);
	jry_bl_string_view(&a,stderr);
	
	jry_bl_string_frees(1,&a);
	printf("\nMEMEORY:%lld\n",jry_bl_malloced_size);		
}
