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
// along with this program. If not, see http://www.gnu.org/licenses/.
// 

#include "ReplicaBase.h"
#include <omnetpp.h>
#include <string.h>
#include <algorithm>
#include "View.h"
#include "Checkpoint.h"
#include "ProtocolMessages_m.h"
#include "ClientReply_m.h"
#include "ViewChangeMessages_m.h"
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
class NewView;
class View;
class ClientBase;
class ProtocolMessage;
class PrePrepare;
class Prepare;
class Commit;
class Certificate;
class Checkpoint;
class Log;

#ifndef REPLICABASE_H_
#define REPLICABASE_H_


class ReplicaBase : public cSimpleModule {
public:

    View* view;

    ReplicaBase();

protected:

    /// Relevant attributes for all replicas

    // PKI keys
    char private_key[65];
    char public_key[65];

    int overlayKey;
    bool amPrimary;

    int mode;   // The protocol the network is operating with.

    Log* log;
    std::map index;

    /// Relevant attributes for the primary

    // Primary's load roof.
    int maxNumberofRequests;


    /// Generic operations
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg);
    void displayOutput(std::string output);
    void setKeys();
    void AES_encrypt(char * ciphertext, char * key);
    void AES_hash(char * ser_request, char * digest);
    bool verifyMessageIntegrity(cMessage * msg);

    void handleTimer(cMessage* msg);

    void handlePrePrepare(PrePrepare* preprepare);
    void handlePrepare(Prepare* prepare);
    bool isTherePrepareQuorum(const char * c_message, int sq_num, int code);
    void handleCommit(Commit* commit);

    /// Primary replica's operations

    void handleHeartbeatTimer(cMessage * msg);
    void handleClientRequest(ClientRequest* c_request);


    /// Backup replica's operations

    // Method to send the node's contribution to the client request.
    void sendVote(cMessage* msg, cModule* targetModule);
    void handleViewChange(ViewChange *v_change);

    void sendToPrimary(cMessage* msg);
    void flood(cMessage* msg);
    void broadcast(cMessage* msg);

};

#endif /* REPLICABASE_H_ */

#include "ClientRequest_m.h"
#include "log.h"

using namespace std;
using namespace omnetpp;

Define_Module(ReplicaBase);

ReplicaBase::ReplicaBase() {}

void ReplicaBase::initialize() {

    mode = 0;

    overlayKey = par("overlayKey");
    setKeys();

    // Setting up the initial view,
    view = new View(
            (int)this->getParentModule()->getParentModule()->par("viewCode"),
            (int)par("numOfReplicas")
        );

    log = new Log();

    // These are set up here only for demonstration purposes: in real applications these will be dynamically
    // updated to set coherent thresholds for sequence numbers depending on the operations throughput.
    view->setH(100);
    view->setL(10);

    amPrimary = false;
    if (overlayKey == view->getPrimaryOverlayKey()) {
        amPrimary = true;

        scheduleAt(simTime(), new cMessage("greeting"));
    }

    if ((int)this->getParentModule()->getParentModule()->par("mode") == 1) {

        // Setting up a dummy checkpoint.
        this->checkpoint = new Checkpoint();
        checkpoint->setSeqNum(36);
        checkpoint->dummyExecute(view->getF());

        if (overlayKey == 7) {
            // Initial View Change scenario: replica 7 has received a request for sequence number 36 (correctly committed),
            // number 41 and number 42.

            // The replica would be stuck waiting for request with sequence number 37, so
            // when it got the request for sequence number 41, it set a timer and kept on doing its job.
            // At most, request 41 and 42 could make it to the fully prepared stage, but not committed because
            // 37 is still missing.

            PrePrepare* request1 = new PrePrepare();
            char mess[100];
            strcpy(mess, "GET");
            strcat(mess,"(\"");
            strcat(mess, "fwki23641hdlju85d52a");
            strcat(mess, "\")");
            request1->setClientMessage(mess);
            request1->setSequenceNumber(36);
            log->append(request1);
            displayOutput("I got a PrePrepare for sequence number " + to_string(36) + ".");
            PrePrepare* request2 = new PrePrepare();
            strcpy(mess, "PUT");
            strcat(mess,"(\"");
            strcat(mess, "go9wn48u3qdb7fe4qdk2, ");
            strcat(mess, "{dummy: 42}\")");
            request2->setClientMessage(mess);

            // Replica realises it's missing something...

            request2->setSequenceNumber(41);
            log->append(request2);
            scheduleAt(simTime() + viewChangeTimeout, new cMessage("view_change"));
            displayOutput("I got a PrePrepare for sequence number " + to_string(41) + ".");


            PrePrepare* request3 = new PrePrepare();
            strcpy(mess, "GET");
            strcat(mess,"(\"");
            strcat(mess, "23498fqn209mfw30ewd1");
            strcat(mess, "\")");
            request3->setClientMessage(mess);
            request3->setSequenceNumber(42);
            log->append(request3);
            displayOutput("I got a PrePrepare for sequence number " + to_string(42) + ".");

            // We simulate the case in which the timer at Replica 7 elapses and only for request 41 the
            // network correctly completed the prepare stage.
            Prepare* p;

            for (int i = 0; i <= 2 * view->getF(); i++) {
                p = new Prepare();
                p->setSenderOverlayKey(overlayKey);
                p->setSequenceNumber(request2->getSequenceNumber());
                p->setClientMessage(mess);

                char digest[37];
                AES_hash(mess, digest);
                p->setClientMessageDigest(digest);

                // Serialising the message.
                char ser_content[100];
                toString(p, ser_content);
                char ser_content_cyphertext[65];
                AES_encrypt(ser_content, ser_content_cyphertext, private_key);
                p->setSignature(ser_content_cyphertext);

                log->append(p);
            }

        }

    }

}

