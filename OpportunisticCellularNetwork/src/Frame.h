

#ifndef FRAME_H_
#define FRAME_H_

#include <vector>
#include "Packet.h"

class Frame : public std::vector<Packet> {

public:
    Frame();

};

#endif /* FRAME_H_ */
