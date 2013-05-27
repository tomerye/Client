/*
 * PacketForClient.h
 *
 *  Created on: May 14, 2013
 *      Author: tomer
 */

#ifndef PACKETFORCLIENT_H_
#define PACKETFORCLIENT_H_

#include <string>

struct PacketForClient {
    int opcode_;
    std::string command_;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & opcode_;
        ar & command_;
    }
};


#endif /* PACKETFORCLIENT_H_ */
