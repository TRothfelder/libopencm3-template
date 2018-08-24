#include "FreeRTOS.h"
#include "task.h"

extern void vApplicationStackOverflowHook(void);
void vApplicationStackOverflowHook(void) {
	for(;;);	// Loop forever here..
}

/*
extern void vApplicationIdleHook(void);
void vApplicationIdleHook(void) {
	for(;;);	// Loop forever here..
}
extern void vApplicationMallocFailedHook(void);
void vApplicationMallocFailedHook(void) {
	for(;;);	// Loop forever here..
}
extern void vApplicationTickHook(void);
void vApplicationTickHook(void) {
	for(;;);	// Loop forever here..
}
*/
