/***************************************************************************
*                                       MICA
* File: testRunner.h
* Workspace: micaComponents
* Project Name: libMica
* Version: v1.0
* Author: Craig Cheney
*
* Brief:
*   Source for the test runner to run unit test
* 
* Authors:
*   Craig Cheney
*
* Change Log:
*   2018.09.07 CC - Document created
********************************************************************************/
#include "testRunner.h"
#include "micaCommon.h"


static uint16_t passCount = ZERO;
static uint16_t testCount = ZERO;
void (*printFunction)(char *pszFmt, ...) = NULL;

/* Macro to test that print function has been set  */
#define testRunner_print(...) do {\
        if(printFunction != NULL){\
            printFunction(__VA_ARGS__);\
        }\
    } while(0)


/*******************************************************************************
* Function Name: testRunner_run()
****************************************************************************//**
* \brief
*  Records the results of a unit test
*
* \param passCount
*   Pointer to the number of test that have passed
*
* \param testCount
*   Pointer to the number of test run
*
* \return
*   None
*******************************************************************************/
void testRunner_run(bool testResult) {
    testCount++;
    passCount += testResult;
}


/*******************************************************************************
* Function Name: testRunner_resetCount()
****************************************************************************//**
* \brief
*  Resets the recorded values of the test
*
* \return
*   None
*******************************************************************************/
void testRunner_resetCount(void) {
    testCount = ZERO;
    passCount = ZERO;
}

/*******************************************************************************
* Function Name: testRunner_printCount()
****************************************************************************//**
* \brief
*  Displays the results of the testing suite
*
* \param testName
*   Name of the test in question
*
* \param error
*   Actual value of error
*
* \param expectedResult, 
*   Expected value of the error
*
* \param optErr
*   An optional Error to display if the test failed for reasons other
*   than a non-zero error code
*
* \return
*   Whether of not the test passed
*******************************************************************************/
void testRunner_printCount(void){
    testRunner_print("\r\n\n> Passed %d/%d tests\r\n", passCount, testCount);
    /* Reset count */
    testRunner_resetCount();
}


/*******************************************************************************
* Function Name: testRunner_printResults()
****************************************************************************//**
* \brief
*  Displays the results of the test
*
* \param testName
*   Name of the test in question
*
* \param error
*   Actual value of error
*
* \param expectedResult, 
*   Expected value of the error
*
* \param optErr
*   An optional Error to display if the test failed for reasons other
*   than a non-zero error code
*
* \return
*   Whether of not the test passed
*******************************************************************************/
bool testRunner_printResults(char* testName, uint32_t error, uint32_t expectedResult, char* optErr){
    bool result = false;
    /* Print results */
    testRunner_print(testName);
    testRunner_print(": ");
    if(error != expectedResult){
        testRunner_print("Failed, Expected: 0x%x, Got: 0x%x\r\n", expectedResult, error);   
    } else if(optErr[0] != ZERO){
        testRunner_print("Failed: ");
        testRunner_print(optErr);
        testRunner_print("\r\n");
    } else {
        testRunner_print("Passed\r\n");   
        result = true;
    }
    return result;
}


/*******************************************************************************
* Function Name: testRunner_setPrintFunction()
****************************************************************************//**
* \brief
*  Sets the function to use when printing the test results
*
* \param fn
*   Pointer to the function to use
*
* \return
*   None
*******************************************************************************/
void testRunner_setPrintFunction( void (*fn)(char *pszFmt, ...) ) {
    printFunction = fn;
}

/* [] END OF FILE */
