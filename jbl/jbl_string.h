/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_STRING_H
#define __JBL_STRING_H
#include "jbl_string_config.h"
#if JBL_STRING_ENABLE==1
/*******************************************************************************************/
/*                            依赖 jbl_gc jbl_malloc jbl_ying jbl_exception               */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_gc.h"
#include "jbl_malloc.h"
#include "jbl_ying.h"
/*******************************************************************************************/
/*                            联动 jbl_stream jbl_var jbl_ll jbl_ht                        */
/*******************************************************************************************/
#include "jbl_stream.h"
#include "jbl_ht_config.h"
#if JBL_VAR_ENABLE==1
typedef	struct	__jbl_var_operators	jbl_var_operators;
typedef	struct	__jbl_var 			jbl_var;
#endif
#if JBL_LL_ENABLE==1
typedef	struct	__jbl_ll			jbl_ll;
#endif
/*******************************************************************************************/
/*                            结构体定义                                                  */
/*******************************************************************************************/
typedef struct __jbl_string
{
	jbl_gc 					gc;		//gc结构
	jbl_string_size_type	len;	//当前字符串长度
	jbl_string_size_type	size;	//当前已申请内存长度,如果size<len则该字符串是const,应及时扩容
	jbl_string_hash_type	h;		//hash值缓冲
	unsigned char *			s;		//字符串
}jbl_string;						//蒟蒻云基础库字符串结构
/*******************************************************************************************/
/*                            以下函数处理传统字符串                                     */
/*******************************************************************************************/
jbl_string_size_type	jbl_strlen								(const unsigned char *a);												//获取一个传统char*字符串第一个'\0'所在的位置(即字符串长度)
jbl_string_hash_type	jbl_strhash								(const unsigned char *s,jbl_string_size_type len);						//获取一个char*字符串的hash值，当len是0时将自动使用字符串长度
/*******************************************************************************************/
/*                            以下函数完成字符串组件启动和停止                           */
/*******************************************************************************************/
void					jbl_string_start						();																		//启动string
void					jbl_string_stop							();																		//关闭string
/*******************************************************************************************/
/*                            以下函实现字符串基本操作                                   */
/*******************************************************************************************/
jbl_string *			jbl_string_new							();																		//新建一个字符串
jbl_string * 			jbl_string_init							(jbl_string *this);														//初始化一个字符串
jbl_string *			jbl_string_free							(jbl_string *this);														//释放一个字符串
jbl_string *			jbl_string_copy							(jbl_string *that);														//复制一个字符串
#define					jbl_string_extend(a,b)					jbl_string_extend_to((a),(b),1,NULL)				//扩展字符串ab字节
jbl_string *			jbl_string_extend_to					(jbl_string *this,jbl_string_size_type size,jbl_uint8 add,jbl_string **pth);							//扩展字符串this到size字节
jbl_string *			jbl_string_clear						(jbl_string *this);														//清空一个字符串
/*******************************************************************************************/
/*                            以下函数实现字符串的缓冲操作                               */
/*******************************************************************************************/
#if JBL_HT_ENABLE==1 && JBL_STRING_USE_CACHE==1 && JBL_HT_SYS_ENABLE==1
jbl_string *			jbl_string_cache_get					(const unsigned char *in);
jbl_string *			jbl_string_cache_replace				(jbl_string *str);
#else
#define					jbl_string_cache_get(x)					jbl_string_add_chars(NULL,(x))
#define					jbl_string_cache_replace(x)				(x)
#endif
/*******************************************************************************************/
/*                            以下函数实现字符串的增添类操作                             */
/*******************************************************************************************/
#define					jbl_string_add_const(x,y)				jbl_string_add_const_length(x,y,jbl_strlen(y))							//添加一个常量char*字符串
jbl_string *			jbl_string_add_const_length				(jbl_string *this,const unsigned char *in,jbl_string_size_type len);	//添加一个长度已知常量char*字符串
#define					jbl_string_add_chars(x,y)				jbl_string_add_chars_length(x,y,jbl_strlen(y))							//添加一个char*字符串
jbl_string *			jbl_string_add_chars_length				(jbl_string *this,const unsigned char *in,jbl_string_size_type len);	//添加一个长度已知的char*字符串
jbl_string *			jbl_string_add_string					(jbl_string *this,jbl_string *in);								//添加一个string
jbl_string *			jbl_string_add_char						(jbl_string *this,const char c);										//添加一个char
#define					jbl_string_add_char_force(this,in)		(this->s[this->len++]=(in))												//强制添加一个char，当字符串为NULL或引用类型时将直接导致RE
#define					jbl_string_add_int64(this,in)			jbl_string_add_int64_length(this,in,0,0)								//添加一个64位整数
jbl_string *			jbl_string_add_int64_length				(jbl_string *this,jbl_int64 in,jbl_uint8 len,char c);					//添加一个length位的64位整数，不足用c补齐	
#define					jbl_string_add_uint64(this,in)			jbl_string_add_uint64_length(this,in,0,0)								//添加一个64位无符号整数
jbl_string *			jbl_string_add_uint64_length			(jbl_string *this,jbl_uint64 in,jbl_uint8 len,char c);					//添加一个length位64位无符号整数，不足用c补齐
#define					jbl_string_add_double(this,in)			jbl_string_add_double_length(this,in,5)									//添加一个浮点数
jbl_string *			jbl_string_add_double_length			(jbl_string *this,double in,unsigned char len);							//添加一个浮点数，保留len位小数
jbl_string *			jbl_string_add_hex						(jbl_string *this,jbl_uint64 in);										//把一个64位无符号整数以16进制形式添加(无前导0)
jbl_string *			jbl_string_add_hex_8bits				(jbl_string *this,jbl_uint8 in);										//把一个 8位无符号整数以16进制形式添加(有前导0)
//TOOD START 实现jbl_file后移除这两个函数
#include <stdio.h>
jbl_string *			jbl_string_add_file						(jbl_string *this,FILE * file);
void					jbl_string_print						(jbl_string *this,FILE * file);
//TODO END
/*******************************************************************************************/
/*                            以下函数实现字符串的设置类操作                             */
/*******************************************************************************************/
#define					jbl_string_set_length(a,l)				(((jbl_string*)jbl_refer_pull(a))->len=l)								//设置一个字符串长度
#define					jbl_string_set_length_force(a,l)		((a)->len=l)															//强制获取一个字符串长度，当字符串为NULL或引用类型时将直接导致RE
/*******************************************************************************************/
/*                            以下函数实现字符串的获取类操作                             */
/*******************************************************************************************/
#define					jbl_string_get_length(a)				(a==NULL?0:((jbl_string*)jbl_refer_pull(a))->len)						//获取一个字符串长度
#define					jbl_string_get_length_force(a)			((a)->len)																//强制获取一个字符串长度，当字符串为NULL或引用类型时将直接导致RE
#define					jbl_string_get_chars(a)					(((jbl_string*)jbl_refer_pull(a))->s)									//获取一个字符串的传统字符串部分
#define					jbl_string_get_chars_force(a)			((a)->s)																//强制获取一个字符串的传统字符串部分，当字符串为NULL或引用类型时将直接导致RE
#define					jbl_string_get_size(a)					(((jbl_string*)jbl_refer_pull(a))->size)								//获取一个字符串大小
#define					jbl_string_get_size_force(a)			((a)->size)																//强制获取一个字符串大小，当字符串为NULL或引用类型时将直接导致RE
unsigned char			jbl_string_get							(jbl_string *this,jbl_string_size_type i);						//获取字符串a的第i位的内容(当越界时返回0)
#define					jbl_string_get_force(a,i)				((a)->s[(i)])															//强制获取字符串a的第i位的内容，当字符串为NULL或引用类型或越界时将直接导致RE

