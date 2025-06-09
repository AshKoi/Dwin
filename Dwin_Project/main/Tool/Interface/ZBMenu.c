/******************************************************************************************************
* Copyright (c) 2013
* 文件名:
* 创建人: 王一凡
* 日　期: 2013/09/12
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
*****************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "Version.h"
#include "Base.h"
#include "ChkCrc.h"
#include "Table.h"
#include "SysRes.h"
#include "GuiwinCfg.h"
#include "Guiwin.h"
#include "Guimenu.h"
#include "Fun.h"
#include "linkcomExt.h"
#include "MemDrv.h"
#include "MemTbl.h"
#include "GB3761SetPrmExt.h"
#include "MemDrvExt.h"
#include "ChkCrc.h"
#include "MemData.h"
#include "DataItem.h"
#include "Gw3761.h"
#include "Lcd.h"
#include "Ac7022.h"
#include "SoftTimer.h"
#include "AppCfg.h"
#include "Key.h"
#include "Macro.h"
#include "rmotlink.h"
#include "ZBMenu.h"
/**************************************************************************************************
                                           宏定义
***************************************************************************************************/
#define GET_KEY_VAL(nKey, nType)	nKey = GetKeyVal(nType)

#define A_MENU_LEN  30

#define DEC2BCD(x)	              ((((x)/10)<<4)+(x)%10)
#define BCD2DEC(x)	              (((x)&0x0F)+((x)>>4)*10)

/**************************************************************************************************
                                           变量定义
***************************************************************************************************/

#define      GRP_LEN               8            //总加组最大配置数
#define      METER_NUM             32           //电能表参数的最大值
#define      INFO_NUM              16           //中文信息

#define      IMP_NAME              "重要信息"
#define      REG_NAME              "常规信息"

/**************************************************************************************************
                                        参数存储偏移信息
***************************************************************************************************/
//实时数据
const MENU_ITEM  g_sRealData[] =
{
    {MENU_ITEM_DISCRIB, "测量点数据", NULL},
	{MENU_ITEM_EXE, "1.当前功率", (void*)CurrWatt},
    {MENU_ITEM_EXE, "2.当前电量",(void*)CurrPower},
    {MENU_ITEM_EXE, "3.负荷曲线", (void*)LoadCurve},
    {MENU_ITEM_EXE, "4.开关状态", (void*)SwitchState},
    {MENU_ITEM_EXE, "5.功控记录", (void*)PowerContrlRd},
    {MENU_ITEM_EXE, "6.电控记录", (void*)ElectronicRd},
    {MENU_ITEM_EXE, "7.遥控记录", (void*)RemotRd},
    {MENU_ITEM_EXE, "8.失电记录", (void*)LosePowerRd},

    {0}
};

//参数定值
const MENU_ITEM  g_sParaFixValue[] =
{
    {MENU_ITEM_DISCRIB, "参数定值", NULL},
	{MENU_ITEM_EXE, "1.时段控参数", (void*)ContrlPara},
    {MENU_ITEM_EXE, "2.厂休控参数", (void*)FactoryPara},
    {MENU_ITEM_EXE, "3.营业保停控", (void*)BusinessPara},
    {MENU_ITEM_EXE, "4.下浮控参数", (void*)FloatPara},
    {MENU_ITEM_EXE, "5.电控参数", (void*)MeterContlr},
    {MENU_ITEM_EXE, "6.KvKiKp", (void*)KvKiKp},
    {MENU_ITEM_EXE, "7.电能表参数", (void*)MeterPara},
    {MENU_ITEM_EXE, "8.配置参数", (void*)ConfigPara},

    {0}
};

//1主菜单--集中器I 型
const MENU_ITEM   g_sZbMenu[] =
{
    {MENU_ITEM_DISCRIB, "主菜单", NULL},
	{MENU_ITEM_SHOW, "1.实时数据", (void*)g_sRealData},
    {MENU_ITEM_SHOW, "2.参数定值", (void*)g_sParaFixValue},
    {MENU_ITEM_EXE, "3.控制状态", (void*)ContrlState},
    {MENU_ITEM_EXE, "4.电能表示数", (void*)MeterData},
    {MENU_ITEM_EXE, "5.中文信息", (void*)ChineseInfo},
    {MENU_ITEM_EXE, "6.购电信息", (void*)BuyInfo},
    {MENU_ITEM_EXE, "7.终端信息", (void*)TerminalInfo},

    {0}
};

