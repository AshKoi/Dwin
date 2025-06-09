/******************************************************************************************************
* Copyright (c) 2013
* �ļ���:
* ������: ��һ��
* �ա���: 2013/09/12
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
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
                                           �궨��
***************************************************************************************************/
#define GET_KEY_VAL(nKey, nType)	nKey = GetKeyVal(nType)

#define A_MENU_LEN  30

#define DEC2BCD(x)	              ((((x)/10)<<4)+(x)%10)
#define BCD2DEC(x)	              (((x)&0x0F)+((x)>>4)*10)

/**************************************************************************************************
                                           ��������
***************************************************************************************************/

#define      GRP_LEN               8            //�ܼ������������
#define      METER_NUM             32           //���ܱ���������ֵ
#define      INFO_NUM              16           //������Ϣ

#define      IMP_NAME              "��Ҫ��Ϣ"
#define      REG_NAME              "������Ϣ"

/**************************************************************************************************
                                        �����洢ƫ����Ϣ
***************************************************************************************************/
//ʵʱ����
const MENU_ITEM  g_sRealData[] =
{
    {MENU_ITEM_DISCRIB, "����������", NULL},
	{MENU_ITEM_EXE, "1.��ǰ����", (void*)CurrWatt},
    {MENU_ITEM_EXE, "2.��ǰ����",(void*)CurrPower},
    {MENU_ITEM_EXE, "3.��������", (void*)LoadCurve},
    {MENU_ITEM_EXE, "4.����״̬", (void*)SwitchState},
    {MENU_ITEM_EXE, "5.���ؼ�¼", (void*)PowerContrlRd},
    {MENU_ITEM_EXE, "6.��ؼ�¼", (void*)ElectronicRd},
    {MENU_ITEM_EXE, "7.ң�ؼ�¼", (void*)RemotRd},
    {MENU_ITEM_EXE, "8.ʧ���¼", (void*)LosePowerRd},

    {0}
};

//������ֵ
const MENU_ITEM  g_sParaFixValue[] =
{
    {MENU_ITEM_DISCRIB, "������ֵ", NULL},
	{MENU_ITEM_EXE, "1.ʱ�οز���", (void*)ContrlPara},
    {MENU_ITEM_EXE, "2.���ݿز���", (void*)FactoryPara},
    {MENU_ITEM_EXE, "3.Ӫҵ��ͣ��", (void*)BusinessPara},
    {MENU_ITEM_EXE, "4.�¸��ز���", (void*)FloatPara},
    {MENU_ITEM_EXE, "5.��ز���", (void*)MeterContlr},
    {MENU_ITEM_EXE, "6.KvKiKp", (void*)KvKiKp},
    {MENU_ITEM_EXE, "7.���ܱ����", (void*)MeterPara},
    {MENU_ITEM_EXE, "8.���ò���", (void*)ConfigPara},

    {0}
};

//1���˵�--������I ��
const MENU_ITEM   g_sZbMenu[] =
{
    {MENU_ITEM_DISCRIB, "���˵�", NULL},
	{MENU_ITEM_SHOW, "1.ʵʱ����", (void*)g_sRealData},
    {MENU_ITEM_SHOW, "2.������ֵ", (void*)g_sParaFixValue},
    {MENU_ITEM_EXE, "3.����״̬", (void*)ContrlState},
    {MENU_ITEM_EXE, "4.���ܱ�ʾ��", (void*)MeterData},
    {MENU_ITEM_EXE, "5.������Ϣ", (void*)ChineseInfo},
    {MENU_ITEM_EXE, "6.������Ϣ", (void*)BuyInfo},
    {MENU_ITEM_EXE, "7.�ն���Ϣ", (void*)TerminalInfo},

    {0}
};

