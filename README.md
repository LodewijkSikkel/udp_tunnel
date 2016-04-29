# udp_tunnel
Generic UDP tunnel between a C and C++ program. The programs are intended to be run at the same time, provided that UDPHandler is a UDP server class that outputs some arbitrary data. UDPHandler is a class that contains a single UDP socket and a timer to output the data at a predefined fixed rate. Both rely on the BOOST library to work (which implies you should download this library). 

BUILD_INSTRUCTIONS: g++ -o example example.cpp udphandler.cpp -std=c++11 -lboost_system -lboost_thread

The udp_echo_client.c file contains a UDP echo client which sends the data it has received on one of its ports. 

BUILD_INSTRUCTIONS: gcc -o udp_echo_client udp_echo_client.c
