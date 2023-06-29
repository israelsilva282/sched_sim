#include <stdio.h>
#include "queue.h"
#include "proc.h"
#include "stats.h"

extern int NPROC;
extern struct queue *ready;
extern struct queue *ready2;
extern struct queue *blocked;
extern struct queue *finished;
extern int MAX_TIME;

void proc_init()
{
    struct proc *p;

    for (int i = 0; i < NPROC; i++)
    {
        p = createproc();
        p->tid = createproc_thread(p);

        // Verifica se o processo deve ir para a primeira fila ou a segunda fila
        if (p->remaining_time <= 0.2 * MAX_TIME)
        {
            p->queue = 0; // Fila de prioridade 1
            enqueue(ready, p);
            count_ready_in(p);
        }
        else
        {
            p->queue = 1; // Fila de prioridade 2
            enqueue(ready2, p);
            count_ready_in(p);
        }
    }
}
