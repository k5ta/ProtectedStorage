#ifndef PS_GOST3411_H
#define PS_GOST3411_H

#include <stdint.h>

namespace gosthash {

	void hash256(const uint8_t* message, const uint64_t& length, uint8_t* out);

}

#endif

