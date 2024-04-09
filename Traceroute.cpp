#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdlib.h>
#include <unistd.h>
#include <set>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include "Traceroute.h"
#include "Sender.h"
#include "Receiver.h"


Traceroute::Traceroute() {
    if ((socketfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        throw std::runtime_error(std::string("socket error: ") + strerror(errno) + ". Try to use 'sudo' \n");
}


Traceroute::~Traceroute() {
    close(socketfd);
}


/**
 * Traces the route to the specified IP address using ICMP packets.
 * 
 * @param ip_address The IP address of the destination to trace.
 */
void Traceroute::trace(const char* ip_address, int max_hops, int response_timeout) {
    bool destination = false;
    int sequence = 1;
    int ppid = getppid();

    Sender sender(ppid, ip_address);
    Receiver receiver;   
    std::vector<PacketData> packets;

    for (int ttl = 1; ttl <= max_hops && !destination; ttl++) {

        // Send packets
        for (int sent_packet = 0; sent_packet < MAX_ICMP_PACKETS_PER_TTL; sent_packet++) {
            sender.sendPacket(socketfd, ttl, sequence++);
        }

        // Get the time of sending
        auto sendTime = std::chrono::high_resolution_clock::now();

        // Setting the time to wait for the package to be received
        struct timeval tv = {};
        tv.tv_sec = response_timeout;
        tv.tv_usec = 0;

        fd_set descriptors;
        FD_ZERO(&descriptors);
        FD_SET(socketfd, &descriptors);

        while (packets.size() < MAX_ICMP_PACKETS_PER_TTL) {

            // Wait for data to be available on the socket with a timeout 'tv'
            int ready = select(socketfd + 1, &descriptors, nullptr, nullptr, &tv);

            if (ready == 0)
                break;

            // Receive a packet
            PacketData packet = receiver.receivePacket(socketfd);    

            if (!packet.ipAddress.empty()) {
                packets.push_back(packet);
                if (packet.ipAddress == ip_address)
                    destination = true;
            }
        }

        printRoute(packets, ttl, sendTime);
        packets.clear();
    }
}



/**
 * Prints the route for a given TTL value based on the received packets.
 * 
 * @param packets A vector containing PacketData objects representing the received packets.
 * @param ttl The Time-to-Live (TTL) value associated with the printed route.
 * @param sendingTime The time when the packets were sent.
 */
void Traceroute::printRoute(std::vector<PacketData>& packets, int ttl, time_point sendingTime) {

    if (packets.empty()) {
        std::cout<<"***\n";
        return;
    }

    std::set<std::string> ip;

    std::cout<< ttl << ". ";

    for (PacketData packet : packets) {
        ip.insert(packet.ipAddress);
    }

    for (std::string i : ip) {
        std::cout<<i<<" ";
    }

    u_int32_t sum_time = 0;
    for (auto packet : packets)
        sum_time += std::chrono::duration_cast<std::chrono::milliseconds>(packet.receiptTime - sendingTime).count();

    std::cout << sum_time / MAX_ICMP_PACKETS_PER_TTL << "ms" << std::endl;
}