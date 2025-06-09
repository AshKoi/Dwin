/**************************************************************************************************
* Copyright (c) 2010-2011 光一科技股份有限公司研发中心
* 文件名:
* 创建人: 研发中心
* 日　期: 2010/08/18
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
***************************************************************************************************/
#ifndef _DATA_ITEM_H_
#define _DATA_ITEM_H_

#include "Os_cpu.h"
#include "AppCfg.h"
#include "RX8025T.h"
#include "Macro.h"
#include "Table.h"

/**************************************************************************************************
                                     基本宏定义
***************************************************************************************************/
#define 	INVLD_ITEM              		0xFFFFFFFF          					//无效项
#define  	INVLD_PROT_ITEM         		0xFFFFFFFF
#define  	INVLD_DATA_ITEM         		0xFFFF              					//无效规约项
#define 	DATA_ITEM_MAP_LEN       		64                  					//数据位图长度
#define 	DATA_ITEM_LEN           		(DATA_ITEM_MAP_LEN * 8)                 //数据项长度



/**************************************************************************************************
                                  数据标准项宏定义
***************************************************************************************************/
//业务数据下标(注：标准数据项修改时务必保证下标同步)
#define     DATA_ITEM_REAL_START    		RD_ZXYGJH           	//实时数据开始下标
#define     DATA_ITEM_REAL_END      		RD_ZJYCWGDKSJ			//实时数据结束下标
#define     DATA_ITEM_DAY_START     		DD_ZXYGJH           	//日数据开始下标
#define     DATA_ITEM_DAY_END       		DD_FXYGXLJH         	//日数据结束下标
#define     DATA_ITEM_MON_START     		MD_ZXYGJH           	//月数据开始下标
#define     DATA_ITEM_MON_END       		MD_FXYGXLJH         	//月数据结束下标



//业务标准数据项:不能超过256个
enum _tagCOL_DATA_ITEM_TBL
{
    //********************************************************************************
    //					       实时数据标准项
    //********************************************************************************
    RD_ZHYGJH=0,			//(0000FF00)组合有功电能示值集合
    RD_ZXYGJH,			    //(0001FF00)正向有功电能示值集合
    RD_FXYGJH,				//(0002FF00)反向有功电能示值集合
    RD_ZXWGJH,				//(0003FF00)组合无功1(正向)电能示值集合
    RD_FXWGJH,				//(0004FF00)组合无功2(反向)电能示值集合

    RD_X1WGJH,              //(0005FF00)一象限无功电能示值集合
    RD_X2WGJH,              //(0006FF00)二象限无功电能示值集合
    RD_X3WGJH,              //(0007FF00)三象限无功电能示值集合
    RD_X4WGJH,              //(0008FF00)四象限无功电能示值集合

    RD_ZXSZJH,			    //(0009FF00)正向视在电能示值集合
    RD_FXSZJH,				//(000AFF00)反向视在电能示值集合
    RD_GLZDNJH,				//(00800000)关联总电能集合
    RD_AXDNJH,				//(00150000)A相电能集合
    RD_BXDNJH,				//(00290000)B相电能集合
    RD_CXDNJH,				//(003D0000)C相电能集合

    RD_ZXYGXLJH,            //(0101FF00)正向有功最大需量及发生时间集合
    RD_FXYGXLJH,            //(0102FF00)反向有功最大需量及发生时间集合
    RD_ZXWGXLJH,            //(0103FF00)组合无功1最大需量及发生时间集合
    RD_FXWGXLJH,            //(0104FF00)组合无功2最大需量及发生时间集合

    RD_DYJH,              	//(0201FF00)电压集合
    RD_DLJH,                //(0202FF00)电流集合
    RD_YGGLJH,              //(0203FF00)有功功率集合
    RD_WGGLJH,              //(0204FF00)无功功率集合
    RD_SZGLJH,              //(0205FF00)视在功率集合
    RD_GLYSJH,              //(0206FF00)功率因数集合
    RD_XWJJH,               //(0207FF00)相位角集合
    RD_DYBXSZJH,            //(0208FF00)电压波形失真集合
    RD_DLBXSZJH,            //(0209FF00)电流波形失真集合

    RD_LXDL,                //(02800001)零线电流
    RD_DWPL,                //(02800002)电网频率
    RD_YFZPJGL,             //(02800003)一分钟有功平均功率
    RD_DQYGXL,              //(02800004)当前有功需量
    RD_DQWGXL,              //(02800005)当前无功需量
    RD_DQSZXL,              //(02800006)当前视在需量
    RD_BLWD,                //(02800007)表内温度
    RD_LBSZDY,              //(02800008)时钟电池电压（内部）
    RD_WBSZDY,              //(02800009)停电抄表电池电压（外部）
    RD_LBDCGZSJ,            //(0280000A)内部电池工作时间

    RD_SBLJH,               //(028001FF)闪变量集合

