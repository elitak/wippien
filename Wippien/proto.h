#ifndef __PROTO_H
#define __PROTO_H

//============================================================
// MAC address, Ethernet header, and ARP
//============================================================

#pragma pack(1)

extern unsigned char MAC_BROADCAST[6];

#define IP_HEADER_SIZE 20

typedef unsigned char MACADDR [6];
typedef unsigned long IPADDR;

//-----------------
// Ethernet address
//-----------------

typedef struct {
  MACADDR addr;
} ETH_ADDR;

typedef struct {
//  ETH_ADDR list[NIC_MAX_MCAST_LIST];
	ETH_ADDR list[32];
} MC_LIST;

//----------------
// Ethernet header
//----------------

typedef struct
{
  MACADDR dest;               /* destination eth addr	*/
  MACADDR src;                /* source ether addr	*/

# define ETH_P_IP   0x0800    /* IPv4 protocol */
# define ETH_P_ARP  0x0806    /* ARP protocol */
  USHORT proto;               /* packet type ID field	*/
} ETH_HEADER, *PETH_HEADER;

//----------------
// ARP packet
//----------------

typedef struct
   {
    MACADDR        m_MAC_Destination;        // Reverse these two
    MACADDR        m_MAC_Source;             // to answer ARP requests
    USHORT         m_Proto;                  // 0x0806

#   define MAC_ADDR_TYPE 0x0001
    USHORT         m_MAC_AddressType;        // 0x0001

    USHORT         m_PROTO_AddressType;      // 0x0800
    UCHAR          m_MAC_AddressSize;        // 0x06
    UCHAR          m_PROTO_AddressSize;      // 0x04

#   define ARP_REQUEST 0x0001
#   define ARP_REPLY   0x0002
    USHORT         m_ARP_Operation;          // 0x0001 for ARP request, 0x0002 for ARP reply

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
  UCHAR    version_len;

  UCHAR    tos;
  USHORT   tot_len;
  USHORT   id;

# define IP_OFFMASK 0x1fff
  USHORT   frag_off;

  UCHAR    ttl;

# define IPPROTO_UDP  17  /* UDP protocol */
# define IPPROTO_TCP   6  /* TCP protocol */
# define IPPROTO_ICMP  1  /* ICMP protocol */
# define IPPROTO_IGMP  2  /* IGMP protocol */
  UCHAR    protocol;

  USHORT   check;
  ULONG    saddr;
  ULONG    daddr;
  /* The options start here. */
} IPHDR;

// -----------
// ICMP header
// -----------
struct ICMPHDR
{
  UCHAR type;                /* message type */
  USHORT code;                /* type sub-code */
  ULONG checksum;
  union
  {
    struct
    {
      USHORT id;
      USHORT sequence;
    } echo;                     /* echo datagram */
    ULONG   gateway;        /* gateway address */
    struct
    {
      USHORT __unused;
      USHORT mtu;
    } frag;                     /* path mtu discovery */
  } un;
};

//-----------
// UDP header
//-----------

typedef struct {
  USHORT   source;
  USHORT   dest;
  USHORT   len;
  USHORT   check;
} UDPHDR;

//--------------------------
// TCP header, per RFC 793.
//--------------------------

typedef struct {
  USHORT      source;    /* source port */
  USHORT      dest;      /* destination port */
  ULONG       seq;       /* sequence number */
  ULONG       ack_seq;   /* acknowledgement number */

# define TCPH_GET_DOFF(d) (((d) & 0xF0) >> 2)
  UCHAR       doff_res;

# define TCPH_FIN_MASK (1<<0)
# define TCPH_SYN_MASK (1<<1)
# define TCPH_RST_MASK (1<<2)
# define TCPH_PSH_MASK (1<<3)
# define TCPH_ACK_MASK (1<<4)
# define TCPH_URG_MASK (1<<5)
# define TCPH_ECE_MASK (1<<6)
# define TCPH_CWR_MASK (1<<7)
  UCHAR       flags;

  USHORT      window;
  USHORT      check;
  USHORT      urg_ptr;
} TCPHDR;

typedef struct {
	ULONG saddr;
	ULONG daddr;
	UCHAR zero;
	UCHAR protocol;
	USHORT length;
} TCPHDRpseudo;


#define	TCPOPT_EOL     0
#define	TCPOPT_NOP     1
#define	TCPOPT_MAXSEG  2
#define TCPOLEN_MAXSEG 4


#define COPY_MAC(dest, src) MoveMemory ((dest), (src), sizeof (MACADDR))
#define CLEAR_MAC(dest)     ZeroMemory ((dest), sizeof (MACADDR))
#define MAC_EQUAL(a,b)      (memcmp ((a), (b), sizeof (MACADDR)) == 0)

#pragma pack()




#endif