#include "main.h"
int main()
{
	printf("--------------------------------" __FILE__ "--------------------------------\n");
	jbl_start();
	jwl_start();
	jbl_string *s1=jbl_string_add_chars(NULL,UC
		"GET / HTTP/1.1\r\n"
		"Host: test0.juruoyun.top:1217\r\n"
		"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36\r\n"
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
		"Accept-Encoding: gzip, deflate\r\n"
		"Accept-Language: zh-CN,zh;q=0.9\r\n"
		"Cache-Control: max-age=0\r\n"
		"Referer: http://test0.juruoyun.top/\r\n"
		"Cookie: id=1; code=7FRWuMuuKnotkD2S4qqydnMwQrEJV4HykR0bmmjpfx41ChsWdw4540709f5c6c74b70576c38cf7f7608d55b974d6625187f24fe780700e42bd57\r\n"
		"Upgrade-Insecure-Requests: 1\r\n");
	jwl_http_reqh * reqh=jwl_http_reqh_decode(s1,NULL);
	s1=jbl_string_free(s1);
	jwl_http_reqh_view(reqh);
	reqh=jwl_http_reqh_free(reqh);
	pchars("--------------------------------" __FILE__ "--------------------------------\n");
	jwl_stop();
	jbl_stop(); 	
}
