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

#ifndef CHECKPOINT_H_
#define CHECKPOINT_H_

#include <omnetpp.h>
#include <list>

using namespace std;
using namespace omnetpp;

class LocalState;
class ChannelState;
class CheckpointMessage;

class Checkpoint {
protected:
    int seq_num;
    LocalState* localStates[11];
    ChannelState* channelStates[20];

    list<CheckpointMessage*> agreeing;

public:
    Checkpoint();
    virtual ~Checkpoint();
    int getSeqNum() const;
    void setSeqNum(int seqNum);
    void dummyExecute(int F);
    list<CheckpointMessage *> getProofMessages();
};

#endif /* CHECKPOINT_H_ */
