#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

// Variáveis globais definidas no 'main'
extern struct queue *ready;    // fila de processos aptos
extern struct queue *ready2;   // segunda fila de processos aptos
extern struct queue *blocked;  // fila de processos bloqueados
extern struct queue *finished; // fila de processos finalizados
// OBS: A fila 'finished' é usada apenas para estatísticas finais

extern int MAX_TIME; // tempo máximo que um processo pode executar ao todo

struct proc *get_higher_priority_queue(struct queue *ready, struct queue *ready2)
{
    int priority = rand() % 100;

    struct proc *selected;

    if (!isempty(ready) && (isempty(ready2) || priority <= 80))
    {
        selected = dequeue(ready);
    }
    else
    {
        selected = dequeue(ready2);
    }

    return selected;
}

struct proc *scheduler(struct proc *current)
{
    struct proc *selected;

    if (current != NULL)
    {
        switch (current->state)
        {
        case READY:
            enqueue(ready2, current);
            count_ready_in(current);
            break;
        case BLOCKED:
            enqueue(blocked, current);
            count_blocked_in(current);
            break;
        case FINISHED:
            enqueue(finished, current);
            count_finished_in(current);
            break;
        default:
            printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    if (isempty(ready) && isempty(ready2))
    {
        return NULL;
    }

    selected = get_higher_priority_queue(ready, ready2);

    count_ready_out(selected); // Atualiza as estatísticas de processos aptos
    selected->state = RUNNING; // Define o estado do processo selecionado como RUNNING (em execução)
    return selected;           // Retorna o processo selecionado para ser executado
}