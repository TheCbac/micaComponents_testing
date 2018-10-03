/***************************************************************************
*                                       MICA
* File: testRunner.h
* Workspace: micaComponents
* Project Name: libMica
* Version: v1.0
* Author: Craig Cheney
*
* Brief:
*   Prototype definitions for the test runner
* 
* Authors:
*   Craig Cheney
*
* Change Log:
*   2018.09.07 CC - Document created
********************************************************************************/
/* Header Guard */
#ifndef testRunner_H
    #define testRunner_H
    /***************************************
    * Included files
    ***************************************/
    #include <stdint.h>
    #include <stdbool.h>
    /***************************************
    * Macro Definitions
    ***************************************/
    #define testRunner_MSG_NONE       ""
    /***************************************
    * Function Prototypes 
    ***************************************/
    void testRunner_run(bool testResult);
    void testRunner_resetCount(void);
    void testRunner_printCount(void);
    bool testRunner_printResults(char* testName, uint32_t error, uint32_t expectedResult, char* optErr);
    void testRunner_setPrintFunction( void (*fn)(char *pszFmt, ...) );

    
#endif /* testRunner_H */
/* [] END OF FILE */