/* Copyrigtime (c) [2020] juruoyun developer team
   Juruoyun basic lib is licensed under the Mulan PSL v1.
   You can use this software according to the terms and conditions of the Mulan PSL v1.
   You may obtain a copy of Mulan PSL v1 at:
      timetp://license.coscl.org.cn/MulanPSL
   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
   PURPOSE.
   See the Mulan PSL v1 for more details.*/
#ifndef __JBL_TIME_H
#define __JBL_TIME_H
#include "jbl_time_config.h"
#if JBL_TIME_ENABLE==1
/*******************************************************************************************/
/*                            依赖jbl_gc jbl_malloc jbl_ying jbl_exception                */
/*******************************************************************************************/
#include "jbl_exception.h"
#include "jbl_gc.h"
#include "jbl_malloc.h"
#include "jbl_ying.h"
/*******************************************************************************************/
/*                            联动jbl_string jbl_var                                       */
/*******************************************************************************************/
#include "jbl_string.h"
#include "jbl_var.h"

#define JBL_TIME_ZONE_ASIA_SHANGHAI 1




/*******************************************************************************************/
/*                            结构体定义                                                  */
/*******************************************************************************************/
typedef struct __jbl_time
{
	jbl_gc gc;									//gc结构
	jbl_uint64 t;								//时间戳，带3位毫秒
}jbl_time;										//蒟蒻云基础库时间结构
typedef struct __jbl_time_decoded
{
	int year,month,day,hour,minute,second,ms;	//格式化的时间，各变量为字面含义
}jbl_time_decoded;								//蒟蒻云基础库格式化时间结构
/*******************************************************************************************/
/*                            全局变量定义                                                */
/*******************************************************************************************/
extern jbl_int8		jbl_time_time_zone;			//处理的并不好
/*******************************************************************************************/
/*                            以下函实现时间基本操作                                     */
/*******************************************************************************************/
jbl_time *			jbl_time_new					();																		//新建一个时间
jbl_time *			jbl_time_init					(jbl_time *this);														//初始化一个时间
jbl_time *			jbl_time_free					(jbl_time *this);														//释放一个时间
jbl_time *			jbl_time_set					(jbl_time *that,jbl_uint64 time);										//设置一个unix时间戳
jbl_time *			jbl_time_copy					(jbl_time *that);														//复制一个时间
/*******************************************************************************************/
/*                            以下函实现时间获取操作                                     */
/*******************************************************************************************/
jbl_time *			jbl_time_now					(jbl_time *this);														//把this设置为当前时间
#define				jbl_time_to_unix(x)				(((jbl_time *)(jbl_refer_pull(x)))->t)									//转换为unix实践出
void				jbl_time_decode					(const jbl_time *this,jbl_time_decoded *tt);							//格式化一个时间戳

/*******************************************************************************************/
/*                            以下函实现时间差操作                                        */
/*******************************************************************************************/
#define				jbl_time_minus(x,y)				(jbl_time_to_unix(x)-jbl_time_to_unix(y))								//计算两个时间的差
/*******************************************************************************************/
/*                            以下函实现时间比较操作                                      */
/*******************************************************************************************/
char				jbl_time_space_ship				(jbl_time *this,jbl_time *that);										//比较两个时间
#define				jbl_time_if_equal(x,y)			(jbl_time_space_ship(x,y)==0)											//判断this是否=that
#define				jbl_time_if_big(x,y)			(jbl_time_space_ship(x,y)>0)											//判断this是否>that
#define				jbl_time_if_small(x,y)			(jbl_time_space_ship(x,y)<0)											//判断this是否<that
#define				jbl_time_if_equal_small(x,y)	(jbl_time_space_ship(x,y)<=0)											//判断this是否>=that
#define				jbl_time_if_equal_big(x,y)		(jbl_time_space_ship(x,y)>=0)											//判断this是否<=that
/*******************************************************************************************/
/*                            以下函实现特殊时间判断操作                                 */
/*******************************************************************************************/
jbl_uint8			jbl_time_if_leap_year			(jbl_uint32 year);														//判断year是不是闰年
jbl_uint8			jbl_time_get_day_of_month		(jbl_uint8 month,jbl_uint32 year);										//获取一个月份的天数
jbl_uint8			jbl_time_get_weekday			(jbl_uint8 day,jbl_uint8 month,jbl_uint32 year);						//获取今天是周几
jbl_uint8			jbl_time_if_dst					(jbl_uint8 day,jbl_uint8 month,jbl_uint32 year);						//判断夏令时

#if JBL_STRING_ENABLE==1
/*******************************************************************************************/
/*                            以下函实现时间对字符串操作                                */
/*******************************************************************************************/
jbl_string *		jbl_time_to_string				(const jbl_time *this,jbl_string *result);								//把time格式化成字符串时间戳
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现时间的浏览操作                                 */
/*******************************************************************************************/
void				jbl_time_view_put				(const jbl_time* this,jbl_stream *out,jbl_int32 format,char*str,jbl_int32 tabs);
#define				jbl_time_view(x)				jbl_time_view_put(x,jbl_stream_stdout,__LINE__,#x " @ "__FILE__,JBL_VIEW_DEFAULT_TABS),jbl_stream_push_char(jbl_stream_stdout,'\n')	//浏览一个字符串
#endif
#if JBL_JSON_ENABLE==1
/*******************************************************************************************/
/*                            以下函实现时间JSON操作                                      */
/*******************************************************************************************/
#if JBL_STRING_ENABLE==1
jbl_string *		jbl_time_json_encode			(const jbl_time* this,jbl_string *out,char format,jbl_int32 tabs);		//把time格式化成JSON
#endif
#if JBL_STREAM_ENABLE==1
void				jbl_time_json_put				(const jbl_time* this,jbl_stream *out,char format,jbl_int32 tabs);
#endif
#endif
#if JBL_VAR_ENABLE==1
/*******************************************************************************************/
/*                            以下函实现时间的var操作                                    */
/*******************************************************************************************/
extern	const		jbl_var_operators				jbl_time_operators;															//time 操作器
jbl_time  *			jbl_Vtime						(jbl_var * this);															//以time的格式使用var
#define				Vis_jbl_time(x)					(jbl_var_get_operators(x)==&jbl_time_operators)								//判断一个var是不是time 
jbl_var *			jbl_Vtime_new					();																			//新建一个time类型的var
jbl_var *			jbl_time_copy_as_var			(jbl_time *that);															//复制为一个var,这里重写了复制函数，因为time属于直接复制类型
#endif




#endif
#endif
