/*
 * PacketForClient.h
 *
 *  Created on: May 14, 2013
 *      Author: tomer
 */

#ifndef PACKETFORCLIENT_H_
#define PACKETFORCLIENT_H_

struct PacketForClient {
    int id_;
    int opcode_;
    std::string file_path_;
    std::string time_;
    int priority_;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & opcode_;
        ar & file_path_;
        ar & id_;
        ar & time_;
        ar & priority_;
    }
};


#endif /* PACKETFORCLIENT_H_ */