void ReplicaBase::handleMessage(cMessage *msg) {

    /// Case 1: the message is the self message of the primary
    if (msg->isSelfMessage()) {

        const char* s_name = msg->getName();

        if (string(s_name) == "greeting") {
            displayOutput("Hello, this is view " + std::to_string(view->getCode()) + " so I'm the primary.");
            this->getParentModule()->bubble("Primary here!");
        }

        if (string(s_name) == "view_change") {
            displayOutput("Timeout! The primary is faulty, I'm starting a view change.");
            this->getParentModule()->bubble("View change!");

            this->mode = 1;

            ViewChange* v_change = sendViewChange(view->getCode() + 1);
            v_change->setSenderOverlayKey(overlayKey);
            this->broadcast(v_change);
        }

    }


    /// Case 2: the message is a client request
    // If I'm not the primary, I relay it to it, if I am I process it.
    ClientRequest * cr = dynamic_cast<ClientRequest*> (msg);
        if (cr != nullptr)
            handleClientRequest(cr);

    /// Case 3: the message is a preprepare from the primary
    PrePrepare * preprepare = dynamic_cast<PrePrepare*> (msg);
    if (preprepare != nullptr && mode == 0) {

        // Checking the bitmap, if I've not already seen it, I'm processing it now.
        if (preprepare->getBitmap(overlayKey) == 0) {

            preprepare->setBitmap(overlayKey, 1);

            // Check if I already processed it (and accepted it) and it was the sender that was not aware.
            bool hit = log->lookup(preprepare);

            // Flooding the network with the preprepare
            this->flood(preprepare);

            if (hit == false)
                handlePrePrepare(preprepare);
        }
        // else displayOutput("I already processed this.");
    }

    /// Case 3: the message is a preprepare from the primary
    Prepare * prepare = dynamic_cast<Prepare*> (msg);
    if (prepare != nullptr && mode == 0) {

        if (prepare->getBitmap(overlayKey) == 0 && isThereCertificate(prepare->getSequenceNumber(), "p") == false) {

            prepare->setBitmap(overlayKey, 1);

            bool hit = log->lookup(prepare);

            this->flood(prepare);

            if (hit == false)
                handlePrepare(prepare);
        }
    }

    /// Case 4: the message is a commit from the primary
    Commit * commit = dynamic_cast<Commit*> (msg);
    if (commit != nullptr && mode == 0) {
        if (commit->getBitmap(overlayKey) == 0 && isThereCertificate(commit->getSequenceNumber(), "c") == false) {

            commit->setBitmap(overlayKey, 1);

            bool hit = log->lookup(commit);

            this->flood(commit);

            if (hit == false)
                handleCommit(commit);
        }
    }

    /// Case 5: the message is a client reply to deliver to the client
    ClientReply * creply = dynamic_cast<ClientReply*> (msg);
    if (creply != nullptr && mode == 0) {
        sendToOriginator(creply);

        if (creply->getOriginator() == overlayKey) {
            send(creply, "g$o", 2);
        }
    }

    ViewChange* vch = dynamic_cast<ViewChange*> (msg);
    if (vch != nullptr) {

        if (vch->getBitmap(overlayKey) == 0 && vch->getNew_view() != this->view->getCode()
                && log->getViewChangeListSize() <= 5) {

            vch->setBitmap(overlayKey, 1);

            bool hit = log->lookupViewChange(vch);

            this->flood(vch);

            if (hit == false)
                handleViewChange(vch);

        }
    }

    /// Case 7: the message is a new view announcement
    NewView* nv = dynamic_cast<NewView*> (msg);
    if (nv != nullptr) {

        if (nv->getNew_view() != view->getCode()) {
            if (verifyMessageIntegrity(msg) == true && verifySetOCorrectness() == true ) {
                view = new View(nv->getNew_view(), (int)par("numOfReplicas") - 1);
            }

            this->getParentModule()->bubble("View change occurred!");
            displayOutput("View change occurred, this is view " + std::to_string(nv->getNew_view()) + ".");

            this->flood(nv);
        }



    }

}

