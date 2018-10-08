/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: ringBuffer_testing.h
* Workspace: micaComponent_testing
* Project: ringBufferTesting_v1_0
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: Support Cube 2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   Header for ringBuffer_testing.c
*
* 2018.10.08  - Document Created
********************************************************************************/

/* Header Guard */
#ifndef ringBuffer_testing_H
    /***************************************
    * Included files
    ***************************************/
    #include "project.h"
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
    bool test_generateBuffers(ringBuffer_S* rb, char* testName, uint16 len, uint32_t expectedResult);
    bool test_destroyBuffers(ringBuffer_S* rb, char* testName, uint32_t expectedResult);
    bool test_memoryLeak(ringBuffer_S* rb, char* testName, uint16 iterations);
    bool test_ringBuffer_push(ringBuffer_S* rb, char* testName, uint32_t expectedResult);
    bool test_ringBuffer_pushArray(ringBuffer_S* rb, uint8_t len, char* testName, uint32_t expectedResult);
    bool test_ringBuffer_pop(ringBuffer_S* rb, char* testName, uint32_t expectedResult);
    /* Lumped tests */
    bool test_ringBuffer_behavior(void);
    bool test_ringBuffer_wrapAround(void);
    
    

#endif /* ringBuffer_testing_H */
/* [] END OF FILE */
