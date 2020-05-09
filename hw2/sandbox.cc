#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <regex> 
#include <string.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    char *wrong_msg = "usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...] \r\n\
        -p: set the path to sandbox.so, default = ./sandbox.so \r\n\
        -d: the base directory that is allowed to access, default = . \r\n\
        --: seperate the arguments for sandbox and for the executed command \r\n";
    
    string cmd = "";
    string so_path = "./sandbox.so";
    string dir_path = "./";

    const char *opt_sring = "p:d:-";
    int opt;
    while((opt = getopt(argc, argv, opt_sring)) != -1) {
        switch(opt) {
            case 'd':
                dir_path = string(optarg);
                cout << dir_path << endl;
                break;
            case 'p':
                so_path = string(optarg);
                cout << so_path << endl;
                break;
            case '-':
                cmd = string(optarg);
                break;
            default:
                printf("%s", wrong_msg);
                // cout << optarg << endl;
                break;
        }
    }

    return 0;
}