#define					jbl_string_get_int64(this)				jbl_string_get_int64_start(this,0)										//获取一个64位整数
jbl_int64				jbl_string_get_int64_start				(jbl_string *this,jbl_string_size_type *start);					//从start位置开始获取一个64位整数
#define					jbl_string_get_uint64(this)				jbl_string_get_uint64_start(this,0)										//获取一个64位无符号整数
jbl_uint64				jbl_string_get_uint64_start				(jbl_string *this,jbl_string_size_type *start);					//从start位置开始获取一个64位无符号整数
#define					jbl_string_get_double(this)				jbl_string_get_double_start(this,0)										//获取一个浮点数
double					jbl_string_get_double_start				(jbl_string *this,jbl_string_size_type *start);					//从start位置开始获取一个浮点数
#define					jbl_string_get_hex(this)				jbl_string_get_hex_start(this,0)										//获取一个16进制表示的64位无符号整数
jbl_uint64				jbl_string_get_hex_start				(jbl_string *this,jbl_string_size_type *start);					//从start位置开始获取一个16进制表示的64位无符号整数						
jbl_uint64				jbl_string_get_hex_start_len			(jbl_string *this,jbl_string_size_type *start,jbl_string_size_type len);	//从start位置开始获取一个16进制表示的,长度为len的64位无符号整数
/*******************************************************************************************/
/*                            以下函数实现字符串的比较类操作                             */
/*******************************************************************************************/
char					jbl_string_space_ship					(jbl_string *this_,jbl_string *that_);						//太空船操作符，参见php的太空船操作符
char					jbl_string_space_ship_chars				(jbl_string *this,const char *that);								//和char*字符串的太空船操作符，参见php的太空船操作符
char					jbl_string_if_equal						(jbl_string *this_,jbl_string *that_);						//判断两个字符串是否相等，这里使用了hash进行判断(如果存在)
#define					jbl_string_if_big(x,y)					(jbl_string_space_ship(x,y)>0)											//判断this是否>that
#define					jbl_string_if_small(x,y)				(jbl_string_space_ship(x,y)<0)											//判断this是否<that
#define					jbl_string_if_equal_small(x,y)			(jbl_string_space_ship(x,y)<=0)											//判断this是否>=that
#define					jbl_string_if_equal_big(x,y)			(jbl_string_space_ship(x,y)>=0)											//判断this是否<=that
/*******************************************************************************************/
/*                            以下函数实现字符串的查找类操作                             */
/*******************************************************************************************/
#define					jbl_string_find_char(this,in)			jbl_string_find_char_start(this,in,0)									//寻找第一个in
jbl_string_size_type	jbl_string_find_char_start				(jbl_string *this,unsigned char in,jbl_string_size_type start);	//从start位开始寻找第一个in
/*******************************************************************************************/
/*                            以下函数实现字符串的hash操作                               */
/*******************************************************************************************/
jbl_string_hash_type	jbl_string_hash							(jbl_string *this);														//hash一个字符串
#define					jbl_string_hash_clear(x)				((((jbl_string*)jbl_refer_pull(x))->h=0))								//清楚hash缓存
/*******************************************************************************************/
/*                            以下函数实现字符串的修改操作                               */
/*******************************************************************************************/
jbl_string *			jbl_string_to_upper_case				(jbl_string *this);														//转成大写字母
jbl_string *			jbl_string_to_lower_case				(jbl_string *this);														//转成小写字母


