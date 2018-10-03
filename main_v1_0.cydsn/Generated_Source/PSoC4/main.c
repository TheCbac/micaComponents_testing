/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: main.c
* Workspace: micaComponent_testing
* Project: main_v1_0
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: supportCube v2.1.2
* PSoC: CYBLE-214015-01
*
* Brief:
*   This is the top-level application file for supportCube.
*
* 2018.09.17  - Document Created
********************************************************************************/
#include <stdbool.h>
#include "project.h"


/* ######################### BEGIN PROGRAM CONFIGURATION ######################### */


//#define MICA_DEBUG
/* ---------------- DEBUG CASE ----------------
* Uncomment ONE of the following
* Debugging will only occur when MICA_DEBUG is defined
*/
#ifdef MICA_DEBUG
//    #define MICA_DEBUG_LEDS     /* Run through the testings of the LEDS */
#endif
/* -------------- END DEBUG CASE --------------  */
   

//#define MICA_TEST
/* ---------------- TEST CASE -----------------
* Uncomment ONE of the following
* Testing will only occur when MICA_TEST is defined
*/
#ifdef MICA_TEST
#endif
/* -------------- END TEST CASE ---------------  */


/* ############################# BEGIN MAIN PROGRAM ############################## */
/* Interrupt Service Routines */
void ISR_isrName(void);

/* Global Variables */
volatile bool flag_isrFlag = false;

#if !defined(MICA_DEBUG) && !defined(MICA_TESTING)
/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
*   The top-level application function for the project.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
int main(void)
{
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    
    /* Infinite Loop */
    for(;;) {
        
    }
}
#endif




/*******************************************************************************
* ISR Name: ISR_isrName()
********************************************************************************
* Summary:
*   <isrUserCase>
* Interrupt: 
*   <nameOfInterruptComponent>
*
*******************************************************************************/
void ISR_isrName(void){
    /* Clear the Interrupt */
    
    /* Set the flag */
    flag_isrFlag = true;
}


/* [] END OF FILE */
