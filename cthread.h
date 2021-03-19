#include <linux/types.h>
#include <stdlib.h>

pid_t cthread_create();
void cthread_exit();
int cthread_join();

int cthread_get_self();
