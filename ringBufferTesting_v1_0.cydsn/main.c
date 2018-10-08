/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: main.c
* Workspace: micaComponents_testing
* Project: ringBuffer_v1_0
* Version: v1.0.0
* Authors: Craig Cheney
* 
* PCB: supportCube 2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   This is the top-level application file for supportCube.
*
* 2018.10.08  - Document Created
********************************************************************************/
#include <stdbool.h>
#include "project.h"
#include "ringBuffer_testing.h"

/* ####################### BEGIN PROGRAM CONFIGURATION ###################### */

//#define MICA_DEBUG
/* ---------------- DEBUG CASE ----------------
* Uncomment ONE of the following
* Debugging will only occur when MICA_DEBUG is defined
*/
#ifdef MICA_DEBUG
    #define MICA_DEBUG_LEDS     /* Run through the testings of the LEDS */
#endif
/* -------------- END DEBUG CASE --------------  */
   

#define MICA_TEST
/* ---------------- TEST CASE -----------------
* Uncomment ONE of the following
* Testing will only occur when MICA_TEST is defined
*/
#ifdef MICA_TEST
    #define MICA_TEST_RINGBUFFER      /* Run the Ring buffer test Suite */
#endif
/* -------------- END TEST CASE ---------------  */


/* ############################# BEGIN PROGRAM ############################## */
/* Interrupt Service Routines */
void ISR_isrName(void);

/* Global Variables */
volatile bool flag_isrFlag = false;

/* Main Program */
#if !defined(MICA_DEBUG) && !defined(MICA_TEST)
/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
*   The top-level application function for the project.
*
*******************************************************************************/
int main(void)
{
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    
    /* Infinite Loop */
    for(;;) {
        
    }
}
#endif /* !defined(MICA_DEBUG) && !defined(MICA_TEST) */
/* End Main Program */

/* ############################ BEGIN DEBUGGER ############################## */
#ifdef MICA_DEBUG
/*******************************************************************************
* DEBUGGER: main()
********************************************************************************
* Summary:
*   Debugging function for the PCB hardware
*******************************************************************************/
int main(void){
    #warning "MICA_DEBUG is enabled"
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    #ifdef MICA_DEBUG_LEDS
        /* Test the onboard LEDs
        Expeted outcome: 
        0. All LEDs off
        1. Red LED on
        2. Green LED on
        3. Blue LED on
        4. Yellow (RB)
        5. Cyan (BG)
        6. Magenta (RG)
        7. White (RGB) LEDs on
        */
        /* Infinite loop */
        LEDS_Test(MICA_TEST_INFINITE);
    /* End MICA_DEBUG_LEDS */
    #else
        #error "At least ONE MICA_DEBUG_<case> must be defined if MICA_DEBUG is defined"
    #endif /* End MICA_DEBUG_<case> */
    /* Infinite loop */
    for(;;){}
}
#endif /* MICA_DEBUG */

