CXXFLAGS = -std=c++11 -Werror -Wall -Wextra
CXX = g++

main : Packet.h main.cpp Traceroute.cpp Receiver.cpp Sender.cpp
	$(CXX) $(CXXFLAGS) -o traceroute main.cpp Traceroute.cpp Receiver.cpp Sender.cpp
