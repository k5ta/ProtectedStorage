#ifndef PS_GOST28147_H
#define PS_GOST28147_H

#include <cstdio>
#include <cstdint>

namespace gostcipher {

	enum class cipherMode {
		encryptMode,
		decryptMode
	};

	void encrypt(char* text, const size_t& length, const uint32_t* key);

	void decrypt(char* text, const size_t& length, const uint32_t* key);

	int encryptAndWrite(char* text, const size_t& size, const uint32_t* key, int fileDescriptor, off_t offset);

	int readAndDecrypt(char* text, const size_t& size, const uint32_t* key, int fileDescriptor, off_t offset);

	char checkLastBlock(char* text);
}

#endif
