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

#include <queue>
#include <algorithm>
#include <omnetpp.h>
#include "ClientBase.h"
#include "ClientReply_m.h"
#include "ReplicaBase.h"
#include "View.h"
#include "ClientRequest_m.h"

using namespace omnetpp;
using namespace std;

Define_Module(ClientBase);

ClientBase::ClientBase() {}

void ClientBase::initialize() {

    // Getting the operation it needs to request (0 for GET and 1 for PUT).
    operation = par("operation");


    // Setting up the initial view,
    view = new View(
            (int)this->getParentModule()->getParentModule()->par("viewCode"),
            (int)par("numOfReplicas")
           );

    if ((int)this->getParentModule()->getParentModule()->par("mode") == 0) {
        // Scheduling the timer self-message that will trigger the sending of the request.
        sendRequest = new cMessage();
        scheduleAt(simTime()+exponential(5.0), sendRequest);
    }

}

void ClientBase::sendGetRequest(char *key) {

    ClientRequest* request = new ClientRequest();

    request->setKey(key);

    char operation[4];
    strcpy(operation, "GET");
    request->setOperation(operation);

    // Computing the message hash digest and the signature.
    char ser_request[21];
    toString(request, ser_request);

    char digest[37];
    AES_hash(ser_request, digest);
    request->setDigest(digest);

    char ciphertext[65];
    AES_encrypt(digest, ciphertext, private_key);
    request->setSignature(ciphertext);

    request->setTimestamp(simTime());

    request->setOriginator(-1);
    request->setClientId(-1);

    string str(request->getKey());
    displayOutput("I'm sending the data store a request for operation GET(\"" + str + "\").");

    send(request, "g$o", 0);
    ongoing.insert(pair<simtime_t, ClientRequest*>(request->getTimestamp(), request));
    reply_counter.insert(pair<simtime_t, int>(request->getTimestamp(), 0));
}

void ClientBase::sendPutRequest(char *key, char *value) {

    ClientRequest* request = new ClientRequest();

    request->setKey(key);
    request->setValue(value);

    char operation[4];
    strcpy(operation, "PUT");
    request->setOperation(operation);

    // Computing the message hash digest and the signature.
    char ser_request[21];
    toString(request, ser_request);

    char digest[37];
    AES_hash(ser_request, digest);
    request->setDigest(digest);

    char ciphertext[65];
    AES_encrypt(digest, ciphertext, private_key);
    request->setSignature(ciphertext);

    request->setTimestamp(simTime());

    request->setOriginator(-1);
    request->setClientId(-1);

    displayOutput("I'm sending the data store a request for operation PUT(\"" + string(request->getKey()) + "\"," +
            string(request->getValue()) + ").");

    send(request, "g$o", 0);
    ongoing.insert(pair<simtime_t, ClientRequest*>(request->getTimestamp(), request));
    reply_counter.insert(pair<simtime_t, int>(request->getTimestamp(), 0));

}


void ClientBase::handleMessage(cMessage *msg) {

    if (msg == sendRequest) {
        // The operation we need to simulate is either a "GET(k)" or a "PUT(k,v)", for the sake of
        // generality we randomly generate a string to represent the key that would be used; the key
        // space in use by the store could be any.
        char key[20];
        strcpy(key,"pmMdYaUZtW5COIMO3k6");

        if (this->operation == 1) {
            // If the operation we need to simulate is a "PUT(k,v)", we use a dummy json text as the value
            // that could also be replaced with anything.
            char value[30];
            strcpy(value,"{dummy1: 12.3, dummy2: [5,6]}");
            sendPutRequest(key, value);
        } else
            sendGetRequest(key);
    }

    ClientReply * cr = dynamic_cast<ClientReply*> (msg);
    if (cr != nullptr) {
        // displayOutput("Simtime" + cr->getTimestamp().str());
        // Checking if its a reply for something ongoing.
        if (ongoing.find(cr->getTimestamp()) == ongoing.end()) {
          // That key was not found.
          return;
        } else {

            ClientRequest* request = ongoing.find(cr->getTimestamp())->second;

            if (reply_counter[cr->getTimestamp()] < view->getF() + 1) {
                std::string str(cr->getClientMessage());
                if (string(request->getOperation()) == "GET") {
                    displayOutput("I received a reply for operation " + string(request->getOperation()) +
                            + "(\"" + string(request->getKey()) + "\").");
                } else {
                    displayOutput("I received a reply for operation " + string(request->getOperation()) +
                            + "(\"" + string(request->getKey()) + "," + string(request->getValue()) + "\").");
                }

                reply_counter[cr->getTimestamp()] = reply_counter[cr->getTimestamp()] + 1;
                displayOutput("I now have " + to_string(reply_counter[cr->getTimestamp()]) + " replies for this.");
            } else if (reply_counter[cr->getTimestamp()] == view->getF() + 1) {
                std::string str(cr->getClientMessage());

                ClientRequest* request = ongoing.find(cr->getTimestamp())->second;

                if (string(request->getOperation()) == "GET") {
                    displayOutput("I received enough consistent replies for operation " + string(request->getOperation()) +
                            + "(\"" + string(request->getKey()) + "\"). Accepting the result " + string(cr->getResult()) + ".");
                } else {
                    displayOutput("I received enough consistent replies for operation " + string(request->getOperation()) +
                            + "(\"" + string(request->getKey()) + "," + string(request->getValue()) + "\"). Accepting the result " + string(cr->getResult()) + ".");
                }

                ongoing.erase(cr->getTimestamp());
                reply_counter.erase(cr->getTimestamp());
            }

        }
    }

}

