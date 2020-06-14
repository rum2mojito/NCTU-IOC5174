#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
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
    long long counter = 0LL;
    int wait_status;
    if (waitpid(child, &wait_status, 0) < 0) errquit("wait");
    ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_EXITKILL);
    while (WIFSTOPPED(wait_status)) {
      long ret;
      unsigned long long rip;
      struct user_regs_struct regs;
      unsigned char *ptr = (unsigned char *)&ret;
      counter++;
#if USE_PEEKUSER
      if ((rip = ptrace(PTRACE_PEEKUSER, child,
                        ((unsigned char *)&regs.rip) - ((unsigned char *)&regs),
                        0)) != 0) {
#else
      if (ptrace(PTRACE_GETREGS, child, 0, &regs) == 0) {
        rip = regs.rax;
#endif
        ret = ptrace(PTRACE_PEEKTEXT, child, rip, 0);
        if ((rip & 0xfff) == 0xb80) {
          for (int i = 0; i < 37; i++) {
            printf(" [%i] %c", i, ptr[i]);
          }
          printf("\n");
        }
      }
      if (ptrace(PTRACE_SINGLESTEP, child, 0, 0) < 0) errquit("ptrace@parent");
      if (waitpid(child, &wait_status, 0) < 0) errquit("wait");
    }
    fprintf(stderr, "## %11d instruction(s) executed\n", counter);
  }
  return 0;
}