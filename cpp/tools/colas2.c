#include <fcntl.h> /* Defines O_* constants */
#include <sys/stat.h> /* Defines mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>



void *sender(void *q) {
	printf("proc1");
	
	return NULL;
}

void *receiver(void *_) {
	mqd_t q = mq_open("mariano",O_CREAT | O_RDWR);
	printf("proc2");
	char buffer[20];
	mq_send(/**(mqd_t*)*/q,"hola mundo",10,0);
	mq_receive(/**(mqd_t*)*/q,buffer,10,0);
	buffer[10]=0;
	printf("%s\n",buffer);
	return NULL;
}

int main () {
	char buffer[20];
	memset(buffer,0,sizeof(buffer));
	struct mq_attr attr;
    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 10;
    attr.mq_curmsgs = 0;

    /* create the message queue */
	mqd_t q = mq_open("/mariano5", O_CREAT | O_RDWR, 0644, &attr);
	printf("%d\n",(int)q);
	mq_send(/**(mqd_t*)*/q,"hola mundo",10,0);
	mq_receive(/**(mqd_t*)*/q,buffer,10,0);
	buffer[10]=0;
	printf("%s\n",buffer);
	
	
	mq_close(q);
	return 0;
}
