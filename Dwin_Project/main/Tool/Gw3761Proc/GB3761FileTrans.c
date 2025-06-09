/******************************************************************************************************
* Copyright (c) 2010-2011 光一科技股份有限公司研发中心
* 文件名:
* 创建人: 研发中心应用开发部
* 日　期: 2011/01/24
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
*****************************************************************************************************/
#include "GB3761FileTrans.h"

FTP_INFO         g_sFtpParaInfo;                      //FTP参数
/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/
/********************************************************************************
* 函数名: GB3761FileTrans
* 输　入:
* 输　出:
* 返  回:
* 功  能: 文件传输
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
		if (RET_ERR_OK != GB3761DaDt2PnFn(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nOffset], &nPn[0], &nPnNum, &nFn[0], &nFnNum, &pInfo->bAllMpFlag))  //Dadt转换Pnfn
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
* 函数名: GB3761FileTransProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 文件传输
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
            nDataLen = 0;                               //数据长度清零，每次都清零
            memset(&nData[0], 0, sizeof(nData));        //清空数据内容
			nDataLen += GB3761FileTransFnProc(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
            if (nDataLen > nMaxDataLen || 0 == nDataLen)   //一个Fn项数据长度超过帧长 丢弃
            {
                continue;
            }
            if ((pInfo->nTxDataLen + nDataLen) > nMaxDataLen)//组帧数据超过帧长 发送
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
                pInfo->nTxDataLen = 0;  //重新组帧
            }
            pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nData[0], nDataLen);
		}
	}
	return RET_ERR_OK;
}


/********************************************************************************
* 函数名: GB3761FileTransFnProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 文件传输
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
    switch (nFn) //处理不同Fn项
    {
    	case F1:      //F1 远程下载处理过程
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

//国网远程下载F1处理过程
INT16U GB3761FileTransGWF1(INT16U nPn, INT8U *pSrcData, INT16U nSrcDataLen, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nFileEn     = 0;                                //文件使用标志
    INT8U   nDownLoadFileEn = 0;
	INT8U   i           = 0;
    INT8U   j           = 0;
    INT8U   nFileID     = 0;                                //文件标识
    INT8U   nFileProp   = 0;                                //文件属性
    INT8U   nFileCmd    = 0;                                //文件指令
    INT16U  nMemFileId  = 0;
    INT16U  nFileSemIdx = 0;                                //文件使用信号量下标
    INT16U  nFileIdx    = 0;                                //文件下标
	INT16U  nDataLen    = 0;
    INT16U  nCRC        = 0;
    INT16U  nTotalNum   = 0;                                //文件总段数
    INT16U  nIndex      = 0;
    INT16U  nCurrLen    = 0;                                //当前长度
    INT16U  nLastLen    = 0;
    INT16U  nTmpIdx     = 0;
    INT32U  nCurrID     = 0;                                //当前标识
    INT32U  nUpGradeRet = 0;
    INT32U  nLastID     = 0xFFFFFFFF;                       //上一标识
    INT32U  nSumLen     = 0;
    INT32S  nSeekLen    = 0;
    INT32S  nFhandle    = -1;                                //文件句柄
    FILE_TRANS_INFO     sFileTInfo;
    RMOT_FTP_INFO       sRmotFtpInfo;                       //远程FTP操作请求

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
        if (0x0 == nFileID)								//清除传输文件，恢复到升级前状态
        {
            InitDownFrmInfo();                          //初始化下载帧信息

            nFileID =  sFileTInfo.nFileID;      		//获取前一次下载的文件信息
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
    			nCurrID = 0xFFFFFFFF;           	//获取文件使用权失败返回错误
    			break;
    		}
    		
    		if (RET_ERR_OK != GetFileSem(nFileSemIdx))  //获取文件使用权
    		{
    		    nCurrID = 0xFFFFFFFF;           	//获取文件使用权失败返回错误   	
    		}
    		else
    		{
        		if (RET_ERR_OK != InitUpgradeFile(nFileIdx))
            	{
            		nCurrID = 0xFFFFFFFF;            //初始化文件失败返回错误
            	}
                else
                {
                    memset(&sFileTInfo, 0, sizeof(sFileTInfo)); //初始化前一次下载文件信息
                    MemWriteXSaveData(MEM_OBJ_X_SAVE_FLH_TERM, 0, MEM_SAVE_FILE_TRANS_INFO_ID, &sFileTInfo, sizeof(sFileTInfo));
                }
				CloseFileHand(nFileIdx);
				FreeFileSem(nFileSemIdx);
			}
        	break;
        }
        else								            //非清除传输文件
	    {
	    	if (0x02 == nFileCmd)           			//组地址升级
        	{
            	INT8U nGrpUpdateVld = 0x55;
            	MemWriteXSaveData(MEM_OBJ_X_SAVE_FLH_TERM, 0, MEM_SAVE_GROUP_UPDATE_VLD_ID, &nGrpUpdateVld, sizeof(nGrpUpdateVld));
            	break;
        	}
        	else if (0x01 == nFileCmd)      			//FTP升级
        	{

                if (nFileID != 1)
                {
                    if (RET_ERR_OK != GetMemFileEnId(nFileID, &nMemFileId))
                    {
                        nCurrID = 0xFFFFFFFF; //升级错误回复
                        break;
                    }
                }

                if (RET_ERR_OK != GetFileSemIdx(nFileID, &nFileSemIdx))
		    	{
		    		nCurrID = 0xFFFFFFFF;
		            break;
		    	}

                memset(&sRmotFtpInfo, 0, sizeof(sRmotFtpInfo));

                //通知停止FTP工作
                StopFTPTask(&sRmotFtpInfo);

                //停止相应文件升级工作
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
        		if (RET_ERR_OK != GetFileSem(nFileSemIdx))  //获取文件使用权
        		{
        		    nCurrID = 0xFFFFFFFF;           	//获取文件使用权失败返回错误
                	break;    	
        		}
        		else
        		{
					FreeFileSem(nFileSemIdx);				//解除文件使用权以便FTP进程去下载
			    }
        		
                //FTP参数解析
                memset(&g_sFtpParaInfo, 0, sizeof(g_sFtpParaInfo));
	            AnalysisFtpPara(&g_sFtpParaInfo, &pSrcData[nTmpIdx], nCurrLen);
	            MemWriteXTempData(MEM_OBJ_X_TEMP_TERM, 0, MEM_TEMP_RMOT_FTP_PARA_ID, &g_sFtpParaInfo, sizeof(g_sFtpParaInfo));

	            //启动FTP文件标志/将FTP信息写入交互区
	            memset(&sRmotFtpInfo, 0, sizeof(sRmotFtpInfo));
                MemReadXTempData(MEM_OBJ_X_TEMP_TERM, 0, MEM_TEMP_RMOT_FTP_INFO_ID, &sRmotFtpInfo, sizeof(sRmotFtpInfo));
	            sRmotFtpInfo.nFtpFileId = nFileID;
                sRmotFtpInfo.nFtpEn = 0x55;
                MemWriteXTempData(MEM_OBJ_X_TEMP_TERM, 0, MEM_TEMP_RMOT_FTP_INFO_ID, &sRmotFtpInfo, sizeof(sRmotFtpInfo));
	
	            //等待链路返回结果
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
	                    nCurrID = 0xFFFFFFFE;   //FTP服务器无法登录
	                    break;
	                }
	
	                if (2 == sRmotFtpInfo.nFtpRet)
	                {
	                    nCurrID = 0xFFFFFFFD;   //FTP服务器中找不到指定文件
	                    break;
	                }
	                OSTimeDlyHMSM(0, 0, 1, 0);
	            }
	
	            if (j >=GB3761_FTP_TIMEOUT)
	            {
	            	nCurrID = 0xFFFFFFFF;       //超时返回文件错误
	            }
	            break;
        	}
	        else if (0x00 == nFileCmd)                  //报文升级
	        {
			    //下载文件标识不对
		        if (RET_ERR_OK != GetFileIdx(nFileID, &nFileIdx))
		        {
		            nCurrID = 0xFFFFFFFF;
		            break;
		        }
		
			    //错误条件判断
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
	
	        	//已成功升级
			    MemReadXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_FILE_TRANS_ID, &nLastID, sizeof(nLastID));
		        if ((0x01 == nFileProp) &&  (0xEEEEEEEE == nLastID))
		        {
					break;
		        }
		
		        //第一次下载或下载另外一个文件 需要更新下载文件信息
		        if ((nFileID   != sFileTInfo.nFileID)
		         || (nTotalNum != sFileTInfo.nTotSegNum)
		         || ((nCurrLen != sFileTInfo.nSegLen) && (nTotalNum != (1 + nCurrID))))
		        {
		        	//文件停止使用
					if (RET_ERR_OK != StopFileEn(nFileID))
					{
						nCurrID = 0xFFFFFFFF;
						break;
					}
		        	
				    if (RET_ERR_OK != GetFileSem(nFileSemIdx))  //获取文件使用权
	        		{
	        		    nCurrID = 0xFFFFFFFF;           	//获取文件使用权失败返回错误    	
	        		}
	        		else
	        		{
		        		if (RET_ERR_OK != InitUpgradeFile(nFileIdx))
		            	{
		            		nCurrID = 0xFFFFFFFF;            //初始化文件失败返回错误
		            	}
		            	CloseFileHand(nFileIdx);
						FreeFileSem(nFileSemIdx);
					}
					
					if (0xFFFFFFFF == nCurrID)
					{
						break;
					}
		        	
		        	//初始化下载帧信息
		            InitDownFrmInfo();
		            sFileTInfo.nFileID    = nFileID;
		            sFileTInfo.nTotSegNum = nTotalNum;
		            sFileTInfo.nSegLen    = nCurrLen;
		            MemWriteXSaveData(MEM_OBJ_X_SAVE_FLH_TERM, 0, MEM_SAVE_FILE_TRANS_INFO_ID, &sFileTInfo, sizeof(sFileTInfo));
		            MemWriteXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_FILE_TRANS_LEN, &nCurrLen, sizeof(nCurrLen));
				}
		
	        	if (0 == nCurrID)      						//第一帧报文
        		{
        			if  (0x1 == nFileID)
        			{
	            		nTmpIdx  += GB3761_FILETRANS_HEAD_LEN;	//兼容之前升级方法    dwn前面有16个字节
	            		nCurrLen -= GB3761_FILETRANS_HEAD_LEN;
            		}
            		nSeekLen = 0;
        		}
        		else
        		{
            		MemReadXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_FILE_TRANS_LEN, &nLastLen, sizeof(nLastLen));
            		if (nTotalNum != (1 + nCurrID))
            		{
                		if (nCurrLen != nLastLen)			//帧长度不一致
                		{
                    		nCurrID = 0xFFFFFFFF;  			//升级文件失败
                    		break;
                		}
                        MemWriteXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_FILE_TRANS_LEN, &nCurrLen, sizeof(nCurrLen));
            		}

            		nSumLen =  nTotalNum * nLastLen;
            		if (nSumLen > MAX_SEC_FILE_LEN)
            		{
                		nCurrID = 0xFFFFFFFF;  				//文件太大
                		break;
            		}
            		nSeekLen = nLastLen * nCurrID - GB3761_FILETRANS_HEAD_LEN;
        		}

	        	//文件停止使用
				if (RET_ERR_OK != StopFileEn(nFileID))
				{
					nCurrID = 0xFFFFFFFF;
					break;
				}
	    		//获取文件使用权
    			if (RET_ERR_OK != GetFileSem(nFileSemIdx))
    			{
    		    	nCurrID = 0xFFFFFFFF;           		//获取文件使用权失败返回错误
            		break;    	
    			}
    			else
    			{
    				nFileEn = 0x55;
    				//先关闭文件
    				CloseFileHand(nFileIdx);
    				//打开所要操作的文件
		        	g_nFileHand[nFileIdx] = pFS->FileOpen((INT8U *)&g_nFileName[nFileIdx][0], OPEN_READWRITE);
		        	nFhandle = g_nFileHand[nFileIdx];
		        	if (nFhandle < 0)
		        	{
		        		nCurrID = 0xFFFFFFFF;  					//打开文件失败
		        		break;
		        	}
		    		
		    		//将下载报文数据写入文件    	
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
	        		//结束帧处理
					if (0x01 == nFileProp)
					{
						if (1 == nFileID) //终端升级文件
						{
							INT8U nTmpBuf[30];
							memset(&nTmpBuf[0], 0, sizeof(nTmpBuf));
							memcpy(&nTmpBuf[0], "/usr/", 5);
							memcpy(&nTmpBuf[5], &g_nFileName[nFileIdx], 20);
							nUpGradeRet = pUpgrade->NandUpgrade(&nTmpBuf[0]);
							if (ERROR(nUpGradeRet))
							{
				    			nCurrID = 0xFFFFFFFF; //升级错误回复
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
							//将启动标志写入对应文件
							INT8U  nDownLoadFileEn = 0x55;
				
							if (RET_ERR_OK != GetMemFileEnId(nFileID, &nMemFileId))
							{
				    			nCurrID = 0xFFFFFFFF; //升级错误回复
							}
							else
							{
								MemWriteXTempData(MEM_OBJ_X_TEMP_TERM, 0, nMemFileId, &nDownLoadFileEn, sizeof(nDownLoadFileEn));	
							}
						}
					
						//置成功标志
						if (0xFFFFFFFF != nCurrID)
						{
							nLastID = 0xEEEEEEEE;
							MemWriteXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_FILE_TRANS_ID, &nLastID, sizeof(nLastID));
							ClearGrpUpdateFlag(0xAA);
						}
						break;
					}
					
	        		//写入当前的文件第N段标识，最后一段不写！
	        		MemWriteXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_FILE_TRANS_ID, &nCurrID, sizeof(nCurrID));  	
    			}
        		break;
	        }
        }
    }

    //升级过程中异常退出
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
