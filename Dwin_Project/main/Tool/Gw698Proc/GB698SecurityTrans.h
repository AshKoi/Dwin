#ifndef _GB698_SET_REQUEST_H_
#define _GB698_SET_REQUEST_H_

#include "Gw698.h"

/**************************************************************************************************
                                          相关结构体声明
***************************************************************************************************/
typedef   struct   _tagTERM698COMPRM
{
    FORMAT08    nStrList;
    FORMAT08   nWorkMode;
    FORMAT08   nOnlineWay;
    FORMAT08   nConnectWay;
    FORMAT08   nConnectMode;
    FORMAT08   nListenPortnum;
    INT8U   nListenPort[15];
    INT8U   nApn[18];
    INT8U   nUser[18];
    INT8U   nPsd[18];
    INT8U   nAgentServerAddr[6];
    INT8U   nAgentPort[3];
    union
    {
        INT16U   nWord;
        struct
        {
            INT8U   nNone;          //仅仅为了冲掉第一个字节空间
            INT8U   nReplyTimes:2;
            INT8U   nTimeout:6;
        }nCon;
    }uCon;
    INT8U   nBeatCycS[3];
}TERM698PRM,*pTERM698PRM;

/**************************************************************************************************
                                          相关函数声明
***************************************************************************************************/
INT8S GB3761SecurityTrans(pAPP_COMM pComm);

#endif