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
    {"Controller", vControllerTask, HARD_RT, 0, 500, 0},
    {"Worker", vWorkerTask, SOFT_RT, 0, 1000, 0},
    {"Worker2", vWorkerTask, SOFT_RT, 0, 10000, 0}
};

TimelineConfig_t cfg = {
    .tasks = tasks,
    .num_tasks = sizeof(tasks) / sizeof(tasks[0])
};

void vConfigureScheduler(TimelineConfig_t *cfg);
int mcd_tasks(TimelineConfig_t *cfg);
int mcd(int a, int b);
int lcm_tasks(TimelineConfig_t *cfg);
int lcm(int a, int b);
int abs(int a);


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
    //vTaskStartScheduler();

    for (;;);
}


void vControllerTask(void *pvParameters){

    for(;;){
        /*
        vTaskSuspend(xWorkerHandle);
        vTaskDelay(1000); // Wait di un secondo
        vTaskResume(xWorkerHandle);
        vTaskDelay(1000);*/
        UART_printf("caiooo");
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
    int minor_cycle = mcd_tasks(cfg);
    int major_cycle = lcm_tasks(cfg);

    char temp[150];
    snprintf(temp, sizeof(temp), "Minor cycle: %d\n", minor_cycle);
    UART_printf(temp);

    snprintf(temp, sizeof(temp), "Major cycle: %d\n", major_cycle);
    UART_printf(temp);
}

// Funzioni MCD / LCM
int mcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int mcd_tasks(TimelineConfig_t *cfg) {
    if (cfg->num_tasks == 0) return 0;
    int result = cfg->tasks[0].end_time - cfg->tasks[0].start_time;
    for (size_t i = 1; i < cfg->num_tasks; i++) {
        result = mcd(result, cfg->tasks[i].end_time - cfg->tasks[i].start_time);
    }
    return result;
}

int lcm(int a, int b) {
    return abs(a * b) / mcd(a, b);
}

int lcm_tasks(TimelineConfig_t *cfg) {
    if (cfg->num_tasks == 0) return 0;
    int result = cfg->tasks[0].end_time - cfg->tasks[0].start_time;
    for (size_t i = 1; i < cfg->num_tasks; i++) {
        result = lcm(result, cfg->tasks[i].end_time - cfg->tasks[i].start_time);
    }
    return result;
}

int abs(int a){
    if(a < 0){
        return a * (-1);
    }
    return a;
}