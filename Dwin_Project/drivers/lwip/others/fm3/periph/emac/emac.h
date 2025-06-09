#ifndef EMAC_H
#define EMAC_H

//-----------------------------------------------------------------------------
/// \dir
/// !Purpose
///
///     Definition of methods and structures for using EMAC
///
/// !Usage
///
/// -# Initialize EMAC with EMAC_Init with MAC address.
/// -# Then the caller application need to initialize the PHY driver before further calling EMAC
///      driver.
/// -# Get a packet from network
///      -# Interrupt mode: EMAC_Set_RxCb to register a function to process the frame packet
///      -# Polling mode: EMAC_Poll for a data packet from network
/// -# Send a packet to network with EMAC_Send.
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------
 // The MAC can support frame lengths up to 1536 bytes.
#include <board.h>
#include "lwip/others/fm3/periph/pio/pio.h"
#include "config.h"
#include "lwipopts.h"
#include "pbuf.h"
#include "mb9bf218s.h"
#include "ethernetif.h"
#include "emac_fm3.h"
#if __ICCARM__
#define __attribute__(...)
#define __align(...)
#endif

#define EMAC_RCV_OFFSET     		0
#define EMAC_PHY_ADDR               31

/*
********************************************************************************************
*             MACRO
********************************************************************************************
*/
#define EMAC_NB_RX_BUF			4							/*接收缓冲区的个数*/
#define EMAC_RX_BUF_SIZE		512L						/*每个接收缓冲区的大小*/
#define EMAC_RXBUF_TOTALSIZE    2048L                       /* 缓冲区的总长,EMAC_NB_RX_BUF*EMAC_RX_BUF_SIZE */


#define EMAC_NB_TX_BUF			2							/*发送缓冲区的个数 */
#define EMAC_TX_BUF_SIZE		1500L                       /*每个发送缓冲区的大小,注意,每个缓冲区的大小都必须大于数据帧的长度*/
#define EMAC_FRAME_LENTGH_MAX       1536

#define EMAC_CAF_DISABLE  0
#define EMAC_CAF_ENABLE   1
#define EMAC_NBC_DISABLE  0
#define EMAC_NBC_ENABLE   1


/// Return for EMAC_Send function
#define EMAC_TX_OK                     0
#define EMAC_TX_BUFFER_BUSY            1
#define EMAC_TX_INVALID_PACKET         2
/// Return for EMAC_Poll function
#define EMAC_RX_OK                   0
#define EMAC_RX_NO_DATA              1
#define EMAC_RX_FRAME_SIZE_TOO_SMALL 2
//-----------------------------------------------------------------------------
// Circular buffer management
//-----------------------------------------------------------------------------
// Return count in buffer
#define CIRC_CNT(head,tail,size) (((head) - (tail)) & ((size)-1))

// Return space available, 0..size-1
// We always leave one free char as a completely full buffer
// has head == tail, which is the same as empty
#define CIRC_SPACE(head,tail,size) CIRC_CNT((tail),((head)+1),(size))

// Return count up to the end of the buffer.
// Carefully avoid accessing head and tail more than once,
// so they can change underneath us without returning inconsistent results
#define CIRC_CNT_TO_END(head,tail,size) \
   ({int end = (size) - (tail); \
     int n = ((head) + end) & ((size)-1); \
     n < end ? n : end;})

// Return space available up to the end of the buffer
#define CIRC_SPACE_TO_END(head,tail,size) \
   ({int end = (size) - 1 - (head); \
     int n = (end + (tail)) & ((size)-1); \
     n <= end ? n : end+1;})

// Increment head or tail
#define CIRC_INC(headortail,size) \
        headortail++;             \
        if(headortail >= size) {  \
            headortail = 0;       \
        }

#define CIRC_EMPTY(circ)     ((circ)->head == (circ)->tail)
#define CIRC_CLEAR(circ)     ((circ)->head = (circ)->tail = 0)

 /*
**********************************************************************
*							 结构体定义区域
**********************************************************************
*/
//-----------------------------------------------------------------------------
//         EMAC Exported functions
//-----------------------------------------------------------------------------
/// Callback used by send function
typedef void (*EMAC_TxCallback)(unsigned int status);
typedef void (*EMAC_RxCallback)(unsigned int status);


/// The DM9161 instance
typedef struct _Dm9161 {

    /// The retry & timeout settings
    unsigned int retryMax;

    /// PHY address ( pre-defined by pins on reset )
    unsigned char phyAddress;

} Dm9161, *pDm9161;	


