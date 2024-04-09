## Description:

This C++ program serves as an analog of the traceroute utility for IPv4 addresses and DNS names. Traceroute is a network diagnostic tool used to track the route packets take across an Internet Protocol (IP) network. It sends packets with gradually increasing Time to Live (TTL) values, allowing it to map the path packets take from the source to the destination.

## Features:

- Accepts both IPv4 addresses and DNS names as input.
- Allows users to specify maximum Time to Live (TTL) and timeout values using additional flags.
- Utilizes the Internet Control Message Protocol (ICMP) to send packets and receive responses from intermediate routers.
- Displays the route taken by packets to reach the destination, along with corresponding round-trip times (RTT) for each hop.
- Helps in identifying network congestion, routing issues, and latency problems along the path to the destination.

### Example
~~~
./traceroute <ip address> -h <number> -t <number>
~~~
Options:

- h - max-hops <value>: Set the maximum hop count.
- t - timeout <value>: Set the timeout value for each probe.

 


![traceroute1](https://github.com/Strong-Tea/Traceroute-Utility/assets/135996451/66930ef6-ef28-4884-a6a3-ae8b26d66ef8)


![traceroute2](https://github.com/Strong-Tea/Traceroute-Utility/assets/135996451/cf15170e-b2cf-4d55-82dd-e8302a02e546)
