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

package opportunisticcellularnetwork;


// THIS IS THE SIMPLE MODULE THAT GENERATE THE INCOMING NETWORK TRAFFIC FOR A SINGLE USER


simple PacketGenerator
{
    parameters:
        double meanExp;					// it's the mean for the exponential distribution for the inte-arrival times
        int numUsers; 					// it used to make a wrap around in order to recover the userID
        bool isVoIP = default(false);	// it is the indicator for the VoIP like scenario in order to change the behaviour of the module
        
    gates:
        output out;						// it is the output gate in order to forward the packet
        
}
