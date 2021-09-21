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

message ProtocolMessage {
	int bitmap[11];
	
	int view;
    int sequenceNumber;
    string clientMessage;
    
    string clientMessageDigest;
    string signature;
}


message PrePrepare extends ProtocolMessage {
	int originator;
	int clientId;
    
}

message Prepare extends ProtocolMessage {
	int senderOverlayKey;
}

message Commit extends ProtocolMessage {
    int senderOverlayKey;
}

// Representing prepare-certificates and committed-local-certificates with messages

message Certificate {
	int view;
    int sequenceNumber;
    string clientMessage;
    int issuerOverlayKey;
}

message PrepareCertificate extends Certificate {
}

message CommitLocalCertificate extends Certificate {

}