    RD_YGDBQSDS,			//(E1000001)有功电能表起始读数
    RD_WGDBQSDS,			//(E1000002)无功电能表起始读数
    RD_DBYGCS,			    //(E1000003)电表常数(有功)
    RD_DBWGCS,			    //(E1000004)电表常数(无功)

    RD_AXDYXBJH,            //(020A01FF)A相电压谐波集合
    RD_BXDYXBJH,            //(020A02FF)B相电压谐波集合
    RD_CXDYXBJH,            //(020A03FF)C相电压谐波集合
    RD_AXDLXBJH,            //(020B01FF)A相电流谐波集合
    RD_BXDLXBJH,            //(020B02FF)B相电流谐波集合
    RD_CXDLXBJH,            //(020B03FF)C相电流谐波集合

	RD_DBRQ,            	//(04000101)电表日期
    RD_DBSJ,            	//(04000102)电表时间

    RD_SYJH,				//(03010000)失压次数
    RD_SLJH,				//(030B0000)失流次数
    RD_DLFXJH,				//(030E0000)电流反向次数

    RD_DBBCJL,              //(03300000)电表编程总次数
    RD_DBYXZTZ,			    //(040005FF)电表运行状态字
    RD_SYJE,			    //(E1000008)剩余金额

    RD_RDYHGLJH,			//(E1000010)日电压合格率统计
    RD_RDLJXZJH,			//(E1000014)日电流极限值统计

    RD_RYGGLJXZJH,			//(E1000018)日有功功率极限值统计
    RD_RWGGLJXZJH,			//(E1000019)日无功功率极限值统计
    RD_RGLYSJXZJH,			//(E100001A)日功率因数极限值统计
    RD_RDLYXTJJH,			//(E100001B)日电流不平衡越限统计
    RD_RDYYXTJJH,			//(E100001C)日电压不平衡越限统计

    RD_RTDZCS,			    //(E1000020)日停电总次数
    RD_RTDZSJ,			    //(E1000021)日停电总时间
    RD_RDXSJJH,			    //(E1000030)日断相数据集合
    RD_RSYSJJH,			    //(E1000031)日失压数据集合
    RD_RSLSJJH,			    //(E1000032)日失流数据集合

    RD_RAXDYBDZ,            //(0xE1000040)日A相电压波动值
    RD_RBXDYBDZ,            //(0xE1000041)日B相电压波动值
    RD_RCXDYBDZ,            //(0xE1000042)日C相电压波动值
    RD_RAXDYSBZ,            //(0xE1000043)日A相电压闪变值
    RD_RBXDYSBZ,            //(0xE1000044)日B相电压闪变值
    RD_RCXDYSBZ,            //(0xE1000045)日C相电压闪变值
    RD_RDYDLZDZJH,          //(0xE10001FF)本日电压电流偏差最大值
    RD_RDYDLZXZJH,          //(0xE10002FF)本日电压电流偏差最小值
    RD_RDYDLPJZJH,          //(0xE10002FF)本日电压电流偏差平均值

    RD_YDYHGLJH,			//(03100010)月电压合格率统计
    RD_YAXDYHGLJH,			//(03100110)月A相电压合格率统计
    RD_YBXDYHGLJH,			//(03100210)月B相电压合格率统计
    RD_YCXDYHGLJH,			//(03100310)月C相电压合格率统计

    RD_YDLJXZJH,			//(E1004014)月电流极限值统计
    RD_YAXDLJXZJH,			//(E1004015)月A相电流极限值统计
    RD_YBXDLJXZJH,			//(E1004016)月B相电流极限值统计
    RD_YCXDLJXZJH,			//(E1004017)月C相电流极限值统计

    RD_YYGGLJXZJH,			//(E1004018)月有功功率极限值统计
    RD_YWGGLJXZJH,			//(E1004019)月无功功率极限值统计
    RD_YGLYSJXZJH,			//(E100401A)月功率因数极限值统计
    RD_YDLYXTJJH,			//(E100401B)月电流不平衡越限统计
    RD_YDYYXTJJH,			//(E100401C)月电压不平衡越限统计

    RD_YTDZCS,			    //(E1004020)月停电总次数
    RD_YTDZSJ,			    //(E1004021)月停电总时间
    RD_YDXSJJH,			    //(E1004030)月断相数据集合
    RD_YSYSJJH,			    //(E1004031)月失压数据集合
    RD_YSLSJJH,			    //(E1004032)月失流数据集合