void ReplicaBase::handleViewChange(ViewChange * vch) {

    log->appendViewChange(vch);

    displayOutput("My log now contains:");
    log->displayViewChangeLog();

    if (verifyMessageIntegrity(vch) == true) {
        ViewChange * myVC = new ViewChange();
        myVC->setNew_view(vch->getNew_view());
        myVC->setCp_seq_num(vch->getCp_seq_num());
        myVC->setSenderOverlayKey(overlayKey);
        myVC->setBitmap(overlayKey, 1);
        broadcast(myVC);
    }

    // If I'm the primary, I need to process that view change message.
    if (view->getNextPrimaryOverlayKey() == overlayKey) {

        if (log->isThereViewChangeQuorum(vch->getNew_view(), view->getF())) {
            // Send new view announcements
            amPrimary = true;
            displayOutput("I am the new primary and this is view " + std::to_string(vch->getNew_view()) + ".");

            ViewChange* newPrimaryViewChange = sendViewChange(vch->getNew_view());
            list<ViewChange*> v(log->getViewChangeList());
            v.push_back(newPrimaryViewChange);

            list<PrePrepare*> o;
            std::map<int, PrePrepare*> ongoing = log->getOngoing();
            map<int, PrePrepare*>::iterator it;

            for (it = ongoing.begin(); it != ongoing.end(); it++){
                o.push_back(it->second);
            }

            NewView* nview = new NewView();
            nview->setNew_view(vch->getNew_view());
            nview->setPps(o);
            nview->setVcs(v);

            this->broadcast(nview);
        }

    }

}


bool ReplicaBase::verifySetOCorrectness() {
    return true;
}

void ReplicaBase::handleClientRequest(ClientRequest *crequest) {
    // The first replica receiving a client request with the field unset sets the clientId
    // it and becomes the originator.
    if (crequest->getOriginator() == -1)
        crequest->setOriginator(overlayKey);
    if (crequest->getClientId() == -1) {
        // Set the clientID randomly, this is just to represent the originator distinguishing
        // clients to route replies correctly.
        srand (time(NULL));
        crequest->setClientId(rand());
    }

    sendToPrimary(crequest);

    if (amPrimary) {

       displayOutput("I've just received a client request.");

       bool isWellFormed = verifyMessageIntegrity(crequest);
       if (!isWellFormed){
           displayOutput("The client request I've received is inconsistent, ignoring it.");
           return;
       }

       // The primary proposes a sequence number.
       srand (time(NULL));
       int seq_num = rand() % 10 + 10;

       PrePrepare * prep = new PrePrepare();
       prep->setView(view->getCode());
       prep->setSequenceNumber(seq_num);
       prep->setOriginator(crequest->getOriginator());
       prep->setTimestamp(crequest->getTimestamp());
       prep->setClientId(crequest->getClientId());

       if (string(crequest->getOperation()) == "GET") {
           char mess[150];
           strcpy(mess, crequest->getOperation());
           strcat(mess,"(\"");
           strcat(mess, crequest->getKey());
           strcat(mess, "\")");

           prep->setClientMessage(mess);
           char digest[37];
           AES_hash(mess, digest);
           prep->setClientMessageDigest(digest);
           displayOutput("Proposing operation " + string(mess)  + " with sequence number "
                   + std::to_string(seq_num) + ".");
       } else {
           char mess[300];
           strcpy(mess, crequest->getOperation());
           strcat(mess,"(\"");
           strcat(mess, crequest->getKey());
           strcat(mess, "\",");
           strcat(mess, crequest->getValue());
           strcat(mess, "\")");

           prep->setClientMessage(mess);
           char digest[37];
           AES_hash(mess, digest);
           prep->setClientMessageDigest(digest);
           displayOutput("Proposing operation " + string(mess)  + " with sequence number "
                   + std::to_string(seq_num) + ".");
       }

       // Serialise the proposal (the preprepare without the client message) and sign it.
       char ser_content[100];
       toString(prep, ser_content);
       char ser_content_cyphertext[65];
       AES_encrypt(ser_content, ser_content_cyphertext, private_key);
       prep->setSignature(ser_content_cyphertext);

       prep->setBitmap(overlayKey, 1);

       // Broadcasting and processing the preprepare myself.
       this->broadcast(prep);
       handlePrePrepare(prep);

    }

    return;
}


