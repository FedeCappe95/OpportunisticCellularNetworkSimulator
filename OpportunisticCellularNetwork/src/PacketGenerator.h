//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __OPPORTUNISTICCELLULARNETWORK_PACKETGENERATOR_H_
#define __OPPORTUNISTICCELLULARNETWORK_PACKETGENERATOR_H_

#include <omnetpp.h>

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class PacketGenerator : public cSimpleModule
{
  protected:
    static int nextId;

  protected:
    int userId;
    int numUsers;
    double meanExp;

    // VoIP simulation parameters
    bool isVoIP;
    const simtime_t nextPacketTime = 0.020; // typical value for VoIP calls (deterministic)
    const simtime_t meanOnOffTime = 0.5;
    bool isOn;

    // timer
    cMessage* beep;
    cMessage* nextVoIP;

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void sendNewPacket();

  public:
    PacketGenerator();
    ~PacketGenerator();
};

#endif
