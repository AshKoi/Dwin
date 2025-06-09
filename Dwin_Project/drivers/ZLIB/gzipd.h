/**************************************************************************************************
* Copyright (c) 2010-2011 ��һ�Ƽ��ɷ����޹�˾�з�����
* �ļ���:
* ������: �з�����
* �ա���: 2010/09/19
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
***************************************************************************************************/
#ifndef _GZIPD_HEADER_
#define _GZIPD_HEADER_

// for data flash
INT32U UpgradeMain( void );
INT32U _API_ZLIB_Init( void );
INT32U _API_ZLIB_Compress( INT8U* dstBuf, INT32U *dstLen, INT8U* srcBuf, INT32U srcLen );
INT32U _API_ZLIB_Uncompress( INT8U* dstBuf, INT32U *dstLen, INT8U* srcBuf, INT32U srcLen );
INT32U _API_ZLIB_CRC32( INT32U *crc, INT8U* buff, INT32U len );

// for nand flash
INT32U UpgradeMainToYAFFS( const INT8U* srcfile );
INT32U UpdateBootFlagToYAFFS( INT8U flag );

#endif