typedef struct _EmacStats {

    // TX errors
    unsigned int tx_packets;    /// Total Number of packets sent
    unsigned int tx_comp;       /// Packet complete
    unsigned int tx_errors;     /// TX errors ( Retry Limit Exceed )
    unsigned int collisions;    /// Collision
    unsigned int tx_exausts;    /// Buffer exhausted
    unsigned int tx_underruns;  /// Under Run, not able to read from memory
    // RX errors
    unsigned int rx_packets;    /// Total Number of packets RX
    unsigned int rx_eof;        /// No EOF error
    unsigned int rx_ovrs;       /// Over Run, not able to store to memory
    unsigned int rx_bnas;       /// Buffer is not available

} EmacStats, *PEmacStats;


/// IP Header structure
__packed
typedef struct _IPheader {
    unsigned char       ip_hl_v;    /// header length and version
    unsigned char       ip_tos;     /// type of service
    unsigned short      ip_len;     /// total length
    unsigned short      ip_id;      /// identification
    unsigned short      ip_off;     /// fragment offset field
    unsigned char       ip_ttl;     /// time to live
    unsigned char       ip_p;       /// protocol
    unsigned short      ip_sum;     /// checksum
    unsigned char       ip_src[4];  /// Source IP address
    unsigned char       ip_dst[4];  /// Destination IP address
    unsigned short      udp_src;    /// UDP source port
    unsigned short      udp_dst;    /// UDP destination port
    unsigned short      udp_len;    /// Length of UDP packet
    unsigned short      udp_xsum;   /// Checksum
} __attribute__ ((packed)) IpHeader, *PIpHeader;    // GCC
/// Ethernet header structure
__packed
typedef struct _EthHdr
{
    unsigned char       et_dest[6]; /// Destination node
    unsigned char       et_src[6];  /// Source node
    unsigned short      et_protlen; /// Protocol or length
} __attribute__ ((packed)) EthHeader, *PEthHeader;  // GCC
/// ARP header structure
__packed
typedef struct _ArpHdr
{
    unsigned short      ar_hrd;     /// Format of hardware address
    unsigned short      ar_pro;     /// Format of protocol address
    unsigned char       ar_hln;     /// Length of hardware address
    unsigned char       ar_pln;     /// Length of protocol address
    unsigned short      ar_op;      /// Operation
    unsigned char       ar_sha[6];  /// Sender hardware address
    unsigned char       ar_spa[4];  /// Sender protocol address
    unsigned char       ar_tha[6];  /// Target hardware address
    unsigned char       ar_tpa[4];  /// Target protocol address
} __attribute__ ((packed)) ArpHeader, *PArpHeader;  // GCC
/// ICMP echo header structure
__packed
typedef struct _IcmpEchoHdr {
    unsigned char       type;       /// type of message
    unsigned char       code;       /// type subcode
    unsigned short      cksum;      /// ones complement cksum of struct
    unsigned short      id;         /// identifier
    unsigned short      seq;        /// sequence number
} __attribute__ ((packed)) IcmpEchoHeader, *PIcmpEchoHeader;    // GCC
typedef void (*EMAC_WakeupCallback)(void);

#ifdef __ICCARM__          // IAR
#pragma pack(4)            // IAR
#endif                     // IAR
/// Describes the type and attribute of Receive Transfer descriptor.
typedef struct _EmacRxTDescriptor {
    unsigned int addr;
    unsigned int status;
} __attribute__((packed, aligned(8))) EmacRxTDescriptor, *PEmacRxTDescriptor;

/// Describes the type and attribute of Transmit Transfer descriptor.
typedef struct _EmacTxTDescriptor {
    unsigned int addr;
    unsigned int status;
} __attribute__((packed, aligned(8))) EmacTxTDescriptor, *PEmacTxTDescriptor;
#ifdef __ICCARM__          // IAR
#pragma pack()             // IAR
#endif                     // IAR

typedef struct _EmacRxBuffer
{
	unsigned int  status;
	unsigned int  strlen;
	unsigned char Buffer[EMAC_RXBUF_TOTALSIZE];
} RxBuffer;
typedef struct _Emac_RxBuffer1
{
	unsigned int  status;
	RxBuffer  Buffer1;
	RxBuffer  Buffer2;
} Rx_Buffer1;

#define EMAC_PHY_AUTO		0
#define EMAC_PHY_10MBIT		1
#define EMAC_PHY_100MBIT	2

#define MAX_ADDR_LEN 		6