void ReplicaBase::handlePrePrepare(PrePrepare* preprepare) {

    std::string str(preprepare->getClientMessage());
    displayOutput("Processing preprepare for operation " + str  + " with "
            "sequence number "  + std::to_string(preprepare->getSequenceNumber()) + ".");

    if (verifyMessageIntegrity(preprepare) == true && preprepare->getView() == view->getCode() &&
            preprepare->getSequenceNumber() >= view->getL() && preprepare->getSequenceNumber() <= view->getH()) {
        // If I get here it means the preprepare is formally correct and meaningful for this view:
        // just one last check i.e. the sequence number was not proposed before in the same view.

        if (log->isThereConflict(preprepare) == false) {
            std::string str(preprepare->getClientMessage());
            displayOutput("I accept operation " + str  + " to be carried out with sequence number "
                    + std::to_string(preprepare->getSequenceNumber()) + ".");

            log->append(preprepare);
            log->initNewMessageCounter(preprepare->getSequenceNumber());

            displayOutput("My log now contains:");
            log->display();
        }

        // Broadcasting a Prepare message.
        Prepare * prepare = new Prepare();

        prepare->setView(view->getCode());
        prepare->setSequenceNumber(preprepare->getSequenceNumber());
        prepare->setClientMessage(preprepare->getClientMessage());
        prepare->setBitmap(overlayKey, 1);

        // Serialisation of the body and signing.
        char ser_content[100];
        toString(prepare, ser_content);
        char ser_content_cyphertext[65];
        AES_encrypt(ser_content, ser_content_cyphertext, private_key);
        prepare->setSignature(ser_content_cyphertext);

        // Broadcasting the prepare.
        this->broadcast(prepare);

    }
}

void ReplicaBase::handlePrepare(Prepare* prepare) {
    std::string str(prepare->getClientMessage());

    displayOutput("Processing a prepare from replica "+ std::to_string(prepare->getSenderOverlayKey()) +" for operation " + str
            + ". It accepted sequence number "  + std::to_string(prepare->getSequenceNumber()) + ".");

    if (verifyMessageIntegrity(prepare) == true && prepare->getView() == view->getCode() &&
        prepare->getSequenceNumber() >= view->getL() && prepare->getSequenceNumber() <= view->getH()
        && log->isTherePrePrepareMatch(prepare->getClientMessage(), prepare->getSequenceNumber(),
            prepare->getView())){
        displayOutput("Its announcement is sensible, I'm storing it.");
        log->append(prepare);

        displayOutput("My log now contains:");
        log->display();

        Prepare * myPrep = new Prepare();
        myPrep->setView(prepare->getView());
        myPrep->setSequenceNumber(prepare->getSequenceNumber());
        myPrep->setClientMessage(prepare->getClientMessage());
        myPrep->setClientMessageDigest(prepare->getClientMessageDigest());
        myPrep->setSenderOverlayKey(overlayKey);
        myPrep->setBitmap(overlayKey, 1);

        // Serialisation of the body and signing.
        char ser_content[100];
        toString(myPrep, ser_content);
        char ser_content_cyphertext[65];
        AES_encrypt(ser_content, ser_content_cyphertext, private_key);
        myPrep->setSignature(ser_content_cyphertext);

        broadcast(myPrep);

        if (log->isTherePrepareQuorum(
                prepare->getClientMessage(),
                prepare->getSequenceNumber(),
                view->getF(),
                prepare->getView()) == true
            ){
            displayOutput("Me and other replicas have reached a quorum for operation " + string(prepare->getClientMessage()) + ".");

            PrepareCertificate * prepareCert = new PrepareCertificate();
            prepareCert->setClientMessage(prepare->getClientMessage());
            prepareCert->setView(view->getCode());
            prepareCert->setIssuerOverlayKey(overlayKey);
            prepareCert->setSequenceNumber(prepare->getSequenceNumber());

            map<string, Certificate*> init;
            init.insert(pair<string, Certificate*>("p", prepareCert));
            init.insert(pair<string, Certificate*>("c", nullptr));

            certificates.insert(pair<int,map<string,Certificate*>>(
                        prepare->getSequenceNumber(),
                        init));


            displayOutput("I'm issuing and storing the prepare certificate.");


            Commit* commit = new Commit();
            commit->setView(view->getCode());
            commit->setSequenceNumber(myPrep->getSequenceNumber());
            commit->setClientMessage(prepare->getClientMessage());
            commit->setClientMessageDigest(myPrep->getClientMessageDigest());

            // Serialisation of the body and signing.
            char ser_content[100];
            toString(commit, ser_content);
            char ser_content_cyphertext[65];
            AES_encrypt(ser_content, ser_content_cyphertext, private_key);
            commit->setSignature(ser_content_cyphertext);

            this->broadcast(commit);
        }

    }

}

