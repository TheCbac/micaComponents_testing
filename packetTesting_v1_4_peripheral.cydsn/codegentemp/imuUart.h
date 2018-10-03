/***************************************************************************//**
* \file imuUart.h
* \version 0.1.0
*
* \brief
*   Simple print functionality
*
* \date Date Created:  2017.12.19
* \date Last Modified: 2017.12.19
*
********************************************************************************
* \copyright
* Copyright 2017, MICA. All rights reserved.
*******************************************************************************/
/* Header Guard */
#ifndef imuUart_H
    #define imuUart_H
    /***************************************
    * Included files
    ***************************************/
    #include "cytypes.h"
    /***************************************
    * Function declarations 
    ***************************************/
    void imuUart_print(char8 *pszFmt,...);
    void imuUart_txTest(uint8 runs);
    void imuUart_putString(char * str);
    void imuUart_putArray(uint8 * array, uint16 length);
    void imuUart_printHeader(char* time, char* date, char * name);
    void imuUart_dummyTxArray(uint8 * array, uint16 len);
    
    /***************************************
    * Macro Definitions
    ***************************************/
//    @TODO
    // #define  imuUart_Start()
    #define imuUart_clearScreen()  imuUart_print("\033[2J\033[1;1H")    /**< Clear the terminal screen */
    /* TX Functions */
    #define imuUart_putChar(ch) UART_UartPutChar(ch)  /**< Wrapper for UART TX function */
    
    /* RX functions */
    #define imuUart_RX_EN  1        /**< Is receiving enabled? */
    #if imuUart_RX_EN
        #define imuUart_getChar() UART_UartGetChar() /**< Wrapper for UART RX function */
    #endif /* imuUart_RX_EN */
    
    /* Logging */ 
    #define imuUart_DEBUG_EN 1   /**< Is log printing enabled? */
    #if imuUart_DEBUG_EN
        #define imuUart_log(...) imuUart_print(__VA_ARGS__)  /**< Log Function */
    #else
        #define imuUart_log(...) do { (void)0; } while(0) /**< Without logging */
    #endif /*  */

    
#endif /* (imuUart_H) */
/* [] END OF FILE */
