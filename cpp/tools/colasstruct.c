#include <fcntl.h> /* Defines O_* constants */
#include <sys/stat.h> /* Defines mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

// compilar con gcc -pthread y -lrt


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

struct asd {
	int a,b;
};
//~ http://stackoverflow.com/questions/23044963/send-struct-in-mq-send
int main () {
	//~ char buffer[20];
	//~ memset(buffer,0,sizeof(buffer));
	struct mq_attr attr;
    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 10;
    attr.mq_curmsgs = 0;
	
	struct asd str1, str2;
	str1.a=11;
	str1.b=12;
	
    /* create the message queue */
    char * data,*data2; //Do appropriate allocation.
	data = (char *) malloc(sizeof(str1));
	data2 = (char *) malloc(sizeof(str1));
	memcpy(data, &str1, sizeof(str1));
	mqd_t q = mq_open("/mariano5", O_CREAT | O_RDWR, 0644, &attr);
	printf("%d\n",(int)q);
	mq_send(/**(mqd_t*)*/q,(const char*) data,sizeof(str1),0);
	mq_receive(/**(mqd_t*)*/q,(char *) data2,sizeof(str2),0);
	
	memcpy(&str2, data2, sizeof(str2));
	
	printf("%d %d\n",str1.a,str1.b);
	printf("%d %d\n",str2.a,str2.b);
	
	
	mq_close(q);
	return 0;
}
