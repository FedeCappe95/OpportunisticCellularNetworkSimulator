

#ifndef PACKET_H_
#define PACKET_H_

#include <omnetpp.h>
#include "PacketMessage_m.h"

using namespace omnetpp;

/*
this class is the abstraction for the sent packet 
*/
class Packet {
  public:
    int userId;
    simtime_t arrivalTime;
    int sizeBytes;

  public:
    Packet();
    virtual ~Packet();
	/* function fo the coonversion into packetmessage in order to send the packet */
    PacketMessage* toPacketMessage();
};

#endif /* PACKET_H_ */
