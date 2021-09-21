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

//
// Further fields can be added to cMessage via message declaration files (.msg) which are translated into C++ classes.
//

message ClientRequest {
    
    // The requested operation over the store may be "GET" and "PUT".
    string operation;
    string key;
    string value;
    
    int originator;
    int clientId;
    
    
    // Timestamp is simtime_t timestamp in the _m class.
    
    string digest;
    string signature;
}
