/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: main.c
* Workspace: DriveBot_v5
* Project: DriveBot_v5.1
* Version: v5.1.0
* Authors: Craig Cheney,
*
* PCB: Support Cube v2.1.1
* PSoC: CYBLE-214015-01
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
#include "supportCubeParser.h"
#include "mockUart.h"

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
//    #define MICA_DEBUG_LEDS         /* Test the onboard LEDs */
    #define MICA_DEBUG_SELF_UART    /* Checks to see if the mica UART RX & TX lines are shorted together */
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
//    #define MICA_TEST_PACKET_HELPERS        /* ADDON to MICA_TEST_PACKETS_ERRORS Test the packet helpers */ 
//    #define MICA_TEST_PACKETS           /* Test Packet communication */
//    #define MICA_TEST_PACKETS_ISR        /* Receive packets from IMU via interrupt and print results */
//    #define MICA_TEST_PERIPHERAL_ID         /* Request the ID from the peripheral and display the results over USB */
//    #define MICA_TEST_SELF_PACKETS         /* REQ: Hard-wired RX & TX lines - Send packets to self for testing. Display the results over USB */
    
#endif
/* -------------- END TEST LEVEL --------------  */

/* --------- Local Variables --------- */

void ISR_imuUart(void);
void ISR_usbUart(void);
void ISR_Timer(void);
/* State variables */
volatile bool flag_pendingRxByte = false;
volatile bool flag_pendingUsbByte = false;
volatile bool timerExpired = false;
volatile uint32_t isrErr = ZERO;
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
    #elif defined MICA_DEBUG_SELF_UART
         /* Checks to see if the mica UART RX & TX lines are shorted together */
        /* Expected outcome:
        0. Green LED on
        1. UART writes a byte out the TX line
        2. UART reads in byte from RX line
        3a. If data is available and matches sent B led toggle
        3b. If data does not match toggle R led  
        */
        imuUart_Start();
        LEDS_Write(LEDS_ON_GREEN);
        MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
        LEDS_Write(LEDS_ON_NONE);
        uint8_t i = ZERO;
        /* Infinite loop */
        for(;;){
            /* Write a byte out over the UART */
            imuUart_putChar(i);
            MICA_delayMs(ONE);
            /* Variables for debugger */
            volatile uint8 bytesPending = UART_IMU_SpiUartGetRxBufferSize();
            volatile uint8_t byte = imuUart_getChar();
            /* Expect one byte in the RX buffer */
            if(bytesPending == ONE && byte == i++){
                LEDS_B_Toggle();
            } else {
                LEDS_R_Toggle();   
            }
            MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
        }
    /* End MICA_DEBUG_SELF_UART */
    #else
        #error "At least ONE MICA_DEBUG_<case> must be defined if MICA_DEBUG is defined"
    #endif /* End MICA_DEBUG_<case> */
    /* Infinite loop */
    for(;;){}
