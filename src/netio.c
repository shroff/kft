#include "netio.h"

struct sigaction timeout;

int from_length;

void read_int3(uint * dest, uchar * src) {
	*dest = src[0] | src[1] << 8 | src[2] << 16;
}
void pack_int3(uchar * dest, int data) {
	dest[0] = data & 0xFF;
	dest[1] = (data >> 8) & 0xFF;
	dest[2] = (data >> 16) & 0xFF;
}
void read_int(uint * dest, uchar * src) {
	*dest = src[0] | src[1] << 8 | src[2] << 16 | src[3] << 24;
}
void pack_int(uchar * dest, int data) {
	dest[0] = data & 0xFF;
	dest[1] = (data >> 8) & 0xFF;
	dest[2] = (data >> 16) & 0xFF;
	dest[3] = (data >> 24) & 0xFF;
}

int write_read_data() {
	write_data();

	src_len = sizeof(*src);

	ualarm(TIMEOUT_USEC, TIMEOUT_USEC);
	while ((in_size = recvfrom(socket_descriptor, in_buffer, max_in_size, 0, (struct sockaddr *) src, &src_len)) < 0) {
		if (errno == EINTR) {
			write_data();
		} else {
			return 1;
		}
	}
	alarm(0);

	return 0;
}

void init() {
        timeout.sa_handler = handleTimeout;
        if (sigfillset(&timeout.sa_mask) < 0) {
                fprintf(stderr, "Could not create timeout handler.\n");
                exit(4);
        }
        timeout.sa_flags = 0;
        if(sigaction(SIGALRM, &timeout, 0) < 0) {
                fprintf(stderr, "Could not set action for signal.\n");
                exit(5);
        }
}

int write_data() {
        int len = sendto_dropper(socket_descriptor, out_buffer, out_size, 0, (struct sockaddr *) dest, sizeof(*dest));
        if(len < out_size) {
                fprintf(stderr, "Could not send data.\n");
		return 1;
        }
	return 0;

}

void handleTimeout(int i) {
}

