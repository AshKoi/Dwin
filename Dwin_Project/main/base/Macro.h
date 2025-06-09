/******************************************************************************************************
* 2013-2014 益邦科技有限公司
* 文件名: 
* 创建人: 王一凡
* 日　期: 2013/09/21
* 修改人: 
* 日　期: 
* 描　述: 
* 版　本: V1.0
*****************************************************************************************************/
#ifndef _MACRO_H_
#define _MACRO_H_
#include "AppCfg.h"
/**************************************************************************************************
                                          基本操作相关宏定义
***************************************************************************************************/
#define 	TEST_BIT(a, b)			    	((a) & (1 << (b)))								//测试位([0, 31])
#define 	SET_BIT(a, b)			    	((a) |= (1 << (b)))								//置位([0, 31])
#define 	CLR_BIT(a, b)			    	((a) &= (~(1 << (b)))) 							//清位([0, 31])
                                        	                                        	
#define 	GET_MIN(a, b)			    	(((a) <= (b)) ? (a) : (b)) 						//获取最小值
#define 	GET_MAX(a, b)			    	(((a) >= (b)) ? (a) : (b)) 						//获取最大值
#define 	GET_MAX3(n1, n2, n3)			GET_MAX(GET_MAX(n1, n2), n3) 					//获取最大值
                                                                                	
#define 	BCD_TO_INT8U(x)   				(((x) >> 4) * 10 + ((x) & 0x0f))				//BCD转换到INT8U
#define 	INT8U_TO_BCD(x)   				((((x) / 10) << 4) | ((x) % 10))		
#define		ASC_TO_BCD(low,hi)	  			((INT8U)(((low) & 0x0f) + (((hi) & 0x0f) << 4)))
#define 	HIHALFBYTE(n)         			((INT8U)(((n) >> 4) & 0x0F)) 	    			//
#define 	LOHALFBYTE(n)         			((INT8U)((n) & 0x0F))
#define 	HIBYTE(w)         				((INT8U)(((INT16U) (w) >> 8) & 0xFF)) 	    	//
#define 	LOBYTE(w)         				((INT8U)(w))
#define		MAKEWORD(low,hi)  				((INT16U)((INT16U)(low) + ((INT16U)(hi) << 8)))

#define 	HIBCD(w)         				(INT8U_TO_BCD((w) / 100)) 	    				//
#define 	LOBCD(w)         				(INT8U_TO_BCD((w) % 100))
#define		MAKEBCDWORD(low,hi)  			((INT16U)((INT16U)(BCD_TO_INT8U(low)) + ((INT16U)(BCD_TO_INT8U(low)) * 100)))

#define 	HIWORD(dw)        				((INT16U)(((INT32U) (dw) >> 16) & 0xFFFF))
#define 	LOWORD(dw)        				((INT16U)(dw))
#define		MAKEDWORD(low,hi) 				((INT32U)((INT32U)(low) + ((INT32U)(hi) << 16)))

#define     STR_OFF(pStr, item)			((INT32U)(&((pStr)0)->item))					//获取结构体元素偏移量
#define		STR_OFF_EX(nAddr, pStr, item)	((INT32U)(&((pStr)nAddr)->item))				//获取结构体元素偏移量(扩展)
#define		STR_DIFF(pStr, item1, item2)	((INT32S)(((INT32U)(&((pStr)0)->item2)) - ((INT32U)(&((pStr)0)->item1))))	//获取结构体内两元素间距
#define     STR_SIZE(pStr, item)   		((INT32U)(sizeof(((pStr)0)->item)))				//获取结构体元素大小
#define     ARR_SIZE_EXT(pStr, item)   	((INT32U)(STR_SIZE(pStr, item) / STR_SIZE(pStr, item[0])))				//获取结构体元素大小
#define     ARR_SIZE(a)                 	((INT32U)(sizeof((a))/sizeof((a[0]))))  		//获取数据元素个数


#ifndef NULL
#define     NULL            				0                
#endif

#ifndef INVLD_DATA
#define     INVLD_DATA              		0xFF			//无效数据
#endif

#ifndef CH_DOG_OUT
#define 	CH_DOG_OUT					    1				//外部狗
#endif


