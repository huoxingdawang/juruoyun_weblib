#include "main.h"

int main(int argc,char** argv)
{
	printf("--------------------------------" __FILE__ "--------------------------------\n");
	jry_bl_start();
	jry_wl_start();
	jry_bl_string ip,data;jry_bl_string_inits(2,&ip,&data);
	if(argc==1)
		jry_bl_string_equal_char_pointer_light(&ip,"127.0.0.1");
	else
		jry_bl_string_equal_char_pointer_light(&ip,argv[1]);
	jry_bl_string_equal_char_pointer(&data,"juruoyun");
	for(int i=0;i<5;i++)
		jry_bl_string_add_string(&data,&data);
	jry_bl_string_view(&data,stderr);
	jry_bl_socket_handle sock;jry_wl_socket_init(&sock,jry_wl_get_binary_ip(&ip),1217,JRY_BL_SOCKET_MODE_CLIENT);
	jry_wl_socket_send(&sock,&data);
	jry_bl_string_clear(&data);
	jry_wl_socket_receive(&sock,&data);
	jry_bl_string_view(&data,stderr);
	
	jry_wl_socket_close(&sock);
	jry_bl_string_frees(2,&ip,&data);
	printf("\nMEMEORY:%lld\n",jry_bl_malloced_size);	
}