void ReplicaBase::handleCommit(Commit* commit) {

    displayOutput("Processing a commit from replica "+ std::to_string(commit->getSenderOverlayKey()) +" for operation " +
            string(commit->getClientMessage()) + ". It accepted sequence number "  + std::to_string(commit->getSequenceNumber()) + ".");

    if (verifyMessageIntegrity(commit) == true && commit->getView() == view->getCode() &&
            commit->getSequenceNumber() >= view->getL() && commit->getSequenceNumber() <= view->getH()){
        displayOutput("Its commit is sensible, I'm storing it.");
        log->append(commit);

        displayOutput("My log now contains:");
        log->display();

        Commit * myComm = new Commit();
        myComm->setView(commit->getView());
        myComm->setSequenceNumber(commit->getSequenceNumber());
        myComm->setClientMessage(commit->getClientMessage());
        myComm->setClientMessageDigest(commit->getClientMessageDigest());
        myComm->setSenderOverlayKey(overlayKey);
        myComm->setBitmap(overlayKey, 1);

        // Serialisation of the body and signing.
        char ser_content[100];
        toString(myComm, ser_content);
        char ser_content_cyphertext[65];
        AES_encrypt(ser_content, ser_content_cyphertext, private_key);
        myComm->setSignature(ser_content_cyphertext);

        broadcast(myComm);

        if (log->isThereCommitQuorum(
             commit->getClientMessage(),
             commit->getSequenceNumber(),
             view->getF(),
             commit->getView()) == true && certificates[commit->getSequenceNumber()]["p"] != nullptr
         ){
            displayOutput("Me and other replicas have reached a quorum for operation " + string(commit->getClientMessage()) + ".");

            CommitLocalCertificate * commitCert = new CommitLocalCertificate();
            commitCert->setClientMessage(commit->getClientMessage());
            commitCert->setView(view->getCode());
            commitCert->setIssuerOverlayKey(overlayKey);
            commitCert->setSequenceNumber(commit->getSequenceNumber());

            displayOutput("I'm issuing and storing the commit certificate.");
            certificates[commit->getSequenceNumber()]["c"] = commitCert;

            string result = executeStoreOperation(commit->getClientMessage(), commit->getSequenceNumber());

            ClientReply* creply = new ClientReply();
            creply->setTimestamp(log->getRequestTimestampFor(commit->getSequenceNumber()));
            creply->setClientMessage(commit->getClientMessage());
            creply->setView(view->getCode());
            creply->setSender(overlayKey);
            creply->setOriginator(log->getOriginatorFor(commit->getSequenceNumber()));
            char * r_char = (char *)result.c_str();
            creply->setResult(r_char);
            creply->setClientId(log->getClientIdFor(commit->getSequenceNumber()));

            creply->setClientMessageDigest(commit->getClientMessageDigest());

            // Serialising the message.
            char ser_content[100];
            toString(creply, ser_content);
            char ser_content_cyphertext[65];
            AES_encrypt(ser_content, ser_content_cyphertext, private_key);
            creply->setSignature(ser_content_cyphertext);

            this->sendToOriginator(creply);
        }
    }
}


void ReplicaBase::flood(cMessage* msg) {

    PrePrepare * preprepare = dynamic_cast<PrePrepare*> (msg);
    if (preprepare != nullptr) {
        simtime_t timestamp = preprepare->getTimestamp();
        PrePrepare* csmsg;

        cGate *arrivalGate = msg->getArrivalGate();
        for (GateIterator i(this); !i.end(); i++) {
            cGate* gate = *i;
            if (gate->getType() == cGate::OUTPUT && gate->getIndex() != arrivalGate->getIndex()) {
                csmsg = preprepare->dup();
                csmsg->setTimestamp(timestamp);
                send(csmsg, gate);
            }
        }

    } else if (dynamic_cast<ViewChange*> (msg) != nullptr) {
        ViewChange* v_change = dynamic_cast<ViewChange*> (msg);
        ViewChange* csmsg;
        int overlaykey = v_change->getSenderOverlayKey();


        cGate *arrivalGate = msg->getArrivalGate();
        for (GateIterator i(this); !i.end(); i++) {
            cGate* gate = *i;
            if (gate->getType() == cGate::OUTPUT && gate->getIndex() != arrivalGate->getIndex()) {
                csmsg = v_change->dup();

                // Cleaning the bitmap
                for (unsigned int j=0; j<11; j++)
                    csmsg->setBitmap(j, 0);
                csmsg->setSenderOverlayKey(overlaykey);
                send(csmsg, gate);
            }
        }

    } else {
        cGate *arrivalGate = msg->getArrivalGate();
        for (GateIterator i(this); !i.end(); i++) {
            cGate* gate = *i;
            if (gate->getType() == cGate::OUTPUT && gate->getIndex() != arrivalGate->getIndex()) {
                cMessage* csmsg = msg->dup();
                send(csmsg, gate);
            }
        }
    }

}

