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

#include "View.h"
#include <omnetpp.h>
#include <string.h>


View::View(int viewCode, int numberOfReplicas) {
    code = viewCode;
    this->numberOfReplicas = numberOfReplicas;
    primaryOverlayKey = (viewCode % numberOfReplicas);
    F = (int) numberOfReplicas / 3;
}

int View::getPrimaryOverlayKey() const {
    return primaryOverlayKey;
}

int View::getNextPrimaryOverlayKey() const {
    return (code + 1) % (numberOfReplicas - 1);
}

int View::getCode() const {
    return code;
}

int View::getH() const {
    return H;
}

void View::setH(int h) {
    H = h;
}

int View::getL() const {
    return L;
}

void View::setL(int l) {
    L = l;
}

int View::getF() const {
    return F;
}
