#include "Antenna.h"
#include <string.h>
#include <algorithm>
#include "PacketMessage_m.h"
#include "Frame.h"

Define_Module(Antenna);

bool compareUserInfos(const UserInfo& ui1, const UserInfo& ui2){
    return ui1.cqi > ui2.cqi;
}

void Antenna::initialize()
{
    numUsers = par("numUsers");
    isUniform = par("isUniform");
    userInfos = vector<UserInfo>(numUsers);

	/*signal registration in order to compute metrics*/
    globalThroughputSignalId = registerSignal("globalThroughput");
    usedRBsSignalId = registerSignal("usedRBs");
    globalQueuesLengthSignalId = registerSignal("globalQueuesLength");
    responseTimeSignalId = registerSignal("responseTime");

    for(int i = 0; i < numUsers; i++){
        userInfos[i].id = i;
    }

    beep = new cMessage("timeSlotStart");
    scheduleAt( simTime() + timeSlotPeriod, beep);
    EV << "Antenna initialized \n";
}

void Antenna::handleMessage(cMessage *msg)
{
    if(strcmp(msg->getFullName(), "newPacket") == 0)
        enqueue(msg);
    else if(strcmp(msg->getFullName(), "timeSlotStart") == 0)
        handleTimeSlot();
}

void Antenna::enqueue(cMessage *msg){
    EV << "ENQUEUE" << endl;

    // store the packet
    Packet p;
    PacketMessage* msg_ = (PacketMessage*)(msg);	/*this cast is equivalent to the "check_and_cast<Type>" directive*/
    p.userId = msg_->getUserId();
    p.sizeBytes = msg_->getSizeBytes();
    p.arrivalTime = (simtime_t)msg_->getArrivalTime();
    cancelAndDelete(msg_);

    // find the user queue and push the new packet
    for(int i = 0; i < numUsers; ++i){
        if(userInfos[i].id == p.userId){
            userInfos[i].q.push(p);
            break;
        }
    }
}

void Antenna::handleTimeSlot(){
    int freeRBs = 25; // at the beginning of the timeslot, all RBs are free
	
	// It will contain the number of bytes sent at this timeslot.
	// It is used to compute the throughput metrics
    int bytesPerFrame = 0; 
    
	Frame frame;

    // compute cqis for each user
    generateCQIs();

    // sort out user, based on their cqi
    sort(userInfos.begin(), userInfos.end(), compareUserInfos);

    // schedule user's traffic
    for(int i = 0; i < numUsers; ++i){
        UserInfo& user = userInfos[i];
		
		/*if there is not free space, break*/
        if(freeRBs <= 0)
            break;
		
		/* free bytes seen by the current user */
        int freeBytes = freeRBs * lookUpCQI[user.cqi];
		/* number of packet in the user queue */
        int numPackets = user.q.size();
		
        int occupiedBytes = 0;
        for(int i = 0; i < numPackets; ++i){
            int packetSize = user.q.front().sizeBytes;
            if(packetSize <= freeBytes){
                EV << " POP ELEMENTO " << endl;
                freeBytes -= packetSize;
                occupiedBytes += packetSize;
                bytesPerFrame += packetSize;
                Packet tmp = user.q.front();
                frame.push_back(tmp);
                user.q.pop();
            }
            else{
                break;
			}
        }
		
        freeRBs -= occupiedBytes / lookUpCQI[user.cqi];
        /* a not full RB has to be sent anyway*/
        if(occupiedBytes % lookUpCQI[user.cqi])
            freeRBs--;
    }

    // send frame out
    sendFrame(frame);

    // compute throughput
    emit(globalThroughputSignalId, bytesPerFrame);
    EV << "GLOBAL THROUGHPUT: " << bytesPerFrame << "B/ts" << endl;

    // used rRBs
    emit(usedRBsSignalId, 25 - freeRBs);
    EV << "USED RB: " << 25 - freeRBs << endl;

    // compute queues statistics
    for(int i = 0; i < numUsers; ++i){
        emit(globalQueuesLengthSignalId, (double)userInfos[i].q.size());
        EV << "QUEUE LENGTH USER: " << userInfos[i].q.size() << endl;
    }

    // compute response time statistics
    for(int i = 0; i < frame.size(); ++i)
        emit(responseTimeSignalId, simTime() - frame[i].arrivalTime);

    // schedule next timeslotStart
    scheduleAt( simTime() + timeSlotPeriod, beep);
}

void Antenna::generateCQIs(){
    for(int i = 0; i < numUsers; ++i){
        if(isUniform)
            userInfos[i].cqi = intuniform(1, 15, i + 2*numUsers);
        else {
			/* 
			from the specification document each user in the binomial scenario 
			needs a different mean value for the binomial generating cqi distribution.
			For each user we divide the interval [0, 14] in (numUsers+1) parts and we select for him the internal part corresponding to userID+1
			in order to compute his "successProbability" value.
			We build a Binomial distribution with probability = "successProbability" and bounded between the [0,14] interval 
			and then we sum "+1" because the CQI values are bounded into [1,15] and not [0,14].
			*/
            double successProbability = (double) ( (double)(userInfos[i].id +1) / (double) (numUsers+1) ) ;
            userInfos[i].cqi = 1 + (binomial(14, successProbability, i + 2*numUsers));
        }
    }
}

void Antenna::sendFrame(Frame& frame){
    size_t frameSize = frame.size();
    for(auto i = 0; i < frameSize; ++i){
        Packet& p = frame[i];
        send(p.toPacketMessage(),"out",p.userId);
    }
}

