void ReplicaBase::broadcast(cMessage* msg) {

    PrePrepare * preprepare = dynamic_cast<PrePrepare*> (msg);
    if (preprepare != nullptr) {
        simtime_t timestamp = preprepare->getTimestamp();
        PrePrepare* csmsg;

        for (GateIterator i(this); !i.end(); i++) {
            cGate* gate = *i;
            if (gate->getType() == cGate::OUTPUT) {
                csmsg = preprepare->dup();
                csmsg->setTimestamp(timestamp);
                send(csmsg, gate);
            }
        }

    } else if (dynamic_cast<ViewChange*> (msg) != nullptr) {
        ViewChange* v_change = dynamic_cast<ViewChange*> (msg);
        int overlaykey = v_change->getSenderOverlayKey();

        ViewChange* csmsg;

        for (GateIterator i(this); !i.end(); i++) {
            cGate* gate = *i;
            if (gate->getType() == cGate::OUTPUT) {
                csmsg = v_change->dup();
                // Cleaning the bitmap
                for (unsigned int j=0; j<11; j++)
                    csmsg->setBitmap(j, 0);
                csmsg->setSenderOverlayKey(overlaykey);
                send(csmsg, gate);
            }
        }

    } else {
        for (GateIterator i(this); !i.end(); i++) {
            cGate* gate = *i;
            if (gate->getType() == cGate::OUTPUT) {
                cMessage* csmsg = msg->dup();
                send(csmsg, gate);
            }
        }
    }


}

void ReplicaBase::sendToPrimary(cMessage* msg) {

    if (amPrimary)
        return;

    switch(overlayKey) {
       case 0:
           if (view->getPrimaryOverlayKey() == 9 || view->getPrimaryOverlayKey() == 2)
               send(msg, "g$o", 2);
           else if (view->getPrimaryOverlayKey() == 3 || view->getPrimaryOverlayKey() == 7)
               send(msg, "g$o", 0);
           else
               send(msg, "g$o", 1);
           break;
       case 1:
           send(msg, "g$o", 0);
           break;
       case 2:
           send(msg, "g$o", 0);
           break;
       case 3:
           if (view->getPrimaryOverlayKey() == 7)
               send(msg, "g$o", 0);
           else
               send(msg, "g$o", 1);
           break;
       case 4:
           if (view->getPrimaryOverlayKey() == 1)
               send(msg, "g$o", 1);
           else
               send(msg, "g$o", 0);
           break;
       case 5:
           if (view->getPrimaryOverlayKey() == 1 || view->getPrimaryOverlayKey() == 4)
               send(msg, "g$o", 1);
           else
               send(msg, "g$o", 0);
           break;
       case 6:
           if (view->getPrimaryOverlayKey() == 1 || view->getPrimaryOverlayKey() == 4 || view->getPrimaryOverlayKey() == 5)
               send(msg, "g$o", 0);
           else
               send(msg, "g$o", 1);
           break;
       case 7:
           send(msg, "g$o", 0);
           break;
       case 8:
           if (view->getPrimaryOverlayKey() == 1 || view->getPrimaryOverlayKey() == 4 || view->getPrimaryOverlayKey() == 5
                   || view->getPrimaryOverlayKey() == 6)
               send(msg, "g$o", 0);
           else
               send(msg, "g$o", 1);
           break;
       case 9:
           if (view->getPrimaryOverlayKey() == 2)
               send(msg, "g$o", 1);
           else
               send(msg, "g$o", 0);
           break;
    }
    return;
}


