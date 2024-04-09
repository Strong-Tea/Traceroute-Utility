#ifndef _PACKET_H_
#define _PACKET_H_


#include <chrono>
#include <string>


typedef std::chrono::high_resolution_clock::time_point time_point;

typedef struct packet_data {
    std::string ipAddress;
    std::string hostName;
    time_point receiptTime;

    packet_data(std::string ipAddress, std::string hostName, time_point receipt_time) :
            ipAddress(ipAddress), hostName(hostName), receiptTime(receipt_time) {}

} PacketData;


#endif