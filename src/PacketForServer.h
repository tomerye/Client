/*
 * Packet.h
 *
 *  Created on: Apr 6, 2013
 *      Author: tomer
 */

#ifndef PACKET_H_
#define PACKET_H_



struct PacketForServer {
	int opcode_;
	std::string file_path_;
	int priority_;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & opcode_;
		ar & file_path_;
		ar & priority_;
	}
};

#endif /* PACKET_H_ */
