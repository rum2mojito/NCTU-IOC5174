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

using namespace std;

char *F_TCP = "/proc/net/tcp";
char *F_UDP = "/proc/net/udp";
char *F_TCP6 = "/proc/net/tcp6";
char *F_UDP6 = "/proc/net/udp6";
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
						if(!fp_status) {
							return "";
						}
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

void hex_to_port(string input)
{
	unsigned int number = (int) strtol(input.c_str(), NULL, 16);

	char *PORT = ":%d";
	if(number == 0) {
		printf(":*");
	} else {
		printf(PORT, number);
	}
}

void hex_to_ip4(string input)
{
    char *output;
	string tmp = "";
	vector<unsigned int> res(4);

	// reverse(input.begin(), input.end());
	for(int i=0; i<input.length(); i++) {
		tmp += input[i];
		if(i%2 == 1) {
			unsigned int number = (int) strtol(tmp.c_str(), NULL, 16);
			res[(int) i/2] = number;
			tmp = "";
		}
	}
    
	char *IP_V4 = "%d.%d.%d.%d";
	printf(IP_V4, res[3], res[2], res[1], res[0]);
}

void output_result(char *proto_type, vector<vector<string> > table_data)
{
	string p_name;
	const char *table_head[4] = { "proto", "Local Address", "Foreign Address", "PID/Program name and arguments" };

	// print banner
	char *banner = "List of %s connetcions:\n";
	printf(banner, proto_type);

	// print table head
	for(int i=0; i<4; i++) {
		cout << table_head[i] << "\t\t";
	}
	cout << "\n";

	// print table data
	for(int i=0; i<table_data.size(); i++) {
		if(i == 0) continue;

		for(int j=0; j<3; j++) {
			if(j>0 && j<3){
				vector<string> tmp_ip;

				tmp_ip = split(table_data[i][j], ":");

				// ip
				if(tmp_ip[0].length() == 8) {
					hex_to_ip4(tmp_ip[0]);
				} else if(tmp_ip[0].length() == 32) {
					cout << tmp_ip[0];
				}

				// port
				hex_to_port(tmp_ip[1]);
				cout << "\t\t";
			} 
			else if(j == 0){
				cout << table_data[i][j] << "\t\t";
			}
		}
		p_name = search_fd(table_data[i][9]);
		cout << table_data[i][9];
		if(p_name != "") {
			cout << "/" << p_name;
		}
		cout << "\n";
	}
}

vector<vector<string> > read_data(ifstream &fp, string proto)
{
	vector<vector<string> > res;
	string data;

	while(getline(fp, data)) {
		vector<string> tmp_v;
		tmp_v = split(data, " ");
		tmp_v[0] = proto;
		res.push_back(tmp_v);
	}

	return res;
}

int main()
{
	ifstream fp_tcp(F_TCP), fp_udp(F_UDP), fp_tcp6(F_TCP6), fp_udp6(F_UDP6);
	string data;

	vector<vector<string> > tcp_data, tcp6_data, udp_data, udp6_data;

	tcp_data = read_data(fp_tcp, "tcp");
	tcp6_data = read_data(fp_tcp6, "tcp6");
	tcp_data.insert(tcp_data.end(), tcp6_data.begin()+1, tcp6_data.end() );
	output_result("TCP", tcp_data);

	udp_data = read_data(fp_udp, "udp");
	udp6_data = read_data(fp_udp6, "udp6");
	udp_data.insert(udp_data.end(), udp6_data.begin()+1	, udp6_data.end() );
	for(int i=0; i<udp_data.size(); i++) {
		if(udp_data[i].size() < 9) continue;
		cout << i << udp_data[i][0] << udp_data[i][0] << endl;
	}
	output_result("UDP", udp_data);

	

	string a = "0000000000000000FFFF0000BF00A8C0";
	cout << a.length();
	
	return 0;
}

