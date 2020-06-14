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
    // ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_EXITKILL);
    while (WIFSTOPPED(wait_status)) {
      long ret;
      long rax;
      struct user_regs_struct regs;
      char *ptr = (char *)&ret;
      counter++;
      if (ptrace(PTRACE_GETREGS, child, 0, &regs) == 0) {
        rax = regs.rax;
        ret = ptrace(PTRACE_PEEKTEXT, child, rax, 0);
        if ((rax & 0xfff) == 0xb80) {
          printf("0x%11x(", rax);
          printf("0x%11x): ", ptr);
          for (int i = 0; i < 37; i++) {
            if (*ptr < 30) break;
            printf("%c", *ptr);
            // 用指針會錯  rax register
            ret = ptrace(PTRACE_PEEKTEXT, child, rax + i + 1, 0);
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