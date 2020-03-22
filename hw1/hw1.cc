#include "src/util.h"

char *F_TCP = "/proc/net/tcp";
char *F_UDP = "/proc/net/udp";
char *F_TCP6 = "/proc/net/tcp6";
char *F_UDP6 = "/proc/net/udp6";
char *FD = "/proc/%s/fd/";

int main(int argc, char *argv[])
{
	ifstream fp_tcp(F_TCP), fp_udp(F_UDP), fp_tcp6(F_TCP6), fp_udp6(F_UDP6);
	string data;
	char *filter = "";
	bool udp_f = false, tcp_f = false;
	regex udp_r_("-u");
	regex udp_r__("--udp");
	regex tcp_r_("-t");
	regex tcp_r__("--tcp");

	// check flag
	for(int i=1; i<argc; i++) {
		if(regex_match(string(argv[i]), udp_r_) || regex_match(string(argv[i]), udp_r__)) {
			udp_f = true;
		}
		else if(regex_match(string(argv[i]), tcp_r_) || regex_match(string(argv[i]), tcp_r__)) {
			tcp_f = true;
		}
		else {
			if(strcmp(filter, "") == 0) {
				filter = argv[i];
			} else {
				cout << "Use:  [-t|--tcp] [-u|--udp] [filter-string]" << endl;
				return 0;
			}
		}
	}

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

	if(!tcp_f && !udp_f) {
		output_result(tcp_p, "TCP", filter);
		output_result(udp_p, "UDP", filter);
	} else {
		if(tcp_f) {
			output_result(tcp_p, "TCP", filter);
		}
		if(udp_f) {
			output_result(udp_p, "UDP", filter);
		}
	}
	
	return 0;
}
