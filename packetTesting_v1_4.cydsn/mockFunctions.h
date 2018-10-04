/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: mockFunctions.h
* Workspace: micaComponents_testing
* Project: packetTesting_v1_4
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: SupportCube 2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   Header for mockFunctions.c
*
* 2018.10.04  - Document Created
********************************************************************************/

/* Header Guard */
#ifndef mockFunctions_H
    /***************************************
    * Included files
    ***************************************/
    #include <stdint.h>
    /***************************************
    * Macro Definitions
    ***************************************/
//    #define DEFINE_NAME             (0x00)  /**< DEFINE NAME Description */
    
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
    /* Mocks */
    uint32_t mock_getRxBytesPending(void);
    uint8_t mock_readRxByte(void);
    uint32_t mock_queueRxByte(uint8_t byte);
    void mock_clearRxQueue(void);      
    void mock_queueArray(uint8_t* arr, uint16_t len);

#endif /* mockFunctions_H */
/* [] END OF FILE */
