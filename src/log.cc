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

#include "log.h"
#include "ProtocolMessages_m.h"
#include "ViewChangeMessages_m.h"

Log::Log() {

}

void Log::initialize() {
    viewChangeListSize = 0;
}

bool Log::lookupViewChange(ViewChange *msg) {

    std::list<ViewChange*>::iterator it = viewChangeList.begin();

    while (it != viewChangeList.end()){
        if (((ViewChange*)(*it))->getSenderOverlayKey() == ((ViewChange*)(msg))->getSenderOverlayKey()) {
            // (!) Simplified view change simulation: just one view change is involved, so "true" means
            // I already have a view change from that sender
            return true;
        }
        it++;
    }
    return false;

}

bool Log::lookup(ProtocolMessage *msg) {

    std::list<ProtocolMessage*>::iterator it = log.begin();

    // Iterate over the map looking for a message of the same type that has the same client message digest.
    while (it != log.end()){

        if ((*msg->getClientMessageDigest()) == (*(*it)->getClientMessageDigest())
                && msg->getSequenceNumber() == (*it)->getSequenceNumber()) {

           if (dynamic_cast<PrePrepare*> (msg) != nullptr && dynamic_cast<PrePrepare*> ((*it)) != nullptr)
            // EV_DETAIL << "Message already in. \n";
               return true;
           if (dynamic_cast<Prepare*> (msg) != nullptr && dynamic_cast<Prepare*> ((*it)) != nullptr) {
               if (((Prepare*)(*it))->getSenderOverlayKey() == ((Prepare*)(msg))->getSenderOverlayKey()) {
                   return true;
               }
           }

        }
        it++;
    }
    return false;

}

void Log::append(ProtocolMessage *msg) {

    log.push_back(msg);

    if (dynamic_cast<PrePrepare*> (msg) != nullptr) {

        PrePrepare* prep = dynamic_cast<PrePrepare*> (msg);
        // If it's a preprepare, and only one gets appended per sequence number, then register it in ongoing.
        ongoing.insert(pair<int,PrePrepare*>(msg->getSequenceNumber(), (PrePrepare*)msg));
    }

    if (dynamic_cast<Prepare*> (msg) != nullptr) {
        counters[msg->getSequenceNumber()]["p"] = counters[msg->getSequenceNumber()]["p"] + 1;
    }

    if (dynamic_cast<Commit*> (msg) != nullptr) {
        counters[msg->getSequenceNumber()]["c"] = counters[msg->getSequenceNumber()]["c"] + 1;
    }
}

bool Log::isThereConflict(ProtocolMessage *msg) {

    std::list<ProtocolMessage*>::iterator it = log.begin();

    while (it != log.end()){
        if (dynamic_cast<PrePrepare*> (msg) != nullptr && dynamic_cast<PrePrepare*> ((*it)) != nullptr &&
            (*it)->getSequenceNumber() == msg->getSequenceNumber() &&
            (*it)->getClientMessageDigest() != msg->getClientMessageDigest())
            return true;
        it++;
    }
    return false;

}

void Log::display() {

    std::list<ProtocolMessage*>::iterator it = log.begin();

    std::string type;

    std::string output;

    while (it != log.end()){

        if (dynamic_cast<PrePrepare*>(*it) != nullptr)
            type = "[Preprepare, ";
        if (dynamic_cast<Prepare*>(*it) != nullptr)
            type = "[Prepare, ";

        // TODO: add the others.

        std::string op((*it)->getClientMessage());

        output += type + op + ", " + std::to_string((*it)->getSequenceNumber());

        if (dynamic_cast<Prepare*>(*it) != nullptr)
            output += ", Replica " + std::to_string(((Prepare*)(*it))->getSenderOverlayKey());

        output += "]";
        it++;
    }

    output += ". \n";

    EV_DETAIL << output;
}

