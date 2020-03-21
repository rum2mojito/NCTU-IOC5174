#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <vector>
#include<bits/stdc++.h> 
#include <dirent.h>
#include <sys/types.h>

using namespace std;

char *F_TCP = "/proc/net/tcp";
char *F_UDP = "/proc/net/udp";
char *F_TCP6 = "/proc/net/tcp6";
char *F_UDP6 = "/proc/net/udp6";

class Proc
{
public:
	string proto, p_name, local_addr, remo_addr;
	int pid, local_port, remo_port;

	Proc() {
		pid = 0;
		local_addr = "";
		local_port = 0;
		remo_addr = "";
		remo_port = 0;
		proto = "";
		p_name = "";
	}
}

void traverse_p()
{
	dirent *fd;

	fd = opendir("/proc/")
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

	vector<vector<string> > table_data;

	

	table_data = read_data(fp_tcp, "tcp");
	output_result("TCP", table_data);

	table_data = read_data(fp_tcp6, "tcp6");
	output_result("TCP", table_data);

	string a = "0000000000000000FFFF0000BF00A8C0";
	cout << a.length();
	
	return 0;
}