#if JBL_JSON_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的JSON操作                               */
/*******************************************************************************************/
jbl_string*				jbl_string_json_encode					(jbl_string* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs);			//JSON编码
jbl_string*				jbl_string_json_put_format				(const void* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs);			//输出JSON格式头
jbl_string*				jbl_string_json_decode					(jbl_string *this,jbl_string* in,jbl_string_size_type *start);				//JSON解码
#if JBL_STREAM_ENABLE==1
void					jbl_string_json_put						(jbl_string* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs);			//从从out JSON格式化一个字符串
#endif
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的浏览操作                               */
/*******************************************************************************************/
jbl_string*				jbl_string_view_put						(jbl_string* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从out浏览一个字符串
#define					jbl_string_view(x)						jbl_string_view_put(x,jbl_stream_stdout,1,JBL_VIEW_DEFAULT_TABS,__LINE__,UC #x,UC __FUNCTION__,UC __FILE__)//浏览一个字符串
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的STREAM操作                             */
/*******************************************************************************************/
extern const 			jbl_stream_operater						jbl_stream_string_operators;											//string的流处理器
jbl_stream *			jbl_string_stream_new					(jbl_string *str);														//新建一个字符串流(不进行复制操作)
#define					jbl_string_stream_free(a)				jbl_stream_free(a)														//释放一个字符串流
#if JBL_VAR_ENABLE==1
jbl_var * 				jbl_string_Vstream_new					(jbl_string *str);														//新建一个var格式的字符串流(不进行复制操作)
#endif
void					jbl_stream_push_string					(jbl_stream *out,jbl_string* this);										//向out推出一个字符串
jbl_string *			jbl_string_read							(jbl_string *this,const unsigned char *c);

#endif
#if JBL_VAR_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现字符串的var操作                                */
/*******************************************************************************************/
extern	const		jbl_var_operators							jbl_string_operators;													//string 操作器
jbl_string *		jbl_Vstring									(jbl_var * this);														//以string的格式使用var
#define				Vis_jbl_string(x)							(jbl_var_get_operators(x)==&jbl_string_operators)						//判断一个var是不是string
jbl_var *			jbl_Vstring_new								();																		//新建一个字符串类型的var
jbl_var *			jbl_string_get_number_start					(jbl_string *this,jbl_string_size_type *start);					//获取一个数字，支持无符号整数，有符号整数，浮点数，e表示的浮点数
#define				jbl_string_copy_as_var(x)					jbl_var_copy_as(x,&jbl_string_operators)								//复制为一个var
#endif
/*******************************************************************************************/
/*                            以下函数实现字符串的切割操作                               */
/*******************************************************************************************/
#if JBL_LL_ENABLE==1
jbl_ll*				jbl_string_cut_start						(jbl_string *this,jbl_ll *list,char cut,jbl_string_size_type start);	//从start开始用cut切割一个字符串
#define				jbl_string_cut(x,y,z)						jbl_string_cut_start(x,y,z,0)											//用cut切割一个字符串
#endif
#else
#define				jbl_string_start()
#define				jbl_string_stop()
#endif	
#endif
