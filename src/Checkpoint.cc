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

#include "Checkpoint.h"
#include "ViewChangeMessages_m.h"

Checkpoint::Checkpoint() {
    // TODO Auto-generated constructor stub
}

int Checkpoint::getSeqNum() const {
    return seq_num;
}

void Checkpoint::setSeqNum(int seqNum) {
    seq_num = seqNum;
}

Checkpoint::~Checkpoint() {
    // TODO Auto-generated destructor stub
}

void Checkpoint::dummyExecute(int F) {

    CheckpointMessage * cp_mess;

    for (int i = 0; i < (2*F) + 1; i++) {
        cp_mess = new CheckpointMessage();
        agreeing.push_back(cp_mess);
    }
}

list<CheckpointMessage*> Checkpoint::getProofMessages() {
    return this->agreeing;
}
