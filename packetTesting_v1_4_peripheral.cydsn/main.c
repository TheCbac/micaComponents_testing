/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: main.c
* Workspace: micaComponents_testing
* Project: packetTesting_v1_4_peripheral
* Version: v1.0.0
* Authors: Craig Cheney
* 
* PCB: IMU v3.5.2
* PSoC: CYBLE-214015-01
*
* Brief:
*   This is the top-level application file for IMU.
*
* 2018.10.02  - Document Created
********************************************************************************/
#include <stdbool.h>
#include "project.h"
#include "imuPacketParser.h"

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
   

//#define MICA_TEST
/* ---------------- TEST CASE -----------------
* Uncomment ONE of the following
* Testing will only occur when MICA_TEST is defined
*/
#ifdef MICA_TEST
    #define MICA_TEST_case      /* Test Description */
#endif
/* -------------- END TEST CASE ---------------  */


/* ############################# BEGIN PROGRAM ############################## */
/* Interrupt Service Routines */
void ISR_imuUart(void);

/* Global Variables */
volatile bool flag_pendingRxByte = false;

/* Main Program */
#if !defined(MICA_DEBUG) && !defined(MICA_TEST)
/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
*   The top-level application function for the project. Writes a packet to the support 
    cube for testing purposes.
*
*******************************************************************************/
int main(void)
{
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    
    /* Start components */
    LEDS_Write(LEDS_ON_GREEN);
    UART_Start();
    
    UART_EnableInt();
    UART_SetCustomInterruptHandler(ISR_imuUart);
    
    /* Create a packet object and initialize */
    packets_BUFFER_FULL_S packetBuffer;
    packets_initialize(&packetBuffer);
    uint32_t error = packets_generateBuffers(&packetBuffer, packets_LEN_PACKET_128);
    /* Ensure packet buffers were created properly */
    if(error){
        LEDS_Write(LEDS_ON_RED);
        for(;;){}
    }
    /* Infinite Loop */
    for(;;) {
    /* See if there are any pending bytes */
        if(flag_pendingRxByte){
            /* Reset the flag */
            flag_pendingRxByte = false;
            /* Read in the byte */
            uint8 newData = imuUart_getChar();
            uint32 err = packets_processRxByte(&packetBuffer, newData);
            /* Inidicate errors */
            if(err){ 
                /* Reset the receive packet */
                packets_flushRxBuffers(&packetBuffer);
            }
            /* Check if complete */
            if(packetBuffer.receive.bufferState == packets_BUFFER_RECEIVE_COMPLETE) {
                LEDS_B_Toggle();
                /* Parse the packet */
                err = packets_parsePacket(&packetBuffer);
                /* Manually test packet response */
//                uint8 sensingPacket[15] = {0x01, 0x01, 0x00, 0x00, 0x00, 0x05, 0xFF, 0xF9, 0xAA};
//                imuUart_putArray(sensingPacket, 9);
                /* Acknowledge the packet */
                err = packets_acknowledgePacket(&packetBuffer, validateImuPacket);
                if(err) {
                    LEDS_Write(LEDS_ON_RED); 
                }
            }
        }
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
    #ifdef MICA_TEST_case
        /* Test descriptions */
    /* End MICA_TEST_case*/
    #else 
        #error "At least ONE MICA_TEST_<case> must be defined if MICA_TEST is defined"
    #endif /* End MICA_TEST_<case> */    
    for(;;){}
}    
#endif /* MICA_TEST */

/* ####################### BEGIN FUNCTION DEFINITIONS ####################### */

/*******************************************************************************
* ISR Name: ISR_processRxByte()
********************************************************************************
* Summary:
*   ISR when data is received over the imu UART
* Interrupt: 
*       Button pressed
*
*******************************************************************************/
void ISR_imuUart(void){
    /* Check if new data is available */
    if( UART_GetRxInterruptSourceMasked() & UART_INTR_RX_NOT_EMPTY){
        flag_pendingRxByte = true;
    }
}
/* [] END OF FILE */
