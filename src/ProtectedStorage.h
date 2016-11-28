#ifndef PROTECTED_STORAGE_H
#define PROTECTED_STORAGE_H

#include <string>
#include <atomic>

enum class storageCreateStatus {
	alreadyCreated,
	fewArguments,
	errorInCreating,
	successfullyCreated
};

enum class storageDestroyStatus {
	nothingToDestroy,
	successfullyDestroyed
};

class ProtectedStorage {

public:
	static ProtectedStorage* getInstance();

	~ProtectedStorage();

	ProtectedStorage(const ProtectedStorage& ) = delete;

	ProtectedStorage(ProtectedStorage&& ) = delete;

	ProtectedStorage& operator=(const ProtectedStorage& ) = delete;

	ProtectedStorage& operator=(ProtectedStorage&& ) = delete;

	storageCreateStatus createStorage(char** data, int size);

	storageDestroyStatus destroyStorage();

private:
	std::string mMountdir;

	std::atomic<bool> mIsMounted;

	ProtectedStorage();

	static ProtectedStorage* pInstance;
};

#endif
