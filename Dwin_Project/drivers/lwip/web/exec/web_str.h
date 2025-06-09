/**********************************************************************
*
*                  监控系统8.0__嵌入式系统构件        
*
**********************************************************************

----------------------------------------------------------------------
文 件 名	: emac_api.c
文件描述	: emac模块应用
创建作者	: HCF
创建日期	: 2007.11.28
======================================================================
版本历史	: 
----------------------------------------------------------------------
修改日期	: 
修改内容	: 
======================================================================
*/


char web_strcmp(char *str,const unsigned char *p );

char *Web_Find_Str_Post(char *str);
char *Web_Post_Cope	(char *s,char *id,char *date,unsigned int *id_len,unsigned int *da_len);
char * string_cmp(char *str,const unsigned char *p );
unsigned char *Web_Combined_row(unsigned char *str,unsigned char strut,unsigned char num,unsigned char *id_1, unsigned char *id_2,
							unsigned char *name,unsigned char *dev_msg,
							unsigned char *up_time);
unsigned char *str_cope(unsigned char *drc,unsigned char *src);