    RD_DBBGDKCS,			//(F165)电表表盖打开次数
    RD_DBBGDKSJ1,			//(F165)电表表盖打开事件1
    RD_DBBGDKSJ2,			//(F165)电表表盖打开事件2
    RD_DBBGDKSJ3,			//(F165)电表表盖打开事件3
    RD_DBBGDKSJ4,			//(F165)电表表盖打开事件4
    RD_DBBGDKSJ5,			//(F165)电表表盖打开事件5
    RD_DBBGDKSJ6,			//(F165)电表表盖打开事件6
    RD_DBBGDKSJ7,			//(F165)电表表盖打开事件7
    RD_DBBGDKSJ8,			//(F165)电表表盖打开事件8
    RD_DBBGDKSJ9,			//(F165)电表表盖打开事件9
    RD_DBBGDKSJ10,			//(F165)电表表盖打开事件10
    RD_DBWGDKCS,			//(F165)电表尾盖打开次数(端钮盒)
    RD_DBWGDKSJ1,			//(F165)电表尾盖打开事件1(端钮盒)
    RD_DBWGDKSJ2,			//(F165)电表尾盖打开事件2(端钮盒)
    RD_DBWGDKSJ3,			//(F165)电表尾盖打开事件3(端钮盒)
    RD_DBWGDKSJ4,			//(F165)电表尾盖打开事件4(端钮盒)
    RD_DBWGDKSJ5,			//(F165)电表尾盖打开事件5(端钮盒)
    RD_DBWGDKSJ6,			//(F165)电表尾盖打开事件6(端钮盒)
    RD_DBWGDKSJ7,			//(F165)电表尾盖打开事件7(端钮盒)
    RD_DBWGDKSJ8,			//(F165)电表尾盖打开事件8(端钮盒)
    RD_DBWGDKSJ9,			//(F165)电表尾盖打开事件9(端钮盒)
    RD_DBWGDKSJ10,			//(F165)电表尾盖打开事件10(端钮盒)
    RD_ZJYCWGDKSJ,			//(F165)最近一次尾盖打开时间


	//********************************************************************************
    //					    日数据标准项
    //********************************************************************************
	DD_FRZTIME,			//(05060001)冻结时间

    DD_ZXYGJH,			    //(05060101)正向有功电能示值集合
    DD_FXYGJH,				//(05060201)反向有功电能示值集合
    DD_ZXWGJH,			    //(05060301)组合无功1（正向）电能示值集合
    DD_FXWGJH,			    //(05060401)组合无功2（反向）电能示值集合
    DD_X1WGJH,              //(05060501)一象限无功电能示值集合
    DD_X2WGJH,              //(05060601)二象限无功电能示值集合
    DD_X3WGJH,              //(05060701)三象限无功电能示值集合
    DD_X4WGJH,              //(05060801)四象限无功电能示值集合

    DD_ZXYGXLJH,            //(05060901)正向有功最大需量及发生时间集合
    DD_FXYGXLJH,            //(05060A01)反向有功最大需量及发生时间集合
    DD_ZXWGXLJH,            //(05060B01)组合无功1最大需量及发生时间集合
    DD_FXWGXLJH,            //(05060C01)组合无功2最大需量及发生时间集合

    DD_RDYHGLJH,			//(E1008010)日电压合格率统计
    DD_RAXDYHGLJH,			//(E1008011)日A相电压合格率统计
    DD_RBXDYHGLJH,			//(E1008012)日B相电压合格率统计
    DD_RCXDYHGLJH,			//(E1008013)日C相电压合格率统计

    DD_RPLHGLJH,			//(E1008014)日频率合格率统计数据
    DD_RAXDLJXZJH,			//(E1008015)日A相电流极限值统计
    DD_RBXDLJXZJH,			//(E1008016)日B相电流极限值统计
    DD_RCXDLJXZJH,			//(E1008017)日C相电流极限值统计

    DD_RYGGLJXZJH,			//(E1008018)日有功功率极限值统计
    DD_RWGGLJXZJH,			//(E1008019)日无功功率极限值统计
    DD_RGLYSJXZJH,			//(E100801A)日功率因数极限值统计
    DD_RZXDLJZJH,			//(E100801B)日正序电流极值统计
    DD_RFXDLJZJH,			//(E100801C)日负序电流极值统计
    DD_RLXDLJZJH,			//(E100801D)日零序电流极值统计数据

    DD_RDLYXTJJH,			//(E1008020)日电流不平衡越限统计
    DD_RDLHGLTJJH,			//(E1008021)日电流不平衡合格率统计

    DD_RAXDYSBHGL,          //(0xE1008022)日A相电压闪变合格率统计
    DD_RBXDYSBHGL,          //(0xE1008023)日B相电压闪变合格率统计
    DD_RCXDYSBHGL,          //(0xE1008024)日C相电压闪变合格率统计
    DD_RAXDYJBHGL,          //(0xE1008025)日A相电压畸变合格率统计
    DD_RBXDYJBHGL,          //(0xE1008026)日B相电压畸变合格率统计
    DD_RCXDYJBHGL,          //(0xE1008027)日C相电压畸变合格率统计

    DD_RAXDYBDZ,            //(0xE1008028)日A相电压波动值
    DD_RBXDYBDZ,            //(0xE1008029)日B相电压波动值
    DD_RCXDYBDZ,            //(0xE100802A)日C相电压波动值
    DD_RTDZCS,			    //(E1008030)日停电总次数
    DD_RTDZSJ,			    //(E1008031)日停电总时间