/**************************************************************************************************
                                           函数定义
***************************************************************************************************/
//实时数据----当前功率
void CurrWatt(void)
{
  INT8U      nBuf[10]                    = {0};
  INT8U*     pBuf                        = &nBuf[0];
  INT8U      nItemId                     = 1;
  INT8U      nStr[5]                    ={0};
  INT8U      nIdex                       = 0;

  //终端总加组配置参数
  //MemReadGrpPrm(F14, 0, pBuf, sizeof(nBuf));

  memset(pBuf, 0, sizeof(nBuf));
  //当前有功功率
  GuiwinTextOutAuto("当前有功功率");
  for(nItemId = 1 ; nItemId <= GRP_LEN ; nItemId++)
  {
    nIdex = 0;
    memset(&nStr[0] ,0 ,sizeof(nStr));
    BcdToAscii(&pBuf[nIdex], 2, TRUE, &nStr[0]);
    GuiwinTextOutAuto("总加组%x:%5s%c.%c%c%cKw",nItemId, "",nStr[nIdex],nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
  }
  //当前无功功率
  GuiwinTextOutAuto("当前无功功率");
  for(nItemId = 1 ; nItemId <= GRP_LEN ; nItemId++)
  {
    nIdex = 0;
    memset(&nStr[0] ,0 ,sizeof(nStr));
    BcdToAscii(&pBuf[nIdex], 2, TRUE, &nStr[0]);
    GuiwinTextOutAuto("总加组%x:%5s%c.%c%c%cKvar",nItemId, "",nStr[nIdex],nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
  }
  //当前脉冲功率
  GuiwinTextOutAuto("当前脉冲功率");
#define   PluseNum    2
  for(nItemId = 1 ; nItemId <= PluseNum ; nItemId++)
  {
    nIdex = 0;
    memset(&nStr[0] ,0 ,sizeof(nStr));
    BcdToAscii(&pBuf[nIdex], 2, TRUE, &nStr[0]);
    GuiwinTextOutAuto("第%x路:%7s%c.%c%c%cKw",nItemId, "",nStr[nIdex],nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
  }
  GuiwinAutoDispProc(NULL , NULL , 0 , NULL);
}

//当前电量日(月)
void CurrPower(void)
{
  //上下翻页总结组的当前电量，左右切换日或月的数据查看
  INT8U      nBuf[20]                    = {0};
  INT8U*     pBuf                        = &nBuf[0];
  INT8U      nSeqCnt                     = 1;
  INT8U      nKey                        = 0;
  char*     pString                      = NULL;
  INT8U      nDayCntFlg                  = 0;
  INT8U      nEndFlg                     = 0;

  memset(pBuf, 0, sizeof(nBuf));
  //终端总加组配置参数
  //MemReadGrpPrm(F14, 0, pBuf, sizeof(nBuf));
  //当前有功功率
#define    DAY		"日"
#define    MON		"月"
  pString = (char*)DAY;
  while(1)
  {
       GuiwinTextOutAuto("   总加组%x %s数据", nSeqCnt ,pString);

       memset(pBuf, 0, sizeof(nBuf));
       sprintf((char*)&pBuf[0], " -------");
      //数据展示部分
       GuiwinTextOutAuto("有功总:%sMWh" ,pBuf);
       GuiwinTextOutAuto("有功尖:%sMWh" ,pBuf);
       GuiwinTextOutAuto("有功峰:%sMWh" ,pBuf);
       GuiwinTextOutAuto("有功平:%sMWh" ,pBuf);
       GuiwinTextOutAuto("有功谷:%sMWh" ,pBuf);
       GuiwinTextOutAuto("");
       GuiwinTextOutAuto("无功总:%sMvarh" ,pBuf);
       GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);
      //按键处理部分
      while(1)
      {
        GET_KEY_VAL(nKey, 1);
        if(nKey != 0xFF)
        {
          if(nKey == KEY_DOWN)
          {
            nSeqCnt++;
            if(nSeqCnt > GRP_LEN)
            {
              nSeqCnt = 1;
            }
          }
          else if (nKey == KEY_UP)
          {
            nSeqCnt--;
            if(nSeqCnt < 1)
            {
              nSeqCnt = GRP_LEN;
            }
          }
          else if((nKey == KEY_LEFT)||(nKey == KEY_RIGHT))
          {
            nDayCntFlg ++;
            if((nDayCntFlg % 2) != 0)
            {
              pString = (char*)MON;
            }
            else
            {
              pString = (char*)DAY;
            }
          }
          else if(nKey == KEY_CANCEL)
          {
            nEndFlg = 1;
          }
          break;
        }
      }
     if(nEndFlg == 1)
     {
        return ;
     }
  }
}

//负荷曲线
void LoadCurve(void)
{
   //画第一象限的坐标图
   LCD_DrawLine2(0, 0, 0, 0, 1);
}


//当前开关量状态
void  SwitchState(void)
{
   GuiwinTextOutAuto("开关号 %d   %d    %d", 1 ,2 ,3);
   GuiwinTextOutAuto("");
   GuiwinTextOutAuto("开关 未接 未接 未接");
   GuiwinTextOutAuto("");
   GuiwinTextOutAuto("变位 未变 未变 未变");
   GuiwinTextOutAuto("");
   GuiwinTextOutAuto("属性 常闭 常闭 常闭");

   GuiwinAutoDispProc(NULL , NULL , 0 , NULL);
}

//记录类型的按键处理
#define  RECORD_NUM        5
#define  RECORD_ST         1
INT32S  Record(INT8U* pRecordCnt)
{
  INT8U   nKey                          = 0;
  INT8U   nTmpRecordCnt                 = 0;
  INT8U   nFlg                          = 0;

  nTmpRecordCnt = *pRecordCnt;

  while(1)
  {
    GET_KEY_VAL(nKey, 1);
    if(nKey != 0xFF)
    {
      if((nKey == KEY_DOWN)||(nKey == KEY_RIGHT))
      {
         nTmpRecordCnt++;
         if(nTmpRecordCnt > RECORD_NUM)
         {
           nTmpRecordCnt = RECORD_ST;
         }
      }
      else if((nKey == KEY_UP)||(nKey == KEY_LEFT))
      {
        nTmpRecordCnt--;
        if(nTmpRecordCnt < RECORD_ST)
        {
          nTmpRecordCnt = RECORD_NUM;
        }
      }
      else if(nKey == KEY_CANCEL)
      {
        nFlg = 1;
      }

     *pRecordCnt = nTmpRecordCnt;
     break;
    }
  }
  return nFlg;
}

//功控记录
void PowerContrlRd(void)
{
  INT8U   nFlgCnt    = 1;
  INT8U   nEndFlg    = 0;
  while(1)
  {
      GuiwinTextOutAuto("");
      GuiwinTextOutAuto("");
      GuiwinTextOutAuto("%4s无功控记录%x","", nFlgCnt);
      GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);
      nEndFlg = Record(&nFlgCnt);
      if(nEndFlg == 1)
      {
        break;
      }
  }

}

//电控记录
void  ElectronicRd(void)
{
  INT8U   nFlgCnt    = 1;
  INT8U   nEndFlg    = 0;
  while(1)
  {
      GuiwinTextOutAuto("");
      GuiwinTextOutAuto("");
      GuiwinTextOutAuto("%4s无电控记录%x","", nFlgCnt);
      GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);
      nEndFlg = Record(&nFlgCnt);
      if(nEndFlg == 1)
      {
        break;
      }
  }
}

