#ifndef PS_GOST28147_H
#define PS_GOST28147_H

#include <stdio.h>
#include <stdint.h>
#include <memory>

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

/*

#ifndef PS_GOST28147_H
#define PS_GOST28147_H

#include <cstdio>
#include <cstdint>
#include <memory>
#include <iostream>

namespace gostcipher {

	enum class cipherMode {
		encryptMode,
		decryptMode
	};

	void ecbBlockIteration(const cipherMode& mode, char* text, const uint32_t* key);

	void ecbCipher(const cipherMode& mode, char* text, const size_t& length, const uint32_t* key);

	std::unique_ptr<char[]> extendText(const char* text, const size_t& length, size_t& extendedLength);

	void encrypt(char* text, const size_t& length, const uint32_t* key);

	void decrypt(char* text, const size_t& length, const uint32_t* key);

	/*
	void ecbBlockIteration(const cipherMode& mode, char* text, const uint32_t* key);

	void encrypt(std::istream& istr, std::ostream& ostr, const uint32_t* key);

	void decrypt(std::istream& istr, std::ostream& ostr, const uint32_t* key);

	*/

/*
void ecbCipher(const cipherMode& mode, char* text, const size_t& length, const uint32_t* key);

std::unique_ptr<char[]> extendText(const char* text, const size_t& length, size_t& extendedLength);

void encrypt(char* text, const size_t& length, const uint32_t* key);

void decrypt(char* text, const size_t& length, const uint32_t* key);
*/
