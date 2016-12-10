#ifndef CIPHERHELPER_H
#define CIPHERHELPER_H

#include <cstdio>
#include <cstdint>

namespace cipherhelper {

	int encryptAndWrite(char* text, const size_t& size, const uint32_t* key, int fileDescriptor, off_t offset);

	int readAndDecrypt(char* text, const size_t& size, const uint32_t* key, int fileDescriptor, off_t offset);
}

#endif