//遥控记录
void RemotRd(void)
{
  INT8U   nFlgCnt    = 1;
  INT8U   nEndFlg    = 0;
  while(1)
  {
      GuiwinTextOutAuto("");
      GuiwinTextOutAuto("");
      GuiwinTextOutAuto("%4s无遥控记录%x","", nFlgCnt);
      GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);
      nEndFlg = Record(&nFlgCnt);
      if(nEndFlg == 1)
      {
        break;
      }
  }
}

//失电记录
void LosePowerRd(void)
{

}

//双向按键处理
INT32S  UpDownLeftRight1(INT8U* pHorizontal ,INT8U* pProject,INT8U* pCaseItem )
{
  INT8U           nKey            = 0;
  INT8S           nHorizontal     = 0;
  INT8S           nCaseItem       = 0;
  INT8U           nFlg            = 0;
  INT8U           nMacro          = 0;
  INT8U           nProject        = 0;

  nHorizontal = *pHorizontal;
  nProject = *pProject;
  nCaseItem = *pCaseItem;

  while(1)
  {
    GET_KEY_VAL(nKey, 1);
    if (nKey != 0xFF)
    {
      //左右按键随其变动 ①
      if(KEY_LEFT == nKey)
      {
         nHorizontal--;
        if(nHorizontal < 1)
        {
          nHorizontal = GRP_LEN;
        }
      }
      else if(KEY_RIGHT == nKey)
      {
        nHorizontal++;
         if(nHorizontal > GRP_LEN)
         {
           nHorizontal = 1;
         }
      }
      //上下键改变屏显，每③次调整更换一次，随之①跟着变化
      else if(KEY_UP == nKey)
      {
        nCaseItem--;
        if(nCaseItem < 1)
        {
            nCaseItem = 3;
            nProject--;
            if(nProject < 1)
            {
                nProject = 3;
                nHorizontal--;
                if(nHorizontal < 1)
                {
                  nHorizontal = GRP_LEN;
                }
            }
        }
      }
      else if(KEY_DOWN == nKey)
      {
        nCaseItem ++;
        if(nCaseItem > 3)
        {
          nCaseItem = 1;
          nProject++;
            if(nProject > 3)
            {
              nProject = 1;
              nHorizontal++;
              if(nHorizontal > GRP_LEN)
              {
                nHorizontal = 1;
              }
            }
        }
      }
      else if(KEY_CANCEL == nKey)
      {
        nFlg = 1 ;
      }
      *pHorizontal = nHorizontal;
      *pProject = nProject;
      *pCaseItem = nCaseItem;
      break;
    }

  }
  return nFlg;
}
//时段控参数
void ContrlPara(void)
{
  INT8U      nBuf[10]                    = {0};
  INT8U*     pBuf                        = &nBuf[0];
  INT8U      nStr[5]                     ={0};
  INT8U      nIdex                       = 0;
  INT8U      nGrpIdx                     = 1;
  INT8U      i                           = 0;
  INT8U      j                           = 2;
  INT8U      nProject                    = 1;
  INT8U      nCycCnt                     = 0;
  INT8U      nFlg                        = 0;
  INT8U      nStepCnt                    = 1;

  memset(pBuf, 0, sizeof(nBuf));
  MemReadGrpPrm(F41, 0, pBuf, sizeof(nBuf));

  while(1)
  {
       //一个完整方案的循环体
      GuiwinTextOutAuto("总加组%x" ,nGrpIdx);
      GuiwinTextOutAuto("方案%x 浮动系数 %02x%" ,nProject ,0);
      //当前有功功率
      switch(nStepCnt)
      {
          case 1:
          {
              for(i = 1 ; i <= 3 ; i++)
      {
        nIdex = 0;
        memset(&nStr[0] ,0 ,sizeof(nStr));
        BcdToAscii(&pBuf[j + 17 * nCycCnt], 2, TRUE, &nStr[0]);
        GuiwinTextOutAuto("%x. 时段 %02x:%02x-%02x:%02x", i,0,0,0,0);
                GuiwinTextOutAuto("    定值 %c%c%c%c" ,nStr[nIdex] ,nStr[nIdex + 1] ,nStr[nIdex + 2] ,nStr[nIdex + 3]);
        j += 2;
              }
              break;
            }
          case 2:
            {
             for(i = 4 ; i <= 6 ; i++)
              {
                nIdex = 0;
                memset(&nStr[0] ,0 ,sizeof(nStr));
                BcdToAscii(&pBuf[j + 17 * nCycCnt], 2, TRUE, &nStr[0]);
                GuiwinTextOutAuto("%x. 时段 %02x:%02x-%02x:%02x", i,0,0,0,0);
                GuiwinTextOutAuto("    定值 %c%c%c%c" ,nStr[nIdex] ,nStr[nIdex + 1] ,nStr[nIdex + 2] ,nStr[nIdex + 3]);
                j += 2;
              }
             break;
          }
          case 3:
              {
             for(i = 7 ; i <= 8 ; i++)
                {
                nIdex = 0;
                memset(&nStr[0] ,0 ,sizeof(nStr));
                BcdToAscii(&pBuf[j + 17 * nCycCnt], 2, TRUE, &nStr[0]);
                GuiwinTextOutAuto("%x. 时段 %02x:%02x-%02x:%02x", i,0,0,0,0);
                GuiwinTextOutAuto("    定值 %c%c%c%c" ,nStr[nIdex] ,nStr[nIdex + 1] ,nStr[nIdex + 2] ,nStr[nIdex + 3]);
                j += 2;
                }
             break;
              }
            }
      GuiwinAutoDispProc(NULL, NULL , 0 , (INT8U*)1);
      nFlg = UpDownLeftRight1(&nGrpIdx ,&nProject,&nStepCnt);
      if (nFlg == 1)
      {
        break;
      }
          }
}
//总加组巡检
INT8S  GroupRout(INT16U* pGrpCnt , INT8U nSwitchFlg)
{
  INT8U   nKey                  = 0;
  INT8S   nTmpGrpCnt            = 0;
  INT8U   nEndFlg               = 0;
  INT8U   MacroVal              = 0;

  nTmpGrpCnt = *pGrpCnt;

  if(nSwitchFlg == 0)
  {
    MacroVal =  GRP_LEN;
  }
  else if(nSwitchFlg == 1)
  {
    MacroVal =  METER_NUM;
  }
  else if(nSwitchFlg == 2)
  {
    MacroVal = INFO_NUM;
  }
  while(1)
  {
    GET_KEY_VAL(nKey, 1);
    if(nKey != 0xFF)
    {
      if((nKey == KEY_DOWN)||(nKey == KEY_RIGHT))
      {
         nTmpGrpCnt ++;
         if(nTmpGrpCnt > MacroVal)
         {
           nTmpGrpCnt = 1;
         }
      }
      else if((nKey == KEY_UP)||(nKey == KEY_LEFT))
      {
          nTmpGrpCnt --;
          if(nTmpGrpCnt < 1)
          {
            nTmpGrpCnt = MacroVal;
          }
      }
      else if(nKey == KEY_CANCEL)
      {
        nEndFlg = 1;
      }
      break;
    }
  }
  *pGrpCnt = nTmpGrpCnt;
  return nEndFlg;
}

