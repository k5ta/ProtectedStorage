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

storageCreateStatus ProtectedStorage::createStorage(storageDataArray& data) {
	if (mIsMounted == true)
		return storageCreateStatus::alreadyCreated;

	if (data.arr == nullptr || data.size < 4)
		return storageCreateStatus::fewArguments;

	std::string password = data.arr[data.size - 1];

	vfsState* vfss = new vfsState;
	gosthash::hashFromStrToU32(password, vfss->key);

	vfss->rootdir = realpath(data.arr[data.size - 3], NULL);
	data.arr[data.size - 3] = data.arr[data.size - 2];

	auto aa = std::make_unique<char*[]>(5);
	aa.get();

	std::packaged_task<int()> task([=]
		{return fuse_main_real(data.size - 2, data.arr, &vfsOper, sizeof(vfsOper), vfss); });
	auto fut = task.get_future();

	std::thread fuseThread(std::move(task));

	auto status = fut.wait_for(std::chrono::milliseconds(100));

	fuseThread.detach();

	if (status == std::future_status::ready)
		return storageCreateStatus::errorInCreating;

	mMountdir = data.arr[data.size - 2];
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
