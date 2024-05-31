
// https://community.element14.com/products/raspberry-pi/b/blog/posts/raspberry-pico---play-with-multi-core-freertos-smp
// double blinky per core
//
// export FREERTOS_KERNEL_PATH=/home/sonny/pico/FreeRTOS-Kernel

#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */

#include "timers.h"   /* Software timer related API prototypes. */


#include <stdio.h>
#include "pico/stdlib.h"

// this is not really needed in a default SMP setup. 
// we run on the default core, and use the other one as extra.
#if ( mainRUN_ON_CORE == 1 )
#include "pico/multicore.h"
#endif


// demo code
#include "hardware/gpio.h"
#define mainLED_TASK_PRIORITY		(tskIDLE_PRIORITY + 2)
#define mainLED_FREQUENCY_MS		(1000 / portTICK_PERIOD_MS)
#define mainTASK_LED1				(2)
#define mainTASK_LED2				(3)
static void prvLedTask(void *pvParameters);


int main() {
	TaskHandle_t xHandle;
	
    stdio_init_all();
    gpio_init(mainTASK_LED2);
    gpio_set_dir(mainTASK_LED2, 1);
    gpio_put(mainTASK_LED2, 1);
    gpio_init(mainTASK_LED1);
    gpio_set_dir(mainTASK_LED1, 1);
    gpio_put(mainTASK_LED1, 1);
	

	xTaskCreate(prvLedTask, "LED", configMINIMAL_STACK_SIZE, (void *)mainTASK_LED1, mainLED_TASK_PRIORITY, &(xHandle) );
	// Define the core affinity mask such that this task can only run on core 0
    vTaskCoreAffinitySet(xHandle, (1 << 0));

	xTaskCreate(prvLedTask, "LED2", configMINIMAL_STACK_SIZE, (void *)mainTASK_LED2, mainLED_TASK_PRIORITY, &(xHandle) );
	// Define the core affinity mask such that this task can only run on core 1
    vTaskCoreAffinitySet(xHandle, (1 << 1));

	vTaskStartScheduler();

    return 0;
}


static void prvLedTask(void *pvParameters) {
	TickType_t xNextWakeTime;
	xNextWakeTime = xTaskGetTickCount();

	for( ;; ) {
		vTaskDelayUntil( &xNextWakeTime, mainLED_FREQUENCY_MS );
		gpio_xor_mask(1u << (uint32_t)pvParameters);
	}
}
