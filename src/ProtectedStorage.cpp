#include "ProtectedStorage.h"
#include "VirtualFilesystem.h"
#include "GostHash.h"
#include <cstdlib>
#include <thread>
#include <iostream>
#include <future>


ProtectedStorage* ProtectedStorage::pInstance = nullptr;

ProtectedStorage* ProtectedStorage::getInstance() {
	if (pInstance == nullptr)
		pInstance = new ProtectedStorage();
	return pInstance;
}

ProtectedStorage::ProtectedStorage()
	: mMountdir(""), mIsMounted(false)
{
}

ProtectedStorage::~ProtectedStorage() {
	if (mIsMounted.load() == true)
		this->destroyStorage();
}

storageCreateStatus ProtectedStorage::createStorage(char** data, int size) {

	if (mIsMounted.load() == true)
		return storageCreateStatus::alreadyCreated;

	if (size < 4)
		return storageCreateStatus::fewArguments;

	std::string password = data[size - 1];

	uint8_t* key8 = new uint8_t[32],
			* pass = new uint8_t[password.length()];
	for (size_t i = 0; i < password.length(); ++i)
		pass[i] = static_cast<uint8_t>(password[i]);

	gosthash::hash256(pass, 8*password.length(), key8);

	vfsState* vfss = new vfsState;
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 4; ++j)
			vfss->key[i] += (static_cast<uint32_t>(key8[i*4 + j])) << (24 - 8*j);
	delete[] pass;
	delete[] key8;

	for (int i = 0; i<size; ++i)
		std::cout << data[i] << std::endl;
	vfss->rootdir = realpath(data[size - 3], NULL);
	data[size - 3] = data[size - 2];
	size -= 2;

	std::packaged_task<int()>
			task([=] {return fuse_main_real(size, data, &vfsOper, sizeof(vfsOper), vfss); });
	auto fut = task.get_future();

	std::thread fuseThread(std::move(task));

	auto status = fut.wait_for(std::chrono::milliseconds(100));

	fuseThread.detach();

	if (status == std::future_status::ready)
		return storageCreateStatus::errorInCreating;

	mMountdir = data[size];
	mIsMounted.store(true);
	return storageCreateStatus::successfullyCreated;
}

storageDestroyStatus ProtectedStorage::destroyStorage() {
	if (mIsMounted.load() == false)
		return storageDestroyStatus::nothingToDestroy;
	std::string command = "fusermount -u ";
	command.append(mMountdir);
	system(command.c_str());
	mMountdir = "";
	mIsMounted.store(false);
	return storageDestroyStatus::successfullyDestroyed;
}
