#include "Receiver.h"

#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <iostream>
#include <netdb.h>
#include <cstring>



/**
 * Receives a packet on the specified socket and constructs a PacketData object with relevant information.
 * 
 * @param socketfd The file descriptor of the socket to receive the packet.
 * @return A PacketData object containing information about the received packet.
 * @throws std::runtime_error if an error occurs during the receive operation.
 */
PacketData Receiver::receivePacket(int socketfd) {
    // IP Header

    struct iphdr ip_response_header{};

    // Receive a packet
    ssize_t data_length_byte = recv(socketfd, &ip_response_header, sizeof(ip_response_header), 0);
    if (data_length_byte < 0) {
        throw std::runtime_error(std::string("recv error: ") + strerror(errno) + " \n");
    }

    // Get the time of receipt
    time_point receiptTime = std::chrono::high_resolution_clock::now();

    // Retrieve IP address
    struct sockaddr_in src_addr{};
    src_addr.sin_addr.s_addr = ip_response_header.saddr;
    char* ip_address = inet_ntoa(src_addr.sin_addr);

    PacketData packet(ip_address, "", receiptTime);

    return packet;
}