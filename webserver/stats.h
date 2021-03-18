#include <semaphore.h>

typedef struct
{
sem_t served_connections;
sem_t served_requests;
sem_t ok_200;
sem_t ko_400;
sem_t ko_403;
sem_t ko_404;
} web_stats;

int init_stats(void);
web_stats *get_stats(void);