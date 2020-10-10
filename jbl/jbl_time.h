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
#include "jbl_pthread.h"
/*******************************************************************************************/
/*                            联动jbl_string jbl_var                                       */
/*******************************************************************************************/
#include "jbl_string.h"
#include "jbl_var.h"

#define JBL_TIME_ZONE_ASIA_SHANGHAI 1


jbl_var_operators_extern(jbl_time_operators);

/*******************************************************************************************/
/*                            结构体定义                                                  */
/*******************************************************************************************/
typedef struct __jbl_time
{
	jbl_gc						gc;
	jbl_var_ops_define			;
	jbl_pthread_lock_define		;
	jbl_int64 t;								//时间戳，带3位毫秒
}jbl_time;										//蒟蒻云基础库时间结构
typedef struct __jbl_time_decoded
{
	int year,ms;	//格式化的时间，各变量为字面含义
	jbl_uint8 week,dom,day,hour,month,minute,second,isleap;
}jbl_time_decoded;								//蒟蒻云基础库格式化时间结构
typedef enum
{
	JBL_TIME_KEY_UNDEFINED,						//未定义
	JBL_TIME_KEY_END,							//结束
	JBL_TIME_KEY_ESCAPE,						//跳过下一个
	
	JBL_TIME_KEY_DOM,							//j   day of month
	JBL_TIME_KEY_DOMWZ,							//d   day of month with 0
	JBL_TIME_KEY_DOW3,							//D   day of week 3 word
	JBL_TIME_KEY_DOWA,							//l   day of week all 
	JBL_TIME_KEY_DOWN,							//N   day of week number
	JBL_TIME_KEY_DOMS,							//S   day of month suffix
	JBL_TIME_KEY_DOWNF0,						//w   day of week number from 0
	JBL_TIME_KEY_DOY,							//z   day of year(未实现)
	
	JBL_TIME_KEY_WOY,							//W   week of year(未实现)
	
	JBL_TIME_KEY_M,								//F   month
	JBL_TIME_KEY_MNWZ,							//m   month number with zero
	JBL_TIME_KEY_M3,							//M   month
	JBL_TIME_KEY_MN,							//n   month number
	JBL_TIME_KEY_TDOM,							//t   total day of month
	
	JBL_TIME_KEY_YILY,							//L   year is leap year
	JBL_TIME_KEY_YISO,							//o   year of ISO-8601(未实现)
	JBL_TIME_KEY_Y,								//Y   year
	JBL_TIME_KEY_Y2,							//y   year 2 number
	
	JBL_TIME_KEY_SA,							//a   am/pm
	JBL_TIME_KEY_A,								//A   AM/PM
	JBL_TIME_KEY_SWATCH,						//B   Swatch Internet 标准时(未实现)
	JBL_TIME_KEY_H12,							//g   12 hour
	JBL_TIME_KEY_H12WZ,							//G   12 hour with zero
	JBL_TIME_KEY_H24,							//h   24 hour
	JBL_TIME_KEY_H24WZ,							//H   12 hour with zero
	JBL_TIME_KEY_MWZ,							//i   minute with zero
	JBL_TIME_KEY_SWZ,							//s   second with zero
	JBL_TIME_KEY_MSWZ,							//u   micro second with zero
	
	JBL_TIME_KEY_TZT,							//e   time zone type(未实现)
	JBL_TIME_KEY_IDST,							//I   is daylight saving time(未实现)
	JBL_TIME_KEY_HTG,							//O   hours to Greenwich time(未实现)
	JBL_TIME_KEY_DWG,							//P   different with Greenwich time(未实现)
	JBL_TIME_KEY_TZ,							//T   time zone	(未实现)
	JBL_TIME_KEY_STG,							//Z   second to Greenwich time(未实现)
	JBL_TIME_KEY_S,								//u   从 Unix 纪元（January 1 1970 00:00:00 GMT）开始至今的秒数
	
}jbl_time_key;
#define				JBL_TIME_FORMAT_RFC1123			"D, j M Y H:i:s e"
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
void				jbl_time_decode					(jbl_time *this,jbl_time_decoded *tt);									//格式化一个时间戳

/*******************************************************************************************/
/*                            以下函实现时间差操作                                        */
/*******************************************************************************************/
jbl_time *			jbl_time_add_second				(jbl_time * this,jbl_uint64 second);									//加ys
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
jbl_string *		jbl_time_to_string				(jbl_time *this,jbl_string *result);									//把time格式化成字符串时间戳
jbl_string *		jbl_time_to_string_format		(jbl_time *this,jbl_string *result,const unsigned char * format);		//把time格式化成字符串时间戳
#endif
#if JBL_STREAM_ENABLE==1
void				jbl_stream_push_time			(jbl_stream *out,jbl_time *this,const unsigned char * format);
#endif
#if JBL_STREAM_ENABLE==1
/*******************************************************************************************/
/*                            以下函数实现时间的浏览操作                                 */
/*******************************************************************************************/
jbl_time*				jbl_time_view_put			(jbl_time* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs,jbl_uint32 line,unsigned char * varname,unsigned char * func,unsigned char * file);	//从out浏览一个时间
#define					jbl_time_view(x)			jbl_time_view_put(x,jbl_stream_stdout,1,JBL_VIEW_DEFAULT_TABS,__LINE__,UC #x,UC __FUNCTION__,UC __FILE__)//浏览一个时间
#endif
#if JBL_JSON_ENABLE==1
/*******************************************************************************************/
/*                            以下函实现时间JSON操作                                      */
/*******************************************************************************************/
#if JBL_STRING_ENABLE==1
jbl_string *		jbl_time_json_encode			(jbl_time* this,jbl_string *out,jbl_uint8 format,jbl_uint32 tabs);		//把time格式化成JSON
#endif
#if JBL_STREAM_ENABLE==1
void				jbl_time_json_put				(jbl_time* this,jbl_stream *out,jbl_uint8 format,jbl_uint32 tabs);
#endif
#endif




#endif
#endif
