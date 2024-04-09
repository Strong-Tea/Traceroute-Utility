#ifndef _TRACEROUTE_H_
#define _TRACEROUTE_H_



#define MAX_ICMP_PACKETS_PER_TTL 3

#include <vector>
#include <chrono>
#include "Packet.h"


typedef std::chrono::high_resolution_clock::time_point time_point;



class Traceroute {
private:
    int socketfd; 

    void printRoute(std::vector<PacketData>& packets, int ttl, time_point sendingTime);

public:

    Traceroute();
    ~Traceroute();

    void trace(const char* ip_address, int max_hops, int response_timeout);
};


#endif 