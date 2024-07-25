#ifndef ASIC_TASK_H_
#define ASIC_TASK_H_

#include "mining.h"
typedef struct
{
    // ASIC may not return the nonce in the same order as the jobs were sent
    // it also may return a previous nonce under some circumstances
    // so we keep a list of jobs indexed by the job id
    // ASIC可能不会按照发送作业的顺序返回随机数
    // 在某些情况下，它也可能返回之前的nonce
    // 因此，我们保留了一个按作业id索引的作业列表
    bm_job ** active_jobs;
} AsicTaskModule;

void ASIC_task(void * pvParameters);

#endif