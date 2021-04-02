void qinit(queue *q) {
	q->head = NULL;
	q->tail = NULL;
}

void enq(queue *q, cthread d) {
	cthread *newdata = (cthread *)malloc(sizeof(cthread));
	newdata->tid = d.tid;
    newdata->stack_size = d.stack_size;	
    newdata->next = NULL;
	if(q->head == NULL) {
		q->head = newdata;
		q->tail = newdata;
	}
	else {
		(q->tail)->next = newdata;
		q->tail = newdata;
	}
	q->tail->next = q->head;
}
cthread deq(queue *q) {
	cthread d, *temp;
	temp = q->head;
	d.tid = temp->tid;
    d.stack_size = temp->stack_size;
	if(q->head == q->tail) {
		q->head = NULL;
		q->tail = NULL;
	}
	else {
		q->tail->next = q->head->next;
		q->head = q->tail->next;
	}		
	free(temp);
	return d;
}
void traverse(queue *q) {
    cthread *ptr;
    ptr = q->head;
    do {
        printf("Elem id: %d\n", ptr->tid);
        ptr = ptr->next;
    }while(ptr != q->head);
}
int qfull(queue *q) {
	return 0;
}
int qempty(queue *q) {
	return (q->head == NULL);
}