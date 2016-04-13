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
typedef struct no_t not_t;


struct fila_t{
	no_t *inicio;
	no_t *fim;
	int qtd_proc:
};
typedef struct fila_t fila_t;

//CRIA FILA
void cria_fila(fila_t *f){
	f->inicio = NULL;
	f->fim = NULL;
	f->qtd_proc = 0;
}

//CRIA NO
no_t *cria_no(processo_t){
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
	if(no->proc.prio < f->fim->proc.prio){
		f->fim->prox = no
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

}

int main(int argc, char *argv[]){

}






















