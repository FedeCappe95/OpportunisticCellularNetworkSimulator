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

network OpportunisticCellularNetwork
{
    parameters:
        int numUsers = default(10);
        @display("bgb=544,212");
    submodules:
        packetGenerators[numUsers]: PacketGenerator {
            numUsers = numUsers;
            @display("p=160,98;i=block/source");
        }
        users[numUsers]: User {
            @display("p=401,98,c");
        }
        antenna: Antenna {
            numUsers = numUsers;
            @display("p=279,98;i=device/antennatower");
        }
    connections:
        for i=0..numUsers-1 {
            packetGenerators[i].out --> antenna.in[i];
            antenna.out[i] --> users[i].in;
        }


}
