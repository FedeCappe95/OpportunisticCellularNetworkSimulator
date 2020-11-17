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

#include "PacketGenerator.h"
#include "PacketMessage_m.h"
#include <string.h>

/*
it the initialization for a static class 
variable that will be use for the userID assignment
*/
int PacketGenerator::nextId = 0;   

/*
Module class needs to be registered with OMNeT++
*/
Define_Module(PacketGenerator);

PacketGenerator::PacketGenerator(){}

void PacketGenerator::initialize()
{
	beep = new cMessage("beep");
    numUsers = par("numUsers");
    isVoIP = par("isVoIP");
    userId = PacketGenerator::nextId;
    PacketGenerator::nextId = (PacketGenerator::nextId + 1) % numUsers;
    EV << "PacketGenerator initialized for userId: " << userId << '\n';
	/*
	Different behaviour for the module if VoIP or not
	*/
    if(isVoIP) {
		/*
		1. start in the off state
		2. schedule the beep msg
		3. initialization of the nextVoIP msg in order to implements the VoIP msg timer
		4. exponential distribution for interarrival times is not needeed, since they are deterministic in VoIP
		*/
		isOn = false;
        EV << "Sono voip" << endl;
        scheduleAt( simTime(),  beep); // schedule the first on or off
        nextVoIP = new cMessage("nextVoIPmsg");
        meanExp = 0;
    }
    else { // no VoIP like scenario
		/*
		1. initialization for the "isOn" variable but it has no	meaning
		2. exponential distribution for interarrival times is needeed, so we recover the value with "par()"
		3. schedule the beep msg
		4. nextVoIP is not needeed
		*/
        isOn = false;
        meanExp = par("meanExp");
        scheduleAt( simTime(),  beep);
        nextVoIP = 0;
    }

}

/*
- NO VOIP CASE: 
	1. The only received message is the beep one, in order to implement the timer.
	2. Each beep sends a new packet.

- VOIP CASE:
	1. beep message is used only to change the ON and OFF state for the packet generator.
	2. the constant time to send the VoIP packets is implemented by "nextVoIP"

*/

void PacketGenerator::handleMessage(cMessage *msg)
{
    if(strcmp(msg->getFullName(), "beep") == 0){
        if(!isVoIP){ // basic scenarios
            sendNewPacket();
			/* the rng id is the userID */
            simtime_t time = exponential(meanExp, userId);
            scheduleAt( simTime() + time, beep);
        }
        else {
            if(isOn){
                cancelEvent(nextVoIP); // need to delete the next message, otherwise you talk even if you're off
                isOn = false;
                EV << "passo allo stato off" << endl;
            }
            else {
                isOn= true;
                scheduleAt( simTime() + nextPacketTime, nextVoIP); // deterministically scheduling next message
                sendNewPacket();
                EV << "passo allo stato on" << endl;
            }

            scheduleAt( simTime() + exponential(meanOnOffTime, userId),  beep); // in any case, schedule the next on/off
        }
    }
    else if(strcmp(msg->getFullName(), "nextVoIPmsg") == 0){
		/* VOIP case*/
        scheduleAt( simTime() + nextPacketTime, nextVoIP); // deterministically scheduling next message
        sendNewPacket();
    }
}

void PacketGenerator::sendNewPacket(){
    PacketMessage* packetMessage = new PacketMessage("newPacket");
    packetMessage->setUserId(userId);
    packetMessage->setArrivalTime(simTime());
	/* 
	we take the RNG "id = userID + numUsers" because 
	the first numUsers rngs are used for the inter-arrival times, 
	the second numUsers rngs are used for the packet size, 
	the third numUsers rngs are used for the CQI values 
	*/
    int packetSize = intuniform(1, 75, userId + numUsers);
    EV << "INVIATO NUOVO PACCHETTO PER USER " << userId << endl << " DIMENSIONE " << packetSize << endl;
    packetMessage->setSizeBytes(packetSize); // uniform
    send(packetMessage, "out");
}

PacketGenerator::~PacketGenerator(){
    cancelAndDelete(beep);
    if(isVoIP)
        cancelAndDelete(nextVoIP);
}
