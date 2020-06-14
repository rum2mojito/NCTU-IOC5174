#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

void errquit(const char *msg) {
  perror(msg);
  exit(-1);
}

int main(int argc, char *argv[]) {
  pid_t child;
  if (argc < 2) {
    fprintf(stderr, "usage: %s program\n", argv[0]);
    return -1;
  }
  if ((child = fork()) < 0) errquit("fork");
  if (child == 0) {
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) errquit("ptrace");
    execvp(argv[1], argv + 1);
    errquit("execvp");
  } else {
    int wait_status;
    int f = open("./no_more_traps.txt", O_RDONLY);

    waitpid(child, &wait_status, 0);
    ptrace(PTRACE_CONT, child, 0, 0);

    long child_code;
    unsigned long patch;
    char *target = malloc(sizeof(char) * 2);
    struct user_regs_struct regs;
    while (waitpid(child, &wait_status, 0) > 0) {
      if (WSTOPSIG(wait_status) != SIGTRAP) {
        continue;
      }

      if (ptrace(PTRACE_GETREGS, child, 0, &regs)) {
        errquit("getregs");
      }

      read(f, target, 2);
      sscanf(target, "%lx", &patch);

      child_code = ptrace(PTRACE_PEEKTEXT, child, regs.rip - 1, 0);
      if (ptrace(PTRACE_POKETEXT, child, regs.rip - 1,
                 (child_code & 0xffffffffffffff00 | patch))) {
        errquit("poketext");
      }

      regs.rip--;
      if (ptrace(PTRACE_SETREGS, child, 0, &regs)) {
        errquit("setregs");
      }
      ptrace(PTRACE_CONT, child, 0, 0);
    }
	close(f);
  }

  return 0;
}