    DD_RDXSJJH,			    //(E1008040)日断相数据集合
    DD_RSYSJJH,			    //(E1008041)日失压数据集合
    DD_RSLSJJH,			    //(E1008042)日失流数据集合

    DD_AXDYXBZDZJH,         //(E10081FF)A相电压谐波最大值
    DD_BXDYXBZDZJH,         //(E10082FF)B相电压谐波最大值
    DD_CXDYXBZDZJH,         //(E10083FF)C相电压谐波最大值
    DD_AXDLXBZDZJH,         //(E10084FF)A相电流谐波最大值
    DD_BXDLXBZDZJH,         //(E10085FF)B相电流谐波最大值
    DD_CXDLXBZDZJH,         //(E10086FF)C相电流谐波最大值

    DD_AXDYXBZXZJH,         //(E10087FF)A相电压谐波最小值
    DD_BXDYXBZXZJH,         //(E10088FF)B相电压谐波最小值
    DD_CXDYXBZXZJH,         //(E10089FF)C相电压谐波最小值
    DD_AXDLXBZXZJH,         //(E1008AFF)A相电流谐波最小值
    DD_BXDLXBZXZJH,         //(E1008BFF)B相电流谐波最小值
    DD_CXDLXBZXZJH,         //(E1008CFF)C相电流谐波最小值

    DD_AXDYXBPJZJH,         //(E10090FF)A相电压谐波平均值
    DD_BXDYXBPJZJH,         //(E10091FF)B相电压谐波平均值
    DD_CXDYXBPJZJH,         //(E10092FF)C相电压谐波平均值
    DD_AXDLXBPJZJH,         //(E10093FF)A相电流谐波平均值
    DD_BXDLXBPJZJH,         //(E10094FF)B相电流谐波平均值
    DD_CXDLXBPJZJH,         //(E10095FF)C相电流谐波平均值

    DD_AXDYXB95JH,          //(E10096FF)A相电压谐波95%值
    DD_BXDYXB95JH,          //(E10097FF)B相电压谐波95%值
    DD_CXDYXB95JH,          //(E10098FF)C相电压谐波95%值
    DD_AXDLXB95JH,          //(E10099FF)A相电流谐波95%值
    DD_BXDLXB95JH,          //(E1009AFF)B相电流谐波95%值
    DD_CXDLXB95JH,          //(E1009BFF)C相电流谐波95%值

    DD_DRQLJTRSJJH,         //(E1009C0F)电容器投入累计时间
    DD_DRQLJTRCSJH,         //(E1009C1F)电容器投入累计次数
    DD_WGBCDNL,             //(E1009C20)日 补偿的无功电能量

    DD_DRLDDLJH,            //(9090)当日零点电量带日期
    DD_S1RLDDLJH,           //(9091)上1日零点电量带日期
    DD_S2RLDDLJH,           //(9092)上2日零点电量带日期
    DD_S3RLDDLJH,           //(9093)上3日零点电量带日期
    DD_S4RLDDLJH,           //(9094)上4日零点电量带日期
    DD_S5RLDDLJH,           //(9095)上5日零点电量带日期
    DD_S6RLDDLJH,           //(9096)上6日零点电量带日期
    DD_S7RLDDLJH,           //(9097)上7日零点电量带日期

    //********************************************************************************
    //					       月数据标准项
    //********************************************************************************

    MD_ZHYGJH,			    //(0000FF01)组合有功电能示值集合
    MD_ZXYGJH,			    //(0001FF01)正向有功电能示值集合
    MD_FXYGJH,				//(0002FF01)反向有功电能示值集合
    MD_ZXWGJH,				//(0003FF01)正向无功电能示值集合
    MD_FXWGJH,				//(0004FF01)反向无功电能示值集合
    MD_X1WGJH,              //(0005FF01)一象限无功电能示值集合
    MD_X2WGJH,              //(0006FF01)二象限无功电能示值集合
    MD_X3WGJH,              //(0007FF01)三象限无功电能示值集合
    MD_X4WGJH,              //(0008FF01)四象限无功电能示值集合

    MD_ZXSZJH,			    //(0009FF01)正向视在电能示值集合
    MD_FXSZJH,				//(000AFF01)反向视在电能示值集合
    MD_GLZDNJH,				//(00800001)关联总电能集合
    MD_AXDNJH,				//(00150001)A相电能集合
    MD_BXDNJH,				//(00290001)B相电能集合
    MD_CXDNJH,				//(003D0001)C相电能集合

