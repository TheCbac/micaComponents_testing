/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: ringBuffer_testing.c
* Workspace: micaComponent_testing
* Project: ringBufferTesting_v1_0
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: Support Cube 2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   Units tests for the Ring Buffers
*
* 2018.10.08  - Document Created
********************************************************************************/
#include "ringBuffer_testing.h"

/*******************************************************************************
* Function Name: test_generateBuffers()
****************************************************************************//**
* \brief
*  Tests the creation of the ring buffer
*
* \param rb [in]
*  Pointer to ring buffer
*
* \param testName [in]
*  Name of the test to print in the results
*
* \param len [in]
*  Length of the buffer to create
*
* \param expectedResult [in]
*  The expected return (error) value of the function
*
* \return
*  A boolean indicating the result of the test
*******************************************************************************/
bool test_generateBuffers(ringBuffer_S* rb, char* testName, uint16 len, uint32_t expectedResult){
    /* Generate the buffer */
    uint32_t error = ringBuffer_init(rb, len, sizeof(uint8_t));
    /* Print results */
    bool result = testRunner_printResults(testName, error, expectedResult, testRunner_MSG_NONE);
    /* Destroy buffers */
    if(!error){
        ringBuffer_free(rb);
    }
    return result;
}

/*******************************************************************************
* Function Name: test_destroyBuffers()
****************************************************************************//**
* \brief
*  Tests the destruction of the ring buffer
*
* \param rb [in]
*  Pointer to ring buffer
*
* \param testName [in]
*  Name of the test to print in the results
*
* \param expectedResult [in]
*  The expected return (error) value of the function
*
* \return
*  A boolean indicating the result of the test
*******************************************************************************/
bool test_destroyBuffers(ringBuffer_S* rb, char* testName, uint32_t expectedResult){
    /* Generate the buffer */
    uint32_t error = ringBuffer_free(rb);
    /* Print results */
    bool result = testRunner_printResults(testName, error, expectedResult, testRunner_MSG_NONE);
    /* Destroy buffers */
    if(!error){
        ringBuffer_free(rb);
    }
    return result;
}


/*******************************************************************************
* Function Name: test_memoryLeak()
****************************************************************************//**
* \brief
*  Create and destroy buffers repeatedly
*
* \param rb
*   Pointer to ring buffer
*
* \param testName
*   Name of test
*
* \param iterations
*  Number of times to run the test
*
* \return
*  Boolean indicating if the test passed
*******************************************************************************/  
bool test_memoryLeak(ringBuffer_S* rb, char* testName, uint16 iterations){
    uint16 i;
    /* Create and destroy a whole buch of times */
    for(i = ZERO; i < iterations; i++){
        ringBuffer_init(rb, 100, sizeof(uint8_t));
        ringBuffer_free(rb);
    }
    /* Succeed if we get here */
    return testRunner_printResults(testName, ZERO, ZERO, testRunner_MSG_NONE);
}

/*******************************************************************************
* Function Name: test_ringBuffer_push()
****************************************************************************//**
* \brief
*  Test the pushhing of a value to the ring buffer
*
* \param rb
*   Pointer to ring buffer
*
* \param testName
*   Name of test
*
* \param iterations
*  Number of times to run the test
*
* \return
*  Boolean indicating if the test passed
*******************************************************************************/ 
bool test_ringBuffer_push(ringBuffer_S* rb, char* testName, uint32_t expectedResult){
    uint8_t val = 0x01;
    uint32_t error = ringBuffer_push(rb, &val); 
    /* Print results */
    return testRunner_printResults(testName, error, expectedResult, testRunner_MSG_NONE);
}

/*******************************************************************************
* Function Name: test_ringBuffer_pushArray()
****************************************************************************//**
* \brief
*  Test the pushing of an array to the ring buffer
*
* \param rb
*   Pointer to ring buffer
*
* \param testName
*   Name of test
*
* \param iterations
*  Number of times to run the test
*
* \return
*  Boolean indicating if the test passed
*******************************************************************************/ 
bool test_ringBuffer_pushArray(ringBuffer_S* rb, uint8_t len, char* testName, uint32_t expectedResult){
    uint8_t val[256], i;
    for(i=ZERO; i < len; i++){
        val[i] = i;
    }
    uint32_t error = ringBuffer_pushArray(rb, val, len); 
    /* Print results */
    return testRunner_printResults(testName, error, expectedResult, testRunner_MSG_NONE);
}