#endif /* MICA_DEBUG */
#ifdef MICA_TEST
        testRunner_setPrintFunction(usbUart_print);
    /* end MICA_TEST_PACKET_SPAWN */
        #if defined MICA_TEST_PACKETS_ERRORS
        /* Unit tests for MICA Errors */
        LEDS_Write(LEDS_ON_GREEN);
        usbUart_Start();
        imuUart_Start();
        usbUart_clearScreen();

        /* Print Program Header */
        usbUart_printHeader(__TIME__, __DATE__, "      PACKET TESTING UNIT TESTS ");
        /* Set the Print function */
        
        /* ### Ensure the packet version ### */
        {
            usbUart_print("\r\n*** Packet Version ***\r\n");
            testRunner_run(testRunner_printResults( "Packet Version 1.4",  packets_VERSION  , 0x104,  testRunner_MSG_NONE));
        }
        
        /* ### Helpers test suite ### */
        { 
            #ifdef MICA_TEST_PACKET_HELPERS
                usbUart_print("\r\n*** Command to Module ***\r\n");
                testRunner_run(test_commandToModule("Control Command to Module",  0x00, packets_ID_MODULE_CONTROL));   
                testRunner_run(test_commandToModule("Control Command to Module",  0x01, packets_ID_MODULE_CONTROL));   
                testRunner_run(test_commandToModule("Control Command to Module",  0x1F, packets_ID_MODULE_CONTROL));   
                testRunner_run(test_commandToModule("Control Response to Module", 0x80, packets_ID_MODULE_CONTROL));   
                testRunner_run(test_commandToModule("Control Response to Module", 0x81, packets_ID_MODULE_CONTROL));   
                testRunner_run(test_commandToModule("Control Response to Module", 0x9F, packets_ID_MODULE_CONTROL));   
                
                testRunner_run(test_commandToModule("Actuation Command to Module",  0x20, packets_ID_MODULE_ACTUATION));  
                testRunner_run(test_commandToModule("Actuation Command to Module",  0x25, packets_ID_MODULE_ACTUATION));   
                testRunner_run(test_commandToModule("Actuation Command to Module",  0x3F, packets_ID_MODULE_ACTUATION));  
                testRunner_run(test_commandToModule("Actuation Response to Module", 0xA0, packets_ID_MODULE_ACTUATION));   
                testRunner_run(test_commandToModule("Actuation Response to Module", 0xB3, packets_ID_MODULE_ACTUATION));   
                testRunner_run(test_commandToModule("Actuation Response to Module", 0xBF, packets_ID_MODULE_ACTUATION));   
                
                testRunner_run(test_commandToModule("Sensing Command to Module",   0x40, packets_ID_MODULE_SENSING));  
                testRunner_run(test_commandToModule("Sensing Command to Module",   0x44, packets_ID_MODULE_SENSING));   
                testRunner_run(test_commandToModule("Sensing Command to Module",   0x5F, packets_ID_MODULE_SENSING));  
                testRunner_run(test_commandToModule("Sensing Response to Module",  0xC0, packets_ID_MODULE_SENSING));   
                testRunner_run(test_commandToModule("Sensing Response to Module",  0xCA, packets_ID_MODULE_SENSING));   
                testRunner_run(test_commandToModule("Sensing Response to Module",  0xDF, packets_ID_MODULE_SENSING));  
                
                testRunner_run(test_commandToModule("Energy Command to Module",   0x60, packets_ID_MODULE_ENERGY));  
                testRunner_run(test_commandToModule("Energy Command to Module",   0x7A, packets_ID_MODULE_ENERGY));   
                testRunner_run(test_commandToModule("Energy Command to Module",   0x7F, packets_ID_MODULE_ENERGY));  
                testRunner_run(test_commandToModule("Energy Response to Module",  0xE0, packets_ID_MODULE_ENERGY));   
                testRunner_run(test_commandToModule("Energy Response to Module",  0xEB, packets_ID_MODULE_ENERGY));   
                testRunner_run(test_commandToModule("Energy Response to Module",  0xFF, packets_ID_MODULE_ENERGY)); 
               
                usbUart_print("\r\n*** Printing Packets ***\r\n");
                /* Setup - Create a packet object and initialize */
                packets_BUFFER_FULL_S packetBuffer1;
                packets_initialize(&packetBuffer1);
                packets_generateBuffers(&packetBuffer1, packets_LEN_PACKET_128);

                uint8 payload[15] = {0xFF, 0x0F};
                packets_PACKET_S testPacket = {
                    .moduleId = packets_ID_MODULE_ENERGY,
                    .cmd = 0x70,
                    .payload = payload,
                    .payloadLen = 2,
                    .payloadMax = packets_LEN_PACKET_128,
                    .flags = ZERO,
                    .error = ZERO
                };
                            
                packets_printPacket(&testPacket, usbUart_print);
                
                packets_destoryBuffers(&packetBuffer1);
            #endif/* End MICA_TEST_PACKET_HELPERS */
        }
        
        /* ### Buffer Generate Test Suite ### */
        {
            /* Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            usbUart_print("\r\n*** Buffer generation ***\r\n");
            /* Nominal Case */
            testRunner_run(test_generateBuffers(&packetBuffer, "Normal", packets_LEN_PACKET_128, packets_ERR_SUCCESS));
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
            packets_generateBuffers(&packetBuffer, packets_LEN_PACKET_128);
            testRunner_run(test_destroyBuffers(&packetBuffer, "Normal", packets_ERR_SUCCESS));
            /* Too long buffer */
            packets_generateBuffers(&packetBuffer, 2000);
            testRunner_run(test_destroyBuffers(&packetBuffer, "Failed Gen", packets_ERR_MEMORY));
            /* Multiple destroy */
            packets_generateBuffers(&packetBuffer, 10);
            packets_destoryBuffers(&packetBuffer);
            testRunner_run(test_destroyBuffers(&packetBuffer, "Multiple destroy", packets_ERR_MEMORY));
            
            /* Clean Up (Unnecessary, here for consistency */
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* ### Memory leak testing ### */
        {
            usbUart_print("\r\n*** Memory Leak Generation ***\r\n");
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
            packets_generateBuffers(&packetBuffer, packets_LEN_PACKET_128);
            /* Local reference to sending buffer */
            packets_PACKET_S* sendPacket = &(packetBuffer.send.packet); 
            /* Define dummy data */
            #define DATA_LEN 0x03
            uint8 dummyData[DATA_LEN] = {0x01, 0x03, 0x05};
            sendPacket->moduleId = packets_ID_MODULE_SENSING;
            sendPacket->cmd = 0xCC;
            sendPacket->payloadLen = DATA_LEN;
            memcpy(sendPacket->payload, dummyData, sendPacket->payloadLen);
            /* Simple working packet */
            testRunner_run(test_packetCreation_stateErrors(&packetBuffer, "Basic Packet", packets_ERR_SUCCESS));
            /* No Payload */
            sendPacket->moduleId = 1;
            sendPacket->payloadLen = ZERO;
            testRunner_run(test_packetCreation_stateErrors(&packetBuffer, "No Payload", packets_ERR_SUCCESS));
            /* Long Packet - no memory */
            sendPacket->payloadLen = packets_LEN_PACKET_128 + 1;
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
            packets_generateBuffers(&packetBuffer, packets_LEN_PACKET_128);
            /* Local reference to sending buffer */
            packets_PACKET_S* sendPacket = &(packetBuffer.send.packet); 
            /* Define valid dummy data */
            #define DATA_LEN 0x03
            uint8 dummyData[DATA_LEN] = {0x01, 0x03, 0x05};
            sendPacket->moduleId = packets_ID_MODULE_CONTROL;
            sendPacket->cmd = packets_CMD_CONTROL_MIN;
            sendPacket->payloadLen = DATA_LEN;
            memcpy(sendPacket->payload, dummyData, sendPacket->payloadLen);
            /* Simple working packet */
            uint8 validPacket[15] = {0x01, 0x00, 0x00, 0x03, 0x01, 0x03, 0x05, 0x00, 0x00, 0xFF, 0xF3, 0xAA};
            testRunner_run(test_packetCreation_packetVals(&packetBuffer, "Simple Packet", validPacket)); 
            /* No payload */
            sendPacket->payloadLen = ZERO;
            uint8 noPayloadPacket[15] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xAA};
            testRunner_run(test_packetCreation_packetVals(&packetBuffer, "No payload", noPayloadPacket)); 
            /* No Acknowledge  Flags */
            sendPacket->flags = packets_FLAG_NO_ACK;
            uint8 respFlagPacket[15] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x02, 0xFF, 0xFD, 0xAA};
            testRunner_run(test_packetCreation_packetVals(&packetBuffer, "No acknowledge flag", respFlagPacket)); 
            /* Response and payload */
            sendPacket->payloadLen = 1;
            dummyData[0] = 0xFF;
            memcpy(sendPacket->payload, dummyData, sendPacket->payloadLen);
            sendPacket->flags = packets_FLAG_NO_ACK;
            uint8 respFlagPacket2[15] = {0x01, 0x00, 0x00, 0x01, 0xFF, 0x00, 0x02, 0xFE, 0xFD, 0xAA};
            testRunner_run(test_packetCreation_packetVals(&packetBuffer, "No acknowledge flag 2", respFlagPacket2)); 
            
            /* Clean Up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* ### Send Packet Test Suite ### */
        {
            usbUart_print("\r\n*** Send Packet ***\r\n");
            /* No tx function registered */
            testRunner_run(test_sendPacket(NULL, "No TX function", packets_ERR_CALLBACK));
            /* Dummy tx registered */
            testRunner_run(test_sendPacket(imuUart_dummyTxArray, "Dummy TX", packets_ERR_SUCCESS));
            /* Mock tx registered */
            testRunner_run(test_sendPacket(mockUart1_TxPutArray, "Mock TX", packets_ERR_SUCCESS));
            /* Real TX */
            testRunner_run(test_sendPacket(imuUart_putArray, "Real TX", packets_ERR_SUCCESS));
        }
        
        /* ### Process RX Byte Test Suite ### */
        {
            usbUart_print("\r\n*** Process RX Byte - State Errors ***\r\n"); 
            /* Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            mockUart1_init(64);
            mockUart2_init(64);
            /* No buffers or RX Functions */
            testRunner_run(test_processRxByte_stateError(&packetBuffer, "No Buffers or RX Functions", packets_ERR_MEMORY | packets_ERR_CALLBACK));
            /* No RX Functions */
            packets_generateBuffers(&packetBuffer, 100);
            testRunner_run(test_processRxByte_stateError(&packetBuffer, "No RX Functions", packets_ERR_CALLBACK));
            /*  No RX get byte */
            packetBuffer.comms.rxGetBytesPending = mockUart1_RxGetBytesPending;
            testRunner_run(test_processRxByte_stateError(&packetBuffer, "No RX get byte function", packets_ERR_CALLBACK));
            /* No RX get pending */
            packetBuffer.comms.rxGetBytesPending = NULL;
            packetBuffer.comms.rxReadByte = mockUart1_RxReadByte;
            testRunner_run(test_processRxByte_stateError(&packetBuffer, "No RX get pending function", packets_ERR_CALLBACK));
            /* Valid RX functions */
            packetBuffer.comms.rxGetBytesPending = mockUart1_RxGetBytesPending;
            packetBuffer.comms.rxReadByte = mockUart1_RxReadByte;
            testRunner_run(test_processRxByte_stateError(&packetBuffer, "Valid RX functions", packets_ERR_SUCCESS));
            /* Buffer overflow */
            packetBuffer.receive.processBuffer.bufferIndex = 100;
            testRunner_run(test_processRxByte_stateError(&packetBuffer, "Buffer overflow", packets_ERR_MEMORY));

            /* Valid Start */
            mockUart1_TxPutByte(packets_SYM_START);
            packetBuffer.comms.rxReadByte = mockUart2_RxReadByte;
            packetBuffer.comms.rxGetBytesPending = mockUart2_RxGetBytesPending;
            testRunner_run(test_processRxByte_stateError(&packetBuffer, "Valid Start", packets_ERR_SUCCESS));
            mockUart1_RxClearQueue();
            /* Invalid start */
            mockUart1_TxPutByte(packets_SYM_START + 1);
            testRunner_run(test_processRxByte_stateError(&packetBuffer, "Inalid Start", packets_ERR_START_SYM));
            
            /* Clean up */
            mockUart1_destroy();
            mockUart2_destroy();
            packets_destoryBuffers(&packetBuffer);
        }
        
        
        /* ### Packet parsing Test Suite - State Errors ### */
        {
            usbUart_print("\r\n*** Packet parsing - State Errors ***\r\n");
            /* Setup - Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            mockUart1_init(64);
            mockUart2_init(64);
            packets_generateBuffers(&packetBuffer, packets_LEN_PACKET_128);
            packetBuffer.comms.rxGetBytesPending = mockUart2_RxGetBytesPending;
            packetBuffer.comms.rxReadByte = mockUart2_RxReadByte;
            packetBuffer.comms.ackCallback = ackHandler_print;
            packetBuffer.comms.cmdCallback = cmdHandler_print;
            /* No Payload, success */
            uint8 noPayloadPacket[15] = {0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFE, 0xAA};
            mockUart2_RxClearQueue();
            mockUart1_TxPutArray(noPayloadPacket, 9);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Valid packet - no payload ", packets_ERR_SUCCESS));
            /* Success, payload */
            uint8 basePacket[15] = {0x01, 0x05, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0xFD, 0xFA, 0xAA};
            mockUart2_RxClearQueue();
            mockUart1_TxPutArray(basePacket, 11);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Valid packet ", packets_ERR_SUCCESS));
            /* Incomplete packet */
            mockUart2_RxClearQueue();
            mockUart1_TxPutArray(noPayloadPacket, 8);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Short packet", packets_ERR_INCOMPLETE));
            /* Invalid start symbol */
            uint8 badStartPacket[15] = {0x02, 0x05, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0xFD, 0xF9, 0xAA};
            mockUart2_RxClearQueue();
            mockUart1_TxPutArray(badStartPacket, 11);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Invalid start sym", packets_ERR_START_SYM | packets_ERR_INCOMPLETE));
            /* Invalid Checksum */
            uint8 badChecksumPacket[15] = {0x01, 0x05, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0xFD, 0xF0, 0xAA};
            mockUart2_RxClearQueue();
            mockUart1_TxPutArray(badChecksumPacket, 11);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Bad Checksum", packets_ERR_CHECKSUM));
            /* Invalid End symbol */
            uint8 badEndPacket[15] = {0x01, 0x05, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0xFD, 0xFA, 0xAB};
            mockUart2_RxClearQueue();
            mockUart1_TxPutArray(badEndPacket, 11);
            testRunner_run(test_packetParsing_stateErrors(&packetBuffer, "Invalid End Symbol", packets_ERR_END_SYM));  
            
            /* Clean up */
            mockUart1_destroy();
            mockUart2_destroy();
            packets_destoryBuffers(&packetBuffer);
        }
        
        
        /* ### Packet parsing Test Suite - Packet vals ### */
        {
            usbUart_print("\r\n*** Packet parsing - Packet Values ***\r\n");
            /* Setup - Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            packets_generateBuffers(&packetBuffer, packets_LEN_PACKET_128);
            mockUart1_init(64);
            mockUart2_init(64);
            packetBuffer.comms.rxGetBytesPending = mockUart1_RxGetBytesPending;
            packetBuffer.comms.rxReadByte = mockUart1_RxReadByte;
            packetBuffer.comms.txPutArray = mockUart2_TxPutArray;
            packetBuffer.comms.ackCallback = ackHandler_print;
            packetBuffer.comms.cmdCallback = cmdHandler_print;
            
            /* No Payload, success */
            uint8 noPayloadPacket[15] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xAA};
