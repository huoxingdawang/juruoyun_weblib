#include "main.h"

int main(int argc,char** argv)
{
	printf("--------------------------------" __FILE__ "--------------------------------\n");
	jry_bl_start();
	jry_wl_start();	
	jry_bl_string ip,data;jry_bl_string_inits(2,&ip,&data);
	jry_bl_string_equal_char_pointer_light(&ip,"0.0.0.0");
	jry_bl_socket_handle sock,client;jry_wl_socket_init(&sock,jry_wl_get_binary_ip(&ip),1217,JRY_BL_SOCKET_MODE_SERVER);
	

	int count=0;
	while(1)
	{
		jry_wl_socket_accept(&sock,&client);
		printf("%d\n",++count);
		jry_wl_socket_receive_safe(&client,&data);
		jry_bl_string_view(&data,stderr);
		jry_bl_string_clear(&data);
		jry_wl_socket_send_safe(&client,&ip);
		jry_wl_socket_close(&client);		
	}	
	jry_wl_socket_close(&sock);	
	jry_bl_string_frees(2,&ip,&data);
	printf("\nMEMEORY:%lld\n",jry_bl_malloced_size);	
}
