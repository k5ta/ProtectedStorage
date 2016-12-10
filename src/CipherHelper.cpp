#include "CipherHelper.h"
#include "GostCipher.h"
#include <unistd.h>
#include <sys/stat.h>
#include <memory>


const size_t bytesIn64Bits = 8;

int firstBlock(char* text, const size_t& size, off_t& off, const uint32_t* key, int fileDescriptor) {
	auto extOff = off % bytesIn64Bits;
	off -= extOff;
	auto extBuf = std::make_unique<char[]>(bytesIn64Bits);
	cipherhelper::readAndDecrypt(extBuf.get(), bytesIn64Bits, key, fileDescriptor, off);
	for (size_t i = extOff; i < (extOff + size < bytesIn64Bits ? extOff + size : bytesIn64Bits); ++i)
		extBuf[i] = text[i - extOff];

	gostcipher::encrypt(extBuf.get(), bytesIn64Bits, key);
	int	ret = pwrite(fileDescriptor, extBuf.get(), bytesIn64Bits, off);
	off += bytesIn64Bits;
	return ret;
}

int lastBlockNotLast(char* text, const size_t& extSize, off_t& off, const uint32_t* key, int fileDescriptor) {
	auto extBuf = std::make_unique<char[]>(bytesIn64Bits);
	cipherhelper::readAndDecrypt(extBuf.get(), bytesIn64Bits, key, fileDescriptor, off);
	for (size_t i = 0; i < extSize; ++i)
		extBuf[i] = text[i - extSize];

	gostcipher::encrypt(extBuf.get(), bytesIn64Bits, key);
	return pwrite(fileDescriptor, extBuf.get(), bytesIn64Bits, off);
}

int lastBlock(char* text, const size_t& extSize, off_t& off, const uint32_t* key, int fileDescriptor) {
	auto extBuf = std::make_unique<char[]>(bytesIn64Bits);
	for (size_t i = 0; i < extSize; ++i)
		extBuf[i] = text[i];
	for (size_t i = extSize; i < bytesIn64Bits; ++i)
		extBuf[i] = static_cast<char>(bytesIn64Bits - extSize);
	gostcipher::ecbBlockIteration(gostcipher::cipherMode::encryptMode, extBuf.get(), key);
	return pwrite(fileDescriptor, extBuf.get(), bytesIn64Bits, off);
}

int cipherhelper::encryptAndWrite(char* text, const size_t& size, const uint32_t* key, int fileDescriptor, off_t offset) {
	if (text == nullptr || key == nullptr || size == 0)
		return 0;

	int ret = 0;
	auto extOff = offset % bytesIn64Bits;
	auto extBuf = std::make_unique<char[]>(bytesIn64Bits);
	if (extOff)
		ret = firstBlock(text, size, offset, key, fileDescriptor);

	size_t extSize = (size - (extOff ? bytesIn64Bits - extOff : 0)) % bytesIn64Bits;
	size_t multipleSize = size - (extOff ? bytesIn64Bits - extOff : 0) - extSize;
	if (multipleSize > size)
		return ret;

	gostcipher::encrypt(text, multipleSize, key);
	ret = pwrite(fileDescriptor, text, multipleSize, offset);
	offset += multipleSize;

	struct stat stats;
	fstat(fileDescriptor, &stats);
	if (static_cast<size_t>(stats.st_size) > offset + bytesIn64Bits)
		return lastBlockNotLast(text + size, extSize, offset, key, fileDescriptor);

	return lastBlock(text + multipleSize, extSize, offset, key, fileDescriptor);
}

char checkLastBlock(char* text) {
	if (text == nullptr)
		return 0;

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

int cipherhelper::readAndDecrypt(char* text, const size_t& size, const uint32_t* key, int fileDescriptor, off_t offset) {
	if (text == nullptr || key == nullptr || size == 0)
		return 0;

	size_t multipleSize = size - size % bytesIn64Bits;
	int ret = pread(fileDescriptor, text, multipleSize, offset - offset % bytesIn64Bits);
	gostcipher::decrypt(text, multipleSize, key);
	ret -= checkLastBlock(text + ret - (ret % bytesIn64Bits ? ret % bytesIn64Bits : bytesIn64Bits));
	return ret;
}