/**************************************************************************************************
                                          接口标准返回值
***************************************************************************************************/
#define     RET_ERR_STAT_OK			    1				//统计时，有效数据
#define     RET_ERR_OK			            0				//执行成功
#define    	RET_ERR_ERROR		            -1			    //执行错误
#define     RET_ERR_POINT		            -2			    //输入参数空指针
#define     RET_ERR_OPT			       	-3			    //输入参数选项错误
#define     RET_ERR_RANG		           	-4				//输入参数范围错误
#define     RET_ERR_NO_ID 		           	-5				//无指定数据项
#define		RET_ERR_CHK			       	-6				//数据校验错误
#define     RET_ERR_HARDWARE	           	-7				//硬件驱动发生错误
#define     RET_ERR_VAL_ZERO		        -8				//统计时数据为0
#define     RET_INVLD  				    -11				//无效值
#define     RET_ZERO                   -12              //值为0
#define     RET_VLD						    -13         	//值有效且为非零
#define     RET_ERR_PNERROR				-20            	//数据标识的信息点错误
#define     CANCEL_VALUE				   	-10000



/**************************************************************************************************
                                         长度宏定义
***************************************************************************************************/
#define     LEN_32                      	32              //32B大小宏定义
#define     LEN_64                      	64              //64B大小宏定义
#define     LEN_128                     	128             //128B大小宏定义
#define     LEN_256                     	256             //256B大小宏定义
#define     LEN_512                     	512             //512B大小宏定义
#define     LEN_1K                      	1024            //1K大小宏定义
#define     LEN_2K                      	2048            //2K大小宏定义
#define     LEN_4K                      	4096            //4K大小宏定义


/**************************************************************************************************
                                          基本数目
***************************************************************************************************/
#define     FOUR_BYTE_HIGH_BIT			0x80000000



/**************************************************************************************************
                                     电表规约宏定义
***************************************************************************************************/
enum _tagRD_METER_FLGS
{
	RD_REAL = 0,											//实时
	RD_DAY,
	RD_MON,
	RD_CRV,													//曲线
	RD_RDAY,

	RD_TD_NUM
};

#define		GB_97_PROT						1
#define		GB_07_PROT						30

#define   	DI_901F                 		0x901F		   	//正向有功集合
#define   	DI_9010                 		0x9010		  	//正向有功总
#define    	DI_C010                 		0xC010       	//日期
#define    	DI_C011                 		0xC011      	//时间
#define  	DI_0001FF00             		0x0001FF00	//正向有功集合
#define   	DI_00010000             		0x00010000	//正向有功总
#define    	DI_04000101             		0x04000101	//年月日星期
#define   	DI_04000102             		0x04000102	//时间
#define    	DI_05060101             		0x05060101	//日冻结正向有功


#define    	DI_D_1                  		0x05040201	//当日1
#define    	DI_D_2                  		0x05040202	//当日2
#define    	DI_D_3                  		0x05040203	//当日3
#define   	DI_D_4                  		0x05040204	//当日4


#define    	DI_S_1                  		0x05040205	//上日1
#define    	DI_S_2                  		0x05040206	//上日2
#define    	DI_S_3                  		0x05040207	//上日3
#define    	DI_S_4                  		0x05040208	//上日4


/**************************************************************************************************
                                          基本宏
***************************************************************************************************/
#define     LEN_AREA_CODE					2				//地区代码长度
#define     LEN_TERM_ADDR					2				//终端地址长度
#define     OAD_LEN                         4
#define     MAX_TERM_GRP_NUM				8				//(F6)组地址数量
#define     MAX_TERM_RELAY_NUM				16				//(F2)中继转发数量
#define     LEN_KEY_PLAN_PRM			    2				//消息认证方案参数长度
#define     LEN_KEY_DATA_PRM			    12				//消息认证方案中待校验数据长度

#define     LEN_IP_ADDR           			4               //IP地址长度
#define     LEN_APN                 		16              //APN长度
#define     LEN_PROXY_USER         		    20              //代理用户名长度
#define     LEN_PROXY_PWD          		    20              //代理用户密码长度

#define     LEN_METER_ADDR              6               //电表地址长度
#define     LEN_METER_PWD               6               //电表密码长度
#define     LEN_METER_NO               	12              //电表局编号长度
#define     LEN_METER_GLBH			   	32				//电表管理编号长度
#define		LEN_METER_PASSWORD			3				//645规约密码长度
#define     LEN_METER_STAT              8               //电表抄表状态长度
#define     LEN_RATE_VOLT				6				//额定电压
#define     LEN_RATE_CURR				6				//额定电流
#define     LEN_MAX_CURR				6				//最大电流
#define     LEN_PW_CHECK                16              //消息认证码长度
#define     LEN_RUN_STAT_FLAG           8               //运行状态字长度



//总加组类型
#define     GRP_TYPE_INVLD              0               //无效总加
#define     GRP_TYPE_PULSE              1               //脉冲总加
#define     GRP_TYPE_METER              2               //电表总加
#define     GRP_TYPE_MIXED              3               //混合总加

