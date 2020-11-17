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

//
// TODO auto-generated module
//
simple Antenna
{
    parameters:
        int numUsers = default(10);
        bool isUniform; // we check if the scenario is the Uniform one
        @signal[globalThroughput] (type=int);
        @statistic[globalThroughputStats](source="globalThroughput"; record=mean,vector, min, max);
        
        @signal[usedRBs] (type=int);
        @statistic[usedRBsStats](source="usedRBs"; record=mean,vector, min, max);
        
        @signal[globalQueuesLength] (type=int);
        @statistic[globalQueuesLengthStats](source="globalQueuesLength"; record=mean,vector, min, max);
        
        @signal[responseTime] (type=double);
        @statistic[responseTimeStats](source="responseTime"; record=mean,vector, min, max);
        
    gates:
        input in[numUsers];
        output out[numUsers];
    
}
