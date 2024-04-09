#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include "Packet.h"


class Receiver {
private:
    struct icmp *getMyIcmpHeader(u_int8_t *icmpPacket) const;
public:
    PacketData receivePacket(int socketfd);
};


#endif