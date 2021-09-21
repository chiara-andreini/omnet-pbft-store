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
#include <list>

using namespace std;
using namespace omnetpp;

class ProtocolMessage;
class PrePrepare;
class ViewChange;

#ifndef LOG_H_
#define LOG_H_

class Log : public cSimpleModule {
protected:

    // E.g. {(17, {p: 4, q: 0, c: 3, k: 1})}
    map<int, map<string, int>> counters;
    // E.g. A replica keeps track of how many view change messages it has received
    // for view 685 {(685, {"vc": 4})}
    map<int, map<string, int>> viewChangeCounters;

    std::list<ProtocolMessage*> log;
    std::map<int, PrePrepare*> ongoing;
    list<ViewChange*> viewChangeList;

    int viewChangeListSize;

    virtual void initialize() override;

    void displayConsoleOutput(std::string output);

public:
    Log();

    bool lookup(ProtocolMessage* msg);
    bool lookupViewChange(ViewChange* msg);
    bool isThereConflict(ProtocolMessage* msg);
    bool isTherePrePrepareMatch(const char *c_message, int sq_num, int code);
    bool isTherePrepareQuorum(const char *c_message, int sq_num, int F, int code);
    bool isThereCommitQuorum(const char *c_message, int sq_num, int F, int code);
    void initNewMessageCounter(int seq_num);
    void initViewChangeMessageCounter (int new_view);
    void append(ProtocolMessage* msg);
    simtime_t getRequestTimestampFor(int sq_num);
    int getOriginatorFor(int sq_num);
    int getClientIdFor(int sq_num);
    void display();
    void displayOutput(std::string output);
    void displayViewChangeLog();
    bool isThereViewChangeQuorum(int new_view_code, int F);
    void appendViewChange(ViewChange* viewChange);

    const list<ViewChange*>& getViewChangeList() const {
        return viewChangeList;
    }

    const std::map<int, PrePrepare*>& getOngoing() const {
        return ongoing;
    }

    int getViewChangeListSize() const;
};

#endif /* LOG_H_ */