    MD_ZXYGXLJH,            //(0101FF01)正向有功最大需量及发生时间集合
    MD_FXYGXLJH,            //(0102FF01)反向有功最大需量及发生时间集合
    MD_ZXWGXLJH,            //(0103FF01)组合无功1最大需量及发生时间集合
    MD_FXWGXLJH,            //(0104FF01)组合无功2最大需量及发生时间集合
    MD_X1WGXLJH,            //(0105FF01)一象限无功最大需量及发生时间集合
    MD_X2WGXLJH,            //(0106FF01)二象限无功最大需量及发生时间集合
    MD_X3WGXLJH,            //(0107FF01)三象限无功最大需量及发生时间集合
    MD_X4WGXLJH,            //(0108FF01)四象限无功最大需量及发生时间集合
    MD_ZXSZXLJH,			//(0109FF01)正向视在需量及发生时间集合
    MD_FXSZXLJH,			//(010AFF01)反向视在需量及发生时间集合
    MD_AXXLJH,				//(01150001)A相需量及发生时间集合
    MD_BXXLJH,				//(01290001)B相需量及发生时间集合
    MD_CXXLJH,				//(013D0001)C相需量及发生时间集合


    MD_YDYHGLJH,			//(E100C010)月电压合格率统计
    MD_YAXDYHGLJH,			//(E100C011)月A相电压合格率统计
    MD_YBXDYHGLJH,			//(E100C012)月B相电压合格率统计
    MD_YCXDYHGLJH,			//(E100C013)月C相电压合格率统计

    MD_YPLHGLJH,			//(E100C014)月频率合格率统计数据
    MD_YAXDLJXZJH,			//(E100C015)月A相电流极限值统计
    MD_YBXDLJXZJH,			//(E100C016)月B相电流极限值统计
    MD_YCXDLJXZJH,			//(E100C017)月C相电流极限值统计

    MD_YYGGLJXZJH,			//(E100C018)月有功功率极限值统计
    MD_YWGGLJXZJH,			//(E100C019)月无功功率极限值统计
    MD_YGLYSJXZJH,			//(E100C01A)月功率因数极限值统计
    MD_YZXDLJZJH,			//(E100C01B)月正序电流极值统计
    MD_YFXDLJZJH,			//(E100C01C)月负序电流极值统计
    MD_YLXDLJZJH,			//(E100C01D)月零序电流极值统计数据

    MD_YDLYXTJJH,			//(E100C020)月电流不平衡越限统计
    MD_YDLHGLTJJH,			//(E100C021)月电流不平衡合格率统计

    MD_YAXDYSBHGL,          //(0xE100C022)月A相电压闪变合格率统计
    MD_YBXDYSBHGL,          //(0xE100C023)月B相电压闪变合格率统计
    MD_YCXDYSBHGL,          //(0xE100C024)月C相电压闪变合格率统计
    MD_YAXDYJBHGL,          //(0xE100C025)月A相电压畸变合格率统计
    MD_YBXDYJBHGL,          //(0xE100C026)月B相电压畸变合格率统计
    MD_YCXDYJBHGL,          //(0xE100C027)月C相电压畸变合格率统计

    MD_YAXDYBDZ,            //(0xE100C028)月A相电压波动值
    MD_YBXDYBDZ,            //(0xE100C029)月B相电压波动值
    MD_YCXDYBDZ,            //(0xE100C02A)月C相电压波动值
    MD_YTDZCS,			    //(E100C030)月停电总次数
    MD_YTDZSJ,			    //(E100C031)月停电总时间

    MD_YDXSJJH,			    //(E100C040)月断相数据集合
    MD_YSYSJJH,			    //(E100C041)月失压数据集合
    MD_YSLSJJH,			    //(E100C042)月失流数据集合

    MD_AXDYXBZDZJH,         //(E100C1FF)A相电压谐波最大值
    MD_BXDYXBZDZJH,         //(E100C2FF)B相电压谐波最大值
    MD_CXDYXBZDZJH,         //(E100C3FF)C相电压谐波最大值
    MD_AXDLXBZDZJH,         //(E100C4FF)A相电流谐波最大值
    MD_BXDLXBZDZJH,         //(E100C5FF)B相电流谐波最大值
    MD_CXDLXBZDZJH,         //(E100C6FF)C相电流谐波最大值

    MD_AXDYXBZXZJH,         //(E100C7FF)A相电压谐波最小值
    MD_BXDYXBZXZJH,         //(E100C8FF)B相电压谐波最小值
    MD_CXDYXBZXZJH,         //(E100C9FF)C相电压谐波最小值
    MD_AXDLXBZXZJH,         //(E100CAFF)A相电流谐波最小值
    MD_BXDLXBZXZJH,         //(E100CBFF)B相电流谐波最小值
    MD_CXDLXBZXZJH,         //(E100CCFF)C相电流谐波最小值

    MD_AXDYXBPJZJH,         //(E100D0FF)A相电压谐波平均值
    MD_BXDYXBPJZJH,         //(E100D1FF)B相电压谐波平均值
    MD_CXDYXBPJZJH,         //(E100D2FF)C相电压谐波平均值
    MD_AXDLXBPJZJH,         //(E100D3FF)A相电流谐波平均值
    MD_BXDLXBPJZJH,         //(E100D4FF)B相电流谐波平均值
    MD_CXDLXBPJZJH,         //(E100D5FF)C相电流谐波平均值

