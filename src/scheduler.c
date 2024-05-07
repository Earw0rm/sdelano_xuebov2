#include "speenlock.h"
#include "scheduler.h"
#include "param.h"


struct speenlock tasks_lock = {
    .cpu_num = -1,
    .locked = 0,
    .name = "clear"
};
//acquire lock for changes this 
static int64_t tasks_count   = -1 ;
static struct task tasks[NPROC] = {0};

//completed tasks
static int64_t tasks_buffer_count   = -1 ;
static struct task task_buffer[NPROC]  = {0};



