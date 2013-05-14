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
		waitForPacket();
	} else {
		std::cout << "error:handleConnect\n";
	}
}

void Client::waitForPacket() {
	std::cout << "waiting for packets from server\n";
	std::cout.flush();
	std::vector<PacketForClient> *packetsVec = new std::vector<PacketForClient>();
	connection_->async_read(*packetsVec,
			boost::bind(&Client::handlePacketAction, this,
					boost::asio::placeholders::error, packetsVec));
}

void Client::handlePacketAction(const boost::system::error_code& e,
		std::vector<PacketForClient> *packetsVec) {

	if (!e) {
		waitForPacket();
		std::cout << "parsing the packet\n";
		for (std::size_t i = 0; i < packetsVec->size(); ++i) {
			std::cout << "Recived id:" << ((*packetsVec)[i]).id_ << std::endl;
			std::cout << "Recived file path:" << ((*packetsVec)[i]).file_path_
					<< std::endl;
			std::cout << "Recived opcode:" << ((*packetsVec)[i]).opcode_
					<< std::endl;
			std::cout.flush();
		}
	} else {
		std::cout << "error while parsing the packet\n";
	}
	delete packetsVec;
}

void Client::sendPacket(PacketForServer packet) {
	std::vector<PacketForServer> *packetsVec = new std::vector<PacketForServer>();
	packetsVec->push_back(packet);
	connection_->async_write(*packetsVec,
			boost::bind(&Client::handleSendPacket, this,
					boost::asio::placeholders::error, packetsVec));

}

void Client::handleSendPacket(boost::system::error_code e,
		std::vector<PacketForServer> *packetsVec) {
	if (!e) {
		std::cout << "Packet sent!\n";
	} else {
		std::cout << "error happen in send packet!\n";
	}
	delete packetsVec;
}

Client::~Client() {
	delete connection_;
}