    MD_AXDYXB95JH,          //(E100D6FF)A相电压谐波95%值
    MD_BXDYXB95JH,          //(E100D7FF)B相电压谐波95%值
    MD_CXDYXB95JH,          //(E100D8FF)C相电压谐波95%值
    MD_AXDLXB95JH,          //(E100D9FF)A相电流谐波95%值
    MD_BXDLXB95JH,          //(E100DAFF)B相电流谐波95%值
    MD_CXDLXB95JH,          //(E100DBFF)C相电流谐波95%值

    MD_WGBCDNL,             //(E100DB20)日 补偿的无功电能量

	//********************************************************************************
    //					     曲线数据标准项
    //********************************************************************************

    //
    CD_ZXYGZ1,				//上1点
    CD_ZXYGZ2,				//上2点
    CD_ZXYGZ3,				//上3点
    CD_ZXYGZ4,				//上4点
    CD_ZXYGZ5,				//上5点
    CD_ZXYGZ6,				//上6点
    CD_ZXYGZ7,				//上7点
    CD_ZXYGZ8,				//上8点
    CD_ZXYGZ9,				//上9点
    CD_ZXYGZ10,				//上10点
    CD_ZXYGZ11,				//上11点
    CD_ZXYGZ12,				//上12点
    CD_ZXYGZ13,				//上13点
    CD_ZXYGZ14,				//上14点
    CD_ZXYGZ15,				//上15点
    CD_ZXYGZ16,				//上16点
    CD_ZXYGZ17,				//上17点
    CD_ZXYGZ18,				//上18点
    CD_ZXYGZ19,				//上19点
    CD_ZXYGZ20,				//上20点
    CD_ZXYGZ21,				//上21点
    CD_ZXYGZ22,				//上22点
    CD_ZXYGZ23,				//上23点
    CD_ZXYGZ24,				//上24点
    CD_ZXYGZ25,				//上25点
    CD_ZXYGZ26,				//上26点
    CD_ZXYGZ27,				//上27点
    CD_ZXYGZ28,				//上28点
    CD_ZXYGZ29,				//上29点
    CD_ZXYGZ30,				//上30点
    CD_ZXYGZ31,				//上31点
    CD_ZXYGZ32,				//上32点
    CD_ZXYGZ33,				//上33点
    CD_ZXYGZ34,				//上34点
    CD_ZXYGZ35,				//上35点
    CD_ZXYGZ36,				//上36点
    CD_ZXYGZ37,				//上37点
    CD_ZXYGZ38,				//上38点
    CD_ZXYGZ39,				//上39点
    CD_ZXYGZ40,				//上40点
    CD_ZXYGZ41,				//上41点
    CD_ZXYGZ42,				//上42点
    CD_ZXYGZ43,				//上43点
    CD_ZXYGZ44,				//上44点
    CD_ZXYGZ45,				//上45点
    CD_ZXYGZ46,				//上46点
    CD_ZXYGZ47,				//上47点
    CD_ZXYGZ48,				//上48点


    CD_ZXYGZ,               //(9010)正有功电量示值总
    CD_FXYGZ,				//(9020)反向有功电量示值总
    CD_ZXWGZ,				//(9110)正向无功电能示值总
    CD_FXWGZ,				//(9120)反向无功电能示值总
    CD_X1WGZ,               //(9130)一象限无功电能示值总
    CD_X2WGZ,               //(9150)二象限无功电能示值总
    CD_X3WGZ,               //(9160)三象限无功电能示值总
    CD_X4WGZ,               //(9140)四象限无功电能示值总

    CD_DYJH,                //(B61F)电压集合
    CD_DLJH,                //(B62F)电流集合
    CD_YGGLJH,              //(B63F)有功功率集合
    CD_WGGLJH,              //(B64F)无功功率集合
    CD_GLYSJH,              //(B65F)功率因数集合

    //********************************************************************************
    //					     广播对时标准项
    //********************************************************************************
    BROADCAST_TIME,            	//广播对时
	//********************************************************************************
    //					     载波校时标准项
    //********************************************************************************
    CA_RD_DBRQ,                 //电能表日期
    CA_RD_DBSJ,
    //********************************************************************************
    //					     参数设置标准项
    //********************************************************************************
    SET_PRM_MP,              //测量点设置

    //********************************************************************************
    //					      结束数据项
    //********************************************************************************
    COL_END_ITEM
};

/**************************************************************************************************
                                          相关数据结构定义
***************************************************************************************************/
//电表抄读数据项
typedef struct _tagDATA_ITEM
{
	INT32U	    nStdItem;	        	//标准项
	INT32U	    nProtItem;		    	//规约项
	INT32U      nProtItem1;		    //规约项1
	INT32U		nOffset;
    INT32S      nItemLen;		    	//数据长度
	INT8U       nItemNum;		    	//步长
}DATA_ITEM, *pDATA_ITEM;

