#include <stdio.h>
#include "queue.h"
#include "proc.h"
#include "stats.h"

extern struct queue *ready;
extern struct queue *ready2;

void proc_interrupt(struct proc *p)
{
    // Verifica a fila original do processo para retorná-lo à mesma fila de início
    if (p->queue == 0)
    {
        enqueue(ready, p);
        count_ready_in(p);
    }
    else if (p->queue == 1)
    {
        enqueue(ready2, p);
        count_ready_in(p);
    }

    p->state = READY;
}
