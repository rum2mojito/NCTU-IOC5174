#define _GNU_SOURCE
// #define DEBUG

#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

#define func_name(a) #a

void always_reject_msg(char *func_name, const char *path);
void access_not_allowed_msg(const char *func_name, const char *path);
int __check_path_allowed();
int check_path_allowed(const char *path);

char REAL_PATH_BUF[PATH_MAX];
char REAL_WORK_DIR[PATH_MAX];

int unlink(const char *pathname) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(pathname) != 0) {
        access_not_allowed_msg(__func__, pathname);
        return -1;
    }

    return (* (int (*)(const char *pathname)) dlsym(RTLD_NEXT, __func__))(pathname);
}

int symlink(const char *path1, const char *path2) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(path1) != 0) {
        access_not_allowed_msg(__func__, path1);
        return -1;
    }
    if(check_path_allowed(path2) != 0) {
        access_not_allowed_msg(__func__, path2);
        return -1;
    }

    return (* (int (*)(const char *path1, const char *path2)) dlsym(RTLD_NEXT, __func__))(path1, path2);
}

int __lxstat(int ver, const char * path, struct stat64 * stat_buf) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(path) != 0) {
        access_not_allowed_msg(__func__, path);
        return -1;
    }

    return (* (int (*)(int ver, const char * path, struct stat64 * stat_buf)) dlsym(RTLD_NEXT, __func__))(ver, path, stat_buf);
}

int __xstat64(int ver, const char * path, struct stat64 * stat_buf) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(path) != 0) {
        access_not_allowed_msg(__func__, path);
        return -1;
    }

    return (* (int (*)(int ver, const char * path, struct stat64 * stat_buf)) dlsym(RTLD_NEXT, __func__))(ver, path, stat_buf);
}

int __xstat(int ver, const char * path, struct stat * stat_buf) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(path) != 0) {
        access_not_allowed_msg(__func__, path);
        return -1;
    }

    return (* (int (*)(int ver, const char * path, struct stat * stat_buf)) dlsym(RTLD_NEXT, __func__))(ver, path, stat_buf);
}

int stat(const char *pathname, struct stat *statbuf) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(pathname) != 0) {
        access_not_allowed_msg(__func__, pathname);
        return -1;
    }

    return (* (int (*)(const char *pathname, struct stat *statbuf)) dlsym(RTLD_NEXT, __func__))(pathname, statbuf);
}

int rmdir(const char *path) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(path) != 0) {
        access_not_allowed_msg(__func__, path);
        return -1;
    }

    return (* (int (*)(const char *path)) dlsym(RTLD_NEXT, __func__))(path);
}

int rename(const char *old_filename, const char *new_filename) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(old_filename) != 0) {
        access_not_allowed_msg(__func__, old_filename);
        return -1;
    }
    if(check_path_allowed(new_filename) != 0) {
        access_not_allowed_msg(__func__, new_filename);
        return -1;
    }

    return (* (int (*)(const char *old_filename, const char *new_filename)) dlsym(RTLD_NEXT, __func__))(old_filename, new_filename);
}

int remove(const char *filename) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(filename) != 0) {
        access_not_allowed_msg(__func__, filename);
        return -1;
    }

    return (* (int (*)(const char *filename)) dlsym(RTLD_NEXT, __func__))(filename);
}

ssize_t readlink(const char *pathname, char *buf, size_t bufsiz) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(pathname) != 0) {
        access_not_allowed_msg(__func__, pathname);
        return -1;
    }

    return (* (ssize_t (*)(const char *pathname, char *buf, size_t bufsiz)) dlsym(RTLD_NEXT, __func__))(pathname, buf, bufsiz);
}

DIR *opendir(const char *name) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(name) != 0) {
        access_not_allowed_msg(__func__, name);
        return NULL;
    }

    return (* (DIR *(*)(const char *name)) dlsym(RTLD_NEXT, __func__))(name);
}

