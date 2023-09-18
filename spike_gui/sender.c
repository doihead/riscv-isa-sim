/*
 * client.c: Client program
 *           to demonstrate interprocess communication
 *           with POSIX message queues
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE_NAME   "/sp-example-server"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

int main (int argc, char **argv)
{
    mqd_t qd_server;   // queue descriptors
    printf ("Server: Hello, World!\n");

    struct mq_attr attr;

    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = 1;
    attr.mq_curmsgs = 0;

    if (argc != 2) {
        exit(-1);
    }

    if ((qd_server = mq_open ("/spike-ps2kbd", O_WRONLY | O_CREAT, 0660, &attr)) == -1) {
        perror ("Server: mq_open (server)");
        exit (1);
    }
    int msg_len = strlen(argv[1]);
        for (int i = 0; i < msg_len; i ++) {
            if (mq_send (qd_server, argv[1] + i, 1, 0) == -1) {
                perror ("Client: Not able to send message to server");
            }
        }
        
    exit (0);
}