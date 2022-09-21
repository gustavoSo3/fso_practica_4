#include <scheduler.h>

#define MAX_QUEUES

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

QUEUE ready[MAX_QUEUES];
QUEUE waitinginevent[MAXTHREAD];

int current_priority;

void scheduler(int arguments)
{
	int old, next;
	int changethread = 0;
	int waitingthread = 0;

	int event = arguments & 0xFF00;
	int callingthread = arguments & 0xFF;

	if (event == TIMER)
	{
		pritf(">> Este es mi print: $d\n", ready[0].head);
		q_count++;
		if (q_count == 2)
		{
			threads[callingthread].status = READY;
			_enqueue(&ready, callingthread);
			changethread = 1;
			q_count = 0;
		}
	}

	if (event == NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos
		threads[callingthread].status = READY;
		_enqueue(&ready[0], callingthread);
	}

	if (event == BLOCKTHREAD)
	{

		threads[callingthread].status = BLOCKED;
		_enqueue(&waitinginevent[blockevent], callingthread);

		changethread = 1;
	}

	if (event == ENDTHREAD)
	{
		threads[callingthread].status = END;
		changethread = 1;
	}

	if (event == UNBLOCKTHREAD)
	{
		threads[callingthread].status = READY;
		_enqueue(&ready, callingthread);
	}

	if (changethread)
	{
		old = currthread;
		next = _dequeue(&ready);

		threads[next].status = RUNNING;
		_swapthreads(old, next);
	}
}