int openat64(int dirfd, const char *pathname, int flags, ...) {
    // TODO
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    mode_t mode ;

    va_list al;
	va_start(al, flags);
	mode = va_arg(al, mode_t);
	va_end(al);

    if(check_path_allowed(pathname) != 0) {
        access_not_allowed_msg(__func__, pathname);
        return -1;
    }
    
    if(mode > 0777) {
        return (* (int (*)(const char *pathname, int flags, ...)) dlsym(RTLD_NEXT, __func__))(pathname, flags);
    } else {
        return (* (int (*)(const char *pathname, int flags, ...)) dlsym(RTLD_NEXT, __func__))(pathname, flags, mode);
    }
}

int openat(int dirfd, const char *pathname, int flags, ...) {
    // TODO
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    mode_t mode ;

    va_list al;
	va_start(al, flags);
	mode = va_arg(al, mode_t);
	va_end(al);

    if(check_path_allowed(pathname) != 0) {
        access_not_allowed_msg(__func__, pathname);
        return -1;
    }
    
    if(mode > 0777) {
        return (* (int (*)(const char *pathname, int flags, ...)) dlsym(RTLD_NEXT, __func__))(pathname, flags);
    } else {
        return (* (int (*)(const char *pathname, int flags, ...)) dlsym(RTLD_NEXT, __func__))(pathname, flags, mode);
    }
}

int open64(const char *pathname, int flags, ...) {
    // TODO
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    mode_t mode ;

    va_list al;
	va_start(al, flags);
	mode = va_arg(al, mode_t);
	va_end(al);

    if(check_path_allowed(pathname) != 0) {
        access_not_allowed_msg(__func__, pathname);
        return -1;
    }

    if(mode > 0777) {
        return (* (int (*)(const char *pathname, int flags, ...)) dlsym(RTLD_NEXT, __func__))(pathname, flags);
    } else {
        return (* (int (*)(const char *pathname, int flags, ...)) dlsym(RTLD_NEXT, __func__))(pathname, flags, mode);
    }
}

int open(const char *pathname, int flags, ...) {
    // TODO
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    mode_t mode ;

    va_list al;
	va_start(al, flags);
	mode = va_arg(al, mode_t);
	va_end(al);

    if(check_path_allowed(pathname) != 0) {
        access_not_allowed_msg(__func__, pathname);
        return -1;
    }

    if(mode > 0777) {
        return (* (int (*)(const char *pathname, int flags, ...)) dlsym(RTLD_NEXT, __func__))(pathname, flags);
    } else {
        return (* (int (*)(const char *pathname, int flags, ...)) dlsym(RTLD_NEXT, __func__))(pathname, flags, mode);
    }
}

int mkdir(const char *path, mode_t mode) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(path) != 0) {
        access_not_allowed_msg(__func__, path);
        return -1;
    }

    return (* (int (*)(const char *path, mode_t mode)) dlsym(RTLD_NEXT, __func__))(path, mode);
}

int link (const char *oldpath, const char *newpath) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(oldpath) != 0) {
        access_not_allowed_msg(__func__, oldpath);
        return -1;
    }
    if(check_path_allowed(newpath) != 0) {
        access_not_allowed_msg(__func__, newpath);
        return -1;
    }

    return (* (int (*)(const char *oldpath, const char *newpath)) dlsym(RTLD_NEXT, __func__))(oldpath, newpath);
}

FILE *fopen64(const char *filename, const char *mode) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(filename) != 0) {
        access_not_allowed_msg(__func__, filename);
        return NULL;
    }

    return (* (FILE *(*)(const char *filename, const char *mode)) dlsym(RTLD_NEXT, __func__))(filename, mode);
}

FILE *fopen(const char *filename, const char *mode) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(filename) != 0) {
        access_not_allowed_msg(__func__, filename);
        return NULL;
    }

    return (* (FILE *(*)(const char *filename, const char *mode)) dlsym(RTLD_NEXT, __func__))(filename, mode);
}