#define    PHY0_RESET_LOW()     bFM3_GPIO_PDOR0_P2 = 0
#define    PHY0_RESET_HIGH()    bFM3_GPIO_PDOR0_P2 = 1

extern   volatile EmacStats EmacStatistics;
typedef struct fm3_emac
{
    /* inherit from Ethernet device */
    struct eth_device parent;

    uint8_t phy_mode;

    /* interface address info. */
    uint8_t  dev_addr[MAX_ADDR_LEN];		/* hw address	*/

    FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC;
    IRQn_Type ETHER_MAC_IRQ;

    __align(4)
    EMAC_DMADESCTypeDef  DMARxDscrTab[EMAC_RXBUFNB], DMATxDscrTab[EMAC_TXBUFNB];
    __align(4)
    uint8_t Rx_Buff[EMAC_RXBUFNB][EMAC_MAX_PACKET_SIZE];
    __align(4)
    uint8_t Tx_Buff[EMAC_TXBUFNB][EMAC_MAX_PACKET_SIZE];

    EMAC_DMADESCTypeDef  *DMATxDescToSet;
    EMAC_DMADESCTypeDef  *DMARxDescToGet;
    //struct rt_semaphore tx_buf_free;
}FM3_EAC_TypeDef;
//-----------------------------------------------------------------------------
/// Describes the statistics of the EMAC.
//-----------------------------------------------------------------------------
extern RxBuffer  Emac_RxBuffer	;

//extern   volatile RxTd rxTd;
//extern   volatile TxTd txTd;
extern struct fm3_emac fm3_emac_device0;
extern struct netif netif0;
//static unsigned char MacAddress[6] = {0xBC, 0x20, 0x06, 0x09, 0x30, 0x11};	

extern void EMAC_EnableMII( void );
extern void EMAC_EnableRMII( void );
int32_t eth_emac_tx( struct fm3_emac *dev, struct pbuf *p);
struct pbuf * eth_emac_rx(struct fm3_emac * dev);
void EMAC_Init( unsigned char id,  unsigned char *pMacAddress,
                unsigned char enableCAF, unsigned char enableNBC );
int Emac_Entry( unsigned char* phwaddr );
static void EMAC_IRQHandler(FM3_EAC_TypeDef * fm3_emac_device);
extern unsigned char EMAC_Poll(unsigned char *pFrame,unsigned int frameSize,unsigned int *pRcvSize);

static int32_t eth_emac_init(FM3_EAC_TypeDef *dev);
//void test_ethernet_driver(void);
uint32_t EMAC_PHY_write(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC, uint16_t PHYAddress, uint16_t PHYReg, uint16_t PHYValue);
uint16_t EMAC_PHY_read(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC, uint16_t PHYAddress, uint16_t PHYReg);
extern unsigned char EMAC_ReadPhy(unsigned char PhyAddress,unsigned char Address,
                                  unsigned int *pValue,    unsigned int retry);

extern unsigned char EMAC_WritePhy(unsigned char PhyAddress,unsigned char Address,
                                   unsigned int Value,       unsigned int retry);

//-----------------------------------------------------------------------------
//         函数声明区域
//-----------------------------------------------------------------------------
/*extern unsigned char EMAC_SetMdcClock( unsigned int mck );
extern void EMAC_EnableMdio( void );
extern void EMAC_DisableMdio( void );

extern unsigned char EMAC_ReadPhy(unsigned char PhyAddress,unsigned char Address,
                                  unsigned int *pValue,    unsigned int retry);

extern unsigned char EMAC_WritePhy(unsigned char PhyAddress,unsigned char Address,
                                   unsigned int Value,       unsigned int retry);
extern void EMAC_SetLinkSpeed(unsigned char speed,unsigned char fullduplex);
extern int Emac_Entry( unsigned char* phwaddr );
extern int Emac_TransmitPacket(unsigned char *pData, unsigned int length, unsigned char end_flag);
extern void EMAC_Init( unsigned char id, unsigned char *pMacAddress,
					   unsigned char enableCAF, unsigned char enableNBC );
extern void EMAC_GetStatistics(EmacStats *pStats, unsigned char reset);
extern void EMAC_Handler(void);
extern unsigned char EMAC_Send(void *pBuffer, unsigned int size,
                               EMAC_TxCallback fEMAC_TxCallback);
extern unsigned char EMAC_Poll(unsigned char *pFrame,unsigned int frameSize,unsigned int *pRcvSize);*/

#endif
/* #ifndef EMAC_H  */