// 抄表97表计数据结构
typedef struct _tagCOL_97_DATA
{
    //INT8U       		        nMeterAddr[LEN_METER_ADDR];		//电表地址(标签)
    STD_TIME    		        sReadTime;                      //读表时间
    //STD_TIME    		        sMeterTime;                     //电表日历时钟
    
	INT8U						nEnergyPP[MAX_FEE_NUM + 1][4];
	INT8U						nEnergyNP[MAX_FEE_NUM + 1][4];
	INT8U						nEnergyPN[MAX_FEE_NUM + 1][4];
	INT8U						nEnergyNN[MAX_FEE_NUM + 1][4];

	INT8U                       nQ1Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nQ2Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nQ3Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nQ4Energy[MAX_FEE_NUM + 1][4];
	
	INT8U                       nPPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nNPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nPNDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nNNDemand[MAX_FEE_NUM + 1][8];
	
	INT8U                       nVolt[MAX_PHASE_NUM][2];
	INT8U                       nCurr[MAX_PHASE_NUM][3];
	INT8U						nPWatt[1 + MAX_PHASE_NUM][3];
	INT8U						nNWatt[1 + MAX_PHASE_NUM][3];
	INT8U						nSWatt[1 + MAX_PHASE_NUM][3];
	INT8U                       nFactor[1 + MAX_PHASE_NUM][2];

	//实时
	INT8U						nDate[4];										//电表日期
	INT8U						nTime[3];										//电表时钟

	//日
	INT8U						nDayTime[5];									//日冻结时间
	INT8U                       nDEnergyPP[MAX_FEE_NUM + 1][4];
	INT8U                       nDEnergyNP[MAX_FEE_NUM + 1][4];
	INT8U                       nDEnergyPN[MAX_FEE_NUM + 1][4];
	INT8U                       nDEnergyNN[MAX_FEE_NUM + 1][4];
	
    INT8U                       nDQ1Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nDQ2Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nDQ3Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nDQ4Energy[MAX_FEE_NUM + 1][4];
	
	INT8U                       nDPPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nDNPDemand[MAX_FEE_NUM + 1][8];
	//INT8U                       nDPNDemand[MAX_FEE_NUM + 1][8];
	//INT8U                       nDNNDemand[MAX_FEE_NUM + 1][8];

	//月
	INT8U                       nMEnergyPP[MAX_FEE_NUM + 1][4];
	INT8U                       nMEnergyNP[MAX_FEE_NUM + 1][4];
	INT8U                       nMEnergyPN[MAX_FEE_NUM + 1][4];
	INT8U                       nMEnergyNN[MAX_FEE_NUM + 1][4];
	
    INT8U                       nMQ1Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nMQ2Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nMQ3Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nMQ4Energy[MAX_FEE_NUM + 1][4];
	
	INT8U                       nMPPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nMNPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nMPNDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nMNNDemand[MAX_FEE_NUM + 1][8];

	
}COL_97_DATA, *pCOL_97_DATA;

// 表07表计数据结构
typedef struct _tagCOL_07_DATA
{
    //INT8U       		        nMeterAddr[LEN_METER_ADDR];		//电表地址(标签)
    STD_TIME    		        sReadTime;                      //读表时间
    //STD_TIME    		        sMeterTime;                     //电表日历时钟
    
	INT8U						nEnergyPP[MAX_FEE_NUM + 1][4];	//正有功
	INT8U						nEnergyNP[MAX_FEE_NUM + 1][4];	//反有功
	INT8U						nEnergyPN[MAX_FEE_NUM + 1][4];
	INT8U						nEnergyNN[MAX_FEE_NUM + 1][4];
						
	INT8U                       nQ1Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nQ2Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nQ3Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nQ4Energy[MAX_FEE_NUM + 1][4];
	
	INT8U                       nPPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nNPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nPNDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nNNDemand[MAX_FEE_NUM + 1][8];
	
	INT8U                       nVolt[MAX_PHASE_NUM][2];
	INT8U                       nCurr[MAX_PHASE_NUM][3];
	INT8U						nPWatt[1 + MAX_PHASE_NUM][3];
	INT8U						nNWatt[1 + MAX_PHASE_NUM][3];
	INT8U						nSWatt[1 + MAX_PHASE_NUM][3];
	INT8U                       nFactor[1 + MAX_PHASE_NUM][2];

	//实时
	INT8U						nDate[4];										//电表日期
	INT8U						nTime[3];										//电表时钟

	//日
	INT8U						nDayTime[5];									//日冻结时间
	INT8U                       nDEnergyPP[MAX_FEE_NUM + 1][4];
	INT8U                       nDEnergyNP[MAX_FEE_NUM + 1][4];
	INT8U                       nDEnergyPN[MAX_FEE_NUM + 1][4];
	INT8U                       nDEnergyNN[MAX_FEE_NUM + 1][4];
	
    INT8U                       nDQ1Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nDQ2Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nDQ3Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nDQ4Energy[MAX_FEE_NUM + 1][4];
	
	INT8U                       nDPPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nDNPDemand[MAX_FEE_NUM + 1][8];
	//INT8U                       nDPNDemand[MAX_FEE_NUM + 1][8];
	//INT8U                       nDNNDemand[MAX_FEE_NUM + 1][8];

	//月
	INT8U                       nMEnergyPP[MAX_FEE_NUM + 1][4];
	INT8U                       nMEnergyNP[MAX_FEE_NUM + 1][4];
	INT8U                       nMEnergyPN[MAX_FEE_NUM + 1][4];
	INT8U                       nMEnergyNN[MAX_FEE_NUM + 1][4];
	
    INT8U                       nMQ1Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nMQ2Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nMQ3Energy[MAX_FEE_NUM + 1][4];
	INT8U                       nMQ4Energy[MAX_FEE_NUM + 1][4];
	
	INT8U                       nMPPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nMNPDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nMPNDemand[MAX_FEE_NUM + 1][8];
	INT8U                       nMNNDemand[MAX_FEE_NUM + 1][8];

}COL_07_DATA, *pCOL_07_DATA;