/**************************************************************************************************
                                           ��������
***************************************************************************************************/
//ʵʱ����----��ǰ����
void CurrWatt(void)
{
  INT8U      nBuf[10]                    = {0};
  INT8U*     pBuf                        = &nBuf[0];
  INT8U      nItemId                     = 1;
  INT8U      nStr[5]                    ={0};
  INT8U      nIdex                       = 0;

  //�ն��ܼ������ò���
  //MemReadGrpPrm(F14, 0, pBuf, sizeof(nBuf));

  memset(pBuf, 0, sizeof(nBuf));
  //��ǰ�й�����
  GuiwinTextOutAuto("��ǰ�й�����");
  for(nItemId = 1 ; nItemId <= GRP_LEN ; nItemId++)
  {
    nIdex = 0;
    memset(&nStr[0] ,0 ,sizeof(nStr));
    BcdToAscii(&pBuf[nIdex], 2, TRUE, &nStr[0]);
    GuiwinTextOutAuto("�ܼ���%x:%5s%c.%c%c%cKw",nItemId, "",nStr[nIdex],nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
  }
  //��ǰ�޹�����
  GuiwinTextOutAuto("��ǰ�޹�����");
  for(nItemId = 1 ; nItemId <= GRP_LEN ; nItemId++)
  {
    nIdex = 0;
    memset(&nStr[0] ,0 ,sizeof(nStr));
    BcdToAscii(&pBuf[nIdex], 2, TRUE, &nStr[0]);
    GuiwinTextOutAuto("�ܼ���%x:%5s%c.%c%c%cKvar",nItemId, "",nStr[nIdex],nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
  }
  //��ǰ���幦��
  GuiwinTextOutAuto("��ǰ���幦��");
#define   PluseNum    2
  for(nItemId = 1 ; nItemId <= PluseNum ; nItemId++)
  {
    nIdex = 0;
    memset(&nStr[0] ,0 ,sizeof(nStr));
    BcdToAscii(&pBuf[nIdex], 2, TRUE, &nStr[0]);
    GuiwinTextOutAuto("��%x·:%7s%c.%c%c%cKw",nItemId, "",nStr[nIdex],nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
  }
  GuiwinAutoDispProc(NULL , NULL , 0 , NULL);
}

//��ǰ������(��)
void CurrPower(void)
{
  //���·�ҳ�ܽ���ĵ�ǰ�����������л��ջ��µ����ݲ鿴
  INT8U      nBuf[20]                    = {0};
  INT8U*     pBuf                        = &nBuf[0];
  INT8U      nSeqCnt                     = 1;
  INT8U      nKey                        = 0;
  char*     pString                      = NULL;
  INT8U      nDayCntFlg                  = 0;
  INT8U      nEndFlg                     = 0;

  memset(pBuf, 0, sizeof(nBuf));
  //�ն��ܼ������ò���
  //MemReadGrpPrm(F14, 0, pBuf, sizeof(nBuf));
  //��ǰ�й�����
#define    DAY		"��"
#define    MON		"��"
  pString = (char*)DAY;
  while(1)
  {
       GuiwinTextOutAuto("   �ܼ���%x %s����", nSeqCnt ,pString);

       memset(pBuf, 0, sizeof(nBuf));
       sprintf((char*)&pBuf[0], " -------");
      //����չʾ����
       GuiwinTextOutAuto("�й���:%sMWh" ,pBuf);
       GuiwinTextOutAuto("�й���:%sMWh" ,pBuf);
       GuiwinTextOutAuto("�й���:%sMWh" ,pBuf);
       GuiwinTextOutAuto("�й�ƽ:%sMWh" ,pBuf);
       GuiwinTextOutAuto("�й���:%sMWh" ,pBuf);
       GuiwinTextOutAuto("");
       GuiwinTextOutAuto("�޹���:%sMvarh" ,pBuf);
       GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);
      //����������
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

//��������
void LoadCurve(void)
{
   //����һ���޵�����ͼ
   LCD_DrawLine2(0, 0, 0, 0, 1);
}


//��ǰ������״̬
void  SwitchState(void)
{
   GuiwinTextOutAuto("���غ� %d   %d    %d", 1 ,2 ,3);
   GuiwinTextOutAuto("");
   GuiwinTextOutAuto("���� δ�� δ�� δ��");
   GuiwinTextOutAuto("");
   GuiwinTextOutAuto("��λ δ�� δ�� δ��");
   GuiwinTextOutAuto("");
   GuiwinTextOutAuto("���� ���� ���� ����");

   GuiwinAutoDispProc(NULL , NULL , 0 , NULL);
}

//��¼���͵İ�������
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

//���ؼ�¼
void PowerContrlRd(void)
{
  INT8U   nFlgCnt    = 1;
  INT8U   nEndFlg    = 0;
  while(1)
  {
      GuiwinTextOutAuto("");
      GuiwinTextOutAuto("");
      GuiwinTextOutAuto("%4s�޹��ؼ�¼%x","", nFlgCnt);
      GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);
      nEndFlg = Record(&nFlgCnt);
      if(nEndFlg == 1)
      {
        break;
      }
  }

}

//��ؼ�¼
void  ElectronicRd(void)
{
  INT8U   nFlgCnt    = 1;
  INT8U   nEndFlg    = 0;
  while(1)
  {
      GuiwinTextOutAuto("");
      GuiwinTextOutAuto("");
      GuiwinTextOutAuto("%4s�޵�ؼ�¼%x","", nFlgCnt);
      GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);
      nEndFlg = Record(&nFlgCnt);
      if(nEndFlg == 1)
      {
        break;
      }
  }
}

//ң�ؼ�¼
void RemotRd(void)
{
  INT8U   nFlgCnt    = 1;
  INT8U   nEndFlg    = 0;
  while(1)
  {
      GuiwinTextOutAuto("");
      GuiwinTextOutAuto("");
      GuiwinTextOutAuto("%4s��ң�ؼ�¼%x","", nFlgCnt);
      GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);
      nEndFlg = Record(&nFlgCnt);
      if(nEndFlg == 1)
      {
        break;
      }
  }
}

//ʧ���¼
void LosePowerRd(void)
{

}

//˫�򰴼�����
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
      //���Ұ�������䶯 ��
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
      //���¼��ı����ԣ�ÿ�۴ε�������һ�Σ���֮�ٸ��ű仯
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
//ʱ�οز���
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
       //һ������������ѭ����
      GuiwinTextOutAuto("�ܼ���%x" ,nGrpIdx);
      GuiwinTextOutAuto("����%x ����ϵ�� %02x%" ,nProject ,0);
      //��ǰ�й�����
      switch(nStepCnt)
      {
          case 1:
          {
              for(i = 1 ; i <= 3 ; i++)
      {
        nIdex = 0;
        memset(&nStr[0] ,0 ,sizeof(nStr));
        BcdToAscii(&pBuf[j + 17 * nCycCnt], 2, TRUE, &nStr[0]);
        GuiwinTextOutAuto("%x. ʱ�� %02x:%02x-%02x:%02x", i,0,0,0,0);
                GuiwinTextOutAuto("    ��ֵ %c%c%c%c" ,nStr[nIdex] ,nStr[nIdex + 1] ,nStr[nIdex + 2] ,nStr[nIdex + 3]);
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
                GuiwinTextOutAuto("%x. ʱ�� %02x:%02x-%02x:%02x", i,0,0,0,0);
                GuiwinTextOutAuto("    ��ֵ %c%c%c%c" ,nStr[nIdex] ,nStr[nIdex + 1] ,nStr[nIdex + 2] ,nStr[nIdex + 3]);
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
                GuiwinTextOutAuto("%x. ʱ�� %02x:%02x-%02x:%02x", i,0,0,0,0);
                GuiwinTextOutAuto("    ��ֵ %c%c%c%c" ,nStr[nIdex] ,nStr[nIdex + 1] ,nStr[nIdex + 2] ,nStr[nIdex + 3]);
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
//�ܼ���Ѳ��
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

//���ݿز���
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
      GuiwinTextOutAuto("���ݿض�ֵ  %c%c%c%cKW" , nStr[nIdex] ,nStr[nIdex + 1] ,nStr[nIdex + 2] ,nStr[nIdex + 3]);
      GuiwinTextOutAuto("��ʼʱ��    %02x:%02x", pBuf[2] ,pBuf[1]);
      GuiwinTextOutAuto("����ʱ��    %.1xСʱ", pBuf[3]);
      GuiwinTextOutAuto("������      %x", pBuf[4]);
      GuiwinAutoDispProc("���ݿز���" , NULL , 0 , (INT8U*)1);
      nFlg = GroupRout(&nGrpIdx ,0);
      if(nFlg == 1)
      {
        break;
      }
  }
}

//Ӫҵ��ͣ�ز���
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
      GuiwinTextOutAuto("��ͣ��ʼʱ��");
      GuiwinTextOutAuto("      20%02x-%02x-%02x", pBuf[2] ,pBuf[1] ,pBuf[0]);
      GuiwinTextOutAuto("��ֹͣͣʱ��");
      GuiwinTextOutAuto("      20%02x-%02x-%02x", pBuf[5] ,pBuf[4] ,pBuf[3]);
      memset(&nStr[0] ,0 ,sizeof(nStr));
      BcdToAscii(&pBuf[6], 2, TRUE, &nStr[0]);
      GuiwinTextOutAuto("���ݿض�ֵ");
      GuiwinTextOutAuto("      %c%c%c%cKW" , nStr[nIdex] ,nStr[nIdex + 1] ,nStr[nIdex + 2] ,nStr[nIdex + 3]);
      GuiwinAutoDispProc("Ӫҵ��ͣ����" , NULL , 0 , (INT8U*)1);
      nFlg = GroupRout(&nGrpIdx ,0);
      if(nFlg == 1)
      {
        break;
      }
  }
}

