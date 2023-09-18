#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>

int main ()
{
    mqd_t qd_server;   // queue descriptors
    printf ("Server: Hello, World!\n");

    struct mq_attr attr;

    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = 1;
    attr.mq_curmsgs = 0;

    if ((qd_server = mq_open ("/spike-ps2kbdtx", O_RDONLY | O_CREAT, 0660, &attr)) == -1) {
        perror ("Server: mq_open (server)");
        exit (1);
    }
    char recieved_byte;

    while (1) {
        // get the oldest message with highest priority
        if (mq_receive (qd_server, &recieved_byte, 1, NULL) != -1) {
            printf("%X\n", recieved_byte);
        }
    }
}