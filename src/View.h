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
#include <string.h>

using namespace std;
using namespace omnetpp;

class ViewChange;
class ReplicaBase;

#ifndef VIEW_H_
#define VIEW_H_

class View {

protected:
    int numberOfReplicas;
    int primaryOverlayKey;
    int code;

    // Given the current view, the client can tolerate up to F number of failures.
    int F;

    // Within the current view, sequence numbers for operations can take values within these bounds.
    int L;
    int H;


public:

    View(int viewCode, int numberOfReplicas);
    int getPrimaryOverlayKey() const;
    int getNextPrimaryOverlayKey() const;
    int getCode() const;
    int getH() const;
    void setH(int h);
    int getL() const;
    void setL(int l);
    int getF() const;
};

#endif /* VIEW_H_ */
