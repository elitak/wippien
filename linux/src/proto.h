
//============================================================
// MAC address, Ethernet header, and ARP
//============================================================

#pragma pack(1)

#define IP_HEADER_SIZE 20

typedef unsigned char MACADDR [6];
typedef unsigned long IPADDR;

//-----------------
// Ethernet address
//-----------------

typedef struct {
  MACADDR addr;
} ETH_ADDR;

//----------------
// Ethernet header
//----------------

typedef struct
{
  MACADDR dest;               /* destination eth addr	*/
  MACADDR src;                /* source ether addr	*/

# define ETH_P_IP   0x0800    /* IPv4 protocol */
# define ETH_P_ARP  0x0806    /* ARP protocol */
  unsigned short proto;               /* packet type ID field	*/
} ETH_HEADER, *PETH_HEADER;

//----------------
// ARP packet
//----------------

typedef struct
   {
    MACADDR        m_MAC_Destination;        // Reverse these two
    MACADDR        m_MAC_Source;             // to answer ARP requests
    unsigned short         m_Proto;                  // 0x0806

#   define MAC_ADDR_TYPE 0x0001
    unsigned short         m_MAC_AddressType;        // 0x0001

    unsigned short         m_PROTO_AddressType;      // 0x0800
    unsigned char          m_MAC_AddressSize;        // 0x06
    unsigned char          m_PROTO_AddressSize;      // 0x04

#   define ARP_REQUEST 0x0001
#   define ARP_REPLY   0x0002
    unsigned short         m_ARP_Operation;          // 0x0001 for ARP request, 0x0002 for ARP reply

    MACADDR        m_ARP_MAC_Source;
    IPADDR         m_ARP_IP_Source;
    MACADDR        m_ARP_MAC_Destination;
    IPADDR         m_ARP_IP_Destination;
   }
ARP_PACKET, *PARP_PACKET;

//----------
// IP Header
//----------

typedef struct {
# define IPH_GET_VER(v) (((v) >> 4) & 0x0F)
# define IPH_GET_LEN(v) (((v) & 0x0F) << 2)
  unsigned char    version_len;

  unsigned char    tos;
  unsigned short   tot_len;
  unsigned short   id;

# define IP_OFFMASK 0x1fff
  unsigned short   frag_off;

  unsigned char    ttl;

  unsigned char    protocol;

  unsigned short   check;
  unsigned long    saddr;
  unsigned long    daddr;
  /* The options start here. */
} IPHDR;

//-----------
// UDP header
//-----------

typedef struct {
  unsigned short   source;
  unsigned short   dest;
  unsigned short   len;
  unsigned short   check;
} UDPHDR;

//--------------------------
// TCP header, per RFC 793.
//--------------------------

typedef struct {
  unsigned short      source;    /* source port */
  unsigned short      dest;      /* destination port */
  unsigned long       seq;       /* sequence number */
  unsigned long       ack_seq;   /* acknowledgement number */

# define TCPH_GET_DOFF(d) (((d) & 0xF0) >> 2)
  unsigned char       doff_res;

# define TCPH_FIN_MASK (1<<0)
# define TCPH_SYN_MASK (1<<1)
# define TCPH_RST_MASK (1<<2)
# define TCPH_PSH_MASK (1<<3)
# define TCPH_ACK_MASK (1<<4)
# define TCPH_URG_MASK (1<<5)
# define TCPH_ECE_MASK (1<<6)
# define TCPH_CWR_MASK (1<<7)
  unsigned char flags;

  unsigned short      window;
  unsigned short      check;
  unsigned short      urg_ptr;
} TCPHDR;

#define	TCPOPT_EOL     0
#define	TCPOPT_NOP     1
#define	TCPOPT_MAXSEG  2
#define TCPOLEN_MAXSEG 4

#pragma pack()
