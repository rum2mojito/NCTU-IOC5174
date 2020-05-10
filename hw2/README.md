### Homework #2

#### <small>Advanced Programming in the UNIX Environment</small>

#### Due: May 11, 2020

### Play in a Sandbox!?

In this homework, we are going to practice library injection and API hijacking. Please implement a sandbox that confines file relevant operations to a user provided directory. You have to implement your sandbox as a <u>shared library (1)</u> (e.g., sandbox.so) and then inject the shared library into a process using using your customized <u>command launcher (2)</u> (e.g., sandbox). If an operation attempts to escape from the sandbox, e.g., open a file or a directory outside the sandbox, you have to reject the request by returning an error code to the function caller. In addition, please also output an error message to the user's <u>terminal</u>. You should output error messages correctly even if the command line has redirected outputs/errors to a file. Please check the functions listed in the section "Requirements" below.

You have to compile your source codes and generate 1) the shared object and 2) the launcher. You don't have to implement any test program by yourself. Instead, you have to work with those binaries already installed in the system.

### Requirements

The list of monitored library calls is shown below. It covers several functions we have introduced in the class.
```
chdir chmod
chown
creat
fopen
link
mkdir
open
openat
opendir
readlink
remove
rename
rmdir
stat
symlink
unlink
```
In addition to the aforementioned library calls, your sandbox ALWAYS has to reject the use of the following functions:
```
execl
execle
execlp
execv
execve
execvp
system
```

### Output Messages

If an invalid access is detected, your program has to output an error message to indicate the error. If an access to an invalid path is detected, please show the target pathname and the corresponding operation. If the use of `system` or `exec*` is rejected, please also show the function name and the command line (for system) or the executable name (for exec*).

### Grading Policy

The grading policy for this homework is listed below:

* [10%] Use Makefile to manage the building process of your program.We will not grade your program if we cannot use <b>make</b> command to build your program.
* [30%] A sandboxed executable can work as usual. Your program cannot change the behavior of a sandboxed executable if the accesses are in the allowed base directory and its subdirectories.
* [30%] Implement file/directory-relevant functions listed in the requirements.
* [20%] Implement `system` and `exec*` functions listed in the requirements.
* [10%] Display error messages.

### Homework Submission

Please pack your files into a single tar.gz archive and submit your homework via the system. Please also provide a Makefile (used for compiling and linking your codes) and a README file (indicating what functions will be monitored) in your archive.

### Hints

Some hints that may simplify your work:

