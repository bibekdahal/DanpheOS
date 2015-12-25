

#include "common.h"

typedef struct task_s
{
    int esp0;

} task;

task tasks[2];
void CreateTask(int id, void*(task)())
{
    //tasks[id].esp0 =
}
