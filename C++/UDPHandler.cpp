/**
 * Copyright (C) 2016 Lodewijk Sikkel <l.n.c.sikkel@tudelft.nl>
 * @file UDPHandler.cpp
 * @brief UDP handler class
 */

#include <boost/thread/mutex.hpp>
#include "UDPHandler.hpp"

boost::mutex global_stream_lock;

void UDPHandler::init( boost::shared_ptr< boost::asio::io_service > io_service ) {
	remote_endpoint_ = udp::endpoint( 
		boost::asio::ip::address::from_string("10.0.2.15"), OUTGOING_PORT );

	// Create thread pool to facilitate creating threads
	boost::thread_group thread_pool;

	// Create timer thread used for sending datagrams
	global_stream_lock.lock();
	std::cout << "[" << boost::this_thread::get_id()
		<< "] Creating timer thread" << std::endl;
	global_stream_lock.unlock();
	thread_pool.create_thread( boost::bind( &UDPHandler::TimerThread, this, io_service ) ); // boost::bind requires us to use shared pointers

	// Create receiver thread
	thread_pool.create_thread( boost::bind( &UDPHandler::ReceiverThread, this ) );

	// Call run() on the io_service object to run the handlers
	io_service->run();

	// Wait for all threads to stop (if so)
	thread_pool.join_all();
}

void UDPHandler::ReceiverThread() {
	global_stream_lock.lock();
	std::cout << "[" << boost::this_thread::get_id()
		<< "] Thread Start" << std::endl;
	global_stream_lock.unlock();

	while(1) {
		global_stream_lock.lock();
		std::cout << "[" << boost::this_thread::get_id()
			<< "] Waiting for data..." << std::endl;
		global_stream_lock.unlock();
      	
      	// Receive data from the socket and set the remote 
      	size_t length = socket_.receive_from(
        	boost::asio::buffer(data_, max_length), remote_endpoint_);

      	global_stream_lock.lock();
		std::cout << "[" << boost::this_thread::get_id()
			<< "] Received " << length << " bytes" << std::endl;
		global_stream_lock.unlock();
	}
}

void UDPHandler::TimerHandler( const boost::system::error_code & error,
				   boost::shared_ptr< boost::asio::deadline_timer > timer ) {
	if( error )
	{
		global_stream_lock.lock();
		std::cout << "[" << boost::this_thread::get_id()
			<< "] Error: " << error << std::endl;
		global_stream_lock.unlock();
	}
	else
	{
		// Send data to the remote endpoint 
		char data[1] = { 99 }; // just some byte of data
		socket_.send_to(boost::asio::buffer(data, 1), remote_endpoint_);

		timer->expires_from_now( boost::posix_time::seconds( 1 ) );
		timer->async_wait( boost::bind( &UDPHandler::TimerHandler, this, _1, timer ) );
	}
}

void UDPHandler::TimerThread( boost::shared_ptr< boost::asio::io_service > io_service ) {
	global_stream_lock.lock();
	std::cout << "[" << boost::this_thread::get_id()
		<< "] Thread Start" << std::endl;
	global_stream_lock.unlock();

	boost::shared_ptr< boost::asio::deadline_timer > timer( 
		new boost::asio::deadline_timer( *io_service )
	);
	timer->expires_from_now( boost::posix_time::seconds( 1 ) );
	timer->async_wait( boost::bind( &UDPHandler::TimerHandler, this, _1, timer ) );
}