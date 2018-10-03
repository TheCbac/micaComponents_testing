/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: imuPacketParser.h
* Workspace: micaComponents_testing
* Project: packetTesting_v1_4_peripheral
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: SupporCube 2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   Header for imuPacketParser.c
*
* 2018.10.023  - Document Created
********************************************************************************/

/* Header Guard */
#ifndef imuPacketParser_H
    /***************************************
    * Included files
    ***************************************/
    #include <stdint.h>
    #include "packets.h"
    /***************************************
    * Macro Definitions
    ***************************************/
    #define CMD_ID                          (0x00)  /**< The ID command */
    #define IMU_ID_DEVICE_MSB               (0x00)
    #define IMU_ID_DEVICE_LSB               (0x02)
    #define IMU_ID_FIRMWARE_MSB             (0x00)
    #define IMU_ID_FIRMWARE_LSB             (0x01)
    
    /***************************************
    * Enumerated Types
    ***************************************/
//    typedef enum {
//        typeVal1,                 /**< val1 Description */
//        typeVal2                  /**< val2 Description */
//    } ENUM_NAME_T;
    
    /***************************************
    * Structures
    ***************************************/
//    typedef struct {
//        uint8_t param;            /**< param Description */
//    } structName_S;
    
    /***************************************
    * Function declarations 
    ***************************************/
    //uint32_t acknowledgePacket(packets_BUFFER_FULL_S* rxPacket);
    uint32_t validateImuPacket(packets_PACKET_S* rxPacket, packets_PACKET_S* txPacket);
//    uint32_t acknowledgePacket(packets_BUFFER_FULL_S* packet, uint32_t (*validateFn)(packets_PACKET_S* rxPacket, packets_PACKET_S* txPacket) );
    
  

#endif /* imuPacketParser_H */
/* [] END OF FILE */
