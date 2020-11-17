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

#include "User.h"
#include "Frame.h"

Define_Module(User);

void User::initialize()
{
    userResponseTimeSignalId = registerSignal("userResponseTime");
    EV << "User initialized\n";
}

void User::handleMessage(cMessage *msg)
{
    PacketMessage* pm = (PacketMessage*)msg;
    emit(userResponseTimeSignalId,simTime() - pm->getArrivalTime());
    cancelAndDelete(msg);
}
