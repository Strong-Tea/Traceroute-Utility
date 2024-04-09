#include "Sender.h"
#include <stdexcept>
#include <cstring>



Sender::Sender(int ppid, const char* ip_address) {
    this->ppid = ppid;

    in_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip_address, &in_address.sin_addr);
    in_address.sin_port = 0;
}


/**
 * Sends an ICMP packet with specified TTL and sequence number through the given socket.
 * 
 * @param socketfd The file descriptor of the socket to use for sending.
 * @param ttl The Time-to-Live (TTL) value to set for the packet.
 * @param sequence The sequence number to include in the packet header.
 * @throws std::runtime_error if an error occurs while setting TTL or sending the packet.
 */
void Sender::sendPacket(int socketfd, int ttl, uint16_t sequence) {

    icmphdr icmp_packet = createPacketHeader(sequence);

    // Set the Time-to-Live (TTL) value for the socket using setsockopt.
    if (setsockopt(socketfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
        throw std::runtime_error(std::string("setsockopt error: ") + strerror(errno) + " \n");
    }

    // Send the ICMP packet (icmp_packet) through the socket (socketfd) using sendto.
    // The packet is sent to the address specified by 'in_address' with a size of 'sizeof(in_address)'.
    // The number of bytes sent is stored in 'bytesSent'.
    ssize_t bytesSent = sendto(socketfd, &icmp_packet, sizeof(icmp_packet), 0, (const struct sockaddr*)&in_address, sizeof(in_address));
    if (bytesSent < static_cast<ssize_t>(sizeof(icmp_packet)))
        throw std::runtime_error(std::string("send error: ") + strerror(errno) + " \n");
}



/**
 * Creates an ICMP packet header with the specified sequence number.
 * 
 * @param sequence The sequence number to include in the packet header.
 * @return The ICMP packet header with the specified sequence number.
 */
icmphdr Sender::createPacketHeader(uint16_t sequence) {
    icmphdr icmp = {};
    icmp.type = ECHO_REQUEST;
    icmp.code = 0;
    icmp.checksum = 0;
    icmp.un.echo.id = ppid;
    icmp.un.echo.sequence = sequence;
    icmp.checksum = checkSumma(&icmp, sizeof(icmp));
    return icmp;
}



/**
 * Calculates the checksum of the given packet data.
 * 
 * @param packet Pointer to the packet data.
 * @param length Length of the packet data in bytes.
 * @return The calculated checksum for the packet data.
 * @throws std::invalid_argument if the length of the packet data is not even.
 */
uint16_t Sender::checkSumma(const void *packet, int length) {
    if (length % 2 != 0)
        throw std::invalid_argument("checksum error: size of struct % 2 != 0");

    u_int32_t sum;
    const auto *ptr = static_cast<const uint16_t *>(packet);

    for (sum = 0; length > 0; length -= 2)
        sum += *ptr++;

    sum = (sum >> 16) + (sum & 0xffff);
    return (u_int16_t) (~(sum + (sum >> 16)));
}