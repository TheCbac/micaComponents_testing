/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: supportCubeParser.c
* Workspace: micaComponents_testing
* Project: packetTesting_v1_4
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: SupporCube 2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   Validator for the support cube
*
* 2018.10.01  - Document Created
********************************************************************************/
#include "supportCubeParser.h"
#include "micaCommon.h"
#include "project.h"


/*******************************************************************************
* Function Name: cmdHandler_supportCube()
****************************************************************************//**
* \brief
*  <function description>
*
* \param rxPacket [in]
*  Pointer to the packet containing the command to validate
*
* \param txPacket [out]
*  Pointer to the response packet
* 
* \return
*  Returns the error of associated with 
*******************************************************************************/
uint32_t cmdHandler_supportCube(packets_PACKET_S* rxPacket, packets_PACKET_S* txPacket){
    /* extract command */
    switch(rxPacket->cmd) {
        case packets_CMD_ID: {
            /* Respond with the device ID */
            uint8_t i = ZERO;
            txPacket->payload[i++] = SUPPORT_ID_DEVICE_MSB;
            txPacket->payload[i++] = SUPPORT_ID_DEVICE_LSB;
            txPacket->payload[i++] = SUPPORT_ID_FIRMWARE_MSB;
            txPacket->payload[i++] = SUPPORT_ID_FIRMWARE_LSB;
            txPacket->payloadLen = i;
            break;
        }
        /* Command not found */
        default:{
            /* Set the invalid command flag */
            txPacket->flags |= packets_FLAG_INVALID_CMD;
        }
    }
    
    return packets_ERR_SUCCESS;
}

/*******************************************************************************
* Function Name: ackHandler_print()
****************************************************************************//**
* \brief
*  Support cube handler for received acknowledge packet. Currently just toggles
*   the blue led.
*
* \param packet [in]
*  Pointer to the received packet
* 
* \return
*  Returns the error of associated with the operation
*******************************************************************************/
uint32_t ackHandler_supportCube(packets_PACKET_S* packet){
    (void) packet;
    LEDS_B_Toggle();
    return packets_ERR_SUCCESS;
}

/* ### Helpers ### */

/*******************************************************************************
* Function Name: cmdHandler_print()
****************************************************************************//**
* \brief
*  Prints a Received Command packet
*
* \param cmdPacket [in]
*  Pointer to the received packet
*
* \param ackPacket [out]
*  Pointer to the response packet to send out
*
* \return
*  Returns the error of associated with the operation
*******************************************************************************/
uint32_t cmdHandler_print(packets_PACKET_S* cmdPacket, packets_PACKET_S* ackPacket){
    (void) ackPacket;
    usbUart_print("Packet Command: 0x%x\r\n", cmdPacket->cmd);
    return packets_ERR_SUCCESS;
}
/*******************************************************************************
* Function Name: ackHandler_print()
****************************************************************************//**
* \brief
*  Prints a Received Command packet
*
* \param packet [in]
*  Pointer to the received packet
* 
* \return
*  Returns the error of associated with the operation
*******************************************************************************/
uint32_t ackHandler_print(packets_PACKET_S* packet){
    usbUart_print("Packet Acknowledged: 0x%x\r\n", packet->cmd);
    return packets_ERR_SUCCESS;
}

/*******************************************************************************
* Function Name: cmdHandler_noop()
****************************************************************************//**
* \brief
*  No Operation command handler
*
* \param cmdPacket [in]
*  Pointer to the received packet
*
* \param ackPacket [out]
*  Pointer to the response packet to send out
*
* \return
*  Returns the error of associated with the operation
*******************************************************************************/
uint32_t cmdHandler_noop(packets_PACKET_S* cmdPacket, packets_PACKET_S* ackPacket){
    (void) cmdPacket;
    (void) ackPacket;
    return packets_ERR_SUCCESS;
}
/*******************************************************************************
* Function Name: ackHandler_noop()
****************************************************************************//**
* \brief
*  No Operation ack handler
*
* \param packet [in]
*  Pointer to the received packet
* 
* \return
*  Returns the error of associated with the operation
*******************************************************************************/
uint32_t ackHandler_noop(packets_PACKET_S* packet){
    (void) packet;
    return packets_ERR_SUCCESS;
}





/* [] END OF FILE */
