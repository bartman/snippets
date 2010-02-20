#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <unistd.h>

#include "i.h"

int
main (void)
{
        IM_PACKET_HOOK_INFO info;

        printf ("%08x (%u)  %s\n",
                        offsetof (IM_PACKET_HOOK_INFO,ulCurrentPacketSize), 
                        sizeof (info.ulCurrentPacketSize),
                        "ulCurrentPacketSize");
        printf ("%08x (%u)  %s\n",
                        offsetof (IM_PACKET_HOOK_INFO,bReadOnly), 
                        sizeof (info.bReadOnly),
                        "bReadOnly");
        printf ("%08x (%u)  %s\n",
                        offsetof (IM_PACKET_HOOK_INFO,bSyncProcess), 
                        sizeof (info.bSyncProcess),
                        "bSyncProcess");
        printf ("%08x (%u)  %s\n",
                        offsetof (IM_PACKET_HOOK_INFO,ulNbBuffer), 
                        sizeof (info.ulNbBuffer),
                        "ulNbBuffer");
        printf ("%08x (%u)  %s\n",
                        offsetof (IM_PACKET_HOOK_INFO,usPacketEtherType), 
                        sizeof (info.usPacketEtherType),
                        "usPacketEtherType");
        printf ("%08x (%u)  %s\n",
                        offsetof (IM_PACKET_HOOK_INFO,FirstNdisBuffer), 
                        sizeof (info.FirstNdisBuffer),
                        "FirstNdisBuffer");
        printf ("%08x (%u)  %s\n",
                        offsetof (IM_PACKET_HOOK_INFO,EthHeaderBuffer), 
                        sizeof (info.EthHeaderBuffer),
                        "EthHeaderBuffer");
        printf ("%08x (%u)  %s\n",
                        offsetof (IM_PACKET_HOOK_INFO,EthHeaderBufferSize), 
                        sizeof (info.EthHeaderBufferSize),
                        "EthHeaderBufferSize");
        printf ("%08x (%u)  %s\n",
                        offsetof (IM_PACKET_HOOK_INFO,IPHeaderBuffer), 
                        sizeof (info.IPHeaderBuffer),
                        "IPHeaderBuffer");
        printf ("%08x (%u)  %s\n",
                        offsetof (IM_PACKET_HOOK_INFO,IPHeaderBufferSize), 
                        sizeof (info.IPHeaderBufferSize),
                        "IPHeaderBufferSize");

        return 0;
}
