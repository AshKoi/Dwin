#include "GB698ActionRequest.h"
#include "Gw698.h"
#include "Gw3761.h"
#include "MemDrvExt.h"
#include "Table.h"
#include "Fun.h"
#include "Macro.h"
#include "SysRes.h"
#include "Base.h"
#include "ChkCrc.h"
#include "Reset.h"
#include "MemData.h"
#include "SoftTimer.h"
#include "rmotlink.h"
#include "MemTbl.h"
#include "SecTask.h"
#include "meter645.h"
#include "PowerOn.h"
#include "Ac7022.h"
#include "IdleTask.h"

/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/
/********************************************************************************
* ������: GB698ActionRequest
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��������
*********************************************************************************/
INT8S GB698ActionRequest(pAPP_COMM pComm)
{
	INT8U     	        nData[256];
    INT8U               nDataLen = 0;
    INT8S               nRet;
    pGB698_FRM_MAP      pMap        = NULL;
    pGB698_FRM_INFO    pInfo  = NULL;
    
    INT8U               nSetReqLen = GW698_APDU_LEN1;   
    if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB698_FRM_INFO)(pComm->pFrmInfo);
    pMap    = (pGB698_FRM_MAP)&pComm->pRxFrmBuf[0];
    
    nRet = GB698ActionRequestSendResp(&pMap->nData[0], &nData[0], &nDataLen);
    pInfo->nAckType = nData[4];
    pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[g_nHeadLen + nSetReqLen], &nData[0], nDataLen);
    if (nDataLen <= 0)
    {
        return RET_ERR_ERROR;
    }
    return RET_ERR_OK;
}


