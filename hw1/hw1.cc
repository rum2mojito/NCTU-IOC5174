#include "src/util.h"
#include <unistd.h>
#include <stdio.h>

#include <getopt.h>

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

	const char *optstring = "uth";
	int f_u = -1, f_t = -1, f_h = -1, opt_index = -1; 
	int c;
	const option opts[] = {
        {"udp", 0, NULL, 'u'},
        {"tcp", 0, NULL, 't'},
        {"help", 0, NULL, 'h'}
    };

	while((c = getopt_long(argc, argv, optstring, opts, &opt_index)) != -1) {
        switch(c) {
            case 'u':
				if(argc == 3) {
					filter = argv[2];
				}
				udp_f = true;
                break;
            case 't':
                if(argc == 3) {
					filter = argv[2];
				}
				tcp_f = true;
                break;
            case 'h':
                cout << "Usage:  [-t|--tcp] [-u|--udp] [filter-string]" << endl;
				return 0;
            case '?':
                cout << "Usage:  [-t|--tcp] [-u|--udp] [filter-string]" << endl;
				return 0;
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
		if(argc == 2) filter = argv[1];
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
