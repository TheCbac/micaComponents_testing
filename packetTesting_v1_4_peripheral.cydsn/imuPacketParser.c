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
#include "imuPacketParser.h"
#include "micaCommon.h"


/*******************************************************************************
* Function Name: validateImuPacket()
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
uint32_t validateImuPacket(packets_PACKET_S* rxPacket, packets_PACKET_S* txPacket){
    /* extract command */
    switch(rxPacket->cmd) {
        case packets_CMD_ID: {
            /* Respond with the device ID */
            uint8_t i = ZERO;
            txPacket->payload[i++] = IMU_ID_DEVICE_MSB;
            txPacket->payload[i++] = IMU_ID_DEVICE_LSB;
            txPacket->payload[i++] = IMU_ID_FIRMWARE_MSB;
            txPacket->payload[i++] = IMU_ID_FIRMWARE_LSB;
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

/* [] END OF FILE */
