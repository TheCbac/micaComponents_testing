/***************************************************************************//**
* \file usbUart.c
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
#include "usbUart.h"
#include "UART_USB_SPI_UART.h"
#include "micaCommon.h"


/*******************************************************************************
* Function Name: usbUart_putString()
****************************************************************************//**
*
* \brief Prints out the string starting at address passsed until a null character
    is reached.
*
* \param Address of array, must be zero terminated
*
* \return
* None
 *******************************************************************************/
void usbUart_putString(char * str){
    /* Iterate until a zero is hit */
    while(*str){
        usbUart_putChar(*str);
        /* Increment index */
        str++;
    }
    
}

/*******************************************************************************
* Function Name: usbUart_putArray()
****************************************************************************//**
*
* \brief Prints out the data starting at the addressed passed in, for the specified 
*   length
*
* \param Array  - Address of array with the data to print
*
* \param length - Length of data to write
*
* \return
* None
 *******************************************************************************/
void usbUart_putArray(uint8 * array, uint16 length){
    uint8 i;
    /* Loop until length satisfacition is met*/
    for(i=ZERO; i < length; i++){
        usbUart_putChar(*array);
        /* Increment index */
        array++;
    }
    
}


/*******************************************************************************
* Function Name: usbUart_changeBase()
****************************************************************************//**
*
* \brief Changes the base to hex from decimal
*
* \param Index
*  new value
*
* \return
* None
 *******************************************************************************/
static inline uint8* usbUart_changeBase(uint32 Index)
{
    return (uint8*)("0123456789ABCDEF"+Index);
}

/*******************************************************************************
* Function Name: usbUart_print()
****************************************************************************//**
*
* \brief Prints out the string
*
* \param pszFmt
*   Pointer to string, zero formatted. String to print out, accepts %d, %s, %x, %c
*
* \return
* None
 *******************************************************************************/
void usbUart_print(char8 *pszFmt,...){
    uint8 *pszVal;
    uint32 iVal, xVal, i = 0, buffer[12], index = 1;
    uint8 cVal;
    uint32 *pArg;
    pArg =(uint32 *)&pszFmt;

    while(*pszFmt)
    {
        if('%' != *pszFmt)
        {
            usbUart_putChar(*pszFmt);
            pszFmt++;
            continue;
        }
        pszFmt++;

        if(*pszFmt == 's')
        {
            pszVal = (uint8*)pArg[index++];
            for(; *pszVal != '\0'; pszVal++)
                usbUart_putChar(*pszVal);
            pszFmt++;
            continue;
        }
        if(*pszFmt == 'd')
        {
            iVal = pArg[index++];
            i = 0;
            do{
                buffer[i++] = iVal % 10;
                iVal /= 10;
            }while(iVal);
            while(i > 0)
            {
                i--;
                usbUart_putChar(*usbUart_changeBase(buffer[i]));
            }
            pszFmt++;
            continue;
        }
        if(*pszFmt == 'c')
        {
            cVal = (uint8)pArg[index++];
            usbUart_putChar(cVal);
            pszFmt++;
            continue;
        }
        if(*pszFmt == 'x')
        {
            xVal = pArg[index++];
            i = 0;
            do{
                buffer[i++] = xVal % 16;
                xVal /= 16;
            }while(xVal);
            if(i%2!=0)
                buffer[i++]=0;
            if(i<2)
                buffer[i++]=0;

            while(i > 0)
            {
                i--;
                usbUart_putChar(*usbUart_changeBase(buffer[i]));
            }
            pszFmt++;
            continue;
        }
        if(pszFmt == '\0')
        {
            break;
        }
    }
}


/*******************************************************************************
* Function Name: usbUart_txTest()
****************************************************************************//**
*
* \brief Transmits the a string listing the compile date and time out over the UART.
*
* \param runs
*    The number of times to execute the test function. Zero indicates
*       infinite runs. 
*
* \return
* None
*******************************************************************************/
void usbUart_txTest(uint8 runs) {
    uint8 i = ZERO;
    /* Run through a specified number of times */
    while( (i++ < runs) || runs == ZERO) {
        /* Print the date and time the program was compiled */
        usbUart_print("Compiled: ");
        usbUart_print(__TIME__);
        usbUart_print(" ");
        usbUart_print(__DATE__);
        usbUart_print("\r\n");
        /* Delay */
        MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
    }
}  

/*******************************************************************************
* Function Name: usbUart_printHeader()
****************************************************************************//**
*
* \brief Displays the program header
*
* \param time
*    Pass in __TIME__ macro
*
* \param date
*    Pass in __DATE__ macro
*
* \param name
*    Name of the program to display 
*
* \return
* None
*******************************************************************************/
    void usbUart_printHeader(char* time, char* date, char * name){
    usbUart_print("**************************************\r\n* ");
    usbUart_print(name);
    usbUart_print("\r\n*\r\n* Compiled at: ");
    usbUart_print(time);
    usbUart_print(" on ");
    usbUart_print(date);
    usbUart_print("\r\n**************************************\r\n");
}
/*******************************************************************************
* Function Name: usbUart_dummyTxArray()
****************************************************************************//**
* \brief
*  Dummy tx function for supressing output. Has no effect.
*
*******************************************************************************/
void  usbUart_dummyTxArray(uint8 * array, uint16 len){
    (void) array;
    (void) len;
}

/* [] END OF FILE */