//厂休控参数
void FactoryPara(void)
{
  INT8U      nBuf[10]                    = {0};
  INT8U*     pBuf                        = &nBuf[0];
  INT8U      nStr[5]                     ={0};
  INT8U      nIdex                       = 0;
  INT16U      nGrpIdx                     = 1;
  INT8U      nFlg                        = 0;

  while(1)
  {
      memset(pBuf, 0, sizeof(nBuf));
      MemReadGrpPrm(F42, nGrpIdx, pBuf, sizeof(nBuf));

      nIdex = 0;
      memset(&nStr[0] ,0 ,sizeof(nStr));
      BcdToAscii(&pBuf[0], 2, TRUE, &nStr[0]);
      GuiwinTextOutAuto("厂休控定值  %c%c%c%cKW" , nStr[nIdex] ,nStr[nIdex + 1] ,nStr[nIdex + 2] ,nStr[nIdex + 3]);
      GuiwinTextOutAuto("起始时间    %02x:%02x", pBuf[2] ,pBuf[1]);
      GuiwinTextOutAuto("延续时间    %.1x小时", pBuf[3]);
      GuiwinTextOutAuto("长休日      %x", pBuf[4]);
      GuiwinAutoDispProc("厂休控参数" , NULL , 0 , (INT8U*)1);
      nFlg = GroupRout(&nGrpIdx ,0);
      if(nFlg == 1)
      {
        break;
      }
  }
}

//营业报停控参数
void BusinessPara(void)
{
  INT8U      nBuf[10]                    = {0};
  INT8U*     pBuf                        = &nBuf[0];
  INT8U      nStr[5]                     ={0};
  INT8U      nIdex                       = 0;
  INT16U      nGrpIdx                     = 1;
  INT8U      nFlg                        = 0;

  while(1)
  {
      memset(pBuf, 0, sizeof(nBuf));
      MemReadGrpPrm(F44, nGrpIdx, pBuf, sizeof(nBuf));

      nIdex = 0;
      GuiwinTextOutAuto("报停起始时间");
      GuiwinTextOutAuto("      20%02x-%02x-%02x", pBuf[2] ,pBuf[1] ,pBuf[0]);
      GuiwinTextOutAuto("报停停止时间");
      GuiwinTextOutAuto("      20%02x-%02x-%02x", pBuf[5] ,pBuf[4] ,pBuf[3]);
      memset(&nStr[0] ,0 ,sizeof(nStr));
      BcdToAscii(&pBuf[6], 2, TRUE, &nStr[0]);
      GuiwinTextOutAuto("厂休控定值");
      GuiwinTextOutAuto("      %c%c%c%cKW" , nStr[nIdex] ,nStr[nIdex + 1] ,nStr[nIdex + 2] ,nStr[nIdex + 3]);
      GuiwinAutoDispProc("营业报停参数" , NULL , 0 , (INT8U*)1);
      nFlg = GroupRout(&nGrpIdx ,0);
      if(nFlg == 1)
      {
        break;
      }
  }
}

//下浮控参数
void FloatPara(void)
{

}

//电控参数
void MeterContlr(void)
{

}

//终端状态量输入参数
void  KvKiKp(void)
{

}



