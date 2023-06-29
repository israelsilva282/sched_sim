#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

extern struct queue *ready;
extern struct queue *ready2;
extern struct queue *blocked;
extern struct queue *finished;

extern int MAX_TIME;

struct proc *get_higher_priority_queue(struct queue *ready, struct queue *ready2)
{
    struct queue *selected_queue;
    struct proc *selected_proc;
    int prioriry = rand() % 100;

    if (prioriry < 80 && !isempty(ready))
    {
        selected_proc = dequeue(ready);
    }
    else if (!isempty(ready2))
    {
        selected_proc = dequeue(ready2);
    }
    else if (!isempty(ready))
    {
        selected_proc = dequeue(ready);
    }
    else
    {
        return NULL;
    }

    return selected_proc;
}

struct proc *scheduler(struct proc *current)
{
    struct proc *selected;
    struct queue *queue;

    /*
     *   Tratando o processo que está atualmente executando
     */

    if (current != NULL)
    {
        switch (current->state)
        {
        case READY:
            if (current->queue == 0)
            {
                enqueue(ready, current);
            }
            else
            {
                enqueue(ready2, current);
            }
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

    /*
     *   Estratégia de seleção de um novo processo para executar
     */
    if (isempty(ready) && isempty(ready2))
    {
        return NULL;
    }

    selected = get_higher_priority_queue(ready, ready2);

    count_ready_out(selected);

    selected->state = RUNNING;

    return selected;
}
