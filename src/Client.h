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
#include <deque>
#include <boost/system/error_code.hpp>

class Client {
public:
	Client(boost::asio::io_service &io_service, const std::string &host,
			const std::string port, u_int32_t id);

	virtual ~Client();
	void sendPacket(PacketForServer *packet);
private:
	void test(const boost::system::error_code& e);
	void sendID(boost::system::error_code e);
	void handleConnect(const boost::system::error_code& e);
	void handleRecv(boost::system::error_code e);
	void handlePacketAction(const boost::system::error_code& e,
			PacketForClient *newPacket);

	void waitForPacket();
	void handleSendPacket(boost::system::error_code e);

	std::deque<PacketForServer*> outPacketsBuffer_;
	u_int32_t id_;
	boost::asio::ip::tcp::socket socket_;
	AsyncSerializationConnection *connection_;
};

#endif /* CLIENT_H_ */