//电能表参数
void MeterPara(void)
{
  INT16U               nSeq                       = 1;
  INT8U                nBuf[40]                   = {0};
  INT8U*               pBuf                       = &nBuf[0];
  INT8U                RecvBuf[A_MENU_LEN]        ={0};
  INT32U               nBaud                      = 0;
  INT8U                Index                      = 0;
  INT8U                nFlg                       = 0;
  METER_INFO	       sMeterInfo;
  pMETER_INFO	       pMeterInfo	              = &sMeterInfo;
  INT16U               nMp                        = 0;

  while(1)
  {
      MemReadMeterPrm(nSeq, F10, &nBuf[0], sizeof(nBuf));

        if (GetMeterInfo(nSeq, pMeterInfo) < 0)
        {
           nMp = 0;
        }
        else
        {
           nMp = Buf2INT16U(&pBuf[3]);
        }
      GuiwinTextOutAuto("电能表序号%02d", nSeq);
      GuiwinTextOutAuto("所属测量点%02d", nMp);
      //测量点端口
      memset(&RecvBuf[0], 0, sizeof(RecvBuf));
      switch(pBuf[Index + 5]&0x1F)
      {
        case 1:
            strcpy((char *)RecvBuf, "交采口");
            break;
        case 2:
            strcpy((char *)RecvBuf, "RS4851");
            break;
        case 3:
            strcpy((char *)RecvBuf, "RS4852");
            break;
        case 4:
            strcpy((char *)RecvBuf, "RS4853");
            break;
        case 30:
            strcpy((char *)RecvBuf, "级联RS485");
            break;
        case 31:
            strcpy((char *)RecvBuf, "载波口");
            break;
        default:
            strcpy((char *)RecvBuf, "无效端口");
            break;
      }
      GuiwinTextOutAuto("通     道:%s",RecvBuf);

      nBaud = Buf2Long(&pBuf[6], 4);
      GuiwinTextOutAuto("通信波特率:%d",nBaud);

    //测量点通信规约
    memset(&RecvBuf[0], 0, sizeof(RecvBuf));
    switch(pBuf[10])
    {
        case 0:
            strcpy((char *)RecvBuf, "无效");
            break;
        case 1:
            strcpy((char *)RecvBuf, "DL/T 645-1997");
            break;
        case 2:
            strcpy((char *)RecvBuf, "交流采样协议");
            break;
        case 30:
            strcpy((char *)RecvBuf, "DL/T 645-2007");
            break;
        case 31:
            strcpy((char *)RecvBuf, "串行载波协议");
            break;
        default:
            strcpy((char *)RecvBuf, "未接电能表");
            break;
    }
    GuiwinTextOutAuto("电表类型%s",RecvBuf);

    Index = 0;
    GuiwinTextOutAuto("表地址%02x%02x%02x%02x%02x%02x",pBuf[Index+16], pBuf[Index+15],
                        pBuf[Index+14], pBuf[Index+13], pBuf[Index+12], pBuf[Index+11]);
    GuiwinAutoDispProc("电能表信息" , NULL , 0 , (INT8U*)1);
    nFlg = GroupRout(&nSeq ,1);
    if(nFlg == 1)
    {
       break;
    }
  }
}

//配置参数
void ConfigPara(void)
{
  ConcerNumQuery();
}

//控制状态
void ContrlState(void)
{

}

//双向按键处理
INT32S  UpDownLeftRight(INT16U* pHorizontal ,INT8U* pCaseItem ,INT8U nSwitchFlg)
{
  INT8U           nKey            = 0;
  INT8S           nHorizontal     = 0;
  INT8S           nCaseItem       = 0;
  INT8U           nFlg            = 0;
  INT8U           nMacro          = 0;
  INT8U           nKeyOff         = 0;

  nHorizontal = *pHorizontal;
  nCaseItem = *pCaseItem;

  if(nSwitchFlg == 0)
  {
    nMacro = METER_NUM;

  }
  else if(nSwitchFlg == 1)
  {
    nMacro = GRP_LEN;

  }

  while(1)
  {
    GET_KEY_VAL(nKey, 1);
    if (nKey != 0xFF)
    {
      //左右按键随其变动 ①
      if(KEY_LEFT == nKey)
      {
        nHorizontal--;
         if(nHorizontal < 1)
         {
           nHorizontal = nMacro;
         }
      }
      else if(KEY_RIGHT == nKey)
      {
        nHorizontal++;
        if(nHorizontal > nMacro)
        {
           nHorizontal = 1;
        }

      }
      //上下键改变屏显，每③次调整更换一次，随之①跟着变化
      else if(KEY_UP == nKey)
      {
        nCaseItem--;
            if(nCaseItem < 1)
            {
            nHorizontal--;
            if(nHorizontal < 1)
            {
              nHorizontal = nMacro;
            }
              nCaseItem = 3;
            }
      }
      else if(KEY_DOWN == nKey)
      {
        nCaseItem ++;
        if(nCaseItem == 4)
          {
         nHorizontal++;

         if(nHorizontal > nMacro)
         {
           nHorizontal = 1;
         }
            nCaseItem = 1;
          }
      }
      else if(KEY_CANCEL == nKey)
      {
        nFlg = 1 ;
      }
      *pHorizontal = nHorizontal;
      *pCaseItem = nCaseItem;
      break;
    }

  }
  return nFlg;
}

