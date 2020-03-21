#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <vector>
#include<bits/stdc++.h> 
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex>

using namespace std;

char *FD = "/proc/%s/fd/";

void find_inode(string &res, string str)
{
	bool record = false;
	string socket_cmp, socket = "socket";
	socket_cmp = socket_cmp.assign(str, 0, 6); 

	if(socket_cmp != socket) return;

	for(int i=0; i<str.length(); i++) {
		if(str[i] == ']') break;

		if(record) {
			// cout << str[i] << endl;;
			res.append(1, str[i]);
		}

		if(str[i] == '[') record = true;
	}
	// if(res != "") cout << res << endl;
}

vector<string> split(const string &str, const string &delim)
{
	vector<string> res;
	if(str.length() == 0) return res;

	char *strs = new char[str.length()+1];
	char *delims = new char[delim.length()+1];

	strcpy(strs, str.c_str());
	strcpy(delims, delim.c_str());

	char *p= strtok(strs, delims);
	while(p) {
		string s = p;
		res.push_back(s);
		p = strtok(NULL, delims);
	}

	free(p);

	return res;
}

string search_fd(string inode)
{
	struct dirent *dp;
	struct dirent *sub_dp;
    char *fullpath;
    const char *path="/proc/";

    DIR *dir = opendir(path);
    while (dp=readdir(dir))
    {
		char checkpath[100] = "/proc/";
		strcat(checkpath, dp->d_name);
		strcat(checkpath, "/fd/");
		DIR *check_dir = opendir(checkpath);
		if (check_dir) {
			DIR *sub_dir = opendir(checkpath);
			while(sub_dp = readdir(sub_dir)) {
				char fd_path[100];
				char readlink_buf[100];
				strcpy(fd_path, checkpath);
				strcat(fd_path, sub_dp->d_name);

				readlink(fd_path, readlink_buf, 100);
				string readlink_buf_str(readlink_buf);
				string match_node = "";

				find_inode(match_node, readlink_buf_str);
				if(match_node != "") {
					if(match_node == inode) {
						char status[100] = "/proc/";
						strcat(status, dp->d_name);
						strcat(status, "/status");
						ifstream fp_status(status);
						string data;
						vector<string> para;
						getline(fp_status, data);
						para = split(data, "\t");
						
						//cout << para[1] << endl;
						// printf("%s\t\t%s\n", fd_path, match_node.c_str());
						return para[1];
					}
				}
			}
			closedir(check_dir);
		}
    }
    closedir(dir); 
	return "";
}

int main()
{
	string p_name;
	p_name = search_fd("26795");
	cout << p_name << endl;
	
	return 0;
}

