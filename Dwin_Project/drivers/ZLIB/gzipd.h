/**************************************************************************************************
* Copyright (c) 2010-2011 光一科技股份有限公司研发中心
* 文件名:
* 创建人: 研发中心
* 日　期: 2010/09/19
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
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
