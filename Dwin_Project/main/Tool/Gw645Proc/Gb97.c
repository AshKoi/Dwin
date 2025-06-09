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
#include "Base.h"
#include "DataItem.h"
#include "Gb97.h"



/**************************************************************************************************
                                          宏
***************************************************************************************************/
#define GB_97DATA_OFF(item)										((INT32U)(&(((pCOL_DATA)0)->uCol97Data.item)))
#ifndef DATAITEM_DEFINE
#define DATAITEM_DEFINE(nStdItem, nProtItem, nUnit, nItemNum)		{nStdItem, nProtItem, INVLD_ITEM, GB_97DATA_OFF(nUnit), 0, nItemNum}
#endif

/**************************************************************************************************
                                           变量
***************************************************************************************************/

const DATA_ITEM	g_sGb97Item[] = {
#if 0
	//			
#endif	
			//实时数据
			DATAITEM_DEFINE(RD_ZXYGJH, 0x901F, nEnergyPP[0][0], 6),
			DATAITEM_DEFINE(RD_ZXYGJH, 0x9010, nEnergyPP[0][0], 1),
			DATAITEM_DEFINE(RD_ZXYGJH, 0x9011, nEnergyPP[1][0], 1),
			DATAITEM_DEFINE(RD_ZXYGJH, 0x9012, nEnergyPP[2][0], 1),
			DATAITEM_DEFINE(RD_ZXYGJH, 0x9013, nEnergyPP[3][0], 1),
			DATAITEM_DEFINE(RD_ZXYGJH, 0x9014, nEnergyPP[4][0], 1),

			DATAITEM_DEFINE(RD_FXYGJH, 0x902F, nEnergyNP[0][0], 6),
			DATAITEM_DEFINE(RD_FXYGJH, 0x9020, nEnergyNP[0][0], 1),
			DATAITEM_DEFINE(RD_FXYGJH, 0x9021, nEnergyNP[1][0], 1),
			DATAITEM_DEFINE(RD_FXYGJH, 0x9022, nEnergyNP[2][0], 1),
			DATAITEM_DEFINE(RD_FXYGJH, 0x9023, nEnergyNP[3][0], 1),
			DATAITEM_DEFINE(RD_FXYGJH, 0x9024, nEnergyNP[4][0], 1),

			DATAITEM_DEFINE(RD_ZXWGJH, 0x911F, nEnergyPN[0][0], 6),
			DATAITEM_DEFINE(RD_ZXWGJH, 0x9110, nEnergyPN[0][0], 1),
			DATAITEM_DEFINE(RD_ZXWGJH, 0x9111, nEnergyPN[1][0], 1),
			DATAITEM_DEFINE(RD_ZXWGJH, 0x9112, nEnergyPN[2][0], 1),
			DATAITEM_DEFINE(RD_ZXWGJH, 0x9113, nEnergyPN[3][0], 1),
			DATAITEM_DEFINE(RD_ZXWGJH, 0x9114, nEnergyPN[4][0], 1),

			DATAITEM_DEFINE(RD_FXWGJH, 0x912F, nEnergyNN[0][0], 6),
			DATAITEM_DEFINE(RD_FXWGJH, 0x9120, nEnergyNN[0][0], 1),
			DATAITEM_DEFINE(RD_FXWGJH, 0x9121, nEnergyNN[1][0], 1),
			DATAITEM_DEFINE(RD_FXWGJH, 0x9122, nEnergyNN[2][0], 1),
			DATAITEM_DEFINE(RD_FXWGJH, 0x9123, nEnergyNN[3][0], 1),
			DATAITEM_DEFINE(RD_FXWGJH, 0x9124, nEnergyNN[4][0], 1),

			DATAITEM_DEFINE(RD_X1WGJH, 0x913F, nQ1Energy[0][0], 6),
			DATAITEM_DEFINE(RD_X1WGJH, 0x9120, nQ1Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X1WGJH, 0x9121, nQ1Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X1WGJH, 0x9122, nQ1Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X1WGJH, 0x9123, nQ1Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X1WGJH, 0x9124, nQ1Energy[0][0], 1),

			DATAITEM_DEFINE(RD_X2WGJH, 0x915F, nQ2Energy[0][0], 6),
			DATAITEM_DEFINE(RD_X2WGJH, 0x9150, nQ2Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X2WGJH, 0x9151, nQ2Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X2WGJH, 0x9152, nQ2Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X2WGJH, 0x9153, nQ2Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X2WGJH, 0x9154, nQ2Energy[0][0], 1),

			DATAITEM_DEFINE(RD_X3WGJH, 0x916F, nQ3Energy[0][0], 6),
			DATAITEM_DEFINE(RD_X3WGJH, 0x9160, nQ3Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X3WGJH, 0x9161, nQ3Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X3WGJH, 0x9162, nQ3Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X3WGJH, 0x9163, nQ3Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X3WGJH, 0x9164, nQ3Energy[0][0], 1),

			DATAITEM_DEFINE(RD_X4WGJH, 0x914F, nQ4Energy[0][0], 6),
			DATAITEM_DEFINE(RD_X4WGJH, 0x9140, nQ4Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X4WGJH, 0x9141, nQ4Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X4WGJH, 0x9142, nQ4Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X4WGJH, 0x9143, nQ4Energy[0][0], 1),
			DATAITEM_DEFINE(RD_X4WGJH, 0x9144, nQ4Energy[0][0], 1),

			DATAITEM_DEFINE(RD_ZXYGXLJH, 0xA01F, nPPDemand[0][0], 6),//当前正向有功最大需量: 07表需量与时间在一起 97表是分开
			DATAITEM_DEFINE(RD_ZXYGXLJH, 0xA010, nPPDemand[0][0], 1),
			DATAITEM_DEFINE(RD_ZXYGXLJH, 0xA011, nPPDemand[1][0], 1),
			DATAITEM_DEFINE(RD_ZXYGXLJH, 0xA012, nPPDemand[2][0], 1),
			DATAITEM_DEFINE(RD_ZXYGXLJH, 0xA013, nPPDemand[3][0], 1),
			DATAITEM_DEFINE(RD_ZXYGXLJH, 0xA014, nPPDemand[4][0], 1),
			
            ////当前正向有功最大需量发生时间
            //DATAITEM_DEFINE(RD_ZXYGXLTJH, 0xB01F, nPPTDemand[0][0], 6),//当前正向有功最大需量发生时间: 07表需量与时间在一起 97表是分开
			//DATAITEM_DEFINE(RD_ZXYGXLTJH, 0xB010, nPPTDemand[0][0], 1),
			//DATAITEM_DEFINE(RD_ZXYGXLTJH, 0xB011, nPPTDemand[1][0], 1),
			//DATAITEM_DEFINE(RD_ZXYGXLTJH, 0xB012, nPPTDemand[2][0], 1),
			//DATAITEM_DEFINE(RD_ZXYGXLTJH, 0xB013, nPPTDemand[3][0], 1),
			//DATAITEM_DEFINE(RD_ZXYGXLTJH, 0xB014, nPPTDemand[4][0], 1),

			DATAITEM_DEFINE(RD_FXYGXLJH, 0xA02F, nNPDemand[0][0], 6),//当前反向有功总最大需量及发生时间
            DATAITEM_DEFINE(RD_FXYGXLJH, 0xA020, nNPDemand[0][0], 1),
            DATAITEM_DEFINE(RD_FXYGXLJH, 0xA021, nNPDemand[1][0], 1),
            DATAITEM_DEFINE(RD_FXYGXLJH, 0xA022, nNPDemand[2][0], 1),
            DATAITEM_DEFINE(RD_FXYGXLJH, 0xA023, nNPDemand[3][0], 1),
            DATAITEM_DEFINE(RD_FXYGXLJH, 0xA024, nNPDemand[4][0], 1),
            
            //当前反向有功总最大需量发生时间
            //DATAITEM_DEFINE(RD_FXYGXLTJH, 0xB02F, nNPTDemand[0][0], 6),//当前反向有功总最大需量发生时间
            //DATAITEM_DEFINE(RD_FXYGXLTJH, 0xB020, nNPTDemand[0][0], 1),
            //DATAITEM_DEFINE(RD_FXYGXLTJH, 0xB021, nNPTDemand[1][0], 1),
            //DATAITEM_DEFINE(RD_FXYGXLTJH, 0xB022, nNPTDemand[2][0], 1),
            //DATAITEM_DEFINE(RD_FXYGXLTJH, 0xB023, nNPTDemand[3][0], 1),
            //DATAITEM_DEFINE(RD_FXYGXLTJH, 0xB024, nNPTDemand[4][0], 1),

			DATAITEM_DEFINE(RD_ZXWGXLJH, 0xA11F, nPNDemand[0][0], 6),//当前正向无功最大需量及发生时间
			DATAITEM_DEFINE(RD_ZXWGXLJH, 0xA110, nPNDemand[0][0], 1),
			DATAITEM_DEFINE(RD_ZXWGXLJH, 0xA111, nPNDemand[1][0], 1),
			DATAITEM_DEFINE(RD_ZXWGXLJH, 0xA112, nPNDemand[2][0], 1),
			DATAITEM_DEFINE(RD_ZXWGXLJH, 0xA113, nPNDemand[3][0], 1),
			DATAITEM_DEFINE(RD_ZXWGXLJH, 0xA114, nPNDemand[4][0], 1),

            //当前正向无功最大需量发生时间
			//DATAITEM_DEFINE(RD_ZXWGXLTJH, 0xB11F, nPNTDemand[0][0], 6),//当前正向无功最大需量发生时间
			//DATAITEM_DEFINE(RD_ZXWGXLTJH, 0xB110, nPNTDemand[0][0], 1),
			//DATAITEM_DEFINE(RD_ZXWGXLTJH, 0xB111, nPNTDemand[1][0], 1),
			//DATAITEM_DEFINE(RD_ZXWGXLTJH, 0xB112, nPNTDemand[2][0], 1),
			//DATAITEM_DEFINE(RD_ZXWGXLTJH, 0xB113, nPNTDemand[3][0], 1),
			//DATAITEM_DEFINE(RD_ZXWGXLTJH, 0xB114, nPNTDemand[4][0], 1),

			DATAITEM_DEFINE(RD_FXWGXLJH, 0xA12F, nNNDemand[0][0], 6),//当前反向无功最大需量及发生时间
            DATAITEM_DEFINE(RD_FXWGXLJH, 0xA120, nNNDemand[0][0], 1),
            DATAITEM_DEFINE(RD_FXWGXLJH, 0xA121, nNNDemand[1][0], 1),
            DATAITEM_DEFINE(RD_FXWGXLJH, 0xA122, nNNDemand[2][0], 1),
            DATAITEM_DEFINE(RD_FXWGXLJH, 0xA123, nNNDemand[3][0], 1),
            DATAITEM_DEFINE(RD_FXWGXLJH, 0xA124, nNNDemand[4][0], 1), 
            
            //当前反向无功最大需量及发生时间
            //DATAITEM_DEFINE(RD_FXWGXLTJH, 0xB12F, nNNDemand[0][0], 6),//当前反向无功最大需量及发生时间
            //DATAITEM_DEFINE(RD_FXWGXLTJH, 0xB120, nNNDemand[0][0], 1),
            //DATAITEM_DEFINE(RD_FXWGXLTJH, 0xB121, nNNDemand[1][0], 1),
            //DATAITEM_DEFINE(RD_FXWGXLTJH, 0xB122, nNNDemand[2][0], 1),
            //DATAITEM_DEFINE(RD_FXWGXLTJH, 0xB123, nNNDemand[3][0], 1),
            //DATAITEM_DEFINE(RD_FXWGXLTJH, 0xB124, nNNDemand[4][0], 1), 

			DATAITEM_DEFINE(RD_DYJH, 0xB61F, nVolt[0][0], 4), //三相电压集合
			DATAITEM_DEFINE(RD_DYJH, 0xB611, nVolt[0][0], 1),//A相电压
			DATAITEM_DEFINE(RD_DYJH, 0xB612, nVolt[1][0], 1),//B相电压
			DATAITEM_DEFINE(RD_DYJH, 0xB613, nVolt[2][0], 1),//C相电压

			DATAITEM_DEFINE(RD_DLJH, 0xB62F, nCurr[0][0], 4),//三相电流集合
			DATAITEM_DEFINE(RD_DLJH, 0xB621, nCurr[0][0], 1),//A相电流
			DATAITEM_DEFINE(RD_DLJH, 0xB622, nCurr[1][0], 1),//B相电流
			DATAITEM_DEFINE(RD_DLJH, 0xB623, nCurr[2][0], 1),//C相电流

			DATAITEM_DEFINE(RD_YGGLJH, 0xB63F, nPWatt[0][0], 5),//有功功率集合
			DATAITEM_DEFINE(RD_YGGLJH, 0xB630, nPWatt[0][0], 1),//总有功功率(瞬时有功功率)
			DATAITEM_DEFINE(RD_YGGLJH, 0xB631, nPWatt[0][0], 1),//A相有功功率
			DATAITEM_DEFINE(RD_YGGLJH, 0xB632, nPWatt[1][0], 1),//B相有功功率
            DATAITEM_DEFINE(RD_YGGLJH, 0xB633, nPWatt[2][0], 1),//C相有功功率

			DATAITEM_DEFINE(RD_WGGLJH, 0xB64F, nNWatt[0][0], 5),//无功功率集合
			DATAITEM_DEFINE(RD_WGGLJH, 0xB640, nNWatt[0][0], 1),//总无功功率(瞬时无功功率)
			DATAITEM_DEFINE(RD_WGGLJH, 0xB641, nNWatt[0][0], 1),//A相无功功率
			DATAITEM_DEFINE(RD_WGGLJH, 0xB642, nNWatt[1][0], 1),//B相无功功率
			DATAITEM_DEFINE(RD_WGGLJH, 0xB643, nNWatt[2][0], 1),//C相无功功率

			//DATAITEM_DEFINE(RD_SZGLJH, 0x0205FF00, nSWatt[0][0], 5),//视在功率集合
			//DATAITEM_DEFINE(RD_SZGLJH, 0x02050000, nSWatt[0][0], 1),//总视在功率
			//DATAITEM_DEFINE(RD_SZGLJH, 0x02050100, nSWatt[1][0], 1),//A相视在功率
			//DATAITEM_DEFINE(RD_SZGLJH, 0x02050200, nSWatt[2][0], 1),//B相视在功率
			//DATAITEM_DEFINE(RD_SZGLJH, 0x02050300, nSWatt[3][0], 1),//C相视在功率

            DATAITEM_DEFINE(RD_GLYSJH, 0xB65F, nFactor[0][0], 5),//功率因数集合
            DATAITEM_DEFINE(RD_GLYSJH, 0xB650, nFactor[0][0], 1),//总功率因数
            DATAITEM_DEFINE(RD_GLYSJH, 0xB651, nFactor[1][0], 1),//A相功率因数
            DATAITEM_DEFINE(RD_GLYSJH, 0xB652, nFactor[2][0], 1),//B相功率因数
            DATAITEM_DEFINE(RD_GLYSJH, 0xB653, nFactor[3][0], 1),//C相功率因数
            
			DATAITEM_DEFINE(RD_DBRQ, 0xC010, nDate[0], 1),			//电能表日历时钟的日期
			DATAITEM_DEFINE(RD_DBSJ, 0xc011, nTime[0], 1),			//电能表日历时钟的时间
			
#if 0
			// 月冻结数据
#endif
			DATAITEM_DEFINE(MD_ZXYGJH, 0x941F, nMEnergyPP[0][0], 6),//上月正向有功电量示值
			DATAITEM_DEFINE(MD_ZXYGJH, 0x9410, nMEnergyPP[0][0], 1),//上月正向有功总
			DATAITEM_DEFINE(MD_ZXYGJH, 0x9411, nMEnergyPP[1][0], 1),//上月正向有功尖
			DATAITEM_DEFINE(MD_ZXYGJH, 0x9412, nMEnergyPP[2][0], 1),//上月正向有功峰
			DATAITEM_DEFINE(MD_ZXYGJH, 0x9413, nMEnergyPP[3][0], 1),//上月正向有功平
			DATAITEM_DEFINE(MD_ZXYGJH, 0x9414, nMEnergyPP[4][0], 1),//上月正向有功谷

			DATAITEM_DEFINE(MD_FXYGJH, 0x942F, nMEnergyNP[0][0], 6),//上月反向有功电量示值
			DATAITEM_DEFINE(MD_FXYGJH, 0x9420, nMEnergyNP[0][0], 1),//上月反向有功总
			DATAITEM_DEFINE(MD_FXYGJH, 0x9421, nMEnergyNP[1][0], 1),//上月反向有功尖
			DATAITEM_DEFINE(MD_FXYGJH, 0x9422, nMEnergyNP[2][0], 1),//上月反向有功峰
			DATAITEM_DEFINE(MD_FXYGJH, 0x9423, nMEnergyNP[3][0], 1),//上月反向有功平
			DATAITEM_DEFINE(MD_FXYGJH, 0x9424, nMEnergyNP[4][0], 1),//上月反向有功谷

			DATAITEM_DEFINE(MD_ZXWGJH, 0x951F, nMEnergyPN[0][0], 6),//上月正向无功电能示值
			DATAITEM_DEFINE(MD_ZXWGJH, 0x9510, nMEnergyPN[0][0], 1),//上月正向无功总
			DATAITEM_DEFINE(MD_ZXWGJH, 0x9511, nMEnergyPN[1][0], 1),//上月正向无功尖
			DATAITEM_DEFINE(MD_ZXWGJH, 0x9512, nMEnergyPN[2][0], 1),//上月正向无功峰
			DATAITEM_DEFINE(MD_ZXWGJH, 0x9513, nMEnergyPN[3][0], 1),//上月正向无功平
			DATAITEM_DEFINE(MD_ZXWGJH, 0x9514, nMEnergyPN[4][0], 1),//上月正向无功谷

			DATAITEM_DEFINE(MD_FXWGJH, 0x952F, nMEnergyNN[0][0], 6),//上月反向无功电能示值
            DATAITEM_DEFINE(MD_FXWGJH, 0x9520, nMEnergyNN[0][0], 1),//上月反向无功总
            DATAITEM_DEFINE(MD_FXWGJH, 0x9521, nMEnergyNN[0][0], 1),//上月反向无功尖
            DATAITEM_DEFINE(MD_FXWGJH, 0x9522, nMEnergyNN[0][0], 1),//上月反向无功峰
            DATAITEM_DEFINE(MD_FXWGJH, 0x9523, nMEnergyNN[0][0], 1),//上月反向无功平
            DATAITEM_DEFINE(MD_FXWGJH, 0x9524, nMEnergyNN[0][0], 1),//上月反向无功谷

			DATAITEM_DEFINE(MD_X1WGJH, 0x953F, nMQ1Energy[0][0], 6),//上月一象无功电能示值
			DATAITEM_DEFINE(MD_X1WGJH, 0x9530, nMQ1Energy[0][0], 1),//上月一象无功总
			DATAITEM_DEFINE(MD_X1WGJH, 0x9531, nMQ1Energy[1][0], 1),//上月一象无功尖
			DATAITEM_DEFINE(MD_X1WGJH, 0x9532, nMQ1Energy[2][0], 1),//上月一象无功峰
			DATAITEM_DEFINE(MD_X1WGJH, 0x9533, nMQ1Energy[3][0], 1),//上月一象无功平
			DATAITEM_DEFINE(MD_X1WGJH, 0x9534, nMQ1Energy[4][0], 1),//上月一象无功谷

			DATAITEM_DEFINE(MD_X2WGJH, 0x955F, nMQ2Energy[0][0], 6),//上月二象无功电能示值
			DATAITEM_DEFINE(MD_X2WGJH, 0x9550, nMQ2Energy[0][0], 1),//上月二象无功尖
			DATAITEM_DEFINE(MD_X2WGJH, 0x9551, nMQ2Energy[1][0], 1),//上月二象无功尖
			DATAITEM_DEFINE(MD_X2WGJH, 0x9552, nMQ2Energy[2][0], 1),//上月一象无功峰
			DATAITEM_DEFINE(MD_X2WGJH, 0x9553, nMQ2Energy[3][0], 1),//上月二象无功平
			DATAITEM_DEFINE(MD_X2WGJH, 0x9554, nMQ2Energy[4][0], 1),//上月二象无功谷

			DATAITEM_DEFINE(MD_X3WGJH, 0x956F, nMQ3Energy[0][0], 6),//上月三象无功电能示值
			DATAITEM_DEFINE(MD_X3WGJH, 0x9560, nMQ3Energy[0][0], 1),//上月三象无功总
			DATAITEM_DEFINE(MD_X3WGJH, 0x9561, nMQ3Energy[1][0], 1),//上月三象无功尖
			DATAITEM_DEFINE(MD_X3WGJH, 0x9562, nMQ3Energy[2][0], 1),//上月三象无功峰
			DATAITEM_DEFINE(MD_X3WGJH, 0x9563, nMQ3Energy[3][0], 1),//上月三象无功平
			DATAITEM_DEFINE(MD_X3WGJH, 0x9564, nMQ3Energy[4][0], 1),//上月三象无功谷

			DATAITEM_DEFINE(MD_X4WGJH, 0x954F, nMQ4Energy[0][0], 6),//上月四象无功电能示值
		    DATAITEM_DEFINE(MD_X4WGJH, 0x9540, nMQ4Energy[0][0], 1),//上月四象无功总
	        DATAITEM_DEFINE(MD_X4WGJH, 0x9541, nMQ4Energy[1][0], 1),//上月四象无功尖
		    DATAITEM_DEFINE(MD_X4WGJH, 0x9542, nMQ4Energy[2][0], 1),//上月四象无功峰
		    DATAITEM_DEFINE(MD_X4WGJH, 0x9543, nMQ4Energy[3][0], 1),//上月四象无功平
			DATAITEM_DEFINE(MD_X4WGJH, 0x9544, nMQ4Energy[4][0], 1),//上月四象无功谷

			DATAITEM_DEFINE(MD_ZXYGXLJH, 0xA41F, nMPPDemand[0][0], 6),//上月正向有功最大需量及发生时间
			DATAITEM_DEFINE(MD_ZXYGXLJH, 0xA410, nMPPDemand[0][0], 1),//上月正向有功总最大需量及发生时间
			DATAITEM_DEFINE(MD_ZXYGXLJH, 0xA411, nMPPDemand[1][0], 1),//上月正向有功尖最大需量及发生时间
			DATAITEM_DEFINE(MD_ZXYGXLJH, 0xA412, nMPPDemand[2][0], 1),//上月正向有功峰最大需量及发生时间
			DATAITEM_DEFINE(MD_ZXYGXLJH, 0xA413, nMPPDemand[3][0], 1),//上月正向有功平最大需量及发生时间
			DATAITEM_DEFINE(MD_ZXYGXLJH, 0xA414, nMPPDemand[4][0], 1),//上月正向有功谷最大需量及发生时间

            ////上月正向有功最大需量发生时间
			//DATAITEM_DEFINE(MD_ZXYGXLTJH, 0xB41F, nMPPTDemand[0][0], 6),//上月正向有功最大需量发生时间
			//DATAITEM_DEFINE(MD_ZXYGXLTJH, 0xB410, nMPPTDemand[0][0], 1),//上月正向有功总最大需量发生时间
			//DATAITEM_DEFINE(MD_ZXYGXLTJH, 0xB411, nMPPTDemand[1][0], 1),//上月正向有功尖最大需量发生时间
			//DATAITEM_DEFINE(MD_ZXYGXLTJH, 0xB412, nMPPTDemand[2][0], 1),//上月正向有功峰最大需量发生时间
			//DATAITEM_DEFINE(MD_ZXYGXLTJH, 0xB413, nMPPTDemand[3][0], 1),//上月正向有功平最大需量发生时间
			//DATAITEM_DEFINE(MD_ZXYGXLTJH, 0xB414, nMPPTDemand[4][0], 1),//上月正向有功谷最大需量发生时间

			DATAITEM_DEFINE(MD_FXYGXLJH, 0xA42F, nMNPDemand[0][0], 6),//上月反向有功最大需量及发生时间
			DATAITEM_DEFINE(MD_FXYGXLJH, 0xA420, nMNPDemand[0][0], 1),//上月反向有功总最大需量及发生时间
			DATAITEM_DEFINE(MD_FXYGXLJH, 0xA421, nMNPDemand[1][0], 1),//上月反向有功尖最大需量及发生时间
			DATAITEM_DEFINE(MD_FXYGXLJH, 0xA422, nMNPDemand[2][0], 1),//上月反向有功峰最大需量及发生时间
			DATAITEM_DEFINE(MD_FXYGXLJH, 0xA423, nMNPDemand[3][0], 1),//上月反向有功平最大需量及发生时间
			DATAITEM_DEFINE(MD_FXYGXLJH, 0xA424, nMNPDemand[4][0], 1),//上月反向有功谷最大需量及发生时间

            ////上月反向有功最大需量发生时间
            //DATAITEM_DEFINE(MD_FXYGXLTJH, 0xB42F, nMNPTDemand[0][0], 6),//上月反向有功最大需量发生时间
			//DATAITEM_DEFINE(MD_FXYGXLTJH, 0xB420, nMNPTDemand[0][0], 1),//上月反向有功总最大需量发生时间
			//DATAITEM_DEFINE(MD_FXYGXLTJH, 0xB421, nMNPTDemand[1][0], 1),//上月反向有功尖最大需量发生时间
			//DATAITEM_DEFINE(MD_FXYGXLTJH, 0xB422, nMNPTDemand[2][0], 1),//上月反向有功峰最大需量发生时间
			//DATAITEM_DEFINE(MD_FXYGXLTJH, 0xB423, nMNPTDemand[3][0], 1),//上月反向有功平最大需量发生时间
			//DATAITEM_DEFINE(MD_FXYGXLTJH, 0xB424, nMNPTDemand[4][0], 1),//上月反向有功谷最大需量发生时间
     
			DATAITEM_DEFINE(MD_ZXWGXLJH, 0xA51F, nMPNDemand[0][0], 6),//上月正向无功最大需量及发生时间
			DATAITEM_DEFINE(MD_ZXWGXLJH, 0xA510, nMPNDemand[0][0], 1),//上月正向无功总最大需量及发生时间
			DATAITEM_DEFINE(MD_ZXWGXLJH, 0xA511, nMPNDemand[1][0], 1),//上月正向无功尖最大需量及发生时间
			DATAITEM_DEFINE(MD_ZXWGXLJH, 0xA512, nMPNDemand[2][0], 1),//上月正向无功峰最大需量及发生时间
			DATAITEM_DEFINE(MD_ZXWGXLJH, 0xA513, nMPNDemand[3][0], 1),//上月正向无功平最大需量及发生时间
			DATAITEM_DEFINE(MD_ZXWGXLJH, 0xA514, nMPNDemand[4][0], 1),//上月正向无功谷最大需量及发生时间

            //上月正向无功最大需量发生时间
			//DATAITEM_DEFINE(MD_ZXWGXLTJH, 0xB51F, nMPNTDemand[0][0], 6),//上月正向无功最大需量发生时间
			//DATAITEM_DEFINE(MD_ZXWGXLTJH, 0xB510, nMPNTDemand[0][0], 1),//上月正向无功总最大需量发生时间
			//DATAITEM_DEFINE(MD_ZXWGXLTJH, 0xB511, nMPNTDemand[1][0], 1),//上月正向无功尖最大需量发生时间
			//DATAITEM_DEFINE(MD_ZXWGXLTJH, 0xB512, nMPNTDemand[2][0], 1),//上月正向无功峰最大需量发生时间
			//DATAITEM_DEFINE(MD_ZXWGXLTJH, 0xB513, nMPNTDemand[3][0], 1),//上月正向无功平最大需量发生时间
			//DATAITEM_DEFINE(MD_ZXWGXLTJH, 0xB514, nMPNTDemand[4][0], 1),//上月正向无功谷最大需量发生时间

            DATAITEM_DEFINE(MD_FXWGXLJH, 0xA52F, nMNNDemand[0][0], 6),//上月反向无功最大需量及发生时间
			DATAITEM_DEFINE(MD_FXWGXLJH, 0xA520, nMNNDemand[0][0], 1),//上月反向无功总最大需量及发生时间
			DATAITEM_DEFINE(MD_FXWGXLJH, 0xA521, nMNNDemand[1][0], 1),//上月反向无功尖最大需量及发生时间
			DATAITEM_DEFINE(MD_FXWGXLJH, 0xA522, nMNNDemand[2][0], 1),//上月反向无功峰最大需量及发生时间
			DATAITEM_DEFINE(MD_FXWGXLJH, 0xA523, nMNNDemand[3][0], 1),//上月反向无功平最大需量及发生时间
			DATAITEM_DEFINE(MD_FXWGXLJH, 0xA524, nMNNDemand[4][0], 1),//上月反向无功谷最大需量及发生时间

            ////上月反向无功最大需量发生时间
            //DATAITEM_DEFINE(MD_FXWGXLTJH, 0xB52F, nMNNTDemand[0][0], 6),//上月反向无功最大需量发生时间
			//DATAITEM_DEFINE(MD_FXWGXLTJH, 0xB520, nMNNTDemand[0][0], 1),//上月反向无功总最大需量发生时间
			//DATAITEM_DEFINE(MD_FXWGXLTJH, 0xB521, nMNNTDemand[1][0], 1),//上月反向无功尖最大需量发生时间
			///DATAITEM_DEFINE(MD_FXWGXLTJH, 0xB522, nMNNTDemand[2][0], 1),//上月反向无功峰最大需量发生时间
			//DATAITEM_DEFINE(MD_FXWGXLTJH, 0xB523, nMNNTDemand[3][0], 1),//上月反向无功平最大需量发生时间
			//DATAITEM_DEFINE(MD_FXWGXLTJH, 0xB524, nMNNTDemand[4][0], 1),//上月反向无功谷最大需量发生时间


			//********************************************************************************
			//									结束数据项
			//********************************************************************************
			{COL_END_ITEM,	 INVLD_ITEM,	INVLD_ITEM,    sizeof(COL_97_DATA),	 sizeof(COL_97_DATA),  1}
};



