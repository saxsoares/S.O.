#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<stdio.h>
#define F 0
#define V 1
#define PROB_PREEMP 0.3
#define PRONTO 1
#define BLOQUEADO 2
#define EXECUTANDO 0
#define PROB_IO_DISPONIVEL 0.9				
#define MIN(a,b) a<b?a:b

struct processo_t{
	unsigned short 	pid;
	unsigned int 	prio;
	int 		quantum;
	int		ttotal_exec;
	unsigned short	estado;
	float		cpu_bound;
	float		io_bound;

	int 		texec;  	// recebe tempo de execucao na cpu
	int 		total_preemp;	// conta o total de preempcoes sofridas
	int 		total_uso_cpu;
	int 		total_io;
	int 		total_tempo_cpu;// tempo corrente da cpu no momento da exec do processo
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

int fila_vazia(fila_t *f){
	if( f->inicio == NULL) return V;
	return F;
}

// RETIRA da fila()
int retira_fila(fila_t *f, processo_t *proc){
	if(fila_vazia(f)) return F;
	no_t *no = f->inicio;
	*proc = no->proc;
	f->inicio = f->inicio->prox;
	if(f->inicio == NULL ) f->fim = NULL;
	free(no);
	return V;
}

int prob(float x){
	float p;
	p = ((float)(rand()%101))/100.0;
	if(p <= x) return V;
	return F;
}

int pega_tempo(int x){
	return(rand()%(x+1));
}


int sub(int a, int b){
	int r = a - b;
	if(r<0) return 0;
	return r;
}

int imprime_header = F;
void imprime_processo (processo_t proc){
	if(!imprime_header){
		printf("pid\tprio\testado\tcpu_bound\tio_bound\tttotal_exec\tquantum\ttexec\ttotal_preemp\ttotal_uso_cpu\ttotal_io\ttotal_tempo_cpu");
		imprime_header = V;
	}
        printf("%d  %d  %d  %.2f   %.2f   %d  %d  %d  %d  %d  %d\n", 
		proc.pid, 
		proc.prio,
		proc.estado,
		proc.cpu_bound,
		proc.io_bound, 
		proc.ttotal_exec,
		proc.quantum,
		proc.texec,
		proc.total_preemp, 
		proc.total_uso_cpu,
		proc.total_io);
}

int total_tempo_cpu = 0;

// executa processo()
void executa_processo(processo_t *proc){
	// preempcao -> EWscalonar retira essse processo
	// para colocar outro de maior prioridade no lugar
	if(prob(PROB_PREEMP)){
		proc->texec = pega_tempo(proc->quantum);

		// AQUI NAO SEI QUAL A  VARIAVEl CORRETA no lugar de ttotal_tempo
		proc->ttotal_exec = sub( proc->ttotal_exec, proc->texec);
		proc->estado = PRONTO;
		proc->total_preemp++;
	}
	else{
		if(prob(proc->cpu_bound)){	//executa CPU
			proc->texec = proc->quantum;
			proc->ttotal_exec = sub( proc->ttotal_exec, proc->texec);
			proc->estado = PRONTO;
			proc->total_uso_cpu++;
		}
		else{				// faz IO
			
			proc->texec = pega_tempo(proc->quantum);
			proc->ttotal_exec = sub( proc->ttotal_exec, proc->texec);
			proc->estado = BLOQUEADO;
			proc->total_io++;
		}
	}
	total_tempo_cpu += MIN(proc->ttotal_exec, proc->texec);
	proc->total_tempo_cpu = total_tempo_cpu;
	imprime_processo(*proc);
}

// escalonador()
void escalonador(fila_t *f){
	processo_t proc;
	while(!fila_vazia(f)){
		bzero(&proc, sizeof(processo_t));
		retira_fila(f, &proc);
		switch(proc.estado){
			case PRONTO:
				proc.estado = EXECUTANDO;
				executa_processo(&proc);
				if(proc.ttotal_exec > 0)
					insere_fila_prio(f, proc);
				break;
			case BLOQUEADO:
				if(prob(PROB_IO_DISPONIVEL)){
					proc.estado = EXECUTANDO;
 	                                executa_processo(&proc);
	       	                        if(proc.ttotal_exec > 0)
                	                        insere_fila_prio(f, proc);
				}
				else{
					insere_fila_prio(f, proc);
				}
				break;
		}
	}
}

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
        proc.cpu_bound = ((float)(rand()%101))/100;
        proc.io_bound = 1 - proc.cpu_bound;
		
	return proc;
}


void imprime_fila(fila_t *f){
	no_t *i;
	for(i = f->inicio; i!=NULL; i=i->prox){
		imprime_processo(i->proc);
	}
}

void cria_todos_processos(fila_t *f, int np){
	int i;
	processo_t proc;
	for(i=0; i<10; i++){
		proc = cria_processo(i);
		insere_fila_prio(f, proc);
	}
}

int main(int argc, char *argv[]){
	if(argc != 2) {
		printf("uso: %s <num_proc>\n", argv[0]);
		return 0;
	}
	int np;
	fila_t f;
	np = atoi(argv[1]);
	cria_fila(&f);
	cria_todos_processos(&f, np);
	escalonador(&f);
//	imprime_fila(&f);
	return 0;
}






















