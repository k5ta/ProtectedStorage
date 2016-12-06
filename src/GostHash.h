#ifndef PS_GOST3411_H
#define PS_GOST3411_H

#include <string>

namespace gosthash {

	void hash256(const uint8_t* message, const uint64_t& length, uint8_t* out);

	void hashFromStr(const std::string& str, uint8_t* out);

	void hashFromStrToU32(const std::string& str, uint32_t* out);
}

#endif