//            usbUart_print("checksum: %x\r\n", packets_computeChecksum16(noPayloadPacket, 6));
            mockUart1_RxClearQueue();
            mockUart2_TxPutArray(noPayloadPacket, 9);
            uint8 payload[15];
            packets_PACKET_S expectedPacket = {
                .moduleId = packets_ID_MODULE_CONTROL,
                .cmd = 0x00,
                .payload = payload,
                .payloadLen = 0,
                .payloadMax = packets_LEN_PACKET_128,
                .flags = ZERO,
                .error = ZERO
            };
            testRunner_run(test_packetParsing_packetVals(&packetBuffer, "No payload ", &expectedPacket));
            
            /* No Payload, success */
            uint8 basePacket[15] = {0x01, 0x10, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0xFD, 0xEF, 0xAA};
            uint8 payload2[15] = {0xFF, 0xFF};
//            usbUart_print("checksum: %x\r\n", packets_computeChecksum16(basePacket, 8));
            mockUart1_RxClearQueue();
            mockUart2_TxPutArray(basePacket, 11);
            packets_PACKET_S expectedPacket2 = {
                .moduleId = packets_ID_MODULE_CONTROL,
                .cmd = 0x10,
                .payload = payload2,
                .payloadLen = 2,
                .payloadMax = packets_LEN_PACKET_128,
                .flags = ZERO,
                .error = ZERO
            };
            testRunner_run(test_packetParsing_packetVals(&packetBuffer, "Payload exists ", &expectedPacket2));
            /* Actuation */
            uint8 actuationPacket[15] = {0x01, 0x22, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0xFD, 0xDD, 0xAA};
            uint8 payloadAct[15] = {0xFF, 0xFF};
            mockUart1_RxClearQueue();
            mockUart2_TxPutArray(actuationPacket, 11);
            packets_PACKET_S expectedPacketAct = {
                .moduleId = packets_ID_MODULE_ACTUATION,
                .cmd = 0x22,
                .payload = payloadAct,
                .payloadLen = 2,
                .payloadMax = packets_LEN_PACKET_128,
                .flags = ZERO,
                .error = ZERO
            };
            testRunner_run(test_packetParsing_packetVals(&packetBuffer, "Actuation Module", &expectedPacketAct));
            /* Sensing Test */
            uint8 sensingPacket[15] = {0x01, 0x40, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0xFD, 0xBF, 0xAA};
            uint8 payload3[15] = {0xFF, 0xFF};
