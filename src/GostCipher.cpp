#include "GostCipher.h"
#include <unistd.h>
#include <memory>

const size_t bytesIn64Bits = 8;
const size_t bytesIn32Bits = 4;
const size_t rounds = 32;
const size_t invKeyRounds = 8;
const size_t sBlocksNum = 8;
const size_t subblockSize = 32;

const uint8_t sBox[8][16] {
	{4, 10, 9, 2, 13, 8, 0, 14, 6, 11, 1, 11, 7, 15, 5, 3},
	{14, 11, 4, 12, 6, 13, 15, 10, 2, 3, 8, 1, 0, 7, 5, 9},
	{5, 8, 1, 13, 10, 3, 4, 2, 14, 15, 12, 7, 6, 0, 9, 11},
	{7, 13, 10, 1, 0, 8, 9, 15, 14, 4, 6, 12, 11, 2, 5, 3},
	{6, 12, 7, 1, 5, 15, 13, 8, 4, 10, 9, 14, 0, 3, 11, 2},
	{4, 11, 10, 0, 7, 2, 1, 13, 3, 6, 8, 5, 9, 12, 15, 14},
	{13, 11, 4, 1, 3, 15, 5, 9, 0, 10, 14, 7, 6, 8, 2, 12},
	{1, 15, 13, 0, 5, 7, 10, 4, 9, 2, 3, 14, 6, 11, 8, 12}
};

uint32_t pow2(const uint32_t& b) {
	return (0x1 << b);
}

uint32_t reverse(const uint32_t& val) {
	uint32_t revv = 0;
	for (size_t i = 0; i < subblockSize/2; ++i) {
		revv += ((val & pow2(i)) << (subblockSize - 1 - 2 * i));
	}
	for (size_t i = subblockSize/2; i < subblockSize; ++i) {
		revv += ((val & pow2(i)) >> (1 + 2 * i - subblockSize));
	}
	return revv;
}

void ecbBlockIteration(const gostcipher::cipherMode& mode, char* text, const uint32_t* key) {
	uint32_t A = 0, B = 0;

	for (size_t j = 0; j < bytesIn32Bits ; ++j)
		A += static_cast<uint32_t>(static_cast<uint8_t>(text[j])) << (24 - j*8);
	A = reverse(A);

	for (size_t j = 0; j < bytesIn32Bits; ++j)
		B += static_cast<uint32_t>(static_cast<uint8_t>(text[bytesIn32Bits + j])) << (24 - j*8);
	B = reverse(B);

	for (size_t j = 0; j < rounds; ++j) {
		uint32_t R = 0;
		if (mode == gostcipher::cipherMode::encryptMode) {
			if (j < rounds - invKeyRounds)
				R = reverse(key[j % 8]);
			else R = reverse(key[7 - j % 8]);
		} else {
			if (j < invKeyRounds)
				R = reverse(key[j % 8]);
			else R = reverse(key[7 - j % 8]);
		}
		R+=A;

		uint32_t subs[8] = {
			(R & 0xF0000000) >> 28,
			(R & 0xF000000) >> 24,
			(R & 0xF00000) >> 20,
			(R & 0xF0000) >> 16,
			(R & 0xF000) >> 12,
			(R & 0xF00) >> 8,
			(R & 0xF0) >> 4,
			(R & 0xF)
		};

		for (size_t k = 0; k < sBlocksNum; ++k)
			subs[k] = sBox[k][subs[k]];

		for (size_t k = 0; k < sBlocksNum; ++k)
			R += (subs[k] << (28 - k*4));

		R = (R << 11) | (R >> 21);
		R ^= B;

		if (j != rounds - 1) {
			B = A;
			A = R;
		}
		else B = R;
	}

	A = reverse(A);
	B = reverse(B);

	for (size_t j = 0; j < bytesIn32Bits; ++j)
		text[j] =
				static_cast<char>((A & (0xFF000000 >> 8*j)) >> (24 - 8*j));
	for (size_t j = 0; j < bytesIn32Bits; ++j)
		text[bytesIn32Bits + j] =
				static_cast<char>((B & (0xFF000000 >> 8*j)) >> (24 - 8*j));
}


void ecbCipherMain(const gostcipher::cipherMode& mode, char* buf, const size_t& length, const uint32_t* key) {

	size_t blocks = length / bytesIn64Bits;

	char* text = buf;

	for (size_t i = 0; i < blocks; ++i) {
		ecbBlockIteration(mode, text, key);
		text+=bytesIn64Bits;
	}
}


void gostcipher::encrypt(char* text, const size_t& size, const uint32_t* key) {
	ecbCipherMain(cipherMode::encryptMode, text, size, key);
}


void gostcipher::decrypt(char* text, const size_t& size, const uint32_t* key) {
	ecbCipherMain(cipherMode::decryptMode, text, size, key);
}


int gostcipher::encryptAndWrite(char* text, const size_t& size, const uint32_t* key, int fileDescriptor, off_t offset) {
	size_t extSize = size % bytesIn64Bits, multipleSize = size - extSize;
	gostcipher::encrypt(text, multipleSize, key);
	int ret = pwrite(fileDescriptor, text, multipleSize, offset);
	auto extBuf = std::make_unique<char[]>(bytesIn64Bits);
	for (size_t i = 0; i < extSize; ++i)
		extBuf[i] = text[multipleSize + i];
	for (size_t i = extSize; i < bytesIn64Bits; ++i)
		extBuf[i] = static_cast<char>(bytesIn64Bits - extSize);
	ecbBlockIteration(cipherMode::encryptMode, extBuf.get(), key);
	ret = pwrite(fileDescriptor, extBuf.get(), bytesIn64Bits, offset + multipleSize);
	return ret;
}


int gostcipher::readAndDecrypt(char* text, const size_t& size, const uint32_t* key, int fileDescriptor, off_t offset) {
	size_t multipleSize = size - size % bytesIn64Bits;
	int ret = pread(fileDescriptor, text, multipleSize, offset);
	gostcipher::decrypt(text, multipleSize, key);
	ret -= checkLastBlock(text + ret - (ret % bytesIn64Bits ? ret % bytesIn64Bits : bytesIn64Bits));
	return ret;
}


char gostcipher::checkLastBlock(char* text) {
	char extra = 0;
	for (char i = 1; i <= static_cast<char>(bytesIn64Bits); ++i) {
		if (text[bytesIn64Bits - 1] == i) {
			extra = i;
			break;
		}
	}

	bool last = (extra ? true : false);
	if (last)
		for (size_t i = bytesIn64Bits - extra; i < bytesIn64Bits - 1; ++i)
			if (text[i] != extra) {
				last = false;
				break;
			}

	return (last ? extra : 0);
}
