#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#define func_name(a) #a


// [sandbox] opendir: access to / is not allowed
void eaccess_msg(char *func_name, const char *path) {
    printf("[sandbox] %s(%s) is not allowed\n", func_name, path);
}

// always reject!
int system(char *command) {
    eaccess_msg(func_name(system), command);
    errno = EACCES;
    return -1;
}

int execvp(const char *file, char *const argv[]) {
    eaccess_msg(func_name(execvp), file);
    errno = EACCES;
    return -1;
}

int execve(const char *filename, char *const argv[],char *const envp[]) {
    eaccess_msg(func_name(execve), filename);
    errno = EACCES;
    return -1;
}

int execv(const char *path, char *const argv[]) {
    eaccess_msg(func_name(execv), path);
    errno = EACCES;
    return -1;
}

int execlp(const char *file, const char *arg, ...) {
    eaccess_msg(func_name(execlp), file);
    errno = EACCES;
    return -1;
}

int execle(const char *path, const char *arg, ...) {
    eaccess_msg(func_name(execle), path);
    errno = EACCES;
    return -1;
}

int execl(const char *path, const char *arg, ...) {
    eaccess_msg(func_name(execl), path);
    errno = EACCES;
    return -1;
}

int rand(){
    printf("hihi\n");
    return 42; //the most random number in the universe
}