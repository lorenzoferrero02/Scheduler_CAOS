#include "./FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include <stdio.h>


typedef enum {
    HARD_RT,
    SOFT_RT
} TaskType_t;

typedef struct {
    const char* task_name;
    TaskFunction_t function;
    TaskType_t type; // HARD_RT or SOFT_RT
    uint32_t start_time;
    uint32_t end_time;
    uint32_t subframe_id;
    uint32_t execution_time; // effective time of the task
} TimelineTaskConfig_t;

typedef struct {
    TimelineTaskConfig_t *tasks; // Puntatore a un array di task
    size_t num_tasks;            // Numero totale di task nella timeline
} TimelineConfig_t;