INT8S GB698ActionRequestSendResp(INT8U*  pRecData, INT8U *pData, INT8U *pDatalen)
{
    INT16U      nOI;
    INT8S       nRet = e_NoObject;
    INT8U       nDataLen = 0;
    INT8U       i = 0;
    INT8U       nTemp[2];
    INT8U       nOffset = 0;
    INT8U       nBuf[30] = {0};
    INT8U       nBufTmp[30] = {0};
    INT8U       nActionWay;
    INT8U       nLen = 0;
    //INT16U      nOI2;
    //INT16U      nOI3;
    INT8U       ii;
    //INT8U       jj;
    //INT8U       nCount=0;
    MT_REPORT_DATA          sAcReport;
    MT_OAD_DATA nOAD;
    MEM_AC_PRM	sAc_Param;
    
    memset(&sAc_Param, 0, sizeof(MEM_AC_PRM));
    nOI = (((INT16U)(pRecData[0]) << 8) | pRecData[1]);
    nActionWay = pRecData[2];

    memcpy(pData, pRecData, OAD_LEN);
    nDataLen += 4;
    nOffset += 4;
    
    if(g_MeterPowerOff && nOI!= 0xFE10)
    {
        nRet = e_NoDefObject;
        pData[nDataLen ++] = nRet;
        pData[nDataLen ++] = 0;           //����������
        *pDatalen = nDataLen;
        return nDataLen;
    }
    
    switch (nOI)
    {
        case 0xF100:     //
            if( 0xB0 == nActionWay)
            {
                nRet = 22;
            }
            break;
        case 0x8000:     //ң����բ
            if( 0x81 == nActionWay)
            {
            
            }
            break;
        case 0xE400:
        {
            if( 2 == nActionWay)
            {
                if(pRecData[nOffset ++] != e_enum)
                {
                    nRet = e_TypeErr;
                    break;
                }

                PowerDataFault(pRecData[nOffset]);
            }
            else if(3 == nActionWay)
            {
                PowerDataFault(0xFF);
            }

            break;
        }
        case 0xE401:
        {
            if( 2 == nActionWay)
            {
                if(pRecData[nOffset ++] != e_enum)
                {
                    nRet = e_TypeErr;
                    break;
                }
                nRet = DemandFault(pRecData[nOffset]);
            }
            else if(3 == nActionWay)
            {
                nRet = DemandFault(0xFF);
            }
            break;
        }
        case 0xFE01://�������
        {
            if (1 == pRecData[5])
            {
                g_MeterInsideWrong[0] |= 0x08;
            }
            else
            {
                g_MeterInsideWrong[0] &= 0xF7;
            }
            nRet = 0;
            break;
        }
        case 0xFE02://���ʱ���쳣
        {
            if (1 == pRecData[5])
            {
                g_MeterInsideWrong[0] |= 0x02;
            }
            else
            {
                g_MeterInsideWrong[0] &= 0xFD;
            }
            nRet = 0;
            break;
        }
        case 0xFE03://���485�˿ڹ���
        {
            g_MeterComFaul = pRecData[5];
            nRet = 0;
            break;
        }
        case 0xFE04://���ַ�쳣
        {
            nOffset += 2;
            for (i = 6; i > 0; i--)
            {
                nBufTmp[i-1] = pRecData[nOffset++];
            }
            
            MemWriteAcPrm(F19, &nBufTmp[0], 6);
            memcpy(&g_nMemAddr[0], &nBufTmp[0], 6);
            nRet = 0;
            break;
        }
        case 0xFE05://�ز�ģ�鹩��
        {
            if (1 == pRecData[5])
            {
                GpioClearOutput(IO_PORT5, IO_PINxA);
            }
            else
            {
                GpioSetOutput(IO_PORT5, IO_PINxA);
            }
            g_ZBPortFault = pRecData[5];
            nRet = 0;
            break;
        }
        case 0xFE06://�ز�ģ�����
        case 0xFE07:
        {
            g_ZBPortFault = pRecData[5];
            nRet = 0;
            break;
        }
        case 0xFE08://��ѹʧѹ
        {
            for(i=0;i<3;i++)
            { 
               if ((0x02 == (g_MeterEventWrong[i]&0x02)) && (0x01 == ((pRecData[5]>>i)&0x01)))
               {
                   nRet = -1; 
                   break; 
               }
               else if (0x01 == ((g_MeterVoltPlusFlg>>i)&0x01))
               {
                   nRet = -1; 
                   break;
               }
               g_MeterEventWrong[i]&=0xFE;
               g_MeterEventWrong[i] |= (0x01&(pRecData[5]>>i));
            }
            if (nRet < 0)
            {
                nRet = e_RefuseRW;
                break;
            }
            nRet = 0;
            break;
        }
        case 0xFE09://��ѹ����
        {
            for (i=0;i<3;i++)
            { 
               g_MeterEventWrong[i]&=0xFD;
               if (0x01&(pRecData[5]>>i))
               {
                 g_MeterEventWrong[i] |= 0x02;
               }
               //g_MeterEventWrong[i] |=(1 <<(0x01&(pRecData[5]>>i)));
            }
            nRet = 0;
            break;
        }
        case 0xFE0A://��ѹԽ��
        {
            for(i=0;i<3;i++)
            { 
               if ((0x02 == (g_MeterEventWrong[i]&0x02)) && (0x01 == ((pRecData[5]>>i)&0x01)))
               {
                   nRet = -1; 
                   break; 
               }
               else if (0x01 == ((pRecData[5]>>i)&0x01))
               {
                   g_MeterEventWrong[i]&=0xFE;
               }
            }
            if (nRet < 0)
            {
                nRet = e_RefuseRW;
                break;
            }
            g_MeterVoltPlusFlg = pRecData[5];
            nRet = 0;
            break;
        }
        case 0xFE0B://����ʧ��
        {
            for(i=0;i<3;i++)
            {  
               g_MeterEventWrong[i]&=0xFB;
               if (0x01&(pRecData[5]>>i))
               {
                 g_MeterEventWrong[i] |= 0x04;
               }
               //g_MeterEventWrong[i] |= (2 <<(0x01&(pRecData[5]>>i)));
            }
            nRet = 0;
            break;
        }
        case 0xFE0C://����
        {
            if (1 == pRecData[5])
            {
              memset(g_MeterAcErrFlg, 0, 4);
            }
            g_MeterAcErrFlg[0] = pRecData[5];
            nRet = 0;
            break;
        }
        case 0xFE0D://ͣ��
        {
            if (1 == pRecData[5])
            {
              memset(g_MeterAcErrFlg, 0, 4);
            }
            g_MeterAcErrFlg[1] = pRecData[5];
            nRet = 0;
            break;
        }
        case 0xFE0E://����
        {
            if (1 == pRecData[5])
            {
              memset(g_MeterAcErrFlg, 0, 4);
            }
            g_MeterAcErrFlg[2] = pRecData[5];
            nRet = 0;
            break;
        }
        case 0xFE0F://��ƽ��
        {
            if (1 == pRecData[5])
            {
              memset(g_MeterAcErrFlg, 0, 4);
            }
            g_MeterAcErrFlg[3] = pRecData[5];
            nRet = 0;
            break;
        }
        case 0xFE10://����ϵ�
        {
            if (1 == pRecData[5])
            {
                GpioClearOutput(IO_PORT5, IO_PINxA);//�ز��ϵ�
            }
            else
            {
                GpioSetOutput(IO_PORT5, IO_PINxA);
                g_nFlgReBoot = 1;
            }
            g_MeterPowerOff = pRecData[5];
            nRet = 0;
            break;
        }
        case 0xFE11://�������¼�
        {
            if (1 == pRecData[5])
            {
                g_SimOCPTrbFlag=0xFF; 
            }
            else
            {
                g_SimOCPTrbFlag=0xBF;
            }
            nRet = 0;
            break;
        }
        case 0xFE12://������¼�
        {
            if (1 == pRecData[5])
            {
                g_SimMeterCoverFlag=0xBF; 
            }
            else
            {
                g_SimMeterCoverFlag=0xFF;
            }
            nRet = 0;
            break;
        }
        case 0xFE13://�����բ�¼�
        {
            if (1 == pRecData[5])
            {
                g_SimMeterTripFlag=0xFF; 
            }
            else
            {
                g_SimMeterTripFlag=0xBF;
            }
            nRet = 0;
            break;
        }
        case 0xFE14://����բ�¼�
        {
            if (1 == pRecData[5])
            {
                g_SimMeterSwitchOnFlag=0xFF; 
            }
            else
            {
                g_SimMeterSwitchOnFlag=0xBF;
            }
            nRet = 0;
            break;
        }
        case 0xFE15://�����Կ�����¼�
        {
            g_SimMeterPasswordFlag = pRecData[5]; 
            
            nRet = 0;
            break;
        }
        case 0xFE16://����ɿ��������¼�
        {
            if (1 == pRecData[5])
            {
                g_SimLoadSwitchAcFlag=0xFF; 
            }
            else
            {
                g_SimLoadSwitchAcFlag=0xBF;
            }
            nRet = 0;
            break;
        }
        case 0xFE17://����Դ�쳣�¼�
        {
            if (1 == pRecData[5])
            {
                g_SimPowerAbnlFlag=0xFF; 
            }
            else
            {
                g_SimPowerAbnlFlag=0xBF;
            }
            nRet = 0;
            break;
        }
        case 0xFE18://���ʱ�ӹ����¼�
        {
            if (1 == pRecData[5])
            {
                g_SimClockTrbFlag=0xFF; 
            }
            else
            {
                g_SimClockTrbFlag=0xBF;
            }
            nRet = 0;
            break;
        }
        case 0xFE19://������оƬ�����¼�
        {
            if (1 == pRecData[5])
            {
                g_SimChipTrbFlag=0xFF; 
            }
            else
            {
                g_SimChipTrbFlag=0xBF;
            }
            nRet = 0;
            break;
        }
        
        case 0xFE1A://���õ��ܱ����ƫ��
        {
            if ((g_ACOffset<80) || (g_ACOffset>120))
            {
                nRet = -1;
                break;
            }
            g_ACOffset = pRecData[5];
            MemWriteAcXFlhBackData(F112, &g_ACOffset, sizeof(g_ACOffset));
            
            nRet = 0;
            break;
        }
        
        case 0xFE1B://�������峣��
        {
            AcCalcJustPluseConst(pRecData[5]);
            nRet = 0;
            break;
        }
        case 0xF201://RS485����������
        {
            if(0x7F == nActionWay)
            {
                MemReadAcPrm(F27, &sAc_Param.nCom1Baud, sizeof(sAc_Param.nCom1Baud));
                ii = pRecData[12];
                g_nUartBpsflg = pRecData[12];
                switch(ii)  //�޸Ĳ�����
                {
                  case 0x00:
                    //g_nUART_BPS_NUM = e_UART_BPS_300;
                    break;
                  case 0x01:
                    //g_nUART_BPS_NUM = e_UART_BPS_600;
                    sAc_Param.nCom1Baud = 1<<1;//�޸�������
                    break;
                  case 0x02:
                    //g_nUART_BPS_NUM = e_UART_BPS_1200;
                    sAc_Param.nCom1Baud = 1<<2;//�޸�������
                    break;
                  case 0x03:
                    //g_nUART_BPS_NUM = e_UART_BPS_2400;
                    sAc_Param.nCom1Baud = 1<<3;//�޸�������
                    break;
                  case 0x04:
                    //g_nUART_BPS_NUM = e_UART_BPS_4800;
                    sAc_Param.nCom1Baud = 1<<4;//�޸�������
                    break;
                  case 0x05:
                    //g_nUART_BPS_NUM = e_UART_BPS_7200;
                    break;
                  case 0x06:
                    //g_nUART_BPS_NUM = e_UART_BPS_9600;
                    sAc_Param.nCom1Baud = 1<<5;//�޸�������
                    break;
                  case 0x07:
                    //g_nUART_BPS_NUM = e_UART_BPS_19200;
                    sAc_Param.nCom1Baud = 1<<6;//�޸�������
                    break;
                  case 0x08:
                    //g_nUART_BPS_NUM = e_UART_BPS_38400;
                    break;
                  case 0x09:
                    //g_nUART_BPS_NUM = e_UART_BPS_57600;
                    break;
                  case 0x0A:
                    //g_nUART_BPS_NUM = e_UART_BPS_115200;
                    break;
                  default:
                    //g_nUART_BPS_NUM = e_UART_BPS_2400;
                    break;
                }
              //  g_nAlterUartBpsflg = 1;
                SetCommCfg(1, ii, 2);        //2400 żУ��
                OpenCommPort(COMM_WH485);
                MemWriteAcPrm(F27, &sAc_Param.nCom1Baud, sizeof(sAc_Param.nCom1Baud));
                
            }
            nRet = 0;
            break;
        }
        case 0x4000:
        {
            if(0x7F == nActionWay)
            {
                GB698Set4000Proc(&pRecData[4], 0, &nRet);
            }
            break;
        }
        case 0x4300:
        {
            if(0x01 == nActionWay)
            {
                ResetDelayHardInit(1000);
            }
            break;
        }
        /*ռδʹ�ô���λ�ô�ȷ��
        case 0x301B://kaigai�¼�������������
        {
            if(0x01 == pRecData[2])//��λ
            {
                MemReadAcEvtProc(F30,1, &sAcReport, sizeof(MT_REPORT_DATA));
                memset(&sAcReport, 0, sizeof(MT_REPORT_DATA));
                MemWriteAcEvtProc(F30, &sAcReport, sizeof(MT_REPORT_DATA), 0);
                nRet = 0;
            }
            else if (0x04 == pRecData[2])//���һ����������
            {
                nRet=GB698AddAssociationObject(&pRecData[0], F41);
            }
            else if(0x05 == pRecData[2])//ɾ��һ����������
            {
                nRet = GB698RemoveAssociationObject(&pRecData[0], F41);
            }
            //nRet = 0;
            break;
        }
        case 0x301f://��բ�¼�������������
        {
            if(0x01 == pRecData[2])//��λ
            {
                MemReadAcEvtProc(F31,1, &sAcReport, sizeof(MT_REPORT_DATA));
                memset(&sAcReport, 0, sizeof(MT_REPORT_DATA));
                MemWriteAcEvtProc(F31, &sAcReport, sizeof(MT_REPORT_DATA), 0);
                nRet = 0;
            }
            else if (0x04 == pRecData[2])//���һ����������
            {
                nRet=GB698AddAssociationObject(&pRecData[0], F42);
            }
            else if(0x05 == pRecData[2])//ɾ��һ����������
            {
                nRet = GB698RemoveAssociationObject(&pRecData[0], F42);
            }
            //nRet = 0;
            break;
        }
        case 0x3020:
        {
            if(0x01 == pRecData[2])//��λ
            {
                MemReadAcEvtProc(F32,1, &sAcReport, sizeof(MT_REPORT_DATA));
                memset(&sAcReport, 0, sizeof(MT_REPORT_DATA));
                MemWriteAcEvtProc(F32, &sAcReport, sizeof(MT_REPORT_DATA), 0);
                nRet = 0;
            }
            else if (0x04 == pRecData[2])//���һ����������
            {
                nRet=GB698AddAssociationObject(&pRecData[0], F43);
            }
            else if(0x05 == pRecData[2])//ɾ��һ����������
            {
                nRet = GB698RemoveAssociationObject(&pRecData[0], F43);
            }
            //nRet = 0;
            break;
        }
        case 0x302B:
        {
            if(0x01 == pRecData[2])//��λ
            {
                MemReadAcEvtProc(F34,1, &sAcReport, sizeof(MT_REPORT_DATA));
                memset(&sAcReport, 0, sizeof(MT_REPORT_DATA));
                MemWriteAcEvtProc(F34, &sAcReport, sizeof(MT_REPORT_DATA), 0);
                nRet = 0;
            }
            else if (0x04 == pRecData[2])//���һ����������
            {
                nRet=GB698AddAssociationObject(&pRecData[0], F45);
            }
            else if(0x05 == pRecData[2])//ɾ��һ����������
            {
                nRet = GB698RemoveAssociationObject(&pRecData[0], F45);
            }
            //nRet = 0;
            break;
        }
        case 0x302C:
        {
            if(0x01 == pRecData[2])//��λ
            {
                MemReadAcEvtProc(F35,1, &sAcReport, sizeof(MT_REPORT_DATA));
                memset(&sAcReport, 0, sizeof(MT_REPORT_DATA));
                MemWriteAcEvtProc(F35, &sAcReport, sizeof(MT_REPORT_DATA), 0);
                nRet = 0;
            }
            else if (0x04 == pRecData[2])//���һ����������
            {
                nRet=GB698AddAssociationObject(&pRecData[0], F46);
            }
            else if(0x05 == pRecData[2])//ɾ��һ����������
            {
                nRet = GB698RemoveAssociationObject(&pRecData[0], F46);
            }
            //nRet = 0;
            break;
        }
        case 0x302E:
        {
            if(0x01 == pRecData[2])//��λ
            {
                MemReadAcEvtProc(F36,1, &sAcReport, sizeof(MT_REPORT_DATA));
                memset(&sAcReport, 0, sizeof(MT_REPORT_DATA));
                MemWriteAcEvtProc(F36, &sAcReport, sizeof(MT_REPORT_DATA), 0);
                nRet = 0;
            }
            else if (0x04 == pRecData[2])//���һ����������
            {
                nRet=GB698AddAssociationObject(&pRecData[0], F47);
            }
            else if(0x05 == pRecData[2])//ɾ��һ����������
            {
                nRet = GB698RemoveAssociationObject(&pRecData[0], F47);
            }
            //nRet = 0;
            break;
        }
        case 0x302F:
        {
            if(0x01 == pRecData[2])//��λ
            {
                MemReadAcEvtProc(F37,1, &sAcReport, sizeof(MT_REPORT_DATA));
                memset(&sAcReport, 0, sizeof(MT_REPORT_DATA));
                MemWriteAcEvtProc(F37, &sAcReport, sizeof(MT_REPORT_DATA), 0);
                nRet = 0;
            }
            else if (0x04 == pRecData[2])//���һ����������
            {
                nRet=GB698AddAssociationObject(&pRecData[0], F48);
            }
            else if(0x05 == pRecData[2])//ɾ��һ����������
            {
                nRet = GB698RemoveAssociationObject(&pRecData[0], F48);
            }
            //nRet = 0;
            break;
        }*/
        default: 
            nRet = e_NoDefObject; 
            break;
    }

    pData[nDataLen ++] = nRet;
    pData[nDataLen ++] = 0;           //����������
    *pDatalen = nDataLen;
    return nDataLen;
}

