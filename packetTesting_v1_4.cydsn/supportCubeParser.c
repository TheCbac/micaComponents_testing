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



/*******************************************************************************
* Function Name: acknowledgePacket()
****************************************************************************//**
* \brief
*  <function description>
*
* \param rxPacket [in]
*  Pointer to the received packet
* 
* \return
*  Returns the error of associated with 
*******************************************************************************/
uint32_t acknowledgePacket(packets_BUFFER_FULL_S* packet){
    /* Validate that the command is valid */
    uint32_t validateErr = validateSupportCubeCmd(&(packet->receive.packet), &(packet->send.packet));
    uint32_t responseErr = packets_ERR_SUCCESS;
    /* Command is valid */
    if(validateErr == packets_ERR_SUCCESS){
        /* Respond if the NO ACK flag is not set */
        if( !(packet->receive.packet.flags & packets_FLAG_NO_ACK)){
            /* Construct the response packet */
            responseErr = packets_constructPacket(packet);
            if(!responseErr){
                /* Send the response packet */
                responseErr = packets_sendPacket(packet);
            }
        }
    /* An error occured during validation */
    } else {
        responseErr = validateErr;
    }
    return responseErr;
}


/*******************************************************************************
* Function Name: validateSupportCubeCmd()
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
uint32_t validateSupportCubeCmd(packets_PACKET_S* rxPacket, packets_PACKET_S* txPacket){
    /* Set the acknowledge response flag by default */
    txPacket->flags = packets_FLAG_ACK;
    /* extract command */
    switch(rxPacket->cmd) {
        case CMD_ID: {
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
* Function Name: processPacketEvents()
****************************************************************************//**
* \brief
*  Manages the packet lifecycle. Needs to be called at least once per 
*
* \param packet [in]
*  Pointer to the packet manage
*
* \param commandCallback [out]
* Callback that gets called when a packet 
* 
* \return
*  Returns an error code if a fatal error occurs, zero otherwise
*******************************************************************************/
uint32_t processPacketEvents(packets_BUFFER_FULL_S* packet){
    // see if RX bytes are avaiable, and process them
    uint16_t rxPending = packet->comms.rxGetBytesPending();
    if(rxPending){
        uint16_t i;
        for(i = ZERO; i < rxPending; i++){
//            packets_processRxByte(packet);
        }
    }
}

/* [] END OF FILE */