//电能表示数
void MeterData(void)
{
    INT8U                nBuf[40]                    = {0};
    INT8U*               pBuf                        = &nBuf[0];
	INT8U                nStepCnt                    = 1;
    INT32S               nFlg                        = 0;
    INT8U                nIdex                       = 0;
    INT16U                nSeq                        = 1;
    INT8U                nLimitFlg                   = 1;
    INT8U                i                           = 0;
    INT8U                nEnumVal                    = 0;
    const  INT8S*   c_OneScreen[]       ={"正向有功总","正向有功总","正向有功总","正向有功总","正向有功总","抄表",0};
    const  INT8S*   c_TwoScreen[]       ={"正向无功总","一象限","二象限","三象限","四象限","抄表",0};
    const  INT8S*   c_ThreeScreen[]     ={"正向有功最大需量","时间","正向无功最大需量","时间","抄表",0};

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
	//测量点
	while(1)
	{
       //标题
       GuiwinTextOutAuto("%4s电能表%d示数" , "" , nSeq);
       memset(pBuf, 0, sizeof(nBuf));
       MemReadMeterPrm(nSeq, F10, &pBuf[0], sizeof(nBuf));

       if(IsAllEqualVal(pBuf, sizeof(nBuf), 0))
       {
          sprintf((char*)&pBuf[0], " -------");
          GuiwinTextOutAuto("表地址%s",pBuf);
       }
       else
       {
       GuiwinTextOutAuto("表地址%02x%02x%02x%02x%02x%02x",pBuf[nIdex+16], pBuf[nIdex+15],
                    pBuf[nIdex+14], pBuf[nIdex+13], pBuf[nIdex+12], pBuf[nIdex+11]);
       }
       switch (nStepCnt)
       {
           case 1: //正向有功总
		   {
            // 第1屏
                nLimitFlg = 1;
                nEnumVal = 4;
                for(i = 0 ; i <= 4 ; i++)
                {
                nIdex = 0;
                memset(pBuf, 0, sizeof(nBuf));
                    MemReadRealData(nSeq, nEnumVal++, pBuf, sizeof(nBuf));
                    if((IsAllEqualVal(pBuf, sizeof(nBuf), 0))&&(nSeq != 1))
                    {
                      sprintf((char*)&pBuf[0], "------.--");
                      nLimitFlg = 0;
                      GuiwinTextOutAuto("%s:%s",c_OneScreen[i] , pBuf);
                    }
                    else
                    {
                      GuiwinTextOutAuto("%s:%02x%02x%02x.%02x",c_OneScreen[i], pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                    }
                }
                if(nLimitFlg == 0)
                {
                nIdex = 0;
                memset(pBuf, 0, sizeof(nBuf));
                  MemReadRealData(nSeq, F2, pBuf, sizeof(nBuf)); //读表时间YY-MM-DD-HH-MM-SS
                  sprintf((char*)&pBuf[0], "--/--/-- --:--:--");
                  GuiwinTextOutAuto("%s%s",c_TwoScreen[5],pBuf);
                }
                else
                {
                GuiwinTextOutAuto("抄表%02x/%02x/%02x  %02x:%02x",pBuf[nIdex + 5],pBuf[nIdex + 4],pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1]);
                }
                break;
            }
            case 2:
            {
                // 第2屏
                nIdex = 0 ;
                nLimitFlg = 1;
                memset(pBuf, 0, sizeof(nBuf));
                MemReadRealData(nSeq, F7, pBuf, sizeof(nBuf)); // 正向无功电能示值
                if((IsAllEqualVal(pBuf, sizeof(nBuf), 0))&&(nSeq != 1))
                {
                  sprintf((char*)&pBuf[0], "------.--");
                  nLimitFlg = 0;
                  GuiwinTextOutAuto("%s:%s",c_TwoScreen[0] , pBuf);
                }
                else
                {
                  GuiwinTextOutAuto("%s:%02x%02x%02x.%02x",c_TwoScreen[1] , pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                }

                nEnumVal = 8;
                for(i = 1 ; i <= 4 ; i++)
                {
                nIdex = 0 ;
                memset(pBuf, 0, sizeof(nBuf));
                    MemReadRealData(nSeq, nEnumVal++, pBuf, sizeof(nBuf)); // 一象限
                    if((IsAllEqualVal(pBuf, sizeof(nBuf), 0))&&(nSeq != 1))
                    {
                      sprintf((char*)&pBuf[0], "------.--");
                      nLimitFlg = 0;
                      GuiwinTextOutAuto("%s:%s",c_TwoScreen[i] , pBuf);
                    }
                    else
                    {
                      GuiwinTextOutAuto("%s:%02x%02x%02x.%02x",c_TwoScreen[i], pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                    }
                }
                if(nLimitFlg == 0)
                {
                nIdex = 0;
                memset(pBuf, 0, sizeof(nBuf));
                  MemReadRealData(nSeq, F2, pBuf, sizeof(nBuf)); //读表时间YY-MM-DD-HH-MM-SS
                  sprintf((char*)&pBuf[0], "--/--/-- --:--:--");
                  GuiwinTextOutAuto("%s%s",c_TwoScreen[5],pBuf);
                }
                else
                {
                GuiwinTextOutAuto("抄表%02x/%02x/%02x  %02x:%02x",pBuf[nIdex + 5],pBuf[nIdex + 4],pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1]);
                }
                break;
            }
            case 3:
            {
                nEnumVal = 12;
                for(i = 0 ; i <= 2 ; i++)
                {
                nIdex = 0;
                    nLimitFlg =1;
                memset(pBuf, 0, sizeof(nBuf));
                    MemReadRealData(nSeq, nEnumVal, pBuf, sizeof(nBuf)); //正向有功需量

                    if((IsAllEqualVal(pBuf, sizeof(nBuf), 0))&&(nSeq != 1))
                    {
                      sprintf((char*)&pBuf[0], " --.-----");
                      nLimitFlg = 0;
                      GuiwinTextOutAuto("%s",c_ThreeScreen[i]);
                      GuiwinTextOutAuto("%11s%skw","", pBuf);
                      sprintf((char*)&pBuf[0], "--/--/-- --:--:--");
                      GuiwinTextOutAuto("%s%s",c_ThreeScreen[i + 1],pBuf);
                    }
                    else
                    {
                      GuiwinTextOutAuto("%s",c_ThreeScreen[i]);
                GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                      GuiwinTextOutAuto("%s%02x/%02x %02x:%02x",c_ThreeScreen[i + 1],pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                    }
                    i += 1;
                    nEnumVal += 2;
                }
                break;
            }
       }
        GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);   //屏显结束语
       nFlg = UpDownLeftRight(&nSeq , &nStepCnt ,0);  //按键处理
        if(nFlg)
        {
          break;
        }
    }
}

//按键处理
INT32S  ChineseKey(INT8U* pInfoCnt ,INT8U nCycCnt ,INT8U* nFlg)
{
  INT8U          nKey           = 0;
  INT8U          nInfoCnt       = 1;
  INT8U          nEndFlg        = 0;
  INT8U          nChoiceFlg     = 0;

  nInfoCnt = *pInfoCnt;
  nChoiceFlg = *nFlg;

  while(1)
  {
    GET_KEY_VAL(nKey, 1);
    if (nKey != 0xFF)
    {
      //左右按键随其变动 ①
      if(nKey == KEY_DOWN)
      {
         nInfoCnt ++;
         if(nInfoCnt > INFO_NUM)
         {
           nInfoCnt = 1;
         }
      }
      else if(nKey == KEY_UP)
      {
          nInfoCnt --;
          if(nInfoCnt < 1)
          {
            nInfoCnt = INFO_NUM;
          }
      }
      else if((nKey == KEY_LEFT)||(nKey == KEY_RIGHT))
      {
        if((nCycCnt % 2) != 0)
        {
          nChoiceFlg = 0;
        }
        else
        {
          nChoiceFlg = 1;
        }
      }
      else if(nKey == KEY_CANCEL)
      {
        nEndFlg = 1;
      }
     *pInfoCnt = nInfoCnt;
     *nFlg = nChoiceFlg;
     break;
    }
  }
  return nEndFlg;
}


//中文信息
void ChineseInfo(void)
{
  //INT8S*                        pString                = NULL ;
  const       INT8S           *c_InfoChoice[]        = {"重要信息" ,"常规信息" , 0};
  INT8U       nChoice          = 1;
  INT8U       nFlg             = 0;
  INT8U                         nDayCntFlg             = 1;
  INT8U                         nSwitchFlg             = 0;

  //pString = (INT8S*)IMP_NAME;

  while(1)
  {
    GuiwinTextOutAuto("     %s%d",c_InfoChoice[nSwitchFlg] ,nChoice);
    GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);
    nDayCntFlg ++;
    nFlg = ChineseKey(&nChoice ,nDayCntFlg ,&nSwitchFlg);
    if(nFlg)
    {
      break;
    }
  }

}

