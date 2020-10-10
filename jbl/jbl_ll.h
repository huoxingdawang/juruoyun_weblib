/* Copyright (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      http://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_LL_H
#define __JBL_LL_H
#include "jbl_ll_config.h"
#if JBL_LL_ENABLE==1
/*******************************************************************************************/
/*                            依赖 jbl_gc jbl_malloc jbl_ying jbl_exception jbl_var        */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_gc.h"
#include "jbl_malloc.h"
#include "jbl_ying.h"
#include "jbl_var.h"
#include "jbl_pthread.h"
/*******************************************************************************************/
/*                            联动jbl_stream jbl_string jbl_ht                             */
/*******************************************************************************************/
#include "jbl_string.h"
#include "jbl_stream.h"
#if JBL_HT_ENABLE==1
typedef struct __jbl_ht jbl_ht;
#endif
/*******************************************************************************************/
/*                            结构体定义                                                  */
/*******************************************************************************************/
typedef struct __jbl_ll_node
{
	struct __jbl_ll_node *	pre;	//上一个
	struct __jbl_ll_node *	nxt;	//下一个
	void *				v;		//载荷
}jbl_ll_node;
typedef struct __jbl_ll
{
	jbl_gc				gc;			//gc结构
	jbl_var_ops_define			;
	jbl_pthread_lock_define		;
	jbl_ll_node *		head;		//链头
	jbl_ll_node *		tail;		//链尾
	jbl_ll_size_type	len;		//长度
}jbl_ll;
/*******************************************************************************************/
/*                            以下函实现链表基本操作                                      */
/*******************************************************************************************/
jbl_var_operators_extern(jbl_ll_operators);
jbl_ll  *		jbl_ll_new					();																//新建一个link list
jbl_ll  *		jbl_ll_free					(jbl_ll *this);													//释放一个link list
jbl_ll  *		jbl_ll_copy					(jbl_ll *that);													//复制一个link list
jbl_ll  *		jbl_ll_extend				(jbl_ll *this,jbl_ll_node **a,jbl_ll_node **b,jbl_ll **pthi);	//分离一个link list
#define 		jbl_ll_foreach(x,y)			for(jbl_ll_node *(y)=((jbl_ll *)jbl_refer_pull(x))->head;(y);(y)=(y)->nxt)	//枚举一个link list(不支持删除,会re)
#define 		jbl_ll_foreach_del(x,y,z)	for(jbl_ll_node *(y)=((jbl_ll *)jbl_refer_pull(x))->head,*(z)=((y)==NULL?NULL:(y)->nxt);(y)!=NULL;(y)=(z),(z)=((y)==NULL?NULL:(y)->nxt))	//枚举一个link list(支持删除操作)
/*******************************************************************************************/
/*                            以下函实现链表插入操作                                     */
/*******************************************************************************************/
jbl_ll  *		jbl_ll_insert				(jbl_ll *this,void *var,jbl_ll_node *after);					//在after后面插入一个var
#define			jbl_ll_add(x,y)				jbl_ll_insert(x,y,x?((jbl_ll *)jbl_refer_pull(x))->tail:NULL)	//在link list最后插入一个var
/*******************************************************************************************/
/*                            以下函实现链表合并操作                                     */
/*******************************************************************************************/
jbl_ll  *		jbl_ll_merge				(jbl_ll *this,jbl_ll *that);									//合并两个link list(直接追加)
#if JBL_HT_ENABLE==1
jbl_ll *		jbl_ll_merge_ht				(jbl_ll *this,jbl_ht *that);				//合并一个hash table 
#endif
/*******************************************************************************************/
/*                            以下函实现链表删除操作                                     */
/*******************************************************************************************/
jbl_ll  *		jbl_ll_delete				(jbl_ll *this,jbl_ll_node *node);								//删除node
#define			jbl_ll_delete_head(x)		jbl_ll_delete(x,((jbl_ll *)jbl_refer_pull(x))->head)			//删除头
#define			jbl_ll_delete_tail(x)		jbl_ll_delete(x,((jbl_ll *)jbl_refer_pull(x))->tail)			//删除尾
/*******************************************************************************************/
/*                            以下函实现链表获取操作                                      */
/*******************************************************************************************/
#define			jbl_ll_get_length(x)		(((jbl_ll *)jbl_refer_pull(x))->len)							//获取一个link list的长度
void *			jbl_llv						(jbl_ll_node *node);											//获取node的值
/*******************************************************************************************/
/*                            以下函实现链表交换操作                                     */
/*******************************************************************************************/
jbl_ll  *		jbl_ll_swap_node			(jbl_ll *this,jbl_ll_node *a,jbl_ll_node *b);					//交换两个节点
/*******************************************************************************************/
/*                            以下函实现链表比较操作                                     */
/*******************************************************************************************/
char			jbl_ll_space_ship			(jbl_ll *this,jbl_ll *that);						//太空船操作符，参见php的太空船操作符
#define			jbl_ll_if_big(x,y)			(jbl_ll_space_ship(x,y)>0)										//判断this是否>that
#define			jbl_ll_if_equal(x,y)		(jbl_ll_space_ship(x,y)==0)										//判断this是否=that
#define			jbl_ll_if_small(x,y)		(jbl_ll_space_ship(x,y)<0)										//判断this是否<that
#define			jbl_ll_if_equal_small(x,y)	(jbl_ll_space_ship(x,y)<=0)										//判断this是否<=that
#define			jbl_ll_if_equal_big(x,y)	(jbl_ll_space_ship(x,y)>=0)										//判断this是否>=that
#if JBL_JSON_ENABLE==1
/*******************************************************************************************/
/*                            以下函实现链表JSON操作                                      */
/*******************************************************************************************/
#if JBL_STRING_ENABLE==1
jbl_string*		jbl_ll_json_encode			(jbl_ll* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs);	//JSON编码
#endif
#if JBL_STREAM_ENABLE==1
void			jbl_ll_json_put				(jbl_ll* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs);	//从out JSON输出一个link list
#endif
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函实现链表插看操作                                      */
/*******************************************************************************************/
jbl_ll*			jbl_ll_view_put					(jbl_ll* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从out浏览一个link list
#define			jbl_ll_view(x)					jbl_ll_view_put(x,jbl_stream_stdout,1,JBL_VIEW_DEFAULT_TABS,__LINE__,UC #x,UC __FUNCTION__,UC __FILE__)//浏览link list
#endif


#endif
#endif
