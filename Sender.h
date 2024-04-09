#ifndef _SENDER_H_
#define _SENDER_H_

#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdlib.h>
#include <unistd.h>
#include "Packet.h"

#define ECHO_REQUEST 8

class Sender {
private:
    int ppid;    
    sockaddr_in  in_address;
public:
    Sender(int ppid, const char* ip_address);
    void sendPacket(int socketfd, int ttl, uint16_t sequence);
    icmphdr createPacketHeader(uint16_t sequence);
    uint16_t checkSumma(const void *packet, int length);
};

#endif