/*******************************************************************************
* Function Name: test_ringBuffer_pop()
****************************************************************************//**
* \brief
*  Test pop a value from the ring buffer
*
* \param rb
*   Pointer to ring buffer
*
* \param testName
*   Name of test
*
* \param iterations
*  Number of times to run the test
*
* \return
*  Boolean indicating if the test passed
*******************************************************************************/ 
bool test_ringBuffer_pop(ringBuffer_S* rb, char* testName, uint32_t expectedResult){
    uint8_t val;
    uint32_t error = ringBuffer_pop(rb, &val); 
    /* Print results */
    return testRunner_printResults(testName, error, expectedResult, testRunner_MSG_NONE);
}

/*******************************************************************************
* Function Name: test_ringBuffer_behavior()
****************************************************************************//**
* \brief
*  Test popping and pushing to a queue
*
* \return
*  Boolean indicating if the test passed
*******************************************************************************/ 
bool test_ringBuffer_behavior(void){
    ringBuffer_S buffer;
    ringBuffer_init(&buffer, 100, sizeof(uint8_t));
    bool result = true;
    /* Initial size */
    uint32_t actualSize = buffer.count;
    uint32_t expectedSize  = 0x00;
    result &= testRunner_printResults("Iniitial size", actualSize, expectedSize, testRunner_MSG_NONE);
    /*  Add byte */
    uint8_t val = 0x01;
    ringBuffer_push(&buffer, &val); 
    actualSize = buffer.count;
    expectedSize  = 0x01;
    result &= testRunner_printResults("One Byte", actualSize, expectedSize, testRunner_MSG_NONE);
    /* Add a second byte */
    val = 0x02;
    ringBuffer_push(&buffer, &val); 
    actualSize = buffer.count;
    expectedSize  = 0x02;
    result &= testRunner_printResults("Two bytes", actualSize, expectedSize, testRunner_MSG_NONE);
    /* Pop Byte 1 */
    uint8_t rVal;
    ringBuffer_pop(&buffer, &rVal);
    uint32_t actualVal = rVal;
    uint32_t expectedVal  = 0x01;
    result &= testRunner_printResults("Pop 1 - val", actualVal, expectedVal, testRunner_MSG_NONE);
    actualSize = buffer.count;
    expectedSize  = 0x01;
    result &= testRunner_printResults("Pop 1 - size", actualSize, expectedSize, testRunner_MSG_NONE);
    /* Pop 2 */
    ringBuffer_pop(&buffer, &rVal);
    actualVal = rVal;
    expectedVal  = 0x02;
    result &= testRunner_printResults("Pop 2 - val", actualVal, expectedVal, testRunner_MSG_NONE);
    actualSize = buffer.count;
    expectedSize  = 0x00;
    result &= testRunner_printResults("Pop 1 - size", actualSize, expectedSize, testRunner_MSG_NONE);
    /* Array */
    uint8_t arr[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    ringBuffer_pushArray(&buffer, arr, 6);
    actualSize = buffer.count;
    expectedSize  = 0x06;
    result &= testRunner_printResults("Array - size", actualSize, expectedSize, testRunner_MSG_NONE);
    /* Array vals */
        ringBuffer_pop(&buffer, &rVal);
    actualVal = rVal;
    expectedVal  = 0x01;
    result &= testRunner_printResults("Array - val 1", actualVal, expectedVal, testRunner_MSG_NONE);
        ringBuffer_pop(&buffer, &rVal);
    actualVal = rVal;
    expectedVal  = 0x02;
    result &= testRunner_printResults("Array - val 2", actualVal, expectedVal, testRunner_MSG_NONE);
        ringBuffer_pop(&buffer, &rVal);
    actualVal = rVal;
    expectedVal  = 0x03;
    result &= testRunner_printResults("Array - val 3", actualVal, expectedVal, testRunner_MSG_NONE);
        ringBuffer_pop(&buffer, &rVal);
    actualVal = rVal;
    expectedVal  = 0x04;
    result &= testRunner_printResults("Array - val 4", actualVal, expectedVal, testRunner_MSG_NONE);
        ringBuffer_pop(&buffer, &rVal);
    actualVal = rVal;
    expectedVal  = 0x05;
    result &= testRunner_printResults("Array - val 5", actualVal, expectedVal, testRunner_MSG_NONE);
        ringBuffer_pop(&buffer, &rVal);
    actualVal = rVal;
    expectedVal  = 0x06;
    result &= testRunner_printResults("Array - val 6", actualVal, expectedVal, testRunner_MSG_NONE);
    /* Free the buffer */
    ringBuffer_free(&buffer);
    /* Print results */
    return result;
}


/*******************************************************************************
* Function Name: test_ringBuffer_wrapAround()
****************************************************************************//**
* \brief
*  Tests the wrap around of the queue
*
* \return
*  Boolean indicating if the test passed
*******************************************************************************/ 
bool test_ringBuffer_wrapAround(void){
    ringBuffer_S buffer;
    ringBuffer_init(&buffer, 6, sizeof(uint8_t));
    bool result = true;
    /* Initial size */
    uint32_t actualSize = buffer.count;
    uint32_t expectedSize  = 0x00;
    result &= testRunner_printResults("Iniitial size", actualSize, expectedSize, testRunner_MSG_NONE);
    /* Add five elements  */
    uint8_t arr[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    ringBuffer_pushArray(&buffer, arr, 5);
    expectedSize  = 0x05;
    actualSize = buffer.count;
    result &= testRunner_printResults("Pushed size", actualSize, expectedSize, testRunner_MSG_NONE);
    /* pop 4 */
    uint8_t rVal;
        ringBuffer_pop(&buffer, &rVal);
    uint8_t expectedVal  = 0x01;
    expectedSize  = 0x04;
    actualSize = buffer.count;
    result &= testRunner_printResults("Wrap Val - 1", rVal, expectedVal, testRunner_MSG_NONE);
    result &= testRunner_printResults("Wrap size- 1", actualSize, expectedSize, testRunner_MSG_NONE);
        ringBuffer_pop(&buffer, &rVal);
    expectedVal  = 0x02;
    expectedSize  = 0x03;
    actualSize = buffer.count;
    result &= testRunner_printResults("Wrap Val - 2", rVal, expectedVal, testRunner_MSG_NONE);    
    result &= testRunner_printResults("Wrap size- 2", actualSize, expectedSize, testRunner_MSG_NONE);
        ringBuffer_pop(&buffer, &rVal);
    expectedVal  = 0x03;
    expectedSize  = 0x02;
    actualSize = buffer.count;
    result &= testRunner_printResults("Wrap Val - 3", rVal, expectedVal, testRunner_MSG_NONE);   
    result &= testRunner_printResults("Wrap size- 3", actualSize, expectedSize, testRunner_MSG_NONE);
        ringBuffer_pop(&buffer, &rVal);
    expectedVal  = 0x04;
    expectedSize  = 0x01;
    actualSize = buffer.count;
    result &= testRunner_printResults("Wrap Val - 4", rVal, expectedVal, testRunner_MSG_NONE);    
    result &= testRunner_printResults("Wrap size- 4", actualSize, expectedSize, testRunner_MSG_NONE);
    /* Push 5 */
    ringBuffer_pushArray(&buffer, arr, 5);
    expectedSize  = 0x06;
    actualSize = buffer.count;
    result &= testRunner_printResults("Wrap size- 5", actualSize, expectedSize, testRunner_MSG_NONE);
    ringBuffer_pop(&buffer, &rVal);
    expectedVal  = 0x05;
    expectedSize = 0x05;
    actualSize = buffer.count;
    result &= testRunner_printResults("Wrap Val - 6", rVal, expectedVal, testRunner_MSG_NONE);    
    result &= testRunner_printResults("Wrap size- 6", actualSize, expectedSize, testRunner_MSG_NONE);
    ringBuffer_pop(&buffer, &rVal);
    expectedVal  = 0x01;
    expectedSize  = 0x04;
    actualSize = buffer.count;
    result &= testRunner_printResults("Wrap Val - 7", rVal, expectedVal, testRunner_MSG_NONE);    
    result &= testRunner_printResults("Wrap size- 7", actualSize, expectedSize, testRunner_MSG_NONE);
    ringBuffer_pop(&buffer, &rVal);
    expectedVal  = 0x02;
    expectedSize  = 0x03;
    actualSize = buffer.count;
    result &= testRunner_printResults("Wrap Val - 8", rVal, expectedVal, testRunner_MSG_NONE);  
    result &= testRunner_printResults("Wrap size- 8", actualSize, expectedSize, testRunner_MSG_NONE);
    
    
    /* Free the buffer */
    ringBuffer_free(&buffer);
    /* Print results */
    return result;
}
/* [] END OF FILE */
