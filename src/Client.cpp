/*
 * Client.cpp
 *
 *  Created on: Apr 7, 2013
 *      Author: tomer
 */

#include "Client.h"

Client::Client(boost::asio::io_service &io_service, const std::string &host,
		const std::string port) :
		p(), socket_(io_service) {
	id = std::rand() % 1000 + 1;
	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(host, port);
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
			resolver.resolve(query);
	boost::asio::async_connect(socket_, endpoint_iterator,
			boost::bind(&Client::sendID, this,
					boost::asio::placeholders::error));
}

void Client::sendID(boost::system::error_code e) {
	if (!e) {
		boost::asio::async_write(socket_, boost::asio::buffer(&id, sizeof(id)),
				boost::bind(&Client::handleConnect, this,
						boost::asio::placeholders::error));
	} else {
		std::cerr << e.message() << std::endl;
	}
}

void Client::handleConnect(const boost::system::error_code& e) {
	if (!e) {
		connection_ = new AsyncSerializationConnection(&socket_);

		Packet *newPacket = new Packet();
		connection_->async_read(newPacket,
				boost::bind(&Client::handlePacketAction, this,
						boost::asio::placeholders::error, newPacket));

	}
}

void Client::handlePacketAction(const boost::system::error_code& e,
		Packet *packet) {
	if (!e) {
		Packet *newPacket = new Packet();
		connection_->async_read(newPacket,
				boost::bind(&Client::handlePacketAction, this,
						boost::asio::placeholders::error, newPacket));

		std::cout << newPacket->id_ << std::endl;
	}
}

void Client::sendPacket(Packet &packet) {
	connection_->async_write(packets_,
			boost::bind(&Client::handleSendPacket, this,
					boost::asio::placeholders::error));

}

void Client::handleSendPacket(boost::system::error_code e) {
	if (!e) {
		std::cout << "Packet sent!\n";
	}
}

Client::~Client() {
	delete connection_;
}

