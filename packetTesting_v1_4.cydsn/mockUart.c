/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: mockUart.c
* Workspace: micaComponents_testing
* Project: packetTesting_v1_4
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: SupportCube 2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   Functions for self testing packet parsers
*
* 2018.10.08  - Document Created
********************************************************************************/
#include "mockUart.h"
#include "project.h"

ringBuffer_S rb1;
ringBuffer_S rb2;

/* *************** UART 1 *************** */

/*******************************************************************************
* Function Name: mockUart1_init()
****************************************************************************//**
* \brief
*  Initializes the ringbuffer with a certain value
*
* \param size
*   Size of the buffer to initialize
*
* \return
*  The error code of the start procedure
*******************************************************************************/
uint32_t mockUart1_init(size_t size){
    return ringBuffer_init(&rb1, size, sizeof(uint8_t)); 
}

/*******************************************************************************
* Function Name: mockUart1_destroy()
****************************************************************************//**
* \brief
*  Frees the initialized ring buffer
*
* \return
*  The error code of the destroy procedure
*******************************************************************************/
uint32_t mockUart1_destroy(void){
    return ringBuffer_free(&rb1); 
}


/*******************************************************************************
* Function Name: mockUart1_RxGetBytesPending()
****************************************************************************//**
* \brief
*  Returns the number of elements in the buffer
*
* \return
*  The number of elements in the buffer
*******************************************************************************/
uint32_t mockUart1_RxGetBytesPending(void){
    return rb1.count;
}


/*******************************************************************************
* Function Name: mockUart1_RxReadByte()
****************************************************************************//**
* \brief
*  Returns the next element in the queue. Returns 0 if no element is queued
*
* \return
*  The next element of the queue
*******************************************************************************/
uint8_t mockUart1_RxReadByte(void){
    uint8_t val;
    uint32_t err = ringBuffer_pop(&rb1, &val);
    if(!err){
        return val;
    }
    return ZERO;
}

/*******************************************************************************
* Function Name: mockUart1_RxClearQueue()
****************************************************************************//**
* \brief
*  Clears the queue.
*
* \return
*  None
*******************************************************************************/
void mockUart1_RxClearQueue(void){
    ringBuffer_clear(&rb1);
}


/*******************************************************************************
* Function Name: mockUart1_TxPutByte()
****************************************************************************//**
* \brief
*   Places a byte into the (RX) ring buffer of mockUart2
*
* \param byte
*   The byte to place in the buffer
*
* \return
*  The error code of the write operation
*******************************************************************************/
uint32_t mockUart1_TxPutByte(uint8_t byte){
    return ringBuffer_push(&rb2, &byte);
}


/*******************************************************************************
* Function Name: mockUart1_TxPutArray()
****************************************************************************//**
* \brief
*  Places an array into the (RX) ring buffer of mockUart2
*
* \param byte
*   The byte to place in the buffer
*
* \param len
*   The number of bytes
*
* \return
*  None
*******************************************************************************/
void mockUart1_TxPutArray(uint8_t* arr, uint16_t len){
    ringBuffer_pushArray(&rb2, arr, len);   
}


/* *************** UART 2 *************** */

/*******************************************************************************
* Function Name: mockUart2_init()
****************************************************************************//**
* \brief
*  Initializes the ringbuffer with a certain value
*
* \param size
*   Size of the buffer to initialize
*
* \return
*  The error code of the start procedure
*******************************************************************************/
uint32_t mockUart2_init(size_t size){
    return ringBuffer_init(&rb2, size, sizeof(uint8_t)); 
}

/*******************************************************************************
* Function Name: mockUart2_destroy()
****************************************************************************//**
* \brief
*  Frees the initialized ring buffer
*
* \return
*  The error code of the destroy procedure
*******************************************************************************/
uint32_t mockUart2_destroy(void){
    return ringBuffer_free(&rb2); 
}


/*******************************************************************************
* Function Name: mockUart2_RxGetBytesPending()
****************************************************************************//**
* \brief
*  Returns the number of elements in the buffer
*
* \return
*  The number of elements in the buffer
*******************************************************************************/
uint32_t mockUart2_RxGetBytesPending(void){
    return rb2.count;
}


/*******************************************************************************
* Function Name: mockUart2_RxReadByte()
****************************************************************************//**
* \brief
*  Returns the next element in the queue. Returns 0 if no element is queued
*
* \return
*  The next element of the queue
*******************************************************************************/
uint8_t mockUart2_RxReadByte(void){
    uint8_t val;
    uint32_t err = ringBuffer_pop(&rb2, &val);
    if(!err){
        return val;
    }
    return ZERO;
}

/*******************************************************************************
* Function Name: mockUart2_RxClearQueue()
****************************************************************************//**
* \brief
*  Clears the queue.
*
* \return
*  None
*******************************************************************************/
void mockUart2_RxClearQueue(void){
    ringBuffer_clear(&rb2);
}


/*******************************************************************************
* Function Name: mockUart2_TxPutByte()
****************************************************************************//**
* \brief
*   Places a byte into the (RX) ring buffer of mockUart1
*
* \param byte
*   The byte to place in the buffer
*
* \return
*  The error code of the write operation
*******************************************************************************/
uint32_t mockUart2_TxPutByte(uint8_t byte){
    return ringBuffer_push(&rb1, &byte);
}


/*******************************************************************************
* Function Name: mockUart2_TxPutArray()
****************************************************************************//**
* \brief
*  Places an array into the (RX) ring buffer of mockUart1
*
* \param byte
*   The byte to place in the buffer
*
* \param len
*   The number of bytes
*
* \return
*  None
*******************************************************************************/
void mockUart2_TxPutArray(uint8_t* arr, uint16_t len){
    ringBuffer_pushArray(&rb1, arr, len);   
}

/* [] END OF FILE */
