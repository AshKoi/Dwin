/**********************************************************************
*
*                  ���ϵͳ8.0__Ƕ��ʽϵͳ����        
*
**********************************************************************

----------------------------------------------------------------------
�� �� ��	: emac_api.c
�ļ�����	: emacģ��Ӧ��
��������	: HCF
��������	: 2007.11.28
======================================================================
�汾��ʷ	: 
----------------------------------------------------------------------
�޸�����	: 
�޸�����	: 
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