//购电信息
void BuyInfo(void)
{
  INT8U          nBuf[20]    ={0};
  INT8U*         pBuf        = &nBuf[0];
  INT8U          nUnit[3]    ={0};
  INT8U          nIdx        = 0;
  INT16U          nGrpIdx     = 1;
  INT8U          nFlg        = 0;

  while(1)
  {
      MemReadGrpPrm(F47, nGrpIdx, pBuf, sizeof(nBuf));

      GuiwinTextOutAuto("      总加组%x",nGrpIdx);
      GuiwinTextOutAuto("购电单号%02x%02x%02x%02x",pBuf[3] ,pBuf[2],pBuf[1],pBuf[0]);
      //购前电量   //购后电量    //剩余电量   //购电时间  //报警门限  //跳闸门限
      memset(&nUnit[0] , 0 , sizeof(nUnit));
      switch(TestBufBit(&pBuf[8] ,1 , 7))
      {
        case 0:
        {
           strcpy((char *)nUnit, "kWh");
        }
        case 1:
        {
           strcpy((char *)nUnit, "kWh");
        }
      }
      GuiwinTextOutAuto("购电量值%d%s",BCD2DEC(pBuf[8])*1000000 + BCD2DEC(pBuf[7])*10000 + BCD2DEC(pBuf[6])*100 + BCD2DEC(pBuf[5]),nUnit);
      memset(&nUnit[0] , 0 , sizeof(nUnit));
      nIdx += 4;
      switch(TestBufBit(&pBuf[8 + nIdx] ,1 , 7))
      {
        case 0:
        {
           strcpy((char *)nUnit, "kWh");
        }
        case 1:
        {
           strcpy((char *)nUnit, "kWh");
        }
      }
      GuiwinTextOutAuto("报警门限%d%s",pBuf[13],nUnit);
      /*memset(&nUnit[0] , 0 , sizeof(nUnit));
      nIdx += 4;
      switch(TestBufBit(pBuf[8 + nIdx] ,1 , 7))
      {
        case 0:
        {
           strcpy((char *)nUnit, "kWh");
        }
        case 1:
        {
           strcpy((char *)nUnit, "kWh");
        }
      }
      GuiwinTextOutAuto("跳闸门限%d%s",BCD2DEC(pBuf[8 + nIdx])*1000000 + BCD2DEC(pBuf[7 + nIdx])*10000 + BCD2DEC(pBuf[6 + nIdx])*100 + BCD2DEC(pBuf[5 + nIdx]),nUnit);*/
      GuiwinTextOutAuto("跳闸门限%xkWh",pBuf[13]);
      GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);
      nFlg = GroupRout(&nGrpIdx ,0);
      if(nFlg == 1)
      {
         break;
      }
  }
}