void ReplicaBase::setKeys() {

    switch(overlayKey) {
            case(0):
                strcpy(public_key, "UHsCGAEwT5OmYJfbgrRAK7ztSCCQbOSDaqe7pwVeRJ684GYkObmcjyPkVaxqpo5q");
                strcpy(private_key, "XiR9aG5MKKaRWpmY3d7BwiXdzaI6v4SVwlkKgW6HRC68xI53U5kVXS1OiiueRcKY");
                break;
            case(1):
                strcpy(public_key, "SR139hHVo1mKGdYJnrdVQTIn6QzpbbIGp4QF3JHEekAHJyojVMDsBNspR5R0t62Y");
                strcpy(private_key, "66u14DWoaNgJlQPxXkrfIegxzCf92Ze9eqz6fQ5TkIX0r1TsLtF476c8rzlmRwwX");
                break;
            case(2):
                strcpy(public_key, "n16cSNSA1UOyAGH0FTXMSsLBsU6p6BfPbLFWHlGdiIU0VChOvcrsd9oseBW7kTdE");
                strcpy(private_key, "4iCZvmRftyAzrWdjX5Egh2DHf4GJDnC8GECnkPWp7UVMo1A1m7AWnRAL9NzqxQy8");
                break;
            case(3):
                strcpy(public_key, "WAWEcnBM4jiaooJT89OXKFqndWqNeTkxCRnCvzZnikaTyJITboCV0AV8LjkYGXtI");
                strcpy(private_key, "XJL1VzYS7xhVzYOL6pBSXyTJfoFejg5bz1J5V0ITfv0C6AGDORUdhWw03uMGrTOz");
                break;
            case(4):
                strcpy(public_key, "0npnYRs5pNGhVnTu6m94QOHM3zAaML6AbPuJKWSXxLxXOFvyNA9JixZH0Yhrn5HJ");
                strcpy(private_key, "Edkog3wFzdgSjswVoX8snszM0DgMTQHeKC6xgsumd1IgN2gOgjiLFs0au3Q8gHfX");
                break;
            case(5):
                strcpy(public_key, "smxbTYla4WOcsggnEIkswsSNQxzit04FttyWvQCc6AUaoUJguwbfXbZpKYBSRkO5");
                strcpy(private_key, "OVRxNq3nc8wKYEMETJpUwIwfqBEvh44wqrKomx9SpZqkUCrBl4UdVnDJ87fa8o37");
                break;
            case(6):
                strcpy(public_key, "jiY8DZV0YZmEGK07I1CnmjNiGR3Ulc7ltnq4NLO5Ig33rKoOjuK2W6CeoDg2EQqB");
                strcpy(private_key, "ZYgOmkS8xbjtFbgY8g8iySbWyt2xfMTCS7SlnTdSAGym0kG4Md3uJSqBzpd8WOSI");
                break;
            case(7):
                strcpy(public_key, "Nk3DVdAtWxy2Kho3KKiqZJwHsaDaMxQGjKPguN5iikCPsfuEAe5uYYyZ2aVYgYaI");
                strcpy(private_key, "beSSGnbehmA2BV0eACywdzvuCCdsqqrVKnhz4d68Wrp66WVaMTQZsEp8TfXroBgt");
                break;
            case(8):
                strcpy(public_key, "OQYkrxF7BMv0C6HEI4rD3wkQ0Bea7VA2fnfOBIGIbalPFG46Zzv2dcQloGU3Cwyb");
                strcpy(private_key, "BorUiFOBsegW80avpso77c0jQYOZAaU3asehTI8xMLVq0axU7Dxjigxnaf3Yzy27");
                break;
            case(9):
                strcpy(public_key, "RuH9QttBtaDoDdBgrPMKdwFzpcSkwuN5i45CPsfuEAe5uHhQgdNyMeuLLXSp426S");
                strcpy(private_key, "Rpersfz9jqiUmU1kaa1loCtCfehEHwJZhdXmf7QDcr6LFKbIxFIo32b5YjGnI4Rm");
                break;
            case(10):
                strcpy(public_key, "jSxq6IqsDMH6rrugFw84KgosK24Sn7yqiw0IZiS8x6QSRo4gO0D7u8nMemhsUFgJ");
                strcpy(private_key, "B7CZ4rEKogk5FZKqOoNeavQ0a96F8ZyLaLhxkh7qTSpVb0dUR66xXULc2InjWbDz");
                break;
            case(11):
                strcpy(public_key, "QdQUWoFT45THoQU3cUK4xGPPGcKmLGA35cvhvqhBfq6uPJvfbGgzZBdS0wwDqhIs");
                strcpy(private_key, "kXUQyVrlYUaEkqMeoWHN69tlt4Bwbs0usCa3UJSWPfoFl5Rgaa9ybeYIYiLv5eFS");
                break;
            case(12):
                strcpy(public_key, "WYhZomqmMaQUOh9UqH02G8LignteFKP7YQvS16OEm2C1JSrwv9a5XK7ZaMpJQY5f");
                strcpy(private_key, "bEID4m0kcFLhGJpXaMlry500eIA6JUxWMb4G2WSVswI3Iel1F7AbFu0L8bQppoWn");
                break;
            case(13):
                strcpy(public_key, "ctLfo435ZfXNUpXjibHINza0gPK7yjXfIxkSbR1rTDcselg2g9enExyN21cF5kBh");
                strcpy(private_key, "BUIEdoweqXx30feQX6uVOsrmaxgKrVQtPtrhrkSvtD55AP2SuyftctmMioK9VZ1e");
                break;
        }

}