INT8S GB698AddAssociationObject(INT8U*  pRecData, INT8U nFn)
{
    INT8S       nRet = e_NoObject;
    INT16U      nOI2;
    INT16U      nOI3;
    INT8U       ii;
    //INT8U       jj;
    INT8U       nCount=0;
    MT_OAD_DATA nOAD;
    
    memset(&nOAD.sOAD[0], 0, sizeof(MT_OAD_DATA));
    
    MemReadAcEvtProc(nFn,1, &nOAD, sizeof(MT_OAD_DATA)); 
    nOI2 = (((INT16U)(pRecData[5]) << 8) | pRecData[6]);
    for(ii=0;ii<64;ii++)
    {
        nOI3 = (((INT16U)(nOAD.sOAD[ii].nD1 << 8) | nOAD.sOAD[ii].nD2));
        if(nOAD.sOAD[ii].nD3 == 0xFF)
        {
            memset(&nOAD.sOAD[ii], 0, sizeof(FORMAT27));
        }
        if(nOI2 ==nOI3)//�ų�OI��ʶ��ͬ
        {
             if((nOAD.sOAD[ii].nD3 == pRecData[7])&&(nOAD.sOAD[ii].nD4 == pRecData[8]))
             {
                nRet = 3;//��ͬ�򷵻�ʧ�ܣ��������ݿ����д˱�ʶ
                break;
             }
             else//�����޸����Ա�־д����н���
             {
                memcpy(&nOAD.sOAD[ii].nD1,&pRecData[5],4);
                MemWriteAcEvtProc(nFn, &nOAD, sizeof(MT_OAD_DATA), 0);
                nRet = 0;
                break;
             }
        }
        if((nOI3 != 0x00)&&(nOAD.sOAD[ii].nD3!=0x00)&&(nOAD.sOAD[ii].nD3!=0x00))
        {
            nCount++;//Ϊû�д˱�ʶʱ��¼���λ�ã�ӦҪȷ�����г�ʼȫΪ00
        }
    }
    if((nRet==0x06)||(nCount==0))
    {
        memcpy(&nOAD.sOAD[nCount].nD1,&pRecData[5],4);
        nCount +=1;
        memset(&nOAD.sOAD[nCount], 0, sizeof(MT_OAD_DATA)-nCount);
        MemWriteAcEvtProc(nFn, &nOAD, sizeof(MT_OAD_DATA), 1);
        nRet = 0;
    }
    return nRet;
}

