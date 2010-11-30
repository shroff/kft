#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include "netio.h"

struct sockaddr_in server;
struct sockaddr_in from;

int debug = 0;
int resend;

uchar *local_filename;

uchar alt;

int process_read(void);
void prepare_packet(void);
void print_usage(void);

int main(int argc, char *argv[]) {
	if(argc < 7 || argc > 8) {
		print_usage();
	}

	int i = 0;

	if(argc == 8) {
		if(strcmp(argv[1], "-d")) {
			print_usage();
		}
		debug = 1;
		i = 1;
	}

	uchar *servername = argv[++i];
	int portno = atoi(argv[++i]);
	uchar *remote_filename = argv[++i];
	local_filename = argv[++i];
	uint max_packetsize = atoi(argv[++i]);
	set_dropper(atoi(argv[++i]));

	if((socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr, "Could not create socket.\n");
		exit(2);
	}

	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = ((int *)gethostbyname(servername)->h_addr)[0];
	server.sin_family = AF_INET;
	server.sin_port = htons(portno);

	src = &from;
	dest = &server;

	max_in_size = max_packetsize;
	in_buffer = (uchar *)malloc(max_in_size*sizeof(uchar));

	out_buffer = malloc(4+strlen(remote_filename));
	out_size = 4+strlen(remote_filename);
	out_buffer[0] = 0;
	pack_int(&out_buffer[1], max_packetsize);
	memcpy(out_buffer + 4, remote_filename, strlen(remote_filename));

	init();


	resend = 0;

	FILE *fp = fopen(local_filename, "w");
	fclose(fp);

	struct timeval stime, etime;
	gettimeofday(&stime, NULL);

	while(1) {
		write_read_data();
		if(process_read()) break;
		prepare_packet();
	}

	gettimeofday(&etime, NULL);

	int ds = etime.tv_sec - stime.tv_sec;
	int du = etime.tv_usec - stime.tv_usec;

	if(du < 0) {
		ds--;
		du += 1000000;
	}

	printf("Transfer completed. Time taken: %d.%d seconds.\n", ds, du);

	return 0;
}

int process_read() {
	uint exp;
	read_int3(&exp, in_buffer+1);


	if(in_size == exp) {
		if(in_buffer[0] == 2) {
			out_buffer[0] = (uchar)(2);
			out_size = 1;	
			write_data();
			return 1;
		}
		if(alt == in_buffer[0]) {
			resend = 1;
			return 0;
		}
		alt = in_buffer[0];

		FILE *fp = fopen(local_filename, "ab");
		if(!fp) {
			fprintf(stderr, "Could not open file %s for writing.", local_filename);
			exit(1);
		}
		fwrite(in_buffer+4, 1, in_size-4, fp);
		fclose(fp);
	} else {
		resend = 1;
	}
	return 0;
}

void prepare_packet() {
	if(resend) {
		resend = 0;
		return;
	}
	out_buffer[0] = (uchar)(alt<<4 | 1);
	out_size = 1;	
}

void print_usage() {
	fprintf(stderr, "Usage: kftclient [-d] <server_name> <server_portno> <remote_filename> <local_filename> <max_packetsize> <loss_percent>i\n");
	exit(1);
}
