#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

typedef struct position {
    int x;
    int y;
} Pos;

typedef struct node_s {
    Pos point;
    struct node_s *next;
} Node_s;

typedef struct handwrite_Control {
    int r;
    int g;
    int b;
    int thk;
    char username[36];
    int dotslen;
} HWCtrl;

int add (Node_s **, Pos);
int del (Node_s **, Pos);
int showList (Node_s **);
int cleanList (Node_s **);
int socket_write (Node_s **, int, int);
int socket_read (Node_s **, int, int);

#endif
