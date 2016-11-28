#ifndef PS_GOST28147_H
#define PS_GOST28147_H

#include <stdio.h>
#include <stdint.h>

namespace gostcipher {

	enum class cipherMode {
		encryptMode,
		decryptMode
	};

	void ecbCipher(const cipherMode& mode, char* text, const size_t& length, const uint32_t* key);

	char* extendText(const char* text, const size_t& length, size_t& extendedLength);

	void encrypt(char* text, const size_t& length, const uint32_t* key);

	void decrypt(char* text, const size_t& length, const uint32_t* key);

}

#endif
