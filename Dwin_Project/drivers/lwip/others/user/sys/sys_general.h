/*
**********************************************************************
*
*                  监控系统8.0__嵌入式系统构件
*
**********************************************************************

----------------------------------------------------------------------
文 件 名	: sys_general.h
文件描述	: 系统通用函数集包含头文件
创建作者	: HCF
创建日期	: 2007.05.31
======================================================================
版本历史	:
----------------------------------------------------------------------
修改日期	:
修改内容	:
======================================================================
*/
#ifndef	_SYS_GENERAL_H_
#define _SYS_GENERAL_H_
#include "lwip/others/user/sys/sys_data.h"

/*
**********************************************************************
*						得到当前系统版本的软件代码长度
**********************************************************************
*/
#define		SYS_RO_LEN					((uint32)&Image$$RO$$Limit - (uint32)&Image$$RO$$Base)
#define		SYS_RW_DATA_LEN				((uint32)&Image$$ZI$$Base - (uint32)&Image$$RW$$Base)
#define		SYS_CODE_LEN				(SYS_RO_LEN + SYS_RW_DATA_LEN)

/*
**********************************************************************
*							超时设定与超时检测	
**********************************************************************
*/
#define 	TIMER_CONSTANT				10
#define 	sys_set_timeout(end_time,timelen)			((end_time) = (OSTime+timelen+TIMER_CONSTANT))
#define 	sys_detect_timeout(end_time)		    	((OSTime) > (end_time+TIMER_CONSTANT))

/*
**********************************************************************
*					66			函数声明区
**********************************************************************
*/

void  sys_general_strncat(uint8 *dest_str, const uint8 *src_str, uint16 len);
uint8 sys_general_strncmp(uint8 *string1, uint8 *string2,uint32 len);
void  sys_general_memncpy(uint8 *string1, const uint8 *string2,uint16 len);
void  sys_general_strnget(uint8 *new_str, uint8 *src_str, uint8 max_len);
void  sys_general_memset(uint8 *addr, uint8 value, uint16 len);
uint8 sys_general_bytes_to_ascii(uint8 ascii);
uint16 sys_general_cal_crc(uint8 *ptr, uint16 len);
void   sys_general_uint8_2_uint32(uint8 *buf_uint8, uint32 *buf_uint32, uint16 counter);
//uint32 sys_general_strfilte(uint8 *src, uint8 *dest,const uint8 *sflag,const *eflag,uint16 srclen);
uint8  *sys_general_findstr(uint8 *src,uint16 srclen,const uint8 *dest,uint16 destlen);
uint16 sys_general_strlen(const uint8 *psrc);
uint8 sys_general_strsgncmp(uint8 *string1,uint8 *string2,uint8 max_len);
void sys_general_hex_2_char(uint16 data,uint8 *da_buff);

#endif
/* _SYS_GENERAL_H_ */

