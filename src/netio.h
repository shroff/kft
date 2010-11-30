#ifndef _NETIO_H__
#define _NETIO_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include "dropper.h"

#define TIMEOUT_USEC 50000


typedef unsigned int uint;
typedef unsigned char uchar;

void read_int3(uint *, uchar *);
void pack_int3(uchar *, int);
void read_int(uint *, uchar *);
void pack_int(uchar *, int);

void handleTimeout(int);

int socket_descriptor;

struct sockaddr_in *src;
int src_len;
struct sockaddr_in *dest;

extern int from_length;

extern int debug;

uchar *in_buffer;
uchar *out_buffer;
uint max_in_size;
int in_size;
uint out_size;

void init();

int write_read_data(void);
int write_data(void);


#endif
