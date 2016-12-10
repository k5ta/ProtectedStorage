#ifndef PS_GOST28147_H
#define PS_GOST28147_H

#include <cstdio>
#include <cstdint>

namespace gostcipher {

	enum class cipherMode {
		encryptMode,
		decryptMode
	};

	void ecbBlockIteration(const gostcipher::cipherMode& mode, char* text, const uint32_t* key);

	void ecbCipherMain(const gostcipher::cipherMode& mode, char* text, const size_t& size, const uint32_t* key);

	void encrypt(char* text, const size_t& length, const uint32_t* key);

	void decrypt(char* text, const size_t& length, const uint32_t* key);

}

#endif
