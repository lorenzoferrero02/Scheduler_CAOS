#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include <stdio.h>
#include "../include/timeline_config.h"


#define mainTASK_PRIORITY    ( tskIDLE_PRIORITY + 1 )


void vControllerTask(void *pvParameters);
void vWorkerTask(void *pvParameters);

TaskHandle_t xControllerHandle = NULL;
TaskHandle_t xWorkerHandle = NULL;


TimelineTaskConfig_t tasks[] = {
    {"Controller", vControllerTask, HARD_RT, 0, 10000, 0},
    {"Worker", vWorkerTask, SOFT_RT, 0, 10000, 0}
};

TimelineConfig_t cfg = {
    .tasks = tasks,
    .num_tasks = sizeof(tasks) / sizeof(tasks[0])
};

void vConfigureScheduler(TimelineConfig_t *cfg);


int main(void) {
    UART_init();

	xTaskCreate(
        vControllerTask,
        "Controller",
        configMINIMAL_STACK_SIZE * 4,
        NULL, 
        mainTASK_PRIORITY,
        &xControllerHandle
    );

    xTaskCreate(
        vWorkerTask,
        "Worker",
        configMINIMAL_STACK_SIZE * 4,
        NULL,  
        mainTASK_PRIORITY,
        &xWorkerHandle
    );
	

    vConfigureScheduler(&cfg);
    vTaskStartScheduler();

    for (;;);
}


void vControllerTask(void *pvParameters){

    for(;;){
        vTaskSuspend(xWorkerHandle);
        vTaskDelay(1000); // Wait di un secondo
        vTaskResume(xWorkerHandle);
        vTaskDelay(1000);
    }
    
    vTaskDelete(NULL); // termina il task dopo la prima esecuzione
}

void vWorkerTask(void *pvParameters){
    char temp[150];
    int i=0;

    for(;;){
		snprintf(temp, sizeof(temp), "Worker says: %d \n", i++);
        UART_printf(temp);
        vTaskDelay(100);
    }

    
    vTaskDelete(NULL); // termina il task dopo la prima esecuzione
}


void vConfigureScheduler(TimelineConfig_t *cfg) {
    for(int i=0; i < cfg->num_tasks; i++){
        if(cfg->tasks[i].type == "HARD_RT"){
            
        } else if(cfg->tasks[i].type == "SOFT_RT"){

        } else {
            UART_printf("Error type task");
        }
    }
}