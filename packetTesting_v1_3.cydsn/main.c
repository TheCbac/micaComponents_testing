/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: main.c
* Workspace: DriveBot_v5
* Project: DriveBot_v5.1
* Version: v5.1.0
* Authors: Craig Cheney, Scott McCuen
* 
* PCB: DriveBot MCU v5.1.0
* PSoC: CY8C4245LQI-483
*
* Brief:
*   This is the top-level application file for the DriveBot microcontoller. It
*   is responsible for receiving commands from the IMU Cube over the UART, and 
*   controlling the Drivebot based on these commands. It outputs the velocity
*   data to IMU, which streams it to the host device. 
* 
* 2018.07.25 CC - Abstract out "boardTesting.c"
* 2018.04.10 CC - Upgrade to v5. Switch to MICA Creator components from libMica
* 2017.08.25 CC - Add libMica
* 2017.08.13 SM - Add motor debugging
* 2017.08.11 CC - Document Created
********************************************************************************/
#include "project.h"
#include <stdbool.h>
#include <stdio.h>
#include "packet_testing.h"
#include "testRunner.h"

/*  -------------- DEBUGGING --------------
* Uncomment MICA_DEBUG to enable general debugging.
* Uncomment MICA_DEBUG_<case> below to
* enable ONE of the debug levels.
*/
//#define MICA_DEBUG

/* -------------- DEBUG CASE --------------
* Uncomment ONE of the following
* Debugging will only occur when MICA_DEBUG is defined
*/
#ifdef MICA_DEBUG
#endif

/* -------------- END DEBUG CASE --------------  */

/*  -------------- Testing --------------
* Uncomment MICA_TEST to enable specific tests.
* Uncomment MICA_TEST_<case> below to
* enable ONE of the test levels. */

#define MICA_TEST

/* -------------- TEST LEVEL --------------
* Uncomment ONE of the following
* Testing will only occur when MICA_TEST is defined
*/
#ifdef MICA_TEST
    #define MICA_TEST_PACKETS_ERRORS       /* Test various error on packts */
//    #define MICA_TEST_PACKETS           /* Test Packet communication */
//    #define MICA_TEST_PACKETS_ISR        /* Receive packets from IMU via interrupt and print results */
#endif
/* -------------- END TEST LEVEL --------------  */

/* --------- Local Variables --------- */