void ReplicaBase::AES_encrypt(char * plaintext, char * ciphertext, char * key) {

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

void ReplicaBase::AES_hash(char * message, char * digest){

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

bool ReplicaBase::verifyMessageIntegrity(cMessage * msg) {
    // This is just to represent in code the activity of integrity validation i.e. the check of the
    // the hash digest and the message signature. We're not interested in really doing it.
    return true;
}

void ReplicaBase::displayOutput(std::string output) {

    char out[200];
    strcpy(out,"Replica ");

    std::string o = std::to_string(overlayKey);
    char *ochar = (char *)o.c_str();
    strcat(out, ochar);

    if (overlayKey == view->getPrimaryOverlayKey())
        strcat(out, " (primary): ");
    else
        strcat(out, ": ");

    char *vchar = (char *)output.c_str();
    strcat(out, vchar);
    strcat(out, "\n");

    EV_DETAIL << out;

}

void ReplicaBase::toString(cMessage* message, char * ser_message) {
    // This is just to represent message serialisation in code. We're not interested in actually doing it.
    strcpy(ser_message, "vQ9vkCimCoEYmy9otpGOUHsj2Yv05vGVN0dZcf5J");
}

string ReplicaBase::executeStoreOperation(const char *c_message, int sq_num) {
    // This is to simulate the execution of the operation, not interested in doing it in reality.
    std::string str(c_message);
    this->getParentModule()->bubble("Executing operation!");
    displayOutput("Executing operation " + str  + " with sequence number " + to_string(sq_num) + ".");

    if (str.find("GET") != std::string::npos) {
        return "{code: 200, field1: fwel234u81hd, field2: eriflsjei2398}";
    } else
        return "{code: 200}";
}

void ReplicaBase::sendToOriginator(ClientReply *response) {

    if (response->getOriginator() == overlayKey)
        return;

    switch(overlayKey) {
       case 0:
           if (response->getOriginator() == 9 || response->getOriginator() == 2)
               send(response, "g$o", 2);
           else if (response->getOriginator() == 3 || response->getOriginator() == 7)
               send(response, "g$o", 0);
           else
               send(response, "g$o", 1);
           break;
       case 1:
           send(response, "g$o", 0);
           break;
       case 2:
           send(response, "g$o", 0);
           break;
       case 3:
           if (response->getOriginator() == 7)
               send(response, "g$o", 0);
           else
               send(response, "g$o", 1);
           break;
       case 4:
           if (response->getOriginator() == 1)
               send(response, "g$o", 1);
           else
               send(response, "g$o", 0);
           break;
       case 5:
           if (response->getOriginator() == 1 || response->getOriginator() == 4)
               send(response, "g$o", 1);
           else
               send(response, "g$o", 0);
           break;
       case 6:
           if (response->getOriginator() == 1 || response->getOriginator() == 4 || response->getOriginator() == 5)
               send(response, "g$o", 0);
           else
               send(response, "g$o", 1);
           break;
       case 7:
           send(response, "g$o", 0);
           break;
       case 8:
           if (response->getOriginator() == 1 || response->getOriginator() == 4 || response->getOriginator() == 5
                   || response->getOriginator() == 6)
               send(response, "g$o", 0);
           else
               send(response, "g$o", 1);
           break;
       case 9:
           if (response->getOriginator() == 2)
               send(response, "g$o", 1);
           else
               send(response, "g$o", 0);
           break;
    }
    return;
}

bool ReplicaBase::isThereCertificate(int sq_num, string cert) {
    if (certificates.find(sq_num) == certificates.end()) {
      return false;
    } else {
        if (cert == "p") {
           return true;
        } if (cert == "c") {
            if (certificates[sq_num]["c"] == nullptr)
                return false;
            else {
                return true;
            }
        }
    }
}

ViewChange* ReplicaBase::sendViewChange(int new_view) {
    ViewChange * v_change = new ViewChange();

     v_change->setNew_view(view->getCode() + 1);
     v_change->setCp_seq_num(checkpoint->getSeqNum());

     list<CheckpointMessage*> cpMessages;
     cpMessages = checkpoint->getProofMessages();
     v_change->setC(cpMessages);

     // Serialising the message.
     char ser_content[100];
     toString(v_change, ser_content);
     char ser_content_cyphertext[65];
     AES_encrypt(ser_content, ser_content_cyphertext, private_key);
     v_change->setSignature(ser_content_cyphertext);

     // Let's pretend that, of the three requests that replica 7 received (for sequence numbers 36, 41 and 42)
     // it got the Prepare phase done correctly for (36, committed) and 41. So, in its view change it needs to communicate
     // request for sequence number 41 (paired with the PrePrepare and 2f correct Prepares for it).

     // Forming 41's p_m set
     list<ProtocolMessage*> p_m;
     int counter = 0;

     std::list<ProtocolMessage*>::iterator it = p_m.begin();

     while (it != p_m.end()){
         if (dynamic_cast<PrePrepare*> ((*it)) != nullptr && (*it)->getSequenceNumber() == 41)
             p_m.push_back((*it));
         else if (dynamic_cast<Prepare*> ((*it)) != nullptr && (*it)->getSequenceNumber() == 41 &&
                 counter < 2 * view->getF()) {
             p_m.push_back((*it));
             counter++;
         }
         it++;
     }

     return v_change;

}
