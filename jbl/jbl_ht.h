/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_HT_H
#define __JBL_HT_H
#include "jbl_ht_config.h"
#if JBL_HT_ENABLE==1
/*******************************************************************************************/
/*                            依赖 jbl_gc jbl_malloc void jbl_ying                      */
/*                                 jbl_exception jbl_string                                */
/*******************************************************************************************/
#include "jbl_gc.h"
#include "jbl_malloc.h"
#include "jbl_var.h"
#include "jbl_ying.h"
#include "jbl_string.h"
#include "jbl_exception.h"
/*******************************************************************************************/
/*                            联动 jbl_stream jbl_ll                                      */
/*******************************************************************************************/
#include "jbl_stream.h"
#if JBL_LL_ENABLE==1
typedef	struct	__jbl_ll			jbl_ll;
#endif
/*******************************************************************************************/
/*                            结构体定义                                                  */
/*******************************************************************************************/
typedef struct __jbl_ht_data
{
	jbl_string_hash_type	h;		//hash值缓存,在int类型作为key时直接保存该值
	jbl_ht_size_type		nxt;	//同hash值的下一个元素所在位置
	jbl_string *			k;		//key值,在int类型作为key时为NULL
	union
	{
		void *			v;		//载荷,当有sys标记时不按照var的格式处理
		jbl_pointer_int		u;		//载荷,不能与v同时使用,当有sys标记时可以用来直接储存一个与当前系统位数相同的无符号整数
	};
}jbl_ht_data;						//蒟蒻云基础库hash表数据体结构
typedef struct __jbl_ht
{
	jbl_gc					gc;		//gc结构
#if JBL_VAR_ENABLE==1
	jbl_var_operators *		var_ops;
#endif
	jbl_ht_size_type		size;	//当前已申请的空间
	jbl_ht_size_type		len;	//当前长度
	jbl_ht_size_type		nxt;	//当前最后一个元素所在的位置
	jbl_ht_data *			data;	//数据体指针,特别注意的,该指针不指向获取内存的头部,有一个sizeof(jbl_ht_size_type)*size大小的偏移
}jbl_ht;							//蒟蒻云基础库hash表结构
/*******************************************************************************************/
/*                            以下函实现哈希表基本操作                                   */
/*******************************************************************************************/
jbl_var_operators_extern(jbl_ht_operators);																//hash table 操作器
jbl_ht  *				jbl_ht_new					();													//新建一个hash table
jbl_ht  *				jbl_ht_free					(jbl_ht *this);										//释放一个hash table
jbl_ht  *				jbl_ht_copy					(jbl_ht *that);										//复制一个hash table
#define					jbl_ht_extend(x,y)			jbl_ht_extend_to(x,y,1,NULL)
jbl_ht  *				jbl_ht_extend_to			(jbl_ht *this,jbl_ht_size_type size,jbl_uint8 add,jbl_ht **pthi);				//把一个hash table扩展到size
#define					jbl_ht_foreach(this,i)		for(jbl_ht_data *i=&((jbl_ht *)jbl_refer_pull(this))->data[0],*____i=&(((jbl_ht *)jbl_refer_pull(this)))->data[((jbl_ht *)jbl_refer_pull(this))->nxt];i!=____i;i++)if(i->v!=NULL)	//枚举一个hash table
jbl_ht  *				jbl_ht_rehash				(jbl_ht *this);										//重新hash一个hash table
/*******************************************************************************************/
/*                            以下函实现哈希表插入操作                                   */
/*******************************************************************************************/
jbl_ht  *				jbl_ht_insert				(jbl_ht *this,jbl_string *k,void *v);				//插入一个var
jbl_ht  *				jbl_ht_insert_chars			(jbl_ht *this,unsigned char *kk,void *v);			//以chars为key插入一个var
jbl_ht  *				jbl_ht_insert_int			(jbl_ht *this,jbl_string_hash_type h,void *v);		//以int为key插入一个var
jbl_ht *				jbl_ht_insert_force			(jbl_ht *this,jbl_string_hash_type h,jbl_string *k,void *v);		//强制插入,忽略互斥性
/*******************************************************************************************/
/*                            以下函实现哈希表删除操作                                   */
/*******************************************************************************************/
jbl_ht  *				jbl_ht_unset				(jbl_ht *this,jbl_string *k);						//删除一个var
jbl_ht  *				jbl_ht_unset_chars			(jbl_ht *this,unsigned char *kk);					//以chars为key删除一个var
jbl_ht  *				jbl_ht_unset_int			(jbl_ht *this,jbl_string_hash_type h);				//以int为key获取一个var
/*******************************************************************************************/
/*                            以下函实现哈希表获取操作                                   */
/*******************************************************************************************/
void *					jbl_htv						(jbl_ht_data *node);								//获取一个node的var
jbl_string * 			jbl_htk						(jbl_ht_data *node);								//获取一个node的key
jbl_string_hash_type	jbl_hth						(jbl_ht_data *node);								//获取一个node的hash
void *					jbl_ht_get					(const jbl_ht *this,jbl_string *k);					//获取一个var
void *					jbl_ht_get_chars			(const jbl_ht *this,unsigned char *kk);				//以chars为key获取一个var
void *					jbl_ht_get_int				(const jbl_ht *this,jbl_string_hash_type h);		//以chars为int获取一个var
jbl_ht_data *			jbl_ht_get_ht_data			(const jbl_ht *this,jbl_string *k);					//获取一个ht_data
jbl_ht_data *			jbl_ht_get_ht_data_chars	(const jbl_ht *this,const jbl_uint8 *chars);		//以chars为key获取一个ht_data
jbl_ht_data *			jbl_ht_get_ht_data_int		(const jbl_ht *this,jbl_string_hash_type h);		//以int为key获取一个ht_data
#define					jbl_ht_get_length(x)		(((jbl_ht *)jbl_refer_pull(x))->len)				//获取一个hash table的长度
/*******************************************************************************************/
/*                            以下函实现哈希表比较操作                                   */
/*******************************************************************************************/
char					jbl_ht_space_ship			(const jbl_ht *this,const jbl_ht *that);			//太空船操作符，参见php的太空船操作符
#define					jbl_ht_if_big(x,y)			(jbl_ht_space_ship(x,y)>0)							//判断两个字符串是否相等
#define					jbl_ht_if_equal(x,y)		(jbl_ht_space_ship(x,y)==0)							//判断this是否>that
#define					jbl_ht_if_small(x,y)		(jbl_ht_space_ship(x,y)<0)							//判断this是否<that
#define					jbl_ht_if_equal_small(x,y)	(jbl_ht_space_ship(x,y)<=0)							//判断this是否>=that
#define					jbl_ht_if_equal_big(x,y)	(jbl_ht_space_ship(x,y)>=0)							//判断this是否<=that
/*******************************************************************************************/
/*                            以下函实现哈希表合并操作                                   */
/*******************************************************************************************/
jbl_ht  *				jbl_ht_merge				(jbl_ht *this,jbl_ht *that);						//合并两个hash table 注意this不会复制,而且this，that中key重复的that将覆盖this
jbl_ht  *				jbl_ht_merge_int			(jbl_ht *this);										//把int key合并为string key
#if JBL_LL_ENABLE==1
jbl_ht *				jbl_ht_merge_ll				(jbl_ht *this,jbl_ll *that);						//合并一个link list
#endif
/*******************************************************************************************/
/*                            以下函实现哈希表sys模式操作                                */
/*******************************************************************************************/
#if JBL_HT_SYS_ENABLE==1
#define					jbl_ht_set_sys(x)			(jbl_gc_set_user1((jbl_ht*)jbl_refer_pull(x)))		//设置sys标记
#define					jbl_ht_reset_sys(x)			(jbl_gc_reset_user1((jbl_ht*)jbl_refer_pull(x)))	//删除sys标记
#define					jbl_ht_is_sys(x)			(jbl_gc_is_user1((jbl_ht*)jbl_refer_pull(x)))		//获取sys标记
#else
#define					jbl_ht_reset_sys(x)			(0)													//删除sys标记
#define					jbl_ht_is_sys(x)			(0)													//获取sys标记
#endif
/*******************************************************************************************/
/*                            以下函实现哈希表JSON操作                                   */
/*******************************************************************************************/
#if JBL_JSON_ENABLE==1
jbl_string *			jbl_ht_json_encode			(const jbl_ht* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs);	//JSON 编码
#if JBL_STREAM_ENABLE==1
void					jbl_ht_json_put				(const jbl_ht* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs);	//从out JSON输出一个hash table
#endif
#endif
/*******************************************************************************************/
/*                            以下函实现哈希表浏览操作                                   */
/*******************************************************************************************/
#if JBL_STREAM_ENABLE==1
jbl_ht*					jbl_ht_view_put						(jbl_ht* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从out浏览一个hash table
#define					jbl_ht_view(x)						jbl_ht_view_put(x,jbl_stream_stdout,1,JBL_VIEW_DEFAULT_TABS,__LINE__,UC #x,UC __FUNCTION__,UC __FILE__)//浏览一个hash table
#endif

#endif
#endif