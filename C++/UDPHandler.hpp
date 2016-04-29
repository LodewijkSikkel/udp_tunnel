/**
 * Copyright (C) 2016 Lodewijk Sikkel <l.n.c.sikkel@tudelft.nl>
 * @file UDPHandler.hpp
 * @brief UDP handler class
 */

#include <iostream>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

#define INCOMING_PORT 8888
#define OUTGOING_PORT 8889

class UDPHandler {
public:
  UDPHandler( boost::asio::io_service &io_service ) 
    : socket_( udp::socket(io_service) )
  {
  	boost::system::error_code error;
    socket_.open( udp::v4(), error );
  	if (error) {
  	  std::cout << "Error: " << error << std::endl;
  	}

  	// Bind to the local endpoint
  	socket_.bind( udp::endpoint( udp::v4(), INCOMING_PORT ) );
  }

  void init( boost::shared_ptr< boost::asio::io_service > io_service );
  void ReceiverThread();
  void TimerHandler( const boost::system::error_code & error,
				     boost::shared_ptr< boost::asio::deadline_timer > timer );
  void TimerThread( boost::shared_ptr< boost::asio::io_service > io_service );

private:
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  enum { max_length = 1024 };
  char data_[max_length];
};