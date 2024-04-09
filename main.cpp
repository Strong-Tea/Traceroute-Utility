#include <iostream>
#include "Traceroute.h"
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netdb.h>


#define MAX_HOPS 30
#define DEFAULT_RESPONSE_TIMEOUT_S 1
#define DEFAULT_RESPONSE_TIMEOUT_MS 0


void help() {
    std::cout << "  sudo ./traceroute 8.8.8.8 -h 30 -t 1 \n"
        "  -h  - hops. \n"
        "  -t  - response_timeout. \n";  
}


/**
 * Resolves the IP address of a hostname.
 * 
 * @param host The hostname to resolve.
 * @return The IP address corresponding to the hostname, or an empty string if resolution fails.
 */
std::string getIpByHostname(const char* host) {
    struct hostent *ghbn = gethostbyname(host);
    if (ghbn) {
        struct in_addr **addr_list = (struct in_addr **)ghbn->h_addr_list;
        if (addr_list[0]) {
            char *ip_address = inet_ntoa(*addr_list[0]);
            if (ip_address) {
                return ip_address;
            }
        } else {
            std::cerr << "No address found for the given hostname\n";
        }
    } else {
        std::cerr << "Failed to resolve hostname\n";
    }
    return "";
}


/**
 * Checks whether the given string represents a valid IPv4 address.
 * 
 * @param str The string to be checked for validity as an IPv4 address.
 * @return True if the string is a valid IPv4 address, false otherwise.
 */
bool isIPAddress(const std::string& str) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, str.c_str(), &(sa.sin_addr)) != 0;
}


int main(int argc, const char *argv[]) {

    if (argc < 2 || argc > 6) {
        help();
        return 0;
    }

    const char *ip_address;
    int max_hops = MAX_HOPS;
    int response_timeout = DEFAULT_RESPONSE_TIMEOUT_S;


    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        help();
        return 0;
    } else {
        ip_address = argv[1];

        // Check an IP address for validity
        if (!isIPAddress(ip_address)) {
            
            // Try to get an ip address by domain name
            std::string i = getIpByHostname(argv[1]);
            if (i == "") {
                exit(0);
            }

            std::cout<<ip_address << " = " << i << "\n";
            std::cout<<"--------------------------\n";
            ip_address = i.c_str();
        }
    }

    // Retrieve additional flags
    for (int i = 2; i < argc; i++) {

        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--hops") == 0) {
            max_hops = atoi(argv[i + 1]);
            i += 1;
        }

        if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--response_timeout") == 0) {
            response_timeout = atoi(argv[i + 1]);
            i += 1;
        }
    }


    try {
        Traceroute traceroute;
        traceroute.trace(ip_address, max_hops, response_timeout);
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }


    return 0;
}