//�¸��ز���
void FloatPara(void)
{

}

//��ز���
void MeterContlr(void)
{

}

//�ն�״̬���������
void  KvKiKp(void)
{

}



//���ܱ����
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
      GuiwinTextOutAuto("���ܱ����%02d", nSeq);
      GuiwinTextOutAuto("����������%02d", nMp);
      //������˿�
      memset(&RecvBuf[0], 0, sizeof(RecvBuf));
      switch(pBuf[Index + 5]&0x1F)
      {
        case 1:
            strcpy((char *)RecvBuf, "���ɿ�");
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
            strcpy((char *)RecvBuf, "����RS485");
            break;
        case 31:
            strcpy((char *)RecvBuf, "�ز���");
            break;
        default:
            strcpy((char *)RecvBuf, "��Ч�˿�");
            break;
      }
      GuiwinTextOutAuto("ͨ     ��:%s",RecvBuf);

      nBaud = Buf2Long(&pBuf[6], 4);
      GuiwinTextOutAuto("ͨ�Ų�����:%d",nBaud);

    //������ͨ�Ź�Լ
    memset(&RecvBuf[0], 0, sizeof(RecvBuf));
    switch(pBuf[10])
    {
        case 0:
            strcpy((char *)RecvBuf, "��Ч");
            break;
        case 1:
            strcpy((char *)RecvBuf, "DL/T 645-1997");
            break;
        case 2:
            strcpy((char *)RecvBuf, "��������Э��");
            break;
        case 30:
            strcpy((char *)RecvBuf, "DL/T 645-2007");
            break;
        case 31:
            strcpy((char *)RecvBuf, "�����ز�Э��");
            break;
        default:
            strcpy((char *)RecvBuf, "δ�ӵ��ܱ�");
            break;
    }
    GuiwinTextOutAuto("�������%s",RecvBuf);

    Index = 0;
    GuiwinTextOutAuto("���ַ%02x%02x%02x%02x%02x%02x",pBuf[Index+16], pBuf[Index+15],
                        pBuf[Index+14], pBuf[Index+13], pBuf[Index+12], pBuf[Index+11]);
    GuiwinAutoDispProc("���ܱ���Ϣ" , NULL , 0 , (INT8U*)1);
    nFlg = GroupRout(&nSeq ,1);
    if(nFlg == 1)
    {
       break;
    }
  }
}

