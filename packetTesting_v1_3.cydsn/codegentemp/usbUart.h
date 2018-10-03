/***************************************************************************//**
* \file usbUart.h
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
#ifndef usbUart_H
    #define usbUart_H
    /***************************************
    * Included files
    ***************************************/
    #include "cytypes.h"
    /***************************************
    * Function declarations 
    ***************************************/
    void usbUart_print(char8 *pszFmt,...);
    void usbUart_txTest(uint8 runs);
    void usbUart_putString(char * str);
    void usbUart_putArray(uint8 * array, uint16 length);
    void usbUart_printHeader(char* time, char* date, char * name);
    void usbUart_dummyTxArray(uint8 * array, uint16 len);
    
    /***************************************
    * Macro Definitions
    ***************************************/
//    @TODO
    // #define  usbUart_Start()
    #define usbUart_clearScreen()  usbUart_print("\033[2J\033[1;1H")    /**< Clear the terminal screen */
    /* TX Functions */
    #define usbUart_putChar(ch) UART_USB_UartPutChar(ch)  /**< Wrapper for UART TX function */
    
    /* RX functions */
    #define usbUart_RX_EN  1        /**< Is receiving enabled? */
    #if usbUart_RX_EN
        #define usbUart_getChar() UART_USB_UartGetChar() /**< Wrapper for UART RX function */
    #endif /* usbUart_RX_EN */
    
    /* Logging */ 
    #define usbUart_DEBUG_EN 1   /**< Is log printing enabled? */
    #if usbUart_DEBUG_EN
        #define usbUart_log(...) usbUart_print(__VA_ARGS__)  /**< Log Function */
    #else
        #define usbUart_log(...) do { (void)0; } while(0) /**< Without logging */
    #endif /*  */

    
#endif /* (usbUart_H) */
/* [] END OF FILE */
