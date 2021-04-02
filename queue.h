typedef struct queue {
	cthread *head;
	cthread *tail;
}queue;

void qinit(queue *);
void enq(queue *, cthread);
cthread deq(queue *);
int qfull(queue *);
int qempty(queue *);
void traverse(queue *);