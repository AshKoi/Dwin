#include "socketapi.h"

//以太网初始化函数
BOOLEAN Init(ENET_PARAM* pEnetParam)
{
    return EnetInit(pEnetParam);
}