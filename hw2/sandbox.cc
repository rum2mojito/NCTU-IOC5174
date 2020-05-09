#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

using namespace std;

int main(int argc, char *argv[]) {
    const char *wrong_msg = "usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...] \r\n\
        -p: set the path to sandbox.so, default = ./sandbox.so \r\n\
        -d: the base directory that is allowed to access, default = . \r\n\
        --: seperate the arguments for sandbox and for the executed command \r\n";
    
    const char *cmd_t = "LD_PRELOAD=%s WORK_DIR=%s";
    const char *cmd_cat = "%s %s";

    char so[] = "./sandbox.so";
    char dir[] = "./";
    char cmd[5000];

    const char *opt_sring = "p:d:-:";
    int opt;
    while((opt = getopt(argc, argv, opt_sring)) != -1) {
        switch(opt) {
            case 'd':
                strcpy(dir, optarg);
                printf("dir: %s\n", dir);
                break;
            case 'p':
                strcpy(so, optarg);
                printf("so: %s\n", so);
                break;
            case '-':
                break;
            default:
                printf("%s", wrong_msg);
                // cout << optarg << endl;
                break;
        }
    }


    if(argc > optind) {
        sprintf(cmd, cmd_t, so, dir);
        for(int i=optind; i<argc; i++) {
            sprintf(cmd, cmd_cat, cmd, argv[i]);
        }
        printf("%s\n", cmd);
    }
    system(cmd);
    
    return 0;
}