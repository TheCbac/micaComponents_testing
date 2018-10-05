/***************************************************************************
*                                       MICA
* File: packet_testing.h
* Workspace: micaComponents_testing
* Project Name: libMica
* Version: v1.packetTesting_v1
* Author: Craig Cheney
*
* Brief:
*  Unit tests for the packets
* 
* Authors:
*   Craig Cheney
*
* Change Log:
*   2018.08.03 CC - Document created
********************************************************************************/
#ifndef PACKET_TESTING_H
    #define PACKET_TESTING_H
    
    /***************************************
    * Included files
    ***************************************/
    #include "project.h"
    /***************************************
    * Function Prototypes 
    ***************************************/
    bool test_generateBuffers(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint16 len, uint32_t expectedResult);
    bool test_generateBuffers_iteration(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint16 iterations);
    bool test_destroyBuffers(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint32_t expectedResult);
    bool test_packetCreation_stateErrors(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint32_t expectedResult);
    bool test_packetCreation_packetVals(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint8_t* expectedPacket);
    bool test_sendPacket(void (*txFunction)(uint8* src, uint16_t len), char* testName, uint32_t expectedResult);
    bool test_processRxByte_stateError(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint32_t expectedResult);
    bool test_packetParsing_stateErrors(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint32_t expectedResult);
    bool test_packetParsing_packetVals(packets_BUFFER_FULL_S* packetBuffer, char* testName, packets_PACKET_S* expectedPacket);
    bool test_commandToModule(char* testName, uint8_t cmd, uint8_t expectedModule);
    bool test_uartSelf(char* testName, uint8_t* data, uint16_t len);
    bool test_selfPacket_wait(packets_BUFFER_FULL_S* packetBuffer, char* testName);
    
    /* Helpers */
    bool comparePacketBuffer(packets_BUFFER_FULL_S* b1, packets_BUFFER_FULL_S* b2);
    bool comparePackets(packets_PACKET_S* p1, packets_PACKET_S* p2);
    bool cmdMatchAckPacket(packets_PACKET_S* cmdPacket, packets_PACKET_S* ackPacket);
    bool compareProcessBuffer(packets_BUFFER_PROCESS_S* p1, packets_BUFFER_PROCESS_S* p2);

    
#endif /* PACKET_TESTING_H */

/* [] END OF FILE */
