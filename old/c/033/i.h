#ifndef __i_h__
#define __i_h__

#define PACKED

#ifdef __cplusplus      
typedef bool __attribute__((mode(word))) BOOLEAN;   
#ifndef TRUE
#   define TRUE true
#endif
#ifndef FALSE 
#   define FALSE false
#endif
#else
typedef enum{FALSE,TRUE} BOOLEAN;
#endif

#define ETHER_ADDR_LEN      6

typedef uint8_t         BYTE;
typedef uint16_t        WORD;
typedef uint32_t        DWORD;
typedef BOOLEAN         BOOL;
typedef unsigned short  USHORT;
typedef void            VOID;
typedef VOID            *PVOID;

typedef struct  _ETHeader {
        BYTE    ether_dhost[ETHER_ADDR_LEN];
        BYTE    ether_shost[ETHER_ADDR_LEN];
        WORD    ether_type;
}ETHeader;

typedef struct _IPHeader {
    BYTE     iph_verlen;     // Version and length 
    BYTE     iph_tos;        // Type of service 
    WORD     iph_length;     // Total datagram length 
    WORD     iph_id;         // Identification 
    WORD     iph_offset;     // Flags, fragment offset 
    BYTE     iph_ttl;        // Time to live 
    BYTE     iph_protocol;   // Protocol 
    WORD     iph_xsum;       // Header checksum 
    DWORD    iph_src;        // Source address 
    DWORD    iph_dest;       // Destination address 
} IPHeader; 

struct PACKED _IM_PACKET_HOOK_INFO {
    DWORD        ulCurrentPacketSize;
    BOOL         bReadOnly;
    BOOL         bSyncProcess;
    DWORD        ulNbBuffer;
    USHORT       usPacketEtherType;
    PVOID        FirstNdisBuffer;
    ETHeader*    EthHeaderBuffer;
    DWORD        EthHeaderBufferSize;
    IPHeader*    IPHeaderBuffer;
    DWORD        IPHeaderBufferSize;
};

typedef struct _IM_PACKET_HOOK_INFO IM_PACKET_HOOK_INFO,
        *PIM_PACKET_HOOK_INFO;

#endif
