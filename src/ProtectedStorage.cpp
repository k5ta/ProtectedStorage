#include "ProtectedStorage.h"
#include "VirtualFilesystem.h"
#include "GostHash.h"
#include <thread>
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
	this->destroyStorage();
}

storageCreateStatus ProtectedStorage::createStorage(char** data, int size) {
	if (mIsMounted == true)
		return storageCreateStatus::alreadyCreated;

	if (data == nullptr)
		return storageCreateStatus::noArguments;

	if (size < 4)
		return storageCreateStatus::fewArguments;

	std::string password = data[size - 1];

	vfsState* vfss = new vfsState;
	gosthash::hashFromStrToU32(password, vfss->key);

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
	mIsMounted = true;
	return storageCreateStatus::successfullyCreated;
}

storageDestroyStatus ProtectedStorage::destroyStorage() {
	if (mIsMounted == false)
		return storageDestroyStatus::nothingToDestroy;
	std::string command = "fusermount -u ";
	command.append(mMountdir);
	if (system(command.c_str()) != 0) {
		return storageDestroyStatus::errorInDestroying;
	}
	mMountdir = "";
	mIsMounted = false;
	return storageDestroyStatus::successfullyDestroyed;
}

bool ProtectedStorage::isMounted() const {
	return mIsMounted;
}
