#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<stdio.h>
#define F 0
#define V 1

struct processo_t{
	unsigned short 	pid;
	unsigned int 	prio;
	float 		quantum;
	float		ttotal_exec;
	unsigned short	estado;
	float		cpu_bound;
	float		io_bound;
};
typedef struct processo_t processo_t;

struct no_t{
	processo_t proc;
	struct no_t *prox;
};
typedef struct no_t no_t;


struct fila_t{
	no_t *inicio;
	no_t *fim;
	int qtd_proc;
};
typedef struct fila_t fila_t;

//CRIA FILA
void cria_fila(fila_t *f){
	f->inicio = NULL;
	f->fim = NULL;
	f->qtd_proc = 0;
}

//CRIA NO
no_t *cria_no(processo_t proc){
	no_t *no = (no_t *) calloc(1, sizeof(no_t));
	if(!no)		NULL;
	no->proc = proc;
	no->prox = NULL;
	return no;
}

// INSERCAO ORDENADA
int insere_fila_prio(fila_t *f, processo_t proc){
	no_t *no = cria_no(proc);
	if(!no)	return F;
	if(f->inicio == NULL) { // FILA VAZIA 
		f->inicio = no;
		f->fim = no;
		return V;
	}
	if(no->proc.prio > f->inicio->proc.prio){
		no->prox = f->inicio;
		f->inicio = no;
		return V;
	}
	if(no->proc.prio <= f->fim->proc.prio){
		f->fim->prox = no;
		f->fim = no;
		return  V;
	}
	no_t *atual, *anterior;
	atual = f->inicio;
	anterior = NULL;
	while(no->proc.prio <= atual->proc.prio){
		anterior = atual;
		atual = atual->prox;
	}
	anterior->prox= no;
	no->prox = atual;
	return V;
}

#define PRONTO 1
#define BLOQUEADO 2
#define EXECUTANDO 0

float get_quantum(unsigned int prio){
	int q;
	switch(prio) {
		case 4:
			q =  100;
			break;
		case 3:
			q = 75;
			break;
		case 2:
			q =  50;
			break;
		case 1:
			q = 25;
			break;
		default:
			q = 10;
	}
	return q;
}

processo_t cria_processo(unsigned short pid){
	processo_t proc;
	proc.pid = pid;
	proc.prio = rand()%5;
	proc.quantum = get_quantum(proc.prio);
	proc.ttotal_exec = rand()%10000;
        proc.estado = PRONTO;
        proc.cpu_bound = (rand()%101)/100;
        proc.io_bound = 1 - proc.cpu_bound;
	return proc;
}

void imprime_processo (processo_t proc){
        printf("pid: %d\n", proc.pid);
	printf("\t prio: %d\n", proc.prio);
        printf("\t quantum: %.2f\n",proc.quantum);
	printf("\t ttotal_exec: %.2f\n", proc.ttotal_exec);
        printf("\t estado: %d\n",proc.estado);
        printf("\t cpu_bound: %.2f\n",proc.cpu_bound);
        printf("\t io_bound: %.2f\n",proc.io_bound);

}

void imprime_fila(fila_t *f){
	no_t *i;
	for(i = f->inicio; i!=NULL; i=i->prox){
		imprime_processo(i->proc);
	}
}

int main(int argc, char *argv[]){
	int i;
	processo_t proc;
	fila_t f;
	cria_fila(&f);
	for(i=0; i<10; i++){
		proc = cria_processo(i);
		insere_fila_prio(&f, proc);
	}
	imprime_fila(&f);
	return 0;
}






