int creat64(const char *pathname, mode_t mode) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(pathname) != 0) {
        access_not_allowed_msg(__func__, pathname);
        return -1;
    }

    return (* (int (*)(const char *pathname, mode_t mode)) dlsym(RTLD_NEXT, __func__))(pathname, mode);
}

int creat(const char *pathname, mode_t mode) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(pathname) != 0) {
        access_not_allowed_msg(__func__, pathname);
        return -1;
    }

    return (* (int (*)(const char *pathname, mode_t mode)) dlsym(RTLD_NEXT, __func__))(pathname, mode);
}

int chown(const char *path, uid_t owner, gid_t group) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(path) != 0) {
        access_not_allowed_msg(__func__, path);
        return -1;
    }

    return (* (int (*)(const char *path, uid_t owner, gid_t group)) dlsym(RTLD_NEXT, __func__))(path, owner, group);
}

int chmod(const char *path, mode_t mode) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(path) != 0) {
        access_not_allowed_msg(__func__, path);
        return -1;
    }

    return (* (int (*)(const char *path, mode_t mode)) dlsym(RTLD_NEXT, __func__))(path, mode);
}

int chdir(const char *path) {
#ifdef DEBUG
    printf("[Debug] %s\n", __func__);
#endif
    if(check_path_allowed(path) != 0) {
        access_not_allowed_msg(__func__, path);
        return -1;
    }

    return (* (int (*)(const char *path)) dlsym(RTLD_NEXT, __func__))(path);
}

// always reject!
int system(char *command) {
    always_reject_msg(func_name(system), command);
    return -1;
}

int execvp(const char *file, char *const argv[]) {
    always_reject_msg(func_name(execvp), file);
    return -1;
}

int execve(const char *filename, char *const argv[],char *const envp[]) {
    always_reject_msg(func_name(execve), filename);
    return -1;
}

int execv(const char *path, char *const argv[]) {
    always_reject_msg(func_name(execv), path);
    return -1;
}

int execlp(const char *file, const char *arg, ...) {
    always_reject_msg(func_name(execlp), file);
    return -1;
}

int execle(const char *path, const char *arg, ...) {
    always_reject_msg(func_name(execle), path);
    return -1;
}

int execl(const char *path, const char *arg, ...) {
    always_reject_msg(func_name(execl), path);
    return -1;
}

// [sandbox] execve(/bin/ls): not allowed
void always_reject_msg(char *func_name, const char *path) {
    printf("[sandbox] %s(%s) is not allowed\n", func_name, path);
    errno = EACCES;
}

// [sandbox] opendir: access to / is not allowed
void access_not_allowed_msg(const char *func_name, const char *path) {
    printf("[sandbox] %s: access to %s is not allowed\n", func_name, path);
    errno = EACCES;
}

/* checking path */
// if allow, it will return 0.
int __check_path_allowed() {
#ifdef DEBUG
    printf("work dir: %s\n", REAL_WORK_DIR);
    printf("path: %s\n", REAL_PATH_BUF);
#endif

    for(int i=0; i<PATH_MAX; i++) {
        if(REAL_WORK_DIR[i] == '\0') {
            return 0;
        }

        if(REAL_WORK_DIR[i] != REAL_PATH_BUF[i]) {
            return -1;
        }
    }
}

int check_path_allowed(const char *path) {
    char *work_dir = getenv("WORK_DIR");
    char *get_dir_real = realpath(work_dir, REAL_WORK_DIR);
    char *get_path_real = realpath(path, REAL_PATH_BUF);

    if(!get_dir_real) {
#ifdef DEBUG
        printf("work dir: %s does not exist.\n", work_dir);
#endif
        return -1;
    }
    if(!get_path_real) {
#ifdef DEBUG
        printf("path: %s does not exist.\n", path);
#endif
        return -1;
    }

    return __check_path_allowed();
}
