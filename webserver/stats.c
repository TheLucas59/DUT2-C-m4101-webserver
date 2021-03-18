#include "stats.h"
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stddef.h>
static web_stats *stats;

int init_stats(void){
	stats=mmap(NULL, sizeof(web_stats), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1, 0);
	sem_init(&stats->served_connections,PTHREAD_PROCESS_SHARED,0);
	sem_init(&stats->served_requests,PTHREAD_PROCESS_SHARED,0);
	sem_init(&stats->ok_200,PTHREAD_PROCESS_SHARED,0);
	sem_init(&stats->ko_400,PTHREAD_PROCESS_SHARED,0);
	sem_init(&stats->ko_403,PTHREAD_PROCESS_SHARED,0);
	sem_init(&stats->ko_404,PTHREAD_PROCESS_SHARED,0);
	return 0;
}

web_stats *get_stats(void){
	return stats;
}

