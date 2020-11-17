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

#ifndef __OPPORTUNISTICCELLULARNETWORK_ANTENNA_H_
#define __OPPORTUNISTICCELLULARNETWORK_ANTENNA_H_

#include <omnetpp.h>
#include <vector>
#include <queue>
#include "Packet.h"
#include "Frame.h"

using namespace std;
using namespace omnetpp;

/* this struct is used only to put together all the user information */
struct UserInfo {
    int cqi;
    int id;
    queue<Packet> q;
};



class Antenna : public cSimpleModule
{
  private:
  /*this is the lookUp table taken from the specification document*/
    const int lookUpCQI[16] = {0, 3, 3, 6, 11, 15, 20, 25, 36, 39, 50, 63, 72, 80, 93, 93};

  protected:
    simsignal_t globalThroughputSignalId;
    simsignal_t usedRBsSignalId;
    simsignal_t globalQueuesLengthSignalId;
    simsignal_t responseTimeSignalId;
    bool isUniform;	/* used to distinguish between the Uniform behaviour and the Binomial one*/
    const simtime_t timeSlotPeriod = 0.001; // 1ms timeslot = network clock
    cMessage* beep;
    int numUsers;
    vector<UserInfo> userInfos;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
	
	/* function that put in the right user queue the incoming packet*/
    virtual void enqueue(cMessage *msg);
	
	/* function that handle the beep message*/
    virtual void handleTimeSlot();
	
	/* function that generate the CQIs for the users*/
    virtual inline void generateCQIs();
	
	/*function that send the frame to all the users */
    virtual void sendFrame(Frame& frame);
};

#endif
