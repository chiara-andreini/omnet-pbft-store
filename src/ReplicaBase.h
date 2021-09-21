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

#include <string.h>
#include <omnetpp.h>
#include <utility>

#include <gmp.h>
#include <stdio.h>

using namespace std;
using namespace omnetpp;

class ClientRequest;
class ClientReply;
class ViewChange;
class View;
class ClientBase;
class Heartbeat;
class ProtocolMessage;
class PrePrepare;
class Prepare;
class Commit;
class Certificate;
class Log;
class Checkpoint;

#ifndef REPLICABASE_H_
#define REPLICABASE_H_


class ReplicaBase : public cSimpleModule {
public:

    View* view;

    ReplicaBase();

protected:

    // PKI keys
    char private_key[65];
    char public_key[65];

    int overlayKey;
    bool amPrimary;

    int mode;   // The protocol the network is operating with.

    Log* log;
    Checkpoint* checkpoint;

    // This map pairs sequence numbers with a map storing two couples, ("p", ...), ("c", ...);
    // values are the certificates when produced. The Log keeps messages separately.
    map<int, map<string, Certificate*>> certificates;

    // TODO: Set a limit using this
    // Primary's load roof.
    int maxNumberofRequests;

    simtime_t viewChangeTimeout = 5;

    /// Generic operations
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg);
    void displayOutput(std::string output);
    void toString(cMessage* message, char * ser_message);

    void setKeys();
    void AES_encrypt(char * plaintext, char * ciphertext, char * key);
    void AES_hash(char * ser_request, char * digest);
    bool verifyMessageIntegrity(cMessage * msg);
    bool verifySetOCorrectness();

    void handleTimer(cMessage* msg);

    void handlePrePrepare(PrePrepare* preprepare);
    void handlePrepare(Prepare* prepare);
    bool isThereCertificate(int sq_num, string cert);
    void handleCommit(Commit* commit);
    string executeStoreOperation(const char * c_message, int sq_num);
    void sendToOriginator(ClientReply* response);

    /// Primary replica's operations
    void handleClientRequest(ClientRequest* crequest);


    /// Backup replica's operations

    // Method to send the node's contribution to the client request.
    void handleViewChange(ViewChange *v_change);
    ViewChange* sendViewChange(int new_view);
    void sendToPrimary(cMessage* msg);
    void flood(cMessage* msg);
    void broadcast(cMessage* msg);

};

#endif /* REPLICABASE_H_ */