/* ############################# BEGIN TESTER ############################### */
#ifdef MICA_TEST
/*******************************************************************************
* TESTER: main()
********************************************************************************
* Summary:
*   Test functions for features and algorithms
*******************************************************************************/
int main (void) {
    #warning "MICA_TEST is enabled"
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    #ifdef MICA_TEST_RINGBUFFER
        /* Run Unit tests on the RING Buffers */
        testRunner_setPrintFunction(usbUart_print);
        /* Unit tests for MICA Errors */
        LEDS_Write(LEDS_ON_GREEN);
        usbUart_Start();
        usbUart_clearScreen();

        /* Print Program Header */
        usbUart_printHeader(__TIME__, __DATE__, "      RING BUFFER UNIT TESTS ");
        /* Set the Print function */
        
        /* ### Ensure the component version ### */
        {
            usbUart_print("\r\n*** Component Version ***\r\n");
            testRunner_run(testRunner_printResults( "Ring Buffer Version",  ringBuffer_VERSION  , 0x0100,  testRunner_MSG_NONE));
        }
        
        /* ### Test the creation of the ring buffer ### */
        {
            usbUart_print("\r\n*** Ring Buffer Creation ***\r\n");
            ringBuffer_S buffer;
            testRunner_run(test_generateBuffers(&buffer, "Standard buffer size", 100, ringBuffer_ERR_SUCCESS));
            testRunner_run(test_generateBuffers(&buffer, "Large buffer size", 10000, ringBuffer_ERR_MEMORY));
            
        }
        
        /* ### Test the destruction of the ring buffer ### */
        {
            usbUart_print("\r\n*** Ring Buffer Creation ***\r\n");
            ringBuffer_S buffer;
            ringBuffer_init(&buffer, 100, sizeof(uint8_t));
            testRunner_run(test_destroyBuffers(&buffer, "Regular destruction", ringBuffer_ERR_SUCCESS));
            ringBuffer_S buffer1;
            testRunner_run(test_destroyBuffers(&buffer1, "Non initiated Buffer", ringBuffer_ERR_MEMORY));
        }
        /* ### Test for memory leaks in the ring buffer ### */
        {
            usbUart_print("\r\n*** Ring Buffer Memory Leaks ***\r\n");
            ringBuffer_S buffer;
            testRunner_run(test_memoryLeak(&buffer, "Memory Leak", 10000));
        }
        /* ### Test pushing to the ring buffer ### */
        {
            usbUart_print("\r\n*** Push ***\r\n");
            ringBuffer_S buffer;
            testRunner_run(test_ringBuffer_push(&buffer, "No buffers inititated", ringBuffer_ERR_MEMORY));
            /* Create buffer */
            ringBuffer_init(&buffer, 10, sizeof(uint8_t));
            testRunner_run(test_ringBuffer_push(&buffer, "Initiated buffer", ringBuffer_ERR_SUCCESS));
            ringBuffer_free(&buffer);
            /* Full bufer */
            uint8_t i, size = 10;
            ringBuffer_init(&buffer, size, sizeof(uint8_t));
            for(i = ZERO; i < size; i++){
                ringBuffer_push(&buffer, &i);   
            }
            testRunner_run(test_ringBuffer_push(&buffer, "Full buffer", ringBuffer_ERR_SPACE));
            /* Free the buffer */
            ringBuffer_free(&buffer);
        }
        /* ### Test pushing array to the ring buffer ### */
        {
            usbUart_print("\r\n*** Push Array ***\r\n");
            ringBuffer_S buffer;
            testRunner_run(test_ringBuffer_pushArray(&buffer, 1, "No buffers inititated", ringBuffer_ERR_MEMORY));
            /* Buffer too small */
            ringBuffer_init(&buffer, 2, sizeof(uint8_t));
            testRunner_run(test_ringBuffer_pushArray(&buffer, 3, "Buffer too small", ringBuffer_ERR_SPACE));
            ringBuffer_free(&buffer);
            /* Space available */
            ringBuffer_init(&buffer, 4, sizeof(uint8_t));
            testRunner_run(test_ringBuffer_pushArray(&buffer, 3, "Valid push array", ringBuffer_ERR_SUCCESS));
            ringBuffer_free(&buffer);
            /* Just right */
            ringBuffer_init(&buffer, 3, sizeof(uint8_t));
            testRunner_run(test_ringBuffer_pushArray(&buffer, 3, "Just Right", ringBuffer_ERR_SUCCESS));
            
            /* Free the buffer */
            ringBuffer_free(&buffer);
        }
        /* ### Test popping from the ring buffer ### */
        {
            usbUart_print("\r\n*** Pop ***\r\n");
            ringBuffer_S buffer;
            testRunner_run(test_ringBuffer_pop(&buffer, "No buffers inititated", ringBuffer_ERR_MEMORY));
            /* Create buffer */
            ringBuffer_init(&buffer, 10, sizeof(uint8_t));
            testRunner_run(test_ringBuffer_pop(&buffer, "Empty Buffer", ringBuffer_ERR_EMPTY));
            /* Valid pop */
            uint8_t val = 0x01;
            ringBuffer_push(&buffer, &val); 
            testRunner_run(test_ringBuffer_pop(&buffer, "Valid pop", ringBuffer_ERR_SUCCESS));
            
            /* Free the buffer */
            ringBuffer_free(&buffer);
        }
        /* ### Test specific values ### */
        {
            usbUart_print("\r\n*** Queue Behavour (Lumped Test) ***\r\n");
             testRunner_run(test_ringBuffer_behavior());
        }
        /* ### Test Wrap around behaviour ### */
        {
            usbUart_print("\r\n*** Wrap Around (Lumped Test) ***\r\n");
             testRunner_run(test_ringBuffer_wrapAround());
        }
        /* ### Test clear behaviour ### */
        {
            usbUart_print("\r\n*** Clear Buffer (Lumped Test) ***\r\n");
             testRunner_run(test_ringBuffer_clear());
        }
            
    /* End MICA_TEST_RINGBUFFER*/
    #else 
        #error "At least ONE MICA_TEST_<case> must be defined if MICA_TEST is defined"
    #endif /* End MICA_TEST_<case> */    
    /* Display test suite results */
    testRunner_printCount();
    /* Enable the button */
    usbUart_print("Press any key to re-run tests\r\n");
    /* Infinite loop */
    for(;;) {
        /* Reset on buton press */
        if(usbUart_getChar()){
            /*Reset the device*/
            CySoftwareReset();   
        }
    }
}    
#endif /* MICA_TEST */

/* ####################### BEGIN FUNCTION DEFINITIONS ####################### */


/* [] END OF FILE */