#define 	MAX_YEAR_SEGM_NUM		    14				//最大时区数
#define		MAX_DAY_TABLE_NUM		    8				//最大日时段表数
#define		MAX_DAY_SEGM_NUM		    14				//最大日时段数
#define	    MAX_HOLIDAY_NUM			    14				//最大公共假日数

#define     MAX_TURN_NUM                2               //控制轮次最大数目


//脉冲、需量计算相关宏
#define     WATT_SLIP_MAX_STEPS         4               //功率滑差最大步长
#define     NEED_CALC_STEPS             15              //需量计算滑差
#define     WATT_CTRL_MAX_SLIP_STEPS    30              //紧急下浮控滑差最大步长
#define     TIME_FIELD_INVAILD          0xFF            //无效时段
#define     FEE_SHARP                   0               //尖
#define     FEE_PEEK                    1               //峰
#define     FEE_EVEN                    2               //平
#define     FEE_VALE                    3               //谷
#define     MAX_FEE_PERD_NUM			10				//电能量费率时段

//脉冲类型
#define     PUL_ATTR_PP                 0               //正向有功
#define     PUL_ATTR_PN                 1               //正向无功
#define     PUL_ATTR_NP                 2               //反向有功
#define     PUL_ATTR_NN                 3               //反向无功

/**************************************************************************************************
                                     交流采样宏定义
***************************************************************************************************/
#define 		LEN_AC_EVENT				256			//事件长度

/**************************************************************************************************
                                             Fn定义宏定义
***************************************************************************************************/
enum _tagFN_ITEM
{
	F1 = 0,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	F21,
	F22,
	F23,
	F24,
	F25,
	F26,
	F27,
	F28,
	F29,
	F30,
	F31,
	F32,
	F33,
	F34,
	F35,
	F36,
	F37,
	F38,
	F39,
	F40,
	F41,
	F42,
	F43,
	F44,
	F45,
	F46,
	F47,
	F48,
	F49,
	F50,
	F51,
	F52,
	F53,
	F54,
	F55,
	F56,
	F57,
	F58,
	F59,
	F60,
	F61,
	F62,
	F63,
	F64,
	F65,
	F66,
	F67,
	F68,
	F69,
	F70,
	F71,
	F72,
	F73,
	F74,
	F75,
	F76,
	F77,
	F78,
	F79,
	F80,
	F81,
	F82,
	F83,
	F84,
	F85,
	F86,
	F87,
	F88,
	F89,
	F90,
	F91,
	F92,
	F93,
	F94,
	F95,
	F96,
	F97,
	F98,
	F99,
	F100,
	F101,
	F102,
	F103,
	F104,
	F105,
	F106,
	F107,
	F108,
	F109,
	F110,
	F111,
	F112,
	F113,
	F114,
	F115,
	F116,
	F117,
	F118,
	F119,
	F120,
	F121,
	F122,
	F123,
	F124,
	F125,
	F126,
	F127,
	F128,
	F129,
	F130,
	F131,
	F132,
	F133,
	F134,
	F135,
	F136,
	F137,
	F138,
	F139,
	F140,
	F141,
	F142,
	F143,
	F144,
	F145,
	F146,
	F147,
	F148,
	F149,
	F150,
	F151,
	F152,
	F153,
	F154,
	F155,
	F156,
	F157,
	F158,
	F159,
	F160,
	F161,
	F162,
	F163,
	F164,
	F165,
	F166,
	F167,
	F168,
	F169,
	F170,
	F171,
	F172,
	F173,
	F174,
	F175,
	F176,
	F177,
	F178,
	F179,
	F180,
	F181,
	F182,
	F183,
	F184,
	F185,
	F186,
	F187,
	F188,
	F189,
	F190,
	F191,
	F192,
	F193,
	F194,
	F195,
	F196,
	F197,
	F198,
	F199,
	F200,
	F201,
	F202,
	F203,
	F204,
	F205,
	F206,
	F207,
	F208,
	F209,
	F210,
	F211,
	F212,
	F213,
	F214,
	F215,
	F216,
	F217,
	F218,
	F219,
	F220,
	F221,
	F222,
	F223,
	F224,
	F225,
	F226,
	F227,
	F228,
	F229,
	F230,
	F231,
	F232,
	F233,
	F234,
	F235,
	F236,
	F237,
	F238,
	F239,
	F240,
	F241,
	F242,
	F243,
	F244,
	F245,
	F246,
	F247,
	F248,

	
	FEND = 0xFFFF	//结束数据项   与ITEM_COL_END一样
};



#endif
