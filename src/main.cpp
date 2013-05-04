/*
 * main.cpp
 *
 *  Created on: Apr 19, 2013
 *      Author: tomer
 */

#include "Client.h"


int main(int argc  , char *argv[]){

	boost::asio::io_service io_service;
	Client client(io_service,"127.0.0.1","55555");
	io_service.run();
	std::cout << "main\n";
	return 0;
}