//终端信息
void  TerminalInfo(void)
{
    INT8U			     nBuf[30]		            = {0};
	INT8U*			     pBuf			            = &nBuf[0];
    INT8U                nData[20]                  = {0};
    INT8U*               pData                      = &nData[0];
    INT8U                nStr[20]                   ={0};
    INT8U                nAPN[30]                    ={0};
    INT8U                nIdx                       = 0;
    INT8U                i                          = 0;
    INT8U                nLen                       = 0;
    INT8U                nStrLen                    = 0;

    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F94, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("行政区码%02x%02x",pBuf[1], pBuf[0]);
    GuiwinTextOutAuto("终端地址%02x%02x(H)",pBuf[3], pBuf[2]);
    GuiwinTextOutAuto("终端地址%05d(D)", Buf2INT16U(&nBuf[2]));
    GuiwinTextOutAuto("终端编号");

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
	strcat((char*)pBuf, (char*)"软件版本:V");
	strcat((char*)pBuf, (char*)PRODUCT);
#ifdef MIN_PRODUCT
	strcat((char*)pBuf, (char*)MIN_PRODUCT);
#endif
	strcat((char*)pBuf, (char*)"-");
	strcat((char*)pBuf, (char*)MAIN_VERSION_INFO);
	strcat((char*)pBuf, (char*)VERSION_INFO);
#ifdef MIN_VERSION_INFO
	strcat((char*)pBuf, (char*)"-");
	strcat((char*)pBuf, (char*)MIN_VERSION_INFO);
#endif
	GuiwinTextOutAuto("%s", pBuf);

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
	strcat((char*)pBuf, (char*)"软件日期:");
	strcat((char*)pBuf, (char*)DATE_INFO);
	GuiwinTextOutAuto("%s", pBuf);
    GuiwinTextOutAuto("硬件版本");
    GuiwinTextOutAuto("发布日期");

    //第二屏
    memset(pBuf, 0, sizeof(nBuf));
    GuiwinTextOutAuto("主用IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
    GuiwinTextOutAuto("主用端口: %05d", pBuf[5] * 256 + pBuf[4]);
    GuiwinTextOutAuto("备用IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[6],pBuf[7],pBuf[8],pBuf[9]);
    GuiwinTextOutAuto("备用端口: %05d", pBuf[11] * 256 + pBuf[10]);
    memcpy(nAPN ,&pBuf[12],30);
    GuiwinTextOutAuto("APN");
    GuiwinTextOutAuto("%s", nAPN);

    //第三屏
    memset(pBuf, 0, sizeof(nBuf));
    memset(pData , 0 ,sizeof(nData));
    MemReadCommPrm(F4, &nBuf[0], sizeof(nBuf));

    //非BCD码不打印-------主站号码
    nIdx = 0;
    for (i = 0; i < 8; i++)
    {
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], pBuf[i], 2, 1, 2);
          //pData[i]等于'F'的部分不显示出来，未显示位可设置
         if(pData[nIdx] == 'f')
         {
             pData[nIdx] = '\0';
             nStrLen = nIdx;
             break;
         }
         else if(pData[nIdx + 1] == 'f')
         {
             pData[nIdx + 1] = '\0';
             nStrLen = nIdx + 1;
             break;
         }
         nIdx += nLen;
    }
    //打印主站号码
    memcpy(nStr ,pData ,nStrLen);
    GuiwinTextOutAuto("主站号码");
    GuiwinTextOutAuto("%3s%s","",nStr);

    //非BCD码不打印-------短信中心
    nIdx = 0;
    nStrLen = 0;
    pData = &nData[0];
    memset(pData , 0 ,sizeof(nData));
    memset(&nStr[0], 0 ,sizeof(nStr));
    for (i = 0; i < 8; i++)
    {
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], pBuf[i + 8], 2, 1, 2);
         //pData[i]等于'F'的部分不显示出来，未显示位可设置
         if(pData[nIdx] == 'f')
         {
             pData[nIdx] = '\0';
             nStrLen = nIdx;
             break;
         }
         else if(pData[nIdx + 1] == 'f')
         {
             pData[nIdx + 1] = '\0';
             nStrLen = nIdx + 1;
             break;
         }
         nIdx += nLen;
    }
    //打印短信中心
    memcpy(nStr ,pData ,nStrLen);
    GuiwinTextOutAuto("短信中心");
    GuiwinTextOutAuto("%3s%s","",nStr);

    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F7, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("终端IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);

    //第四屏
        memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F7, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("终端IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
    GuiwinTextOutAuto("子网掩码");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[4],pBuf[5],pBuf[6],pBuf[7]);
    GuiwinTextOutAuto("网关地址");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[8],pBuf[9],pBuf[10],pBuf[11]);
    GuiwinTextOutAuto("侦听端口: %05d", pBuf[61] * 256 + pBuf[60]);

    pBuf = &nBuf[0];
    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F8, &nBuf[0], sizeof(nBuf));
    //链接模式
    switch(((nBuf[0] >> 7) & 0x01))
    {
        case 0:
            strcpy((char *)nBuf, "TCP");
            break;
        case 1:
            strcpy((char *)nBuf, "UDP");
            break;
    }
    GuiwinTextOutAuto("连接模式:%1s%s","",nBuf);
    //工作模式
    pBuf = &nBuf[0];
    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F8, &nBuf[0], sizeof(nBuf));
    switch((pBuf[0] >> 4) & 0x03)
    {
        case 0:
            strcpy((char *)nBuf, "混合模式");
            break;
        case 1:
            strcpy((char *)nBuf, "客户机模式");
            break;
        case 2:
            strcpy((char *)nBuf, "服务器模式");
            break;
        default:
            strcpy((char *)nBuf, "无效");
            break;
    }
    GuiwinTextOutAuto("工作模式:");
    GuiwinTextOutAuto("%10s%s","", nBuf);
    GuiwinAutoDispProc(NULL , NULL , 0 , NULL);
}
