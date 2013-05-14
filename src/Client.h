/*
 * Client.h
 *
 *  Created on: Apr 7, 2013
 *      Author: tomer
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>
#include "PacketForServer.h"
#include "PacketForClient.h"
#include "AsyncSerializationConnection.h"
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <boost/system/error_code.hpp>
class Client {
public:
	Client(boost::asio::io_service &io_service, const std::string &host,
			const std::string port, int id);

	virtual ~Client();
	void sendPacket(PacketForServer p);
private:
	void test(const boost::system::error_code& e);
	void sendID(boost::system::error_code e);
	void handleConnect(const boost::system::error_code& e);
	void handleRecv(boost::system::error_code e);
	void handlePacketAction(const boost::system::error_code& e,
			std::vector<PacketForClient> *packetsVec);

	void waitForPacket();
	void handleSendPacket(boost::system::error_code e,
			std::vector<PacketForServer> *packetsVec);

	std::vector<PacketForServer> packets_;
	PacketForServer p;
	size_t id_;
	boost::asio::ip::tcp::socket socket_;
	AsyncSerializationConnection *connection_;
};

#endif /* CLIENT_H_ */
