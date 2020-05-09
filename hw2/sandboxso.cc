#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#define func_name(a) #a


// [sandbox] execve(/bin/ls): not allowed
void eaccess_msg(char *func_name, const char *path) {
    printf("[sandbox] %s(%s) is not allowed\n", func_name, path);
    errno = EACCES;
}

// [sandbox] opendir: access to / is not allowed

// always reject!
int system(char *command) {
    eaccess_msg(func_name(system), command);
    errno = EACCES;
    return -1;
}

int execvp(const char *file, char *const argv[]) {
    eaccess_msg(func_name(execvp), file);
    return -1;
}

int execve(const char *filename, char *const argv[],char *const envp[]) {
    eaccess_msg(func_name(execve), filename);
    return -1;
}

int execv(const char *path, char *const argv[]) {
    eaccess_msg(func_name(execv), path);
    return -1;
}

int execlp(const char *file, const char *arg, ...) {
    eaccess_msg(func_name(execlp), file);
    return -1;
}

int execle(const char *path, const char *arg, ...) {
    eaccess_msg(func_name(execle), path);
    return -1;
}

int execl(const char *path, const char *arg, ...) {
    eaccess_msg(func_name(execl), path);
    return -1;
}

int rand(){
    printf("hihi\n");
    return 1;
}