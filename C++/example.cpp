/**
 * Copyright (C) 2016 Lodewijk Sikkel <l.n.c.sikkel@tudelft.nl>
 * @file example.cpp
 * @brief Example udp handler program
 *
 * BUILD INSTRUCTIONS: g++ -o example example.cpp udphandler.cpp -std=c++11 -lboost_system -lboost_thread
 * 
 */

#include <iostream>
#include "UDPHandler.hpp"

int main( int argc, char * argv[] )
{
	boost::shared_ptr< boost::asio::io_service > io_service(
		new boost::asio::io_service
	);
	boost::shared_ptr< boost::asio::io_service::work > work(
		new boost::asio::io_service::work( *io_service )
	);

	UDPHandler server( *io_service );
    server.init( io_service );

    work.reset();

	return 0;
}