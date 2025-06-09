/******************************************************************************************************
* Copyright (c) 2010-2011 ��һ�Ƽ��ɷ����޹�˾�з�����
* �ļ���:
* ������: �з�����Ӧ�ÿ�����
* �ա���: 2011/01/24
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
*****************************************************************************************************/
#include "GB3761FileTrans.h"

FTP_INFO         g_sFtpParaInfo;                      //FTP����
/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/
/********************************************************************************
* ������: GB3761FileTrans
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �ļ�����
*********************************************************************************/
INT8S GB3761FileTrans(pAPP_COMM pComm)
{
	INT16U 		nPn[8];
	INT16U 		nFn[GB3761_MAX_FN_NUM];
	INT16U		nPnNum   = 0;
	INT8U		nFnNum   = 0;
	INT16U  	nOffset  = 0;
	pGB3761_FRM_INFO pInfo = NULL;

	if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)(pComm->pFrmInfo);
	while (nOffset + 4 <= pInfo->nRxDataLen)
	{
		memset(&nPn[0], 0xFFFF, sizeof(nPn));
        memset(&nFn[0], 0, sizeof(nFn));
		if (RET_ERR_OK != GB3761DaDt2PnFn(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nOffset], &nPn[0], &nPnNum, &nFn[0], &nFnNum, &pInfo->bAllMpFlag))  //Dadtת��Pnfn
		{
            break;
        }
        nOffset += 4;
        if (RET_ERR_OK != GB3761FileTransProc(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
	}
	return RET_ERR_OK;
}

/********************************************************************************
* ������: GB3761FileTransProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �ļ�����
*********************************************************************************/
INT8S GB3761FileTransProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
	INT16U    	i = 0;
	INT16U    	j = 0;
	INT16U		nDataLen = 0;
	INT8U     	nData[128];
    INT16U      nMaxDataLen = 0;
	pGB3761_FRM_INFO pInfo = NULL;

    if (NULL == pComm || NULL == pPn || NULL == pFn || NULL == pOffset)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)(pComm->pFrmInfo);
    nMaxDataLen = pComm->nFrmLen - GB3761_FRM_RES_LEN;
	for (i = 0; i < nPnNum; i++)
	{
		for (j = 0; j < nFnNum; j++)
		{
            nDataLen = 0;                               //���ݳ������㣬ÿ�ζ�����
            memset(&nData[0], 0, sizeof(nData));        //�����������
			nDataLen += GB3761FileTransFnProc(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
            if (nDataLen > nMaxDataLen || 0 == nDataLen)   //һ��Fn�����ݳ��ȳ���֡�� ����
            {
                continue;
            }
            if ((pInfo->nTxDataLen + nDataLen) > nMaxDataLen)//��֡���ݳ���֡�� ����
            {
                pInfo->bSendFlag = TRUE;
                if (RET_ERR_OK != GB3761FillTxFrm(pComm))
                {
                    return RET_ERR_ERROR;
                }
                if (RET_ERR_OK != GB3761SendTxFrm(pComm))
                {
                    return RET_ERR_ERROR;
                }
                pInfo->nTxDataLen = 0;  //������֡
            }
            pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nData[0], nDataLen);
		}
	}
	return RET_ERR_OK;
}


/********************************************************************************
* ������: GB3761FileTransFnProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �ļ�����
*********************************************************************************/
INT16U GB3761FileTransFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    INT16U  nDataLen = 0;
    pGB3761_FRM_INFO pInfo = NULL;

    if (NULL == pComm || NULL == pOffset || NULL == pData)
    {
        return 0;
    }
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    if (RET_ERR_OK != GB3761PnFn2DADT(nPn, nFn, &pData[nDataLen]))
    {
        return 0;
    }
    nDataLen += 4;
    switch (nFn) //����ͬFn��
    {
    	case F1:      //F1 Զ�����ش������
        {
            nDataLen += GB3761FileTransGWF1(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset, &pData[nDataLen], nMaxDataLen);
            break;
        }
        default:
        {
        	pInfo->bNoDataFlag = TRUE;
            break;
        }
    }
	if (nDataLen > 4)
    {
    	return nDataLen;
    }
    return 0;
}

