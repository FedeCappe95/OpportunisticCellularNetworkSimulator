

#include "Packet.h"

Packet::Packet() {
    // TODO Auto-generated constructor stub

}

Packet::~Packet() {
    // TODO Auto-generated destructor stub
}

PacketMessage* Packet::toPacketMessage(){
    PacketMessage* ret = new PacketMessage();
    ret->setArrivalTime(arrivalTime);
    ret->setUserId(userId);
    ret->setSizeBytes(sizeBytes);
    return ret;
}