//���ò���
void ConfigPara(void)
{
  ConcerNumQuery();
}

//����״̬
void ContrlState(void)
{

}

//˫�򰴼�����
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
      //���Ұ�������䶯 ��
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
      //���¼��ı����ԣ�ÿ�۴ε�������һ�Σ���֮�ٸ��ű仯
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

//���ܱ�ʾ��
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
    const  INT8S*   c_OneScreen[]       ={"�����й���","�����й���","�����й���","�����й���","�����й���","����",0};
    const  INT8S*   c_TwoScreen[]       ={"�����޹���","һ����","������","������","������","����",0};
    const  INT8S*   c_ThreeScreen[]     ={"�����й��������","ʱ��","�����޹��������","ʱ��","����",0};

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
	//������
	while(1)
	{
       //����
       GuiwinTextOutAuto("%4s���ܱ�%dʾ��" , "" , nSeq);
       memset(pBuf, 0, sizeof(nBuf));
       MemReadMeterPrm(nSeq, F10, &pBuf[0], sizeof(nBuf));

       if(IsAllEqualVal(pBuf, sizeof(nBuf), 0))
       {
          sprintf((char*)&pBuf[0], " -------");
          GuiwinTextOutAuto("���ַ%s",pBuf);
       }
       else
       {
       GuiwinTextOutAuto("���ַ%02x%02x%02x%02x%02x%02x",pBuf[nIdex+16], pBuf[nIdex+15],
                    pBuf[nIdex+14], pBuf[nIdex+13], pBuf[nIdex+12], pBuf[nIdex+11]);
       }
       switch (nStepCnt)
       {
           case 1: //�����й���
		   {
            // ��1��
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
                  MemReadRealData(nSeq, F2, pBuf, sizeof(nBuf)); //����ʱ��YY-MM-DD-HH-MM-SS
                  sprintf((char*)&pBuf[0], "--/--/-- --:--:--");
                  GuiwinTextOutAuto("%s%s",c_TwoScreen[5],pBuf);
                }
                else
                {
                GuiwinTextOutAuto("����%02x/%02x/%02x  %02x:%02x",pBuf[nIdex + 5],pBuf[nIdex + 4],pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1]);
                }
                break;
            }
            case 2:
            {
                // ��2��
                nIdex = 0 ;
                nLimitFlg = 1;
                memset(pBuf, 0, sizeof(nBuf));
                MemReadRealData(nSeq, F7, pBuf, sizeof(nBuf)); // �����޹�����ʾֵ
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
                    MemReadRealData(nSeq, nEnumVal++, pBuf, sizeof(nBuf)); // һ����
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
                  MemReadRealData(nSeq, F2, pBuf, sizeof(nBuf)); //����ʱ��YY-MM-DD-HH-MM-SS
                  sprintf((char*)&pBuf[0], "--/--/-- --:--:--");
                  GuiwinTextOutAuto("%s%s",c_TwoScreen[5],pBuf);
                }
                else
                {
                GuiwinTextOutAuto("����%02x/%02x/%02x  %02x:%02x",pBuf[nIdex + 5],pBuf[nIdex + 4],pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1]);
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
                    MemReadRealData(nSeq, nEnumVal, pBuf, sizeof(nBuf)); //�����й�����

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
        GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);   //���Խ�����
       nFlg = UpDownLeftRight(&nSeq , &nStepCnt ,0);  //��������
        if(nFlg)
        {
          break;
        }
    }
}