//����Զ������F1�������
INT16U GB3761FileTransGWF1(INT16U nPn, INT8U *pSrcData, INT16U nSrcDataLen, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nFileEn     = 0;                                //�ļ�ʹ�ñ�־
    INT8U   nDownLoadFileEn = 0;
	INT8U   i           = 0;
    INT8U   j           = 0;
    INT8U   nFileID     = 0;                                //�ļ���ʶ
    INT8U   nFileProp   = 0;                                //�ļ�����
    INT8U   nFileCmd    = 0;                                //�ļ�ָ��
    INT16U  nMemFileId  = 0;
    INT16U  nFileSemIdx = 0;                                //�ļ�ʹ���ź����±�
    INT16U  nFileIdx    = 0;                                //�ļ��±�
	INT16U  nDataLen    = 0;
    INT16U  nCRC        = 0;
    INT16U  nTotalNum   = 0;                                //�ļ��ܶ���
    INT16U  nIndex      = 0;
    INT16U  nCurrLen    = 0;                                //��ǰ����
    INT16U  nLastLen    = 0;
    INT16U  nTmpIdx     = 0;
    INT32U  nCurrID     = 0;                                //��ǰ��ʶ
    INT32U  nUpGradeRet = 0;
    INT32U  nLastID     = 0xFFFFFFFF;                       //��һ��ʶ
    INT32U  nSumLen     = 0;
    INT32S  nSeekLen    = 0;
    INT32S  nFhandle    = -1;                                //�ļ����
    FILE_TRANS_INFO     sFileTInfo;
    RMOT_FTP_INFO       sRmotFtpInfo;                       //Զ��FTP��������

	if (NULL == pSrcData || NULL == pOffset || NULL == pData)
	{
		return 0;
	}
	nIndex = *pOffset;
	if ((nIndex + 11) > nSrcDataLen)
    {
    	*pOffset = nSrcDataLen;
    	return 0;
    }

	for (i = 0; i < 1; i++)
	{
        nFileID   = pSrcData[nIndex];
        nFileProp = pSrcData[nIndex + 1];
        nFileCmd  = pSrcData[nIndex + 2];
        nTotalNum = pSrcData[nIndex + 3] + ((pSrcData[nIndex + 4]<<8) & 0xFF00);
        nCurrID   = pSrcData[nIndex + 5] + ((pSrcData[nIndex + 6]<<8) & 0xFF00)
                    + ((pSrcData[nIndex + 7]<<16) & 0xFF0000) + ((pSrcData[nIndex + 8]<<24) & 0xFF000000);
        nCurrLen  = pSrcData[nIndex + 9] + ((pSrcData[nIndex + 10]<<8) & 0xFF00);
        nIndex += 11;

        if ((nIndex + nCurrLen) > nSrcDataLen)
        {
            nCurrID = nLastID;
            *pOffset = nSrcDataLen;
            break;
        }
        nTmpIdx = nIndex;
        nIndex += nCurrLen;
        *pOffset = nIndex;

        memset(&sFileTInfo, 0, sizeof(sFileTInfo));
        MemReadXSaveData(MEM_OBJ_X_SAVE_FLH_TERM, 0, MEM_SAVE_FILE_TRANS_INFO_ID, &sFileTInfo, sizeof(sFileTInfo));
        if (0x0 == nFileID)								//��������ļ����ָ�������ǰ״̬
        {
            InitDownFrmInfo();                          //��ʼ������֡��Ϣ

            nFileID =  sFileTInfo.nFileID;      		//��ȡǰһ�����ص��ļ���Ϣ
            if (!ChkRange(nFileID, 1, 7))
            {
                break;
            }

    		if (RET_ERR_OK != GetFileSemIdx(nFileID, &nFileSemIdx))
	    	{
	    		nCurrID = 0xFFFFFFFF;
	            break;
	    	}
		    	
		    if (RET_ERR_OK != GetFileIdx(nFileID, &nFileIdx))
        	{
        		nCurrID = 0xFFFFFFFF;
	            break;
	    	}
	    	
    		if (RET_ERR_OK != StopFileEn(nFileID))
    		{
    			nCurrID = 0xFFFFFFFF;           	//��ȡ�ļ�ʹ��Ȩʧ�ܷ��ش���
    			break;
    		}
    		
    		if (RET_ERR_OK != GetFileSem(nFileSemIdx))  //��ȡ�ļ�ʹ��Ȩ
    		{
    		    nCurrID = 0xFFFFFFFF;           	//��ȡ�ļ�ʹ��Ȩʧ�ܷ��ش���   	
    		}
    		else
    		{
        		if (RET_ERR_OK != InitUpgradeFile(nFileIdx))
            	{
            		nCurrID = 0xFFFFFFFF;            //��ʼ���ļ�ʧ�ܷ��ش���
            	}
                else
                {
                    memset(&sFileTInfo, 0, sizeof(sFileTInfo)); //��ʼ��ǰһ�������ļ���Ϣ
                    MemWriteXSaveData(MEM_OBJ_X_SAVE_FLH_TERM, 0, MEM_SAVE_FILE_TRANS_INFO_ID, &sFileTInfo, sizeof(sFileTInfo));
                }
				CloseFileHand(nFileIdx);
				FreeFileSem(nFileSemIdx);
			}
        	break;
        }
        else								            //����������ļ�
	    {
	    	if (0x02 == nFileCmd)           			//���ַ����
        	{
            	INT8U nGrpUpdateVld = 0x55;
            	MemWriteXSaveData(MEM_OBJ_X_SAVE_FLH_TERM, 0, MEM_SAVE_GROUP_UPDATE_VLD_ID, &nGrpUpdateVld, sizeof(nGrpUpdateVld));
            	break;
        	}
        	else if (0x01 == nFileCmd)      			//FTP����
        	{

                if (nFileID != 1)
                {
                    if (RET_ERR_OK != GetMemFileEnId(nFileID, &nMemFileId))
                    {
                        nCurrID = 0xFFFFFFFF; //��������ظ�
                        break;
                    }
                }

                if (RET_ERR_OK != GetFileSemIdx(nFileID, &nFileSemIdx))
		    	{
		    		nCurrID = 0xFFFFFFFF;
		            break;
		    	}

                memset(&sRmotFtpInfo, 0, sizeof(sRmotFtpInfo));

                //ֹ֪ͨͣFTP����
                StopFTPTask(&sRmotFtpInfo);

                //ֹͣ��Ӧ�ļ���������
                if (nFileID != 1)
                {
                    nDownLoadFileEn = 0;
                    MemReadXTempData(MEM_OBJ_X_TEMP_TERM, 0, nMemFileId, &nDownLoadFileEn, sizeof(nDownLoadFileEn));
                    if (0xAA != nDownLoadFileEn)
                    {
                        nDownLoadFileEn = 0xAA;
                        MemWriteXTempData(MEM_OBJ_X_TEMP_TERM, 0, nMemFileId, &nDownLoadFileEn, sizeof(nDownLoadFileEn));
                    }
                }
        		if (RET_ERR_OK != GetFileSem(nFileSemIdx))  //��ȡ�ļ�ʹ��Ȩ
        		{
        		    nCurrID = 0xFFFFFFFF;           	//��ȡ�ļ�ʹ��Ȩʧ�ܷ��ش���
                	break;    	
        		}
        		else
        		{
					FreeFileSem(nFileSemIdx);				//����ļ�ʹ��Ȩ�Ա�FTP����ȥ����
			    }
        		
                //FTP��������
                memset(&g_sFtpParaInfo, 0, sizeof(g_sFtpParaInfo));
	            AnalysisFtpPara(&g_sFtpParaInfo, &pSrcData[nTmpIdx], nCurrLen);
	            MemWriteXTempData(MEM_OBJ_X_TEMP_TERM, 0, MEM_TEMP_RMOT_FTP_PARA_ID, &g_sFtpParaInfo, sizeof(g_sFtpParaInfo));

	            //����FTP�ļ���־/��FTP��Ϣд�뽻����
	            memset(&sRmotFtpInfo, 0, sizeof(sRmotFtpInfo));
                MemReadXTempData(MEM_OBJ_X_TEMP_TERM, 0, MEM_TEMP_RMOT_FTP_INFO_ID, &sRmotFtpInfo, sizeof(sRmotFtpInfo));
	            sRmotFtpInfo.nFtpFileId = nFileID;
                sRmotFtpInfo.nFtpEn = 0x55;
                MemWriteXTempData(MEM_OBJ_X_TEMP_TERM, 0, MEM_TEMP_RMOT_FTP_INFO_ID, &sRmotFtpInfo, sizeof(sRmotFtpInfo));
	
	            //�ȴ���·���ؽ��
	            for (j = 0; j < GB3761_FTP_TIMEOUT; j++)
	            {
	                memset(&sRmotFtpInfo, 0, sizeof(sRmotFtpInfo));
	                MemReadXTempData(MEM_OBJ_X_TEMP_TERM, 0, MEM_TEMP_RMOT_FTP_INFO_ID, &sRmotFtpInfo, sizeof(sRmotFtpInfo));
	                if (0 == sRmotFtpInfo.nFtpRet)
	                {
	                    break;
	                }
	
	                if (1 == sRmotFtpInfo.nFtpRet)
	                {
	                    nCurrID = 0xFFFFFFFE;   //FTP�������޷���¼
	                    break;
	                }
	
	                if (2 == sRmotFtpInfo.nFtpRet)
	                {
	                    nCurrID = 0xFFFFFFFD;   //FTP���������Ҳ���ָ���ļ�
	                    break;
	                }
	                OSTimeDlyHMSM(0, 0, 1, 0);
	            }
	
	            if (j >=GB3761_FTP_TIMEOUT)
	            {
	            	nCurrID = 0xFFFFFFFF;       //��ʱ�����ļ�����
	            }
	            break;
        	}
	        else if (0x00 == nFileCmd)                  //��������
	        {
			    //�����ļ���ʶ����
		        if (RET_ERR_OK != GetFileIdx(nFileID, &nFileIdx))
		        {
		            nCurrID = 0xFFFFFFFF;
		            break;
		        }
		
			    //���������ж�
		    	if ((nCurrID > nTotalNum) || (0x00 != nFileProp && 0x01 != nFileProp))
		        {
		            nCurrID = 0xFFFFFFFF;
		            break;
		        }
		
		    	if (RET_ERR_OK != GetFileSemIdx(nFileID, &nFileSemIdx))
		    	{
		    		nCurrID = 0xFFFFFFFF;
		            break;
		    	}
	
	        	//�ѳɹ�����
			    MemReadXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_FILE_TRANS_ID, &nLastID, sizeof(nLastID));
		        if ((0x01 == nFileProp) &&  (0xEEEEEEEE == nLastID))
		        {
					break;
		        }
		
		        //��һ�����ػ���������һ���ļ� ��Ҫ���������ļ���Ϣ
		        if ((nFileID   != sFileTInfo.nFileID)
		         || (nTotalNum != sFileTInfo.nTotSegNum)
		         || ((nCurrLen != sFileTInfo.nSegLen) && (nTotalNum != (1 + nCurrID))))
		        {
		        	//�ļ�ֹͣʹ��
					if (RET_ERR_OK != StopFileEn(nFileID))
					{
						nCurrID = 0xFFFFFFFF;
						break;
					}
		        	
				    if (RET_ERR_OK != GetFileSem(nFileSemIdx))  //��ȡ�ļ�ʹ��Ȩ
	        		{
	        		    nCurrID = 0xFFFFFFFF;           	//��ȡ�ļ�ʹ��Ȩʧ�ܷ��ش���    	
	        		}
	        		else
	        		{
		        		if (RET_ERR_OK != InitUpgradeFile(nFileIdx))
		            	{
		            		nCurrID = 0xFFFFFFFF;            //��ʼ���ļ�ʧ�ܷ��ش���
		            	}
		            	CloseFileHand(nFileIdx);
						FreeFileSem(nFileSemIdx);
					}
					
					if (0xFFFFFFFF == nCurrID)
					{
						break;
					}
		        	
		        	//��ʼ������֡��Ϣ
		            InitDownFrmInfo();
		            sFileTInfo.nFileID    = nFileID;
		            sFileTInfo.nTotSegNum = nTotalNum;
		            sFileTInfo.nSegLen    = nCurrLen;
		            MemWriteXSaveData(MEM_OBJ_X_SAVE_FLH_TERM, 0, MEM_SAVE_FILE_TRANS_INFO_ID, &sFileTInfo, sizeof(sFileTInfo));
		            MemWriteXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_FILE_TRANS_LEN, &nCurrLen, sizeof(nCurrLen));
				}
		
	        	if (0 == nCurrID)      						//��һ֡����
        		{
        			if  (0x1 == nFileID)
        			{
	            		nTmpIdx  += GB3761_FILETRANS_HEAD_LEN;	//����֮ǰ��������    dwnǰ����16���ֽ�
	            		nCurrLen -= GB3761_FILETRANS_HEAD_LEN;
            		}
            		nSeekLen = 0;
        		}
        		else
        		{
            		MemReadXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_FILE_TRANS_LEN, &nLastLen, sizeof(nLastLen));
            		if (nTotalNum != (1 + nCurrID))
            		{
                		if (nCurrLen != nLastLen)			//֡���Ȳ�һ��
                		{
                    		nCurrID = 0xFFFFFFFF;  			//�����ļ�ʧ��
                    		break;
                		}
                        MemWriteXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_FILE_TRANS_LEN, &nCurrLen, sizeof(nCurrLen));
            		}

            		nSumLen =  nTotalNum * nLastLen;
            		if (nSumLen > MAX_SEC_FILE_LEN)
            		{
                		nCurrID = 0xFFFFFFFF;  				//�ļ�̫��
                		break;
            		}
            		nSeekLen = nLastLen * nCurrID - GB3761_FILETRANS_HEAD_LEN;
        		}

	        	//�ļ�ֹͣʹ��
				if (RET_ERR_OK != StopFileEn(nFileID))
				{
					nCurrID = 0xFFFFFFFF;
					break;
				}
	    		//��ȡ�ļ�ʹ��Ȩ
    			if (RET_ERR_OK != GetFileSem(nFileSemIdx))
    			{
    		    	nCurrID = 0xFFFFFFFF;           		//��ȡ�ļ�ʹ��Ȩʧ�ܷ��ش���
            		break;    	
    			}
    			else
    			{
    				nFileEn = 0x55;
    				//�ȹر��ļ�
    				CloseFileHand(nFileIdx);
    				//����Ҫ�������ļ�
		        	g_nFileHand[nFileIdx] = pFS->FileOpen((INT8U *)&g_nFileName[nFileIdx][0], OPEN_READWRITE);
		        	nFhandle = g_nFileHand[nFileIdx];
		        	if (nFhandle < 0)
		        	{
		        		nCurrID = 0xFFFFFFFF;  					//���ļ�ʧ��
		        		break;
		        	}
		    		
		    		//�����ر�������д���ļ�    	
		    		nCRC = GetCrc16(&pSrcData[nTmpIdx], nCurrLen);
	        		pFS->FileSeek(nFhandle, nSeekLen, SEEKFROM_BEGIN);
	        		nUpGradeRet = pFS->FileWrite(&pSrcData[nTmpIdx], nCurrLen, nFhandle);
	        		if (nUpGradeRet != nCurrLen)
	        		{
	            		nCurrID = 0xFFFFFFFF;
	            		break;
	        		}
	        		pFS->FileFlush(nFhandle);
	        		pFS->FileSeek(nFhandle, nSeekLen, SEEKFROM_BEGIN);
	        		memset(&pSrcData[nTmpIdx], 0, nCurrLen);
	        		nUpGradeRet = pFS->FileRead(&pSrcData[nTmpIdx], nCurrLen, nFhandle);
	        		if (nUpGradeRet != nCurrLen)
	        		{
	            		nCurrID = 0xFFFFFFFF;
	            		break;
	        		}
	        		if (nCRC != GetCrc16(&pSrcData[nTmpIdx], nCurrLen))
	        		{
	            		nCurrID = 0xFFFFFFFF;
	            		break;
	        		}
	        		UpdateDownFrmInfo(nCurrID);
	        		
	        		CloseFileHand(nFileIdx);
	        		FreeFileSem(nFileSemIdx);
	        		nFileEn = 0xAA;
	        		//����֡����
					if (0x01 == nFileProp)
					{
						if (1 == nFileID) //�ն������ļ�
						{
							INT8U nTmpBuf[30];
							memset(&nTmpBuf[0], 0, sizeof(nTmpBuf));
							memcpy(&nTmpBuf[0], "/usr/", 5);
							memcpy(&nTmpBuf[5], &g_nFileName[nFileIdx], 20);
							nUpGradeRet = pUpgrade->NandUpgrade(&nTmpBuf[0]);
							if (ERROR(nUpGradeRet))
							{
				    			nCurrID = 0xFFFFFFFF; //��������ظ�
							}
							else
							{
				    			INT8U nShareData = 0x55;
				    			INT8U ii = 0;
				    		
				    			for (ii = 0; ii < 3; ii++)
				    			{
				        			if (RET_ERR_OK == MemWriteXTempData(MEM_OBJ_X_TEMP_TERM, 0, MEM_TEMP_RESET_INFO_ID, &nShareData, sizeof(nShareData)))
				        			{
				           				break;
				        			}
				    			}
							}
						}
						else
						{
							//��������־д���Ӧ�ļ�
							INT8U  nDownLoadFileEn = 0x55;
				
							if (RET_ERR_OK != GetMemFileEnId(nFileID, &nMemFileId))
							{
				    			nCurrID = 0xFFFFFFFF; //��������ظ�
							}
							else
							{
								MemWriteXTempData(MEM_OBJ_X_TEMP_TERM, 0, nMemFileId, &nDownLoadFileEn, sizeof(nDownLoadFileEn));	
							}
						}
					
						//�óɹ���־
						if (0xFFFFFFFF != nCurrID)
						{
							nLastID = 0xEEEEEEEE;
							MemWriteXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_FILE_TRANS_ID, &nLastID, sizeof(nLastID));
							ClearGrpUpdateFlag(0xAA);
						}
						break;
					}
					
	        		//д�뵱ǰ���ļ���N�α�ʶ�����һ�β�д��
	        		MemWriteXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_FILE_TRANS_ID, &nCurrID, sizeof(nCurrID));  	
    			}
        		break;
	        }
        }
    }

    //�����������쳣�˳�
    if (0x55 == nFileEn)
    {
		CloseFileHand(nFileIdx);
		FreeFileSem(nFileSemIdx);
    }

    pData[nDataLen++] = (INT8U)nCurrID;
    pData[nDataLen++] = (INT8U)(nCurrID>>8);
    pData[nDataLen++] = (INT8U)(nCurrID>>16);
    pData[nDataLen++] = (INT8U)(nCurrID>>24);
	return nDataLen;
}