/**************************************************************************************************
                                          函数
***************************************************************************************************/
/********************************************************************************
* 函数名:GetGb97IdTbl
* 输　入:
* 输　出:
* 返  回:
* 功  能: 获取97表Tbl
*********************************************************************************/
void* GetGb97IdTbl(void)
{
	return (void*)((pDATA_ITEM)&g_sGb97Item[0]);
}


/********************************************************************************
* 函数名:Gb97ColDataToRealData
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
INT32S Gb97ColDataToRealData(void* pColData)
{
	return 0;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 核查函数
*********************************************************************************/
INT32S Gb97ItemInfoChk(void)
{
	pDATA_ITEM          pGb97Item	= (pDATA_ITEM)(&g_sGb97Item[0]);
	pDATA_ITEM          pGb97Item1	= NULL;
	pDATA_ITEM          pGb97Item2	= NULL;
	INT32S				i			= 0;
	
	// 1. 步长判断: 不能为0/累加步长不能越过最后一项   数据标识判断 偏移量判断: 一直变大
	pGb97Item1	= pGb97Item;
	while(1)
	{
		if (COL_END_ITEM == pGb97Item1->nStdItem)
		{
			break;
		}
		// 1.1. 步长不能为0
		if (0 == pGb97Item1->nItemNum)
		{
			return -1;
		}

		// 1.2. 步长合法性判断，累加步长不能越过最后一项
		pGb97Item2 = pGb97Item1;
		for (i = 0; i < pGb97Item1->nItemNum; i++)
		{
			pGb97Item2++; 
			if (i != pGb97Item1->nItemNum - 1)
			{
				if (COL_END_ITEM == pGb97Item2->nStdItem)
				{
					return -1;
				}
			}
		}

		// 1.3. 数据标识判断
		pGb97Item2 = pGb97Item1;
		while(1)
		{
			pGb97Item2++;
			if (COL_END_ITEM == pGb97Item2->nStdItem)
			{
				break;	
			}
			if (pGb97Item2->nProtItem == pGb97Item1->nProtItem)
			{
				return -1;
			}
		}
		
		// 1.4 偏移量判断: 一直变大
		pGb97Item2 = pGb97Item1 + 1;
		if (pGb97Item2->nOffset < pGb97Item1->nOffset)
		{
			return -1;
		}
		
		pGb97Item1++;
	}

	// 2. 标准项判断:越过步长后的标准项与本身不能相同
	pGb97Item1	= pGb97Item;
	while(1)
	{
		if (COL_END_ITEM == pGb97Item1->nStdItem)
		{
			break;
		}
		pGb97Item2 = pGb97Item1;
		while(1)
		{
			pGb97Item2 += pGb97Item2->nItemNum;
			if (COL_END_ITEM == pGb97Item2->nStdItem)
			{
				break;
			}
			if (pGb97Item2->nStdItem == pGb97Item1->nStdItem)
			{
				return -1;
			}
		}
		
		pGb97Item1 += pGb97Item1->nItemNum;
	}

	return 0;
}