//��������
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
      //���Ұ�������䶯 ��
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


//������Ϣ
void ChineseInfo(void)
{
  //INT8S*                        pString                = NULL ;
  const       INT8S           *c_InfoChoice[]        = {"��Ҫ��Ϣ" ,"������Ϣ" , 0};
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

//������Ϣ
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

      GuiwinTextOutAuto("      �ܼ���%x",nGrpIdx);
      GuiwinTextOutAuto("���絥��%02x%02x%02x%02x",pBuf[3] ,pBuf[2],pBuf[1],pBuf[0]);
      //��ǰ����   //�������    //ʣ�����   //����ʱ��  //��������  //��բ����
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
      GuiwinTextOutAuto("������ֵ%d%s",BCD2DEC(pBuf[8])*1000000 + BCD2DEC(pBuf[7])*10000 + BCD2DEC(pBuf[6])*100 + BCD2DEC(pBuf[5]),nUnit);
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
      GuiwinTextOutAuto("��������%d%s",pBuf[13],nUnit);
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
      GuiwinTextOutAuto("��բ����%d%s",BCD2DEC(pBuf[8 + nIdx])*1000000 + BCD2DEC(pBuf[7 + nIdx])*10000 + BCD2DEC(pBuf[6 + nIdx])*100 + BCD2DEC(pBuf[5 + nIdx]),nUnit);*/
      GuiwinTextOutAuto("��բ����%xkWh",pBuf[13]);
      GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);
      nFlg = GroupRout(&nGrpIdx ,0);
      if(nFlg == 1)
      {
         break;
      }
  }
}