// TODO: trying
void Log::displayViewChangeLog() {

    std::list<ViewChange*>::iterator it = viewChangeList.begin();

    std::string type = "[ViewChange, ";

    std::string output;

    while (it != viewChangeList.end()){

        output += type + std::to_string((*it)->getSenderOverlayKey());

        output += "]";
        it++;
    }

    output += ". \n";

    EV_DETAIL << output;
}


bool Log::isTherePrePrepareMatch(const char *c_message, int sq_num, int code) {
    // TODO: implement this to simply check the ongoing map
    return true;
}

bool Log::isTherePrepareQuorum(const char *c_message, int sq_num, int F, int code) {
    string a = to_string(counters[sq_num]["p"]);
    displayConsoleOutput("Checking for quorum with " + a + " prepares");

    if (counters[sq_num]["q"] == 1)
        // Quorum already dealt with.
        return false;
    else if (counters[sq_num]["p"] < 2 * F + 1)
        return false;
    else {
        counters[sq_num]["q"] = 1;
        return true;
    }

}

void Log::displayConsoleOutput(std::string output) {

    char out[200];

    char *vchar = (char *)output.c_str();
    strcpy(out, vchar);
    strcat(out, "\n");

    EV_DETAIL << out;

}

void Log::initNewMessageCounter(int seq_num) {

    map<string,int> init;
    // init.insert(pair<string, int>("pp", 1));
    init.insert(pair<string, int>("p", 0));
    // TODO: I'm just trying this: we might have a q = 1 to denote the quorum has already been seen and considered,
    // (so, no need to rebuild the certificate nor respread the commits).
    init.insert(pair<string, int>("q", 0));
    init.insert(pair<string, int>("c", 0));
    init.insert(pair<string, int>("k", 0));

    counters.insert(pair<int,map<string,int>>(
            seq_num,
            init));
    // this->displayConsoleOutput("I added a new counting pair in the counters map.");
}

bool Log::isThereCommitQuorum(const char *c_message, int sq_num, int F, int code) {
    string c = to_string(counters[sq_num]["c"]);
    displayConsoleOutput("Checking for quorum with " + c + " commits");

    if (counters[sq_num]["k"] == 1)
        // Quorum already dealt with.
        return false;
    else if (counters[sq_num]["c"] < 2 * F + 1)
        return false;
    else {
        counters[sq_num]["k"] = 1;
        return true;
    }

}

simtime_t Log::getRequestTimestampFor(int sq_num) {
    return ongoing[sq_num]->getTimestamp();
}

int Log::getOriginatorFor(int sq_num) {
    return ongoing[sq_num]->getOriginator();
}

int Log::getClientIdFor(int sq_num) {
    return ongoing[sq_num]->getClientId();
}

void Log::displayOutput(std::string output) {

    char out[200];
    strcpy(out,"Log: ");

    char *vchar = (char *)output.c_str();
    strcat(out, vchar);
    strcat(out, "\n");

    EV_DETAIL << out;

}

bool Log::isThereViewChangeQuorum(int new_view_code, int F) {
    if (viewChangeCounters[new_view_code]["k"] == 1)
        // Quorum already dealt with, view already changed, nothing should be done.
        return false;
    else if (viewChangeCounters[new_view_code]["vc"] < 2 * F)
        return false;
    else {
        viewChangeCounters[new_view_code]["k"] = 1;
        return true;
    }
}

void Log::initViewChangeMessageCounter (int new_view) {
    map<string,int> init;
    init.insert(pair<string, int>("k", 0));
    init.insert(pair<string, int>("vc", 0));
    viewChangeCounters.insert(pair<int,map<string,int>>(new_view, init));
}

void Log::appendViewChange(ViewChange *viewChange) {
    viewChangeList.push_back(viewChange);
    this->viewChangeListSize++;
    viewChangeCounters[viewChange->getNew_view()]["vc"] = viewChangeCounters[viewChange->getNew_view()]["vc"] + 1;

}

int Log::getViewChangeListSize() const {
    return viewChangeListSize;
}
