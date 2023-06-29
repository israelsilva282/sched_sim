#include <stdio.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

extern struct queue *ready;
extern struct queue *ready2;
extern struct queue *blocked;
extern struct queue *finished;

extern int MAX_TIME;

struct proc *get_higher_time(struct queue *ready)
{
    struct proc *p_higher_time = NULL;
    int max_time = -1;

    for (struct proc *p = ready->head; p != NULL; p = p->next)
    {
        if (p->remaining_time > max_time)
        {
            max_time = p->remaining_time;
            p_higher_time = p;
        }
    }

    if (p_higher_time != NULL)
    {
        dequeue_bypid(ready, p_higher_time->pid);
    }

    return p_higher_time;
}

struct proc *scheduler(struct proc *current)
{
    struct proc *selected;

    /*
     *   Tratando o processo que está atualmente executando
     */

    if (current != NULL)
    {
        switch (current->state)
        {
        case READY:
            enqueue(ready, current);
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

    if (isempty(ready))
    {
        return NULL;
    }

    selected = get_higher_time(ready);

    count_ready_out(selected);

    selected->state = RUNNING;

    return selected;
}