//            usbUart_print("checksum: %x\r\n", packets_computeChecksum16(sensingPacket, 8));
            mockUart1_RxClearQueue();
            mockUart2_TxPutArray(sensingPacket, 11);
            packets_PACKET_S expectedPacket3 = {
                .moduleId = packets_ID_MODULE_SENSING,
                .cmd = 0x40,
                .payload = payload3,
                .payloadLen = 2,
                .payloadMax = packets_LEN_PACKET_128,
                .flags = ZERO,
                .error = ZERO
            };
            testRunner_run(test_packetParsing_packetVals(&packetBuffer, "Sensing Module", &expectedPacket3));
            /* Energy Module */
            uint8 energyPacket[15] = {0x01, 0x70, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0xFD, 0x8F, 0xAA};
            uint8 payloadEnergy[15] = {0xFF, 0xFF};
            mockUart1_RxClearQueue();
            mockUart2_TxPutArray(energyPacket, 11);
            packets_PACKET_S expectedPacketEnergy = {
                .moduleId = packets_ID_MODULE_ENERGY,
                .cmd = 0x70,
                .payload = payloadEnergy,
                .payloadLen = 2,
                .payloadMax = packets_LEN_PACKET_128,
                .flags = ZERO,
                .error = ZERO
            };
            testRunner_run(test_packetParsing_packetVals(&packetBuffer, "Energy Module", &expectedPacketEnergy));
            /* Clean up */
            mockUart1_destroy();
            mockUart2_destroy();
            packets_destoryBuffers(&packetBuffer);
        }
       
        
        /* ### Self Packet Test - Wait ### */
        {
            usbUart_print("\r\n*** Self Packet Test - Wait ***\r\n");
            /* Setup - Create a packet object and initialize */
            packets_BUFFER_FULL_S device1, device2;
            packets_initialize(&device1);
            packets_initialize(&device2);
            packets_generateBuffers(&device1, packets_LEN_PACKET_128);
            packets_generateBuffers(&device2, packets_LEN_PACKET_128);
            mockUart1_init(64);
            mockUart2_init(64);
            /* Set up devices */
            device1.comms.rxGetBytesPending = mockUart1_RxGetBytesPending;
            device1.comms.rxReadByte = mockUart1_RxReadByte;
            device1.comms.txPutArray = mockUart1_TxPutArray;
            device1.comms.ackCallback = ackHandler_print;
            device1.comms.cmdCallback = cmdHandler_print;
            device2.comms.rxGetBytesPending = mockUart2_RxGetBytesPending;
            device2.comms.rxReadByte = mockUart2_RxReadByte;
            device2.comms.txPutArray = mockUart2_TxPutArray;
            device2.comms.ackCallback = ackHandler_print;
            device2.comms.cmdCallback = cmdHandler_print;
            /* Local Variables */
            packets_PACKET_S* txPacket = &(device1.send.packet);
            
            /* Blank packet */
            testRunner_run(test_selfPacket_wait(&device1, &device2, "Wait - UnInitialized Packet"));
            /* None zero packet */
            txPacket->cmd = 0x01;
            testRunner_run(test_selfPacket_wait(&device1, &device2, "Wait - New command"));
            /* With Payload */
            txPacket->cmd = 0x01;
            uint8_t data[10] = {0x01, 0xff, 0xCC, 0xDD, 0x3E};
            txPacket->payloadLen = 5;
            memcpy(txPacket->payload, data, txPacket->payloadLen);
            testRunner_run(test_selfPacket_wait(&device1, &device2, "Wait - Payload"));
            
            mockUart1_destroy();
            mockUart2_destroy();
            packets_destoryBuffers(&device1);
        }
        
         /* ### Validate Commands - Support Cube ### */
        {
            /* Create two different packet buffers and use mock uarts to parse
            uarts should be aligned for both RX and TX (i.e. device1 -> uart 1)*/
            usbUart_print("\r\n***Validate Commands - Support Cube ***\r\n");
            /* Setup - Create a packet object and initialize */
            packets_BUFFER_FULL_S device1, device2;
            uint32_t err = packets_initialize(&device1);
            err |= packets_initialize(&device2);
            err |= packets_generateBuffers(&device1, packets_LEN_PACKET_128);
            err |= packets_generateBuffers(&device2, packets_LEN_PACKET_128);
            if(err){
                usbUart_print("Failed to allocate memory");
            }
            mockUart1_init(64);
            mockUart2_init(64);
            /* Register callback functions - device 1 */
            device1.comms.rxGetBytesPending = mockUart1_RxGetBytesPending;
            device1.comms.rxReadByte = mockUart1_RxReadByte;
            device1.comms.txPutArray = mockUart1_TxPutArray;
            device1.comms.ackCallback = ackHandler_noop;
            device1.comms.cmdCallback = cmdHandler_supportCube;
            packets_PACKET_S* txPacket = &device1.send.packet;
            /* Register callback functions - device 2 */
            device2.comms.rxGetBytesPending = mockUart2_RxGetBytesPending;
            device2.comms.rxReadByte = mockUart2_RxReadByte;
            device2.comms.txPutArray = mockUart2_TxPutArray;
            device2.comms.ackCallback = ackHandler_noop;
            device2.comms.cmdCallback = cmdHandler_supportCube;

            /* Expected Packet */
            packets_PACKET_S expectedAck;
            memset(&expectedAck, 0, sizeof(packets_PACKET_S));
            expectedAck.flags = packets_FLAG_ACK;
            uint8_t expectedPayload[20] = {ZERO};
            expectedAck.payload = expectedPayload;
            
            /* ID command */
            txPacket->cmd = packets_CMD_ID;
            txPacket->payloadLen = ZERO;
            expectedAck.payloadLen = 0x04;
            expectedPayload[0] = SUPPORT_ID_DEVICE_MSB;
            expectedPayload[1] = SUPPORT_ID_DEVICE_LSB;
            expectedPayload[2] = SUPPORT_ID_FIRMWARE_MSB;
            expectedPayload[3] = SUPPORT_ID_DEVICE_LSB;
            testRunner_run(test_validateSupport_command(&device1, &device2, "ID Command", &expectedAck));
            mockUart1_RxClearQueue();
            /* Unknown Support Command */
            txPacket->cmd = 0x02;
            txPacket->payloadLen = ZERO;
            expectedAck.payloadLen = ZERO;
            expectedAck.flags = packets_FLAG_ACK | packets_FLAG_INVALID_CMD;
            testRunner_run(test_validateSupport_command(&device1, &device2, "Unknown command", &expectedAck));
            
            /* Clean-up */
            mockUart1_destroy();
            mockUart2_destroy();
            packets_destoryBuffers(&device1);
            packets_destoryBuffers(&device2);
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
        usbUart_Start();
        imuUart_Start();
        LEDS_Write(LEDS_ON_GREEN);
        /* Initialize variables */
        packets_BUFFER_FULL_S packetBuffer;
        packets_initialize(&packetBuffer);
        uint32 error = packets_generateBuffers(&packetBuffer, packets_LEN_PACKET_128);
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
        usbUart_Start();
        imuUart_Start();
        LEDS_Write(LEDS_ON_GREEN);
        /* Initialize variables */
        packets_BUFFER_FULL_S packetBuffer;
        packets_initialize(&packetBuffer);
        uint32 error = packets_generateBuffers(&packetBuffer, packets_LEN_PACKET_128);
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
    #elif defined MICA_TEST_PERIPHERAL_ID
        /* Request the ID from the peripheral and display the results over USB */
          /* Start the Components */
        usbUart_Start();
        imuUart_Start();
        LEDS_Write(LEDS_ON_GREEN);
        /* Initialize variables */
        packets_BUFFER_FULL_S packetBuffer;
        packets_initialize(&packetBuffer);
        uint32 error = packets_generateBuffers(&packetBuffer, packets_LEN_PACKET_128);
        /* Ensure packet buffers were created properly */
        if(error){
            LEDS_Write(LEDS_ON_RED);
            for(;;){}
        }
        /* Enable the uart interrupts */
        UART_IMU_EnableInt();
        UART_IMU_SetCustomInterruptHandler(ISR_imuUart);
        UART_USB_EnableInt();
        UART_USB_SetCustomInterruptHandler(ISR_usbUart);
        
        /* Print Program Header */
        usbUart_clearScreen();
        usbUart_printHeader(__TIME__, __DATE__, "      Peripheral Packet Test ");
        usbUart_print("\r\nPress 'enter' to request the ID from the peripheral\r\n");
        usbUart_print("Press 'c' to reset the LED to green\r\n");
        usbUart_print("Press 'space' to reset the device\r\n\r\n");
        
        
        /* Infinite loop */
        for(;;){
            /* See if there are any pending bytes */
            if(flag_pendingRxByte){
                /* Reset the flag */
                flag_pendingRxByte = false;
                uint32_t bytesPending = UART_IMU_SpiUartGetRxBufferSize();
                usbUart_print("Bytes Received: %d\r\n" , bytesPending);
                /* Read in the data */
                uint32_t i;
                for(i = ZERO; i < bytesPending; i++){
                    uint8 newData = imuUart_getChar();
                    usbUart_print("rx:0x%x\r\n", newData);
                    uint32 err = packets_processRxByte(&packetBuffer, newData);
                    /* Inidicate errors */
                    if(err){ 
                        usbUart_print("err: 0x%x\r\n", err);
                        /* Reset the receive packet */
                        packets_flushRxBuffers(&packetBuffer);
                    }
                    /* Check if packet is complete */
                    if(packetBuffer.receive.bufferState == packets_BUFFER_RECEIVE_COMPLETE) {
                        LEDS_Write(LEDS_ON_MAGENTA);
                        /* Parse the packet */
                        packets_parsePacket(&packetBuffer);
                        /* Clear the screen and print out the data */
                        packets_printPacket(&(packetBuffer.receive.packet), usbUart_print);
                    }
                }
            }
            /* Wait for a usb key to be hit */
            if(flag_pendingUsbByte){
                /* Reset flag */
                flag_pendingUsbByte = false;
                /* Read in the byte */
                uint8 usbData = usbUart_getChar();
                if(usbData == '\r'){
                    usbUart_print("Requesting device ID..\r\n");   
                    /* Initiate the request */
                    packetBuffer.send.packet.cmd = packets_CMD_ID;
                    uint32_t err = packets_sendPacket(&packetBuffer);
                    if(!err){
                        usbUart_print("Packet successfully sent, awaiting response\r\n");
                    } else {
                        usbUart_print("Packet failed to send with error 0x%x", err);   
                    }
                } else if(usbData == 'c'){
                    usbUart_print("Reset LEDs to green\r\n");
                    LEDS_Write(LEDS_ON_GREEN);   
                } else if(usbData == ' '){
                    /* Reset the device */
                    CySoftwareReset();
                }
            }
        }
    /* End MICA_TEST_PERIPHERAL_ID */
    #elif defined MICA_TEST_SELF_PACKETS
        /* REQ: Hard-wired RX & TX lines - Send packets to self for testing. Display the results over USB */
        /* See hardware test MICA_DEBUG_SELF_UART */
                  /* Start the Components */
        usbUart_Start();
        imuUart_Start();
        LEDS_Write(LEDS_ON_GREEN);

        /* Print Program Header */
        usbUart_clearScreen();
        usbUart_printHeader(__TIME__, __DATE__, "      Self Packet Test ");
        usbUart_print("\r\nDevice imuUart RX & TX must be wired together\r\n");
        usbUart_print("Press 'space' to reset the device\r\n\r\n");
        
        
        /* ### Self UART Test - Wait ### */
        {
            usbUart_print("\r\n*** Self UART Test ***\r\n");
            uint8_t data[255];
            uint8_t i;
            for(i = ZERO; i<255; i++){
                data[i] = i;   
            }
            /* Run a single byte; */
            testRunner_run(test_uartSelf("1 Byte", data, 1));
            testRunner_run(test_uartSelf("8 Bytes", data, 8));
            testRunner_run(test_uartSelf("64 Bytes", data, 64));
            testRunner_run(test_uartSelf("packets_LEN_PACKET_128", data, packets_LEN_PACKET_128));
            
        }
        
        /* ### Self Packet Test - Wait ### */
        {
            usbUart_print("\r\n*** Self Packet Test - Wait ***\r\n");
            /* Setup - Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            packets_generateBuffers(&packetBuffer, packets_LEN_PACKET_128);
            packetBuffer.comms.txPutArray = imuUart_putArray;
            packetBuffer.comms.rxGetBytesPending = imuUart_getRxBufferSize;
            packetBuffer.comms.rxReadByte = imuUart_getChar;
            packetBuffer.comms.ackCallback = ackHandler_print;
            packetBuffer.comms.cmdCallback = cmdHandler_print;
            /* Local Variables */
            packets_PACKET_S* txPacket = &(packetBuffer.send.packet);
            
            /* Blank packet */
            testRunner_run(test_selfPacket_HW_wait(&packetBuffer, "Wait - UnInitialized Packet"));
            /* None zero packet */
            txPacket->cmd = 0x01;
            testRunner_run(test_selfPacket_HW_wait(&packetBuffer, "Wait - New command"));
            /* With Payload */
            txPacket->cmd = 0x01;
            uint8_t data[10] = {0x01, 0xff, 0xCC, 0xDD, 0x3E};
            txPacket->payloadLen = 5;
            memcpy(txPacket->payload, data, txPacket->payloadLen);
            testRunner_run(test_selfPacket_HW_wait(&packetBuffer, "Wait - Payload"));
            
            packets_destoryBuffers(&packetBuffer);
        }

               /* ### Self Packet Test - Async ### */
        {
            usbUart_print("\r\n*** Self Packet Test - Async ***\r\n");
            /* Setup - Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            packets_generateBuffers(&packetBuffer, packets_LEN_PACKET_128);
            /* Local Variables */
            packets_PACKET_S* txPacket = &(packetBuffer.send.packet);
            
            /* Blank packet */
//            testRunner_run(test_selfPacket_async(&packetBuffer, "Async - UnInitialized Packet"));
//            /* None zero packet */
//            txPacket->cmd = 0x01;
//            testRunner_run(test_selfPacket_async(&packetBuffer, "Async - New command"));
//            /* With Payload */
//            txPacket->cmd = 0x01;
//            uint8_t data[10] = {0x01, 0xff, 0xCC, 0xDD, 0x3E};
//            txPacket->payloadLen = 5;
//            memcpy(txPacket->payload, data, txPacket->payloadLen);
//            testRunner_run(test_selfPacket_async(&packetBuffer, "Async - Payload"));
            
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* Display test suite results */
        testRunner_printCount();
        /* Infinite loop */
        for(;;){

            /* Read in the byte */
            uint8 usbData = usbUart_getChar();  
            if(usbData == ' '){
                /* Reset the device */
                CySoftwareReset();
            }
        }
    /* End MICA_TEST_SELF_PACKETS */
    #else 
        #error "At least ONE MICA_TEST_<case> must be defined if MICA_TEST is defined"
    #endif
    #if defined(MICA_DEBUG) && defined(MICA_TEST)
        #error "MICA_DEBUG and MICA_TEST may not be defined at the same time"
    #endif
#endif /* MICA_TEST */
/* %%%%%%%%%%%%%%%%%%  End Debugging & Testing  %%%%%%%%%%%%%%%%%% */
    
    /* Fall through Infinite Loop */
    for(;;){ }
} /* End main */

/*******************************************************************************
* ISR Name: ISR_imuUart()
********************************************************************************
* Summary:
*   ISR when data is received over the imu UART. Clearing this interrupt is 
*   not required, as it is a level sensitive interrupt.
* Interrupt: 
*       Internal to UART_IMU
*
*******************************************************************************/
void ISR_imuUart(void){
    /* Check if new data is available */
    if( UART_IMU_GetRxInterruptSourceMasked() & UART_IMU_INTR_RX_NOT_EMPTY){
        flag_pendingRxByte = true;
    }
}

/*******************************************************************************
* ISR Name: ISR_usbUart()
********************************************************************************
* Summary:
*   ISR when data is received over the usb UARTClearing this interrupt is 
*   not required, as it is a level sensitive interrupt.
* Interrupt: 
*       Internal to UART_USB
*
*******************************************************************************/
void ISR_usbUart(void){
    /* Check if new data is available */
    if( UART_USB_GetRxInterruptSourceMasked() & UART_USB_INTR_RX_NOT_EMPTY){
        flag_pendingUsbByte = true;
    }
}

/* [] END OF FILE */