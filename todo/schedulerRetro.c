#include <scheduler.h>

#define MAX_QUEUES 20

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

		if (current_priority < MAX_QUEUES - 1)
		{
			_enqueue(&ready[current_priority + 1], callingthread);
		}
		else
		{
			_enqueue(&ready[current_priority], callingthread);
		}

		threads[callingthread].status = READY;
		changethread = 1;
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
		_enqueue(&ready[current_priority], callingthread);
	}

	if (changethread)
	{
		old = currthread;

		for (current_priority = 0; current_priority < MAX_QUEUES; current_priority++)
		{
			if (!_emptyq(&ready[current_priority]))
				break;
		}

		next = _dequeue(&ready[current_priority]);
		threads[next].status = RUNNING;
		_swapthreads(old, next);
	}
}
