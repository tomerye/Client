/*
 * main.cpp
 *
 *  Created on: Apr 19, 2013
 *      Author: tomer
 */

#include "Client.h"


int main(int argc  , char *argv[]){

	boost::asio::io_service io_service;
	Client client(io_service,"127.0.0.1","22922");

	return 0;
}
