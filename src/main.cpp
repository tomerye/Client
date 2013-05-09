/*
 * main.cpp
 *
 *  Created on: Apr 19, 2013
 *      Author: tomer
 */

#include "Client.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>



int main(int argc, char *argv[]) {
	if (argc != 2) {
		return 1;
	}

	boost::asio::io_service io_service;
	boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
	Client client(io_service, "127.0.0.1", "55555", std::atoi(argv[1]));
//	boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
//	io_service.run();
	Packet p;
	p.id_ = 333;
	p.file_path_ = "sfdsf";
	p.opcode_ = "ddd";
	sleep(10);
	client.sendPacket(p);
	sleep(10);
//	t.join();
//	io_service.run();
	t.join();
	std::cout << "main\n";
	return 0;
}