INT8S GB698RemoveAssociationObject(INT8U*  pRecData, INT8U nFn)
{
    INT8S       nRet = e_NoObject;
    INT16U      nOI2;
    INT16U      nOI3;
    INT8U       ii;
    INT8U       jj;
    //INT8U       nCount=0;
    MT_OAD_DATA nOAD;
    
    memset(&nOAD.sOAD[0], 0, sizeof(MT_OAD_DATA));
    
    MemReadAcEvtProc(nFn,1, &nOAD, sizeof(MT_OAD_DATA)); 
    nOI2 = (((INT16U)(pRecData[5]) << 8) | pRecData[6]);
    for(ii=0;ii<64;ii++)
    {
        nOI3 = (((INT16U)(nOAD.sOAD[ii].nD1 << 8) | nOAD.sOAD[ii].nD2));

        if(nOI2 ==nOI3)//����OI��ʶ��ͬ
        {
              jj = ii+1;//���ҵ���ͬ��ʶ�����������ǰ��
              memcpy(&nOAD.sOAD[ii].nD1,&nOAD.sOAD[jj],sizeof(MT_OAD_DATA)-jj);
              memset(&nOAD.sOAD[63], 0, 1);//ǰ�ƺ����һ��������0��
              nRet = 0;
        }
    }
    MemWriteAcEvtProc(nFn, &nOAD, sizeof(MT_OAD_DATA), 0);
    return nRet;
}