// 抄表数据结构
typedef union _tagCOL_DATA
{
	COL_97_DATA					uCol97Data;
	COL_07_DATA					uCol07Data;
}COL_DATA, *pCOL_DATA;



//表计信息
typedef struct _tagMETER_INFO
{
	INT16U					nSeq;							// 0时无效
	INT16U					nMp;
	INT8U					nMeterAddr[6];
	INT8U					nProt;
	INT8U					nPort;							// 抄表端口
	INT8U					nImUser;
	INT8U					nRdFlgs;						// 抄表标志
}METER_INFO, *pMETER_INFO;

/**************************************************************************************************
                                         AC事件数据项
***************************************************************************************************/
#define         MAX_AC_SY_NUM           0x35                //AC失压最大数据项
#define         MAX_AC_GY_NUM           0x35                //AC过压最大数据项
#define         MAX_AC_SL_NUM           0x31                //AC失流最大数据项
#define         MAX_AC_GL_NUM           0x31                //AC过流最大数据项
#define         MAX_AC_CL_NUM           0x31                //AC潮流反向最大数据项
#define         MAX_AC_GZ_NUM           0x31                //AC过载最大数据项
#define         MAX_AC_DX_NUM           0x35                //AC断相最大数据项
#define         MAX_AC_DYNX_NUM	        0x22                //AC电压逆相序最大数据项

extern    const     INT8U       g_nACSY[];                  //AC失压
extern    const     INT8U       g_nACGY[];                  //AC过压
extern    const     INT8U       g_nACSL[];                  //AC失流
extern    const     INT8U       g_nACGL[];                  //AC过流
extern    const     INT8U       g_nACGZ[];                  //AC过载
extern    const     INT8U       g_nACDX[];                  //AC断相
extern    const     INT8U       g_nACDYNX[];                //AC电压逆相序


/**************************************************************************************************
                                          相关全局变量声明
***************************************************************************************************/

/**************************************************************************************************
                                         初始化数据项
***************************************************************************************************/

/**************************************************************************************************
                                         补充抄读数据项
***************************************************************************************************/
INT32S 			GetItemMap(INT8U nRdFlags, INT8U nIsImUser, INT8U nIsRs485, INT8U* pItemMap);
INT32S 			IsCurrRdItem(INT8U* pItemMap, INT32U nStdItem);

INT32S 			Gb645ToColData(INT8U nProt, INT8U* pGb645Frm, void* pDataItem, void* pColData);
INT32S 			MeterMpInit(void);
INT32S 			MeterMpMap(INT8U nType);
INT32S 			SetRdMeterFlags(INT16U nSeq, INT8U nFlgBit);
INT32S          ClrRdMeterFlags(INT16U nSeq, INT8U nRdTd);
INT32S 			GetRdMeterFlags(INT16U nSeq, INT8U* pRdFlags);
INT32S 			SetBlockFlg(INT16U nSeq, INT8U nBlk);
INT32S 			GetBlockFlg(INT16U nSeq);
INT32S 			GetMeterInfo(INT16U nSeq, void* pInfo);
INT32S          GetSeqFromAddr(INT8U* pAddr);
INT16U 			GetMpFromSeq(INT16U nSeq);
INT16U 			GetSeqFromMp(INT16U nMp);
INT16U 			GetImSeqFromSeq(INT16U nSeq);

INT32S          GetPulsePrm(INT16U nSeq, pMEM_PULSE_PRM pPulsePrm);

INT8S           SetRdMeterFlagsAllMp(INT8U nRdTd);
INT8S           ClrRdMeterFlagsAllMp(INT8U nRdTd);


#endif