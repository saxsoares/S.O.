#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>


int arquivo = 0;
int impressora = 0;

sem_t mutex_arq;
sem_t mutex_imp;

void usa_recurso(int *recurso){
	(*recurso)++;
}

void *processo_a(void *args){
	while(1){
		printf("A: sem_wait(&mute_arq): %d\n", arquivo);
		sem_wait(&mutex_arq);
		printf("A: sem_wait(&mute_imp): %d\n", impressora);
		sem_wait(&mutex_imp);
		usa_recurso(&arquivo);
		usa_recurso(&impressora);
		sem_post(&mutex_imp);
		sem_post(&mutex_arq);
	}
}

void *processo_b(void *args){
	while(1){
		printf("B: sem_wait(&mute_imp): %d\n", impressora);
		sem_wait(&mutex_imp);
		printf("B: sem_wait(&mute_arq): %d\n", arquivo);
		sem_wait(&mutex_arq);
		usa_recurso(&arquivo);
		usa_recurso(&impressora);
		sem_post(&mutex_arq);
		sem_post(&mutex_imp);
	}
}

int main(int argc, char *argv[]){
	sem_init(&mutex_arq, 0, 1);	
	sem_init(&mutex_imp, 0, 1);	
	pthread_t ta, tb;
	pthread_create(&ta, NULL, processo_a, NULL);
	pthread_create(&tb, NULL, processo_b, NULL);
	pthread_join(ta, NULL);
	pthread_join(tb, NULL);
	return 0;
}
