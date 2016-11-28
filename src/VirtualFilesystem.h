#ifndef PS_VIRTUAL_FS_H
#define PS_VIRTUAL_FS_H

#define FUSE_USE_VERSION 29

#include <fuse.h>

struct vfsState {
	char* rootdir;

	uint32_t key[8] = {};
};

#define vfsData ((struct vfsState *) fuse_get_context()->private_data)


//	functions:

int vfs_getattr(const char* path, struct stat* stats);

int vfs_readlink(const char* path, char* link, size_t size);

int vfs_mknod(const char* path, mode_t mode, dev_t dev);

int vfs_mkdir(const char* path, mode_t mode);

int vfs_unlink(const char* path);

int vfs_rmdir(const char* path);

int vfs_symlink(const char* path, const char* link);

int vfs_rename(const char* path, const char* npath);

int vfs_link(const char* path, const char* npath);

int vfs_chmod(const char* path, mode_t mode);

int vfs_chown(const char* path, uid_t uid, gid_t gid);

int vfs_truncate(const char* path, off_t off);

int vfs_utime(const char* path, struct utimbuf* utbuf);

int vfs_open(const char* path, struct fuse_file_info* ffinfo);

int vfs_read(const char* path, char* buf, size_t size, off_t off, struct fuse_file_info* ffinfo);

int vfs_write(const char* path, const char* buf, size_t size, off_t off, struct fuse_file_info* ffinfo);

int vfs_statfs(const char* path, struct statvfs* statv);

int vfs_flush(const char* path, struct fuse_file_info* ffinfo);

int vfs_release(const char* path, struct fuse_file_info* ffinfo);

int vfs_fsync(const char* path, int datasync, struct fuse_file_info* ffinfo);

#ifdef HAVE_SYS_XATTR_H

int vfs_setxattr(const char* path, const char* name, const char* value, size_t size, int flags);

int vfs_getxattr(const char* path, const char* name, char* value, size_t size);

int vfs_listxattr(const char* path, char* list, size_t size);

int vfs_removexattr(const char* path, const char* name);

#endif

int vfs_opendir(const char* path, struct fuse_file_info* ffinfo);

int vfs_readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t off, struct fuse_file_info* ffinfo);

int vfs_releasedir(const char* path, struct fuse_file_info* ffinfo);

int vfs_fsyncdir(const char* path, int datasync, struct fuse_file_info* ffinfo);

void* vfs_init(struct fuse_conn_info* conn);

void vfs_destroy(void* userData);

int vfs_access(const char* path, int mask);

int vfs_ftruncate(const char* path, off_t off, struct fuse_file_info* ffinfo);

int vfs_fgetattr(const char* path, struct stat* stats, struct fuse_file_info* ffinfo);


//	fuse-struct:

extern struct fuse_operations vfsOper;

#endif