void ISR_imuUart(void);
void ISR_Timer(void);
/* State variables */
volatile bool flag_pendingRxByte = false;
volatile bool timerExpired = false;
/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
*   The top-level application function for the project.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
int main(void) {


    /* Enable global interrupts */
    CyGlobalIntEnable;
    
/* %%%%%%%%%%%%%%%%%% Begin Debugging %%%%%%%%%%%%%%%%%% */
#ifdef MICA_DEBUG
 
#endif /* MICA_DEBUG */
#ifdef MICA_TEST
    /* end MICA_TEST_PACKET_SPAWN */
        #if defined MICA_TEST_PACKETS_ERRORS
        /* Unit tests for MICA Errors */
        LEDS_Write(LEDS_ON_GREEN);
        UART_USB_Start();
        usbUart_clearScreen();

        /* Print Program Header */
        usbUart_printHeader(__TIME__, __DATE__, "      PACKET TESTING UNIT TESTS ");
        /* Set the Print function */
        testRunner_setPrintFunction(usbUart_print);
        
        /* ### Buffer Generate Test Suite ### */
        {
            /* Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            usbUart_print("\r\n*** Buffer generation ***\r\n");
            /* Nominal Case */
            testRunner_run(test_generateBuffers(&packetBuffer, "Normal", packets_LEN_BLOCK_PACKET, packets_ERR_SUCCESS));
            /* Long test */
            testRunner_run(test_generateBuffers(&packetBuffer, "Long", 1000, packets_ERR_MEMORY));
            /* Multiple create */
            packets_generateBuffers(&packetBuffer, 10);
            testRunner_run(test_generateBuffers(&packetBuffer, "Multiple Create", 10, packets_ERR_STATE));
            
            /* Clean Up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* ### Buffer Destroy Test Suite ### */
        {
            /* Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            usbUart_print("\r\n*** Buffer Destruction ***\r\n");
            /* Buffer not generated */
            testRunner_run( test_destroyBuffers(&packetBuffer, "Non-generated", packets_ERR_MEMORY));
            /* Normal packet */
            packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
            testRunner_run(test_destroyBuffers(&packetBuffer, "Normal", packets_ERR_SUCCESS));
            /* Too long buffer */
            packets_generateBuffers(&packetBuffer, 2000);
            testRunner_run(test_destroyBuffers(&packetBuffer, "Failed Gen", packets_ERR_MEMORY));
            /* Multiple destroy */
            packets_generateBuffers(&packetBuffer, 10);
            packets_destoryBuffers(&packetBuffer);
            testRunner_run(test_destroyBuffers(&packetBuffer, "Multiple destroy", packets_ERR_MEMORY));
            
            /* Clean Up (Uncessary, here for consistency */
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* ### Memory leak testing ### */
        {
            usbUart_print("\r\n*** Memroy Leak Generation ***\r\n");
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            testRunner_run(test_generateBuffers_iteration(&packetBuffer,"Repeat Generations", 1000));
        }
        
        
        /* ### Packet Creation Test Suite - State Errors ### */
        {
            usbUart_print("\r\n*** Construct Packet Test Suite - State Errors ***\r\n");
            /* Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
            /* Local reference to sending buffer */
            packets_PACKET_S* sendPacket = &(packetBuffer.send.packet); 
            /* Define dummy data */
            #define DATA_LEN 0x03
            uint8 dummyData[DATA_LEN] = {0x01, 0x03, 0x05};
            sendPacket->moduleId = 5;
            sendPacket->cmd = 0xCC;
            sendPacket->payload = dummyData;
            sendPacket->payloadLen = DATA_LEN;
            /* Simple working packet */
            testRunner_run(test_packetCreation_stateErrors(&packetBuffer, "Basic Packet", packets_ERR_SUCCESS));
            /* Incorrect module ID */
            sendPacket->moduleId = 10;
            testRunner_run(test_packetCreation_stateErrors(&packetBuffer, "Module ID: 10", packets_ERR_MODULE));
            /* Module ID 0 */
            sendPacket->moduleId = 0;
            testRunner_run(test_packetCreation_stateErrors(&packetBuffer, "Module ID: 0", packets_ERR_SUCCESS));
            /* No Payload */
            sendPacket->moduleId = 1;
            sendPacket->payloadLen = ZERO;
            testRunner_run(test_packetCreation_stateErrors(&packetBuffer, "No Payload", packets_ERR_SUCCESS));
            /* Long Packet - no memory */
            sendPacket->payloadLen = packets_LEN_BLOCK_PACKET + 1;
            testRunner_run(test_packetCreation_stateErrors(&packetBuffer, "Long Payload - no memory", ( packets_ERR_MEMORY )));
            /* Long Packet - max payload, and memory */
            sendPacket->payloadLen = 1024;
            testRunner_run(test_packetCreation_stateErrors(&packetBuffer, "Long Payload - max payload, memory", (packets_ERR_LENGTH | packets_ERR_MEMORY)));
            
            /* Clean Up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* ### Packet Creation Test Suite - Packet Values ### */
        {
            usbUart_print("\r\n*** Construct Packet Test Suite - Packet Values ***\r\n");
            /* Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
            /* Local reference to sending buffer */
            packets_PACKET_S* sendPacket = &(packetBuffer.send.packet); 
            /* Define valid dummy data */
            #define DATA_LEN 0x03
            uint8 dummyData[DATA_LEN] = {0x01, 0x03, 0x05};
            sendPacket->moduleId = 5;
            sendPacket->cmd = 0xCC;
            sendPacket->payload = dummyData;
            sendPacket->payloadLen = DATA_LEN;
            /* Simple working packet */
            uint8 validPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x03, 0x01, 0x03, 0x05, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x22, 0xAA};
            testRunner_run(test_packetCreation_packetVals(&packetBuffer, "Simple Packet", validPacket)); 
            /* No payload */
            sendPacket->payloadLen = ZERO;
            uint8 noPayloadPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x2E, 0xAA};
            testRunner_run(test_packetCreation_packetVals(&packetBuffer, "No payload", noPayloadPacket)); 
            /* Reponse Flags */
            sendPacket->flags = packets_FLAG_RESP;
            uint8 respFlagPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x2D, 0xAA};
            testRunner_run(test_packetCreation_packetVals(&packetBuffer, "Response Flag", respFlagPacket)); 
            /* Response and payload */
            sendPacket->payloadLen = 1;
            dummyData[0] = 0xFF;
            sendPacket->flags = packets_FLAG_RESP;
            uint8 respFlagPacket2[15] = {0x01, 0x05, 0xCC, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x2D, 0xAA};
            testRunner_run(test_packetCreation_packetVals(&packetBuffer, "Response Flag 2", respFlagPacket2)); 
            
            /* Clean Up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* ### Send Packet Test Suite ### */
        {
            usbUart_print("\r\n*** Send Packet ***\r\n");
            /* Setup - Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
            /* Basic Packet */
            testRunner_run(test_sendPacket(&packetBuffer, "Send Basic Packet", packets_ERR_SUCCESS));
            testRunner_run(test_sendPacket(&packetBuffer, "Send Basic Packet 2", packets_ERR_SUCCESS));
            
        
            /* Clean Up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* ### Process RX Byte Test Suite ### */
        {
            usbUart_print("\r\n*** Process RX Byte - State Errors ***\r\n");
            /* Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            /* No buffers generated */
            uint8 bytes[1] = {packets_SYM_START};
            testRunner_run(test_processRxByte_stateError(&packetBuffer, bytes, 1, "No Buffers", packets_ERR_MEMORY));
            /* Generate buffer */
            packets_generateBuffers(&packetBuffer, 100);
            /* Buffer overflow */
            packetBuffer.receive.processBuffer.bufferIndex = 100;
            bytes[0] = packets_SYM_START;
            testRunner_run(test_processRxByte_stateError(&packetBuffer, bytes, 1, "Buffer overflow", packets_ERR_MEMORY));
            /* Valid Start Byte */
            bytes[0] = packets_SYM_START;
            testRunner_run(test_processRxByte_stateError(&packetBuffer, bytes, 1, "Valid Start", packets_ERR_SUCCESS));
            /* Invalid Start Byte */
            bytes[0] = packets_SYM_START + 1;
            testRunner_run(test_processRxByte_stateError(&packetBuffer, bytes, 1, "invalid Start", packets_ERR_START_SYM));
            
            /* Clean up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        
        /* ### Packet parsing Test Suite - State Errors ### */
        {
            usbUart_print("\r\n*** Packet parsing - State Errors ***\r\n");
            /* Setup - Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
            /* Local references */
            packets_BUFFER_PROCESS_S* txBuffer = &(packetBuffer.send.processBuffer);
            /* No Payload, success */
            uint8 noPayloadPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x2E, 0xAA};
            txBuffer->bufferIndex = 12;
            memcpy(txBuffer->buffer, noPayloadPacket, txBuffer->bufferIndex);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Valid packet - no payload ", packets_ERR_SUCCESS));
            /* Success, payload */
            uint8 basePacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFD, 0x2E, 0xAA};
            txBuffer->bufferIndex = 14;
            memcpy(txBuffer->buffer, basePacket, txBuffer->bufferIndex);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Valid packet ", packets_ERR_SUCCESS));
            /* Incomplete packet */
            txBuffer->bufferIndex = 11;
            memcpy(txBuffer->buffer, noPayloadPacket,txBuffer->bufferIndex);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Short packet", packets_ERR_INCOMPLETE));
            /* Invalid start symbol */
            uint8 badStartPacket[15] = {0x02, 0x05, 0xCC, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFD, 0x2E, 0xAA};
            txBuffer->bufferIndex = 14;
            memcpy(txBuffer->buffer, badStartPacket, txBuffer->bufferIndex);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Invalid start sym", packets_ERR_START_SYM));
            /* Invalid module */
            uint8 badModulePacket[15] = {0x01, 0x07, 0xCC, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFD, 0x2E, 0xAA};
            txBuffer->bufferIndex = 14;
            memcpy(txBuffer->buffer, badModulePacket, txBuffer->bufferIndex);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Invalid Module", packets_ERR_MODULE));
            /* Invalid Checksum */
            uint8 badChecksumPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFD, 0xF0, 0xAA};
            txBuffer->bufferIndex = 14;
            memcpy(txBuffer->buffer, badChecksumPacket, txBuffer->bufferIndex);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Bad Checksum", packets_ERR_CHECKSUM));
            /* Invalid End symbol */
            uint8 badEndPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFD, 0x2E, 0xAB};
            txBuffer->bufferIndex = 14;
            memcpy(txBuffer->buffer, badEndPacket, txBuffer->bufferIndex);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Invalid End Symbol", packets_ERR_END_SYM));
        
            /* Clean up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        
        /* ### Packet parsing Test Suite - Packet vals ### */
        {
            usbUart_print("\r\n*** Packet parsing - Packet Values ***\r\n");
            /* Setup - Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer1;
            packets_initialize(&packetBuffer1);
            packets_generateBuffers(&packetBuffer1, packets_LEN_BLOCK_PACKET);
            /* Local references */
            packets_BUFFER_PROCESS_S* txBuffer = &(packetBuffer1.send.processBuffer);
            /* No Payload, success */
            uint8 noPayloadPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x2E, 0xAA};
            txBuffer->bufferIndex = 12;
            memcpy(txBuffer->buffer, noPayloadPacket, txBuffer->bufferIndex);
            uint8 payload[15];
            packets_PACKET_S expectedPacket = {
                .moduleId = 0x05,
                .cmd = 0xCC,
                .payload = payload,
                .payloadLen = 0,
                .payloadMax = packets_LEN_BLOCK_PACKET,
                .flags = ZERO,
                .error = ZERO
            };
            testRunner_run(test_packetParsing_packetVals(&packetBuffer1, "No payload ", &expectedPacket));
            
            /* No Payload, success */
            uint8 basePacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFD, 0x2E, 0xAA};
            uint8 payload2[15] = {0xFF, 0xFF};
            
            txBuffer->bufferIndex = 14;
            memcpy(txBuffer->buffer, basePacket, txBuffer->bufferIndex);
            packets_PACKET_S expectedPacket2 = {
                .moduleId = 0x05,
                .cmd = 0xCC,
                .payload = payload2,
                .payloadLen = 2,
                .payloadMax = packets_LEN_BLOCK_PACKET,
                .flags = ZERO,
                .error = ZERO
            };
            testRunner_run(test_packetParsing_packetVals(&packetBuffer1, "Payload payload ", &expectedPacket2));
            /* Clean up */
            packets_destoryBuffers(&packetBuffer1);
        }
        
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
        
    /* End MICA_TEST_PACKETS_ERRORS */
    #elif defined MICA_TEST_PACKETS
        /* Receive a packet from the IMU and print the result via the USB uart */
        /* Start the Components */
        UART_USB_Start();
        UART_IMU_Start();
        LEDS_Write(LEDS_ON_GREEN);
        /* Initialize variables */
        packets_BUFFER_FULL_S packetBuffer;
        packets_initialize(&packetBuffer);
        uint32 error = packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
        /* Ensure packet buffers were created properly */
        if(error){
            LEDS_Write(LEDS_ON_RED);
            for(;;){}
        }
        /* Infinite loop */
        for(;;){
            uint32 data = UART_IMU_UartGetByte();
            /* See if data is available */
            if( !(data & UART_IMU_UART_RX_UNDERFLOW) ){
                /* Process packet byte, check if packet complete */
                uint32 err = packets_processRxByte(&packetBuffer, (uint8) data); 
                /* Inidicate errors */
                if(err){
                    LEDS_G_Toggle();   
                    /* Reset the receive packet */
                    packets_flushRxBuffers(&packetBuffer);
                }
                /* Check if complete */
                if(packetBuffer.receive.bufferState == packets_BUFFER_RECEIVE_COMPLETE) {
                    LEDS_B_Toggle();
                    /* Parse the packet */
                    packets_parsePacket(&packetBuffer);
                    packets_PACKET_S* rxPacket = &(packetBuffer.receive.packet);
                    /* Clear the screen and print out the data */
                    usbUart_print("\n\r\nModule: %x \r\nCommand: %x\r\nPayload Len: %x\r\n", rxPacket->moduleId, rxPacket->cmd, rxPacket->payloadLen);
                    usbUart_print("Payload:");
                    uint8 i;
                    for(i=ZERO; i< rxPacket->payloadLen; i++){
                        usbUart_print(" %x", rxPacket->payload[i]);
                    }
                }
            }
        }
    /* End MICA_TEST_PACKETS */
    #elif defined MICA_TEST_PACKETS_ISR
        /* Receive a packet from the IMU Uart via an interrupt and print the result via the USB uart */
        /* Start the Components */
        UART_USB_Start();
        UART_IMU_Start();
        LEDS_Write(LEDS_ON_GREEN);
        /* Initialize variables */
        packets_BUFFER_FULL_S packetBuffer;
        packets_initialize(&packetBuffer);
        uint32 error = packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
        /* Ensure packet buffers were created properly */
        if(error){
            LEDS_Write(LEDS_ON_RED);
            for(;;){}
        }
        /* Enable the uart interrupts */
        UART_IMU_EnableInt();
        UART_IMU_SetCustomInterruptHandler(ISR_imuUart);

        /* Infinite loop */
        for(;;){
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
                    packets_parsePacket(&packetBuffer);
                    packets_PACKET_S* rxPacket = &(packetBuffer.receive.packet);
                    /* Clear the screen and print out the data */
                    usbUart_print("\n\r\nModule: %x \r\nCommand: %x\r\nPayload Len: %x\r\n", rxPacket->moduleId, rxPacket->cmd, rxPacket->payloadLen);
                    usbUart_print("Payload:");
                    uint8 i;
                    for(i=ZERO; i< rxPacket->payloadLen; i++){
                        usbUart_print(" %x", rxPacket->payload[i]);
                    }
                }
            }
        }
    /* End MICA_TEST_PACKETS_ISR */
    #else 
        #error "At least ONE MICA_TEST_<case> must be defined if MICA_TEST is defined"
    #endif
    #if defined(MICA_DEBUG) && defined(MICA_TEST)
        #error "MICA_DEBUG and MICA_TEST may not be defined at the same time"
    #endif
#endif /* MICA_TEST */
/* %%%%%%%%%%%%%%%%%%  End Debugging & Testing  %%%%%%%%%%%%%%%%%% */
    
    /* Infinite Loop */
    for(;;)
    {

    }
}

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
    if( UART_IMU_GetRxInterruptSourceMasked() & UART_IMU_INTR_RX_NOT_EMPTY){
        flag_pendingRxByte = true;
    }
}

/* [] END OF FILE */