void ClientBase::AES_encrypt(char * plaintext, char * ciphertext, char * key) {

    // Simulation of the encryption algorithm with a dummy string generator.
    int randomNumber, counter;
    std::string o;
    char *ochar;

    // initialize random seed
    srand(time(NULL));
    randomNumber = rand() % 10;
    srand(time(NULL));
    counter = rand() % 10;

    o = std::to_string(counter);
    ochar = (char *)o.c_str();

    switch(randomNumber) {
        case(0):
            strcpy(ciphertext, "ty8tA9DmbJg0kZJlci6IBcBsa7BvPCrPFjND6eFsaZtH1S5SJ11dyXijBIm3UXG");
            strcat(ciphertext, ochar);
            break;
        case(1):
            strcpy(ciphertext, "bhQcqe2wlfQMPodzlQxvCxEwj9i0LXxW1VtDLIaiGT1pWE6a4peQnlnllypBRY6");
            strcat(ciphertext, ochar);
            break;
        case(2):
            strcpy(ciphertext, "3FrklMwzdq38JQ4PSPtGsmqreOKLEuwWOJSVRl7SRYkDc6OtNltrxJWjqRWdfKs");
            strcat(ciphertext, ochar);
            break;
        case(3):
            strcpy(ciphertext, "qnOkTAxP1n8jTUgq89OXKFqndWqNeTkxCRnCvzZnikaTyJITboCV0AV8LjkYGXt");
            strcat(ciphertext, ochar);
            break;
        case(4):
            strcpy(ciphertext, "hSEzzu1Z0mFBANRX6m94QOHM3zAaML6AbPuJKWSXTGRXG31krxgMuyNL0Yhrn5H");
            strcat(ciphertext, ochar);
            break;
        case(5):
            strcpy(ciphertext, "BxEAeXljEBlxq3FAEIkswsSNQxzit04FttyWvQCc6AUaoUJguwbfXbZpKYBSRkO");
            strcat(ciphertext, ochar);
            break;
        case(6):
            strcpy(ciphertext, "EYW6znUi1pQzNKdNI1CnmjNiGR3Ulc7ltnq4NLO5Ig33rKoOjuK2W6CeoDg2EQq");
            strcat(ciphertext, ochar);
            break;
        case(7):
            strcpy(ciphertext, "deULhAHJYqiS737gKKiqZJwHsaDaMxQGjKPgMG4J272XktDKN31yYYyZ2aVYgYa");
            strcat(ciphertext, ochar);
            break;
        case(8):
            strcpy(ciphertext, "rrgw3vLCKtNQ8eNYI4rD3wkQ0Bea7VA2fnfOBIGIbalPFG46Zzv2dcQloGU3Cwy");
            strcat(ciphertext, ochar);
            break;
        case(9):
            strcpy(ciphertext, "YcBsS8TDIhinYMFnrPMKdwFzpcSkwTUc20NzulqOCMoEpHhQgdNyMeuLLXSp426");
            strcat(ciphertext, ochar);
            break;
    }

    return;

}

void ClientBase::AES_hash(char * ser_request, char * digest){

    // Simulation of the hash algorithm with a dummy string generator.
    int randomNumber, counter;
    std::string o;
    char *ochar;

    // initialize random seed
    srand(time(NULL));
    randomNumber = rand() % 10;
    srand(time(NULL));
    counter = rand() % 10;

    o = std::to_string(counter);
    ochar = (char *)o.c_str();

    switch(randomNumber) {
        case(0):
            strcpy(digest, "7DoKABoLk3p8ou9NutRUkt5XGTUHsj2Yv05");
            strcat(digest, ochar);
            break;
        case(1):
            strcpy(digest, "exFLmlQwZezFhRsJutRUHsj2Yv05vGVN0dZ");
            strcat(digest, ochar);
            break;
        case(2):
            strcpy(digest, "6IWuwKybBQoCcePMutRUkt5XGTUHsj2Yv05");
            strcat(digest, ochar);
            break;
        case(3):
            strcpy(digest, "t1mMgg33m0dC1qw0utRUkt5XGTspgSBGweJ");
            strcat(digest, ochar);
            break;
        case(4):
            strcpy(digest, "IQWqn54o9RzyJkH4utRUktUHsj2Yv05vGVN");
            strcat(digest, ochar);
            break;
        case(5):
            strcpy(digest, "37FTuGjywgSvQThAutRUkt5UHsj2Yv05vGV");
            strcat(digest, ochar);
            break;
        case(6):
            strcpy(digest, "uMAF5dOQn5hy1zEKutRUkt5XGTspgSBGweJ");
            strcat(digest, ochar);
            break;
        case(7):
            strcpy(digest, "PUu4dwBWGP6BpLaWutRUkt5XGTspgUHsj2Y");
            strcat(digest, ochar);
            break;
        case(8):
            strcpy(digest, "dnLQBKaY2vcy3UHsj2Yv05vGVN0dZcf5JSB");
            strcat(digest, ochar);
            break;
        case(9):
            strcpy(digest, "EQ5YRFm8RxOpKQMvutRUkt5XGTspgSBGweJ");
            strcat(digest, ochar);
            break;
        }

    return;
}

void ClientBase::toString(ClientRequest* message, char * ser_message) {
    // This is just to represent message serialisation in code. We're not interested in actually doing it.
    strcpy(ser_message, "vQ9vkCimCoEYmy9otpGOUHsj2Yv05vGVN0dZcf5J");
}

void ClientBase::displayOutput(std::string output) {

    char out[200];
    strcpy(out,"Client: ");

    char *vchar = (char *)output.c_str();
    strcat(out, vchar);
    strcat(out, "\n");

    EV_DETAIL << out;

}
