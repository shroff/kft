#include <string.h>
#include "netio.h"

/*****************************************************************
 *                  RESPONSE HEADER INFORMATION                  *
 *****************************************************************
 *   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   *
 *  ack  |       length          |             data              *
 *                             data                              *
 *                             data                              *
 *                             data                              *
 *****************************************************************/
/*****************************************************************
 *                        INITIAL REQUEST                        *
 *****************************************************************
 *   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   *
 *   0   |       max_packet      |           filename            *
 *                           filename                            *
 *                           filename                            *
 *****************************************************************/
/*****************************************************************
 *                      SUCCESSIVE REQUESTS                      *
 *****************************************************************
 *   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   *
 *   1   |                                                       *
 *****************************************************************/

#define HEADER_SIZE 

struct sockaddr_in server;
struct sockaddr_in client;

uint serving;
uint packet_size;
uint data_size;
uint data_offset;
uchar *filename;
uint sent;
uchar resend;
uchar alt;
uint fno;


int finished;
int debug = 0;

void print_usage(void);
void accept_connection(void);
int prepare_packet(void);
void process_read(void);
void process_new_connection(void);

int main(int argc, char * argv[]) {

	if(argc < 3 || argc > 4) {
		print_usage();
	}

	int i = 0;

	if(argc == 4) {
		if(strcmp(argv[1], "-d")) {
			print_usage();
		}
		debug = 1;
		i = 1;
	}

	int portno = atoi(argv[++i]);
	set_dropper(atoi(argv[++i]));

	if((socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr, "Could not create socket.\n");
		exit(2);
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(portno);

	init();

	if(bind(socket_descriptor, (struct sockaddr *) &server, sizeof(server)) < 0) {
		fprintf(stderr, "Could not bind socket to port %d.\n", portno);
		exit(3);
	}

	if(debug)
		printf("Server started on port %d.\n", portno);

	serving = 0;

	max_in_size = 1024;
	in_buffer = (uchar *)malloc(max_in_size*sizeof(uchar));

	src = &client;

	while(1) {
		if(!serving) {
			accept_connection();
		} else if(!prepare_packet()) {
			if(write_read_data()) {
				printf("Error while receiving data. Terminating Connection");
				finished = 0;
				serving = 0;
				free(out_buffer);
				continue;
			}
			process_read();
		}
	}

	return 0;
}


void accept_connection() {
	if(debug)
		printf("Waiting for connection.\n");

	src_len = sizeof(*src);
	if((in_size = recvfrom(socket_descriptor, in_buffer, max_in_size, 0, (struct sockaddr *) src, &src_len)) <= 0) {
		if(debug)
			printf("Could not read data.\n");
		return;
	}
	process_new_connection();
}

void process_new_connection() {
	if(debug) {
		int i;
		printf("Length: %d\n", in_size);
                for(i=0; i<in_size; i++) {
                        printf("%x|", in_buffer[i]);
                }
		printf("\n");
	}
	dest = src;

	if(in_buffer[0] != 0) return;

	read_int(&packet_size, in_buffer + 1);
	packet_size &= 0x00FFFFFF;
	filename = (uchar *)malloc(in_size-4+1);
	memcpy(filename, in_buffer+4, in_size-4);
	filename[in_size-sizeof(packet_size)] = '\0';

	out_buffer = (uchar *)malloc(packet_size*sizeof(uchar));
	serving = 1;
	data_offset = 0;
	out_size = packet_size;
	data_size = packet_size-4;
	alt = 1;
	fno = 0;

	if(debug) {
		FILE *fp = fopen(filename, "rb");
		if(!fp) {
			printf("Accepted Connection.\nFile %s does not exist.\n", (char *)filename);
			return;
		}
		fseek(fp, 0, SEEK_END);

		uint size = ftell(fp);
		fclose(fp);
		printf("Accepted connection.\nFile: %s, File Size: %u\nPacket size: %u, Data per packet: %u, Packets required: %u\n", (char *)filename, size, packet_size, data_size, (size+data_size-1)/data_size);
	}
}


int prepare_packet() {
	if(resend) {
		resend = 0;
		return 0;
	}
	if(finished) {
		if(debug)
			printf("File sent. Cleaning up.\n");
		out_size = 4;
		out_buffer[0] = (uchar)2;
		goto end;
	}

	out_buffer[0] = (uchar)alt;

	FILE *fp = fopen(filename, "rb");
	if(fp == NULL) {
		fprintf(stderr, "Sending terminating connection\n", filename);
		out_size = 4;
		out_buffer[0] = (uchar)3;
		goto end;
	}
	fseek(fp, data_offset, SEEK_SET);
	int len = fread(out_buffer+4, 1, data_size, fp);
	fclose(fp);

	if(data_size != len) {
		out_size = len+4;
		finished = 1;
	}

	end:

	pack_int3(out_buffer+1, out_size);

	return 0;
}

void process_read() {
	if(in_buffer[0] == 0) { /* New Connection */
		if(debug)
			if(finished)
				printf("Received a new connection. Never got terminating message from last connection.\n");
			else
				printf("Received a new connection. Terminating last connection\n");
		finished = 0;
		free(out_buffer);
		process_new_connection();
	} else if(in_buffer[0] == 2) { /* Old connection closed */
		if(debug)
			printf("Transaction successfully completed. File sent\n");
		finished = 0;
		serving = 0;
		free(out_buffer);
	} else if(in_buffer[0] == 3) { /* Connection closed after file was not found */
		if(debug)
			printf("Transaction successfully completed. File not foud on local server.\n");
		finished = 0;
		serving = 0;
		free(out_buffer);
	} else if(in_buffer[0] >> 4 != alt) {
		if(debug)
			printf("Resending old packet.\n");
		resend = 1;
	} else {
		if(debug) {
			printf("Frame %d sent successfully.\n", fno++);
		}
		alt = (alt+1)%2;
		data_offset += data_size;
	}
}

void print_usage() {
	fprintf(stderr, "Usage: kftserver [-d] <portno> <loss_percent>\n");
	exit(1);
}