1. We will test your program on Ubuntu 18.04. For the best compatibility, you may develop your program on Ubuntu 18.04.
2. You may need to define macros to simplify your implementation.
3. You may consider working with # and ## operators in macros.
4. For variable-length function parameters, consider working with [stdarg.h](http://man7.org/linux/man-pages/man0/stdarg.h.0p.html).
5. You may consider working with __attribute__((constructor)). If you don't know what is that, please google for it!
6. The implementation for some library functions may be different from its well-known prototypes. For example, the actual implementation for <b>stat</b> in GNU C library is <b>__xstat</b>. Therefore, you may be not able to find symbol <b>stat</b> in the binary. In case that you are not sure about the _real_ symbols used in C library, try to work with <b>readelf</b> or <b>nm</b> to get the symbol names.
7. Sample macro implementation is available [here](macro.txt).
8. You may use `ltrace` command to check what library functions are called by a program.

#### Running Examples

Suppose you have compiled your homework files into <b>sandbox.so</b> and your launcher files into <b>sandbox</b>, respectively, several examples from our simple implementation are given below.

NOTE: the sample program only implements parts of listed function. The output of the following sample messages are simply for demonstration purpose. The actual functions captured by your sandbox may be different depending on the completeness of your implementation and the runtime environment.
```sh
# default message
$ ./sandbox
no command given.

<em># show help message ...</em>
$ ./sandbox -h
./sandbox: invalid option -- 'h'
usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...]
        -p: set the path to sandbox.so, default = ./sandbox.so
        -d: the base directory that is allowed to access, default = .
        --: separate the arguments for sandbox and for the executed command

<em># the regular 'ls' command</em>
$ ls
Makefile  sandbox  sandbox.c  sandbox.so  sandboxso.c

<em># sandboxed 'ls' command</em>
$ ./sandbox ls
Makefile  sandbox  sandbox.c  sandbox.so  sandboxso.c

<em># pass -a to 'ls' command: bad example</em>
$ ./sandbox ls -a
./sandbox: invalid option -- 'a'
usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...]
        -p: set the path to sandbox.so, default = ./sandbox.so
        -d: the base directory that is allowed to access, default = .
        --: seperate the arguments for sandbox and for the executed command

<em># pass -a to 'ls' command</em>
$ ./sandbox -- ls -a
.  ..  .gdb_history  Makefile  sandbox  sandbox.c  sandbox.so  sandboxso.c

<em># the regular 'ls /' command</em>
$ ls /
bin   data  etc   lib    lib64   lost+found  mnt  proc  root  sbin  snap  sys  usr  vmlinuz
boot  dev   home  lib32  libx32  media       opt  repo  run   sh    srv   tmp  var  vmlinuz.old

<em># sandboxed 'ls /' command</em>
$ ./sandbox -- ls /
[sandbox] opendir: access to / is not allowed
ls: cannot open directory '/'

<em># sandboxed 'ls /' command: access granted by -d</em>
$ ./sandbox -d / ls /
bin   data  etc   lib    lib64   lost+found  mnt  proc  root  sbin  snap  sys  usr  vmlinuz
boot  dev   home  lib32  libx32  media       opt  repo  run   sh    srv   tmp  var  vmlinuz.old

<em># another sandboxed output</em>
$ ./sandbox -- ls -la / Makefile
-rw-rw-r--  1 chuang chuang  224 Oct 27 21:13 Makefile

[sandbox] opendir: access to / is not allowed
ls: cannot open directory '/'

<em># redirect output to /dev/null</em>
$ ./sandbox -- ls -la / Makefile >/dev/null
[sandbox] opendir: access to / is not allowed
ls: cannot open directory '/'

<em># redirect output and error to /dev/null: this is an optional feature</em>
$ ./sandbox -- ls -la / Makefile >/dev/null 2>&1
[sandbox] opendir: access to / is not allowed

<em># execve is always not allowed ...</em>
<em># NOTE: this sample does not implement `open' and `stat' functions ...</em>
$ ./sandbox -- sh -c 'ls'
[sandbox] execve(/bin/ls): not allowed
[sandbox] execve(/usr/games/ls): not allowed
[sandbox] execve(/usr/local/games/ls): not allowed
[sandbox] execve(/snap/bin/ls): not allowed
sh: 1: ls: Permission denied

<em># execve is always not allowed ...</em>
<em># NOTE: this sample implements `open' and `stat' functions ...</em>
$ ./sandbox -- sh -c 'ls'
[sandbox] open: access to /dev/tty is not allowed
[sandbox] open: access to /dev/pts/2 is not allowed
[sandbox] __xstat: access to /usr/local/sbin/sh is not allowed
[sandbox] __xstat: access to /usr/local/bin/sh is not allowed
[sandbox] __xstat: access to /usr/bin/sh is not allowed
[sandbox] fopen: access to /etc/passwd is not allowed
[sandbox] __xstat: access to /usr/local/sbin/ls is not allowed
[sandbox] __xstat: access to /usr/local/bin/ls is not allowed
[sandbox] __xstat: access to /usr/bin/ls is not allowed
sh: ls: command not found

```

#### Some Other Hints ...

When testing your homework, you may inspect symbols used by an executable. We have mentioned that you are not able to see any symbol if the symbols were stripped using <b>strip</b> command. However, you may consider working with <b>readelf</b> command. For example, we can check the symbols that are unknown to the binary:
```
$ nm /usr/bin/wget
nm: /usr/bin/wget: no symbols
$ readelf --syms /usr/bin/wget | grep open
    72: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND freopen64@GLIBC_2.2.5 (2)
    73: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND iconv_open@GLIBC_2.2.5 (2)
   103: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND gzdopen
   107: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND fdopen@GLIBC_2.2.5 (2)
   119: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND open64@GLIBC_2.2.5 (2)
   201: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND fopen64@GLIBC_2.2.5 (2)`

Alternatively, you may consider using <b>nm -D</b> to read symbols.

To simplify the process to find real symbol names, you may compile the [sample program](sample.c) with the following two commands, and check the dynamic linkage symbols in the corresponding executables.

`<em># regular version</em>
$ gcc sample.c -o sample1

<em># enable large-file-support feature</em>
$ gcc sample.c -o sample2 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
```

***
