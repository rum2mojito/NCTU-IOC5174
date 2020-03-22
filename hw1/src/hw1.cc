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
#include <arpa/inet.h>
#include <string>

using namespace std;

char *F_TCP = "/proc/net/tcp";
char *F_UDP = "/proc/net/udp";
char *F_TCP6 = "/proc/net/tcp6";
char *F_UDP6 = "/proc/net/udp6";
char *FD = "/proc/%s/fd/";

class Proc{
public:
	string proto;
	string p_name;
	string l_ip;
	string l_port;
	string r_ip;
	string r_port;
	string inode;
	string p_id;

	Proc() {
		proto = "";
		p_name = "";
		l_ip = "";
		l_port = "";
		r_ip = "";
		r_port = "";
		inode = "";
		p_id = "";
	}
};

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

vector<string> search_fd(string inode)
{
	// vector<string> [0]: p_id; [1]: p_name
	struct dirent *dp;
	struct dirent *sub_dp;
    char *fullpath;
	vector<string> res(2);
    const char *path="/proc/";

	res[0] = "";
	res[1] = "";
    DIR *dir = opendir(path);
    while (dp=readdir(dir))
    {
		char checkpath[100] = "/proc/";
		strcat(checkpath, dp->d_name);
		strcat(checkpath, "/fd/");
		// DIR *check_dir = opendir(checkpath);
		DIR *sub_dir = opendir(checkpath);
		
		if (sub_dir) {
			// DIR *sub_dir = opendir(checkpath);
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
						res[0] = dp->d_name;
						char status_path[100] = "/proc/";
						strcat(status_path, dp->d_name);
						strcat(status_path, "/status");
						ifstream fp_status(status_path);
						if(!fp_status) {
							return res;
						}
						string data;
						vector<string> para;
						getline(fp_status, data);
						para = split(data, "\t");

						res[1] = para[1];
						return res;
					}
				}
			}
			closedir(sub_dir);
		}
    }
    closedir(dir); 
	return res;
}

string hex_to_port(string input)
{
	unsigned int number = (int) strtol(input.c_str(), NULL, 16);

	char *PORT = ":%d";
	if(number == 0) {
		return string("*");
	} else {
		char buffer [33];
		sprintf(buffer,"%d",number);
		// itoa(number, buffer, 10);
		return string(buffer);
	}
}

vector<Proc*> get_result(vector<vector<string> > table_data, char *proto_type)
{
	vector<string> p_status;
	vector<Proc*> p_v;

	for(int i=0; i<table_data.size(); i++) {
		if(i == 0) continue;
		Proc *p = new Proc();

		for(int j=0; j<3; j++) {
			if(j>0 && j<3){
				vector<string> tmp_ip;

				tmp_ip = split(table_data[i][j], ":");

				// ip
				struct in_addr sa;
				sa.s_addr = (int)strtol(tmp_ip[0].c_str(), NULL, 16);
				char ip_str[64];
				if(tmp_ip[0].length() == 8) {
					inet_ntop(AF_INET, &sa.s_addr, ip_str, sizeof ip_str);
					if(j == 1) p->l_ip = ip_str;
					else p->r_ip = ip_str;
				} else if(tmp_ip[0].length() == 32) {
					inet_ntop(AF_INET6, &sa.s_addr, ip_str, sizeof ip_str);
					if(j == 1) p->l_ip = ip_str;
					else p->r_ip = ip_str;
				}

				// port
				string port = hex_to_port(tmp_ip[1]);
				if(j == 1) p->l_port = port;
				else p->r_port = port;
			} 
			else if(j == 0){
				p->proto = table_data[i][j];
			}
		}
		p_status = search_fd(table_data[i][9]);
		p->p_name = p_status[1];
		p->p_id = p_status[0];
		p->inode = string(table_data[i][9]);
		p_v.push_back(p);
	}

	return p_v;
}

void output_result(vector<Proc*> p_v, char *proto_type, char*filter) {
	int width = 30;
	const char *table_head[5] = { "proto", "Local Address", "Foreign Address", "INODE", "PID/Program name and arguments" };

	// print banner
	char *banner = "List of %s connetcions:\n";
	printf(banner, proto_type);

	// print table head
	for(int i=0; i<5; i++) {
		if(i == 0 || i ==3) {
			cout << left << setw(int(width/3)) << table_head[i];
			continue;
		}
		cout << left << setw(width) << table_head[i];
	}
	cout << "\n";

	for(int i=0; i<p_v.size(); i++) {
		bool flag = true;
		if(filter != "") {
			flag = false;
			if(strstr(p_v[i]->proto.c_str(), filter)) flag = true;
			if(strstr(p_v[i]->l_ip.c_str(), filter)) flag = true;
			if(strstr(p_v[i]->l_port.c_str(), filter)) flag = true;
			if(strstr(p_v[i]->r_ip.c_str(), filter)) flag = true;
			if(strstr(p_v[i]->r_port.c_str(), filter)) flag = true;
			if(strstr(p_v[i]->inode.c_str(), filter)) flag = true;
			if(strstr(p_v[i]->p_id.c_str(), filter)) flag = true;
			if(strstr(p_v[i]->p_name.c_str(), filter)) flag = true;
		}

		if(flag) {
			cout << left << setw(int(width/3)) << p_v[i]->proto;
			cout << left << setw(width) << p_v[i]->l_ip+":"+p_v[i]->l_port;
			cout << left << setw(width) << p_v[i]->r_ip+":"+p_v[i]->r_port;
			cout << left << setw(int(width/3)) << p_v[i]->inode;
			if(p_v[i]->p_id != "") {
				cout << left << setw(width) << p_v[i]->p_id+"/"+p_v[i]->p_name;
			} else {
				cout << left << setw(width) << "-";
			}
			cout << "\n";
		}
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

int main(int argc, char *argv[])
{
	ifstream fp_tcp(F_TCP), fp_udp(F_UDP), fp_tcp6(F_TCP6), fp_udp6(F_UDP6);
	string data;
	char *filter;

	vector<vector<string> > tcp_data, tcp6_data, udp_data, udp6_data;
	vector<Proc*> tcp_p, udp_p;

	tcp_data = read_data(fp_tcp, "tcp");
	tcp6_data = read_data(fp_tcp6, "tcp6");
	tcp_data.insert(tcp_data.end(), tcp6_data.begin()+1, tcp6_data.end() );


	udp_data = read_data(fp_udp, "udp");
	udp6_data = read_data(fp_udp6, "udp6");
	udp_data.insert(udp_data.end(), udp6_data.begin()+1	, udp6_data.end());

	tcp_p = get_result(tcp_data, "TCP");
	udp_p = get_result(udp_data, "UDP");
	filter = "";

	if(argc == 1) {
		output_result(tcp_p, "TCP", filter);
		cout << "\n";
		output_result(udp_p, "UDP", filter);
	} 
	else if(argc >= 2) {
		if(argc > 2) {
			filter = argv[2];
		}
		if(strcmp(argv[1],"-t") == 0 || strcmp(argv[1], "--tcp") == 0) {
			output_result(tcp_p, "TCP", filter);
		}
		else if(strcmp(argv[1], "-u")  == 0 || strcmp(argv[1], "--udp") == 0) {
			output_result(udp_p, "UDP", filter);
		}
		else {
			cout << "Bad args <" << argv[1] << ">" << endl;
			cout << "Use:  [-t|--tcp] [-u|--udp] [filter-string]" << endl;
		}
	}
	
	return 0;
}