//�ն���Ϣ
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

    GuiwinTextOutAuto("��������%02x%02x",pBuf[1], pBuf[0]);
    GuiwinTextOutAuto("�ն˵�ַ%02x%02x(H)",pBuf[3], pBuf[2]);
    GuiwinTextOutAuto("�ն˵�ַ%05d(D)", Buf2INT16U(&nBuf[2]));
    GuiwinTextOutAuto("�ն˱��");

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
	strcat((char*)pBuf, (char*)"����汾:V");
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
	strcat((char*)pBuf, (char*)"�������:");
	strcat((char*)pBuf, (char*)DATE_INFO);
	GuiwinTextOutAuto("%s", pBuf);
    GuiwinTextOutAuto("Ӳ���汾");
    GuiwinTextOutAuto("��������");

    //�ڶ���
    memset(pBuf, 0, sizeof(nBuf));
    GuiwinTextOutAuto("����IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
    GuiwinTextOutAuto("���ö˿�: %05d", pBuf[5] * 256 + pBuf[4]);
    GuiwinTextOutAuto("����IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[6],pBuf[7],pBuf[8],pBuf[9]);
    GuiwinTextOutAuto("���ö˿�: %05d", pBuf[11] * 256 + pBuf[10]);
    memcpy(nAPN ,&pBuf[12],30);
    GuiwinTextOutAuto("APN");
    GuiwinTextOutAuto("%s", nAPN);

    //������
    memset(pBuf, 0, sizeof(nBuf));
    memset(pData , 0 ,sizeof(nData));
    MemReadCommPrm(F4, &nBuf[0], sizeof(nBuf));

    //��BCD�벻��ӡ-------��վ����
    nIdx = 0;
    for (i = 0; i < 8; i++)
    {
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], pBuf[i], 2, 1, 2);
          //pData[i]����'F'�Ĳ��ֲ���ʾ������δ��ʾλ������
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
    //��ӡ��վ����
    memcpy(nStr ,pData ,nStrLen);
    GuiwinTextOutAuto("��վ����");
    GuiwinTextOutAuto("%3s%s","",nStr);

    //��BCD�벻��ӡ-------��������
    nIdx = 0;
    nStrLen = 0;
    pData = &nData[0];
    memset(pData , 0 ,sizeof(nData));
    memset(&nStr[0], 0 ,sizeof(nStr));
    for (i = 0; i < 8; i++)
    {
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], pBuf[i + 8], 2, 1, 2);
         //pData[i]����'F'�Ĳ��ֲ���ʾ������δ��ʾλ������
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
    //��ӡ��������
    memcpy(nStr ,pData ,nStrLen);
    GuiwinTextOutAuto("��������");
    GuiwinTextOutAuto("%3s%s","",nStr);

    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F7, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("�ն�IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);

    //������
        memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F7, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("�ն�IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
    GuiwinTextOutAuto("��������");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[4],pBuf[5],pBuf[6],pBuf[7]);
    GuiwinTextOutAuto("���ص�ַ");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[8],pBuf[9],pBuf[10],pBuf[11]);
    GuiwinTextOutAuto("�����˿�: %05d", pBuf[61] * 256 + pBuf[60]);

    pBuf = &nBuf[0];
    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F8, &nBuf[0], sizeof(nBuf));
    //����ģʽ
    switch(((nBuf[0] >> 7) & 0x01))
    {
        case 0:
            strcpy((char *)nBuf, "TCP");
            break;
        case 1:
            strcpy((char *)nBuf, "UDP");
            break;
    }
    GuiwinTextOutAuto("����ģʽ:%1s%s","",nBuf);
    //����ģʽ
    pBuf = &nBuf[0];
    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F8, &nBuf[0], sizeof(nBuf));
    switch((pBuf[0] >> 4) & 0x03)
    {
        case 0:
            strcpy((char *)nBuf, "���ģʽ");
            break;
        case 1:
            strcpy((char *)nBuf, "�ͻ���ģʽ");
            break;
        case 2:
            strcpy((char *)nBuf, "������ģʽ");
            break;
        default:
            strcpy((char *)nBuf, "��Ч");
            break;
    }
    GuiwinTextOutAuto("����ģʽ:");
    GuiwinTextOutAuto("%10s%s","", nBuf);
    GuiwinAutoDispProc(NULL , NULL , 0 , NULL);
}
