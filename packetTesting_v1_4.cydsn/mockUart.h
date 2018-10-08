/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: mockUart.h
* Workspace: micaComponents_testing
* Project: packetTesting_v1_4
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: SupportCube 2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   Header for mockUart.c
*
* 2018.10.08  - Document Created
********************************************************************************/

/* Header Guard */
#ifndef mockUart_H
    /***************************************
    * Included files
    ***************************************/
    #include <stdint.h>
    #include <stdlib.h>
    /***************************************
    * Macro Definitions
    ***************************************/

    
    /***************************************
    * Enumerated Types
    ***************************************/

    
    /***************************************
    * Structures
    ***************************************/

    
    /***************************************
    * Function declarations 
    ***************************************/
    uint32_t mockUart1_init(size_t size);
    uint32_t mockUart1_destroy(void);
    uint32_t mockUart1_RxGetBytesPending(void);
    uint8_t mockUart1_RxReadByte(void);
    void mockUart1_RxClearQueue(void);      
    uint32_t mockUart1_TxPutByte(uint8_t byte);
    void mockUart1_TxPutArray(uint8_t* arr, uint16_t len);    

    uint32_t mockUart2_init(size_t size);
    uint32_t mockUart2_destroy(void);
    uint32_t mockUart2_RxGetBytesPending(void);
    uint8_t mockUart2_RxReadByte(void);
    void mockUart2_RxClearQueue(void);      
    uint32_t mockUart2_TxPutByte(uint8_t byte);
    void mockUart2_TxPutArray(uint8_t* arr, uint16_t len);    

#endif /* mockUart_H */
/* [] END OF FILE */
