/*
 * Client.cpp
 *
 *  Created on: Apr 7, 2013
 *      Author: tomer
 */

#include "Client.h"

using namespace std;
Client::Client(boost::asio::io_service &io_service, const std::string &host,
		const std::string port, int id) :
		p(), socket_(io_service), id_(id) {

	std::cout << "Client ID:" << id << std::endl;
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
		cout << "sendID:" << id_ << endl;
		boost::asio::async_write(socket_,
				boost::asio::buffer(&id_, sizeof(id_)),
				boost::bind(&Client::handleConnect, this,
						boost::asio::placeholders::error));
	} else {
		std::cerr << e.message() << std::endl;
	}
}

void Client::handleConnect(const boost::system::error_code& e) {
	cout << "handleConnect\n";
	if (!e) {
		connection_ = new AsyncSerializationConnection(&socket_);

		Packet *newPacket = new Packet();
		connection_->async_read(newPacket,
				boost::bind(&Client::handlePacketAction, this,
						boost::asio::placeholders::error, newPacket));

	} else {
		std::cout << "error:handleConnect/n";
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
		delete packet;
	}
}

void Client::sendPacket(Packet packet) {
	Packet p;
	p.id_ = 333;
	p.file_path_ = "sfdsf";
	p.opcode_ = "ddd";
	packets_.push_back(p);
	sleep(3);
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

