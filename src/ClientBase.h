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

#include <omnetpp.h>
#include <string>
#include <queue>

using namespace omnetpp;
using namespace std;

class ClientRequest;
class ClientReply;
class ClientACK;
class ReplyTimeout;
class ViewChange;
class ReplicaBase;
class View;

#ifndef CLIENTBASE_H_
#define CLIENTBASE_H_

class ClientBase : public cSimpleModule {
public:

    ClientBase();

protected:

    // Client PKI keys
    char private_key[65] = "0q3igRJxv0DRNvHcfOetkw3xrVKSlHvZ30w790JWv1A22czPpVNjYlgWrj5636d4";
    char public_key[65] = "zXUowFM86xXNwpROrzJI07mClur1oKJrnw6aBf1EPPSRTTCpCcVOjgfPzU3K3nyn";

    View* view;

    short operation;
    map<simtime_t, ClientRequest*> ongoing;
    map<simtime_t, int> reply_counter;

    // Main operations
    virtual void initialize();
    void sendGetRequest(char * key);
    void sendPutRequest(char * key, char * value);
    void toString(ClientRequest* message, char * ser_message);
    void displayOutput(std::string output);
    void AES_encrypt(char * plaintext, char * cyphertext, char * key);
    void AES_hash(char * ser_request, char * digest);
    void AES_sign(char * pri_key);

    // Methods to handle messages received from the replicas.
    virtual void handleMessage(cMessage *msg);
    void handleTimerMessage(cMessage* msg);
    void handleClientReply(ClientReply* cr);
    void handleViewChange(ViewChange* vc);

    /// Delays and timer messages
    cMessage* sendRequest;

    // Maximum Time for reply evaluation and arrival.
    // simtime_t MTResponse;

};

#endif /* CLIENTBASE_H_ */
