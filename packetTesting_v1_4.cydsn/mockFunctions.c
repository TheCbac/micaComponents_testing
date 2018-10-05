/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: mockFunctions.c
* Workspace: micaComponents_testing
* Project: packetTesting_v1_4
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: SupportCube 2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   Functions for testing the receive functionality of the parser. Should just
* turn this into a ring buffer.
*
* 2018.10.04  - Document Created
********************************************************************************/
#include "mockFunctions.h"
#include "micaCommon.h"

/* Variables */
uint8_t rxQueue[128];
uint8_t queueHead = ZERO;  /**< Index of the next element to pop*/
uint8_t queueTail = ZERO;  /**< Index of the next element to enqueue*/

/*******************************************************************************
* Function Name: mock_getRxBytesPending()
****************************************************************************//**
* \brief
*  Get the number of bytes queued 
*
* \return
*  The number of bytes in the queue
*******************************************************************************/
uint32_t mock_getRxBytesPending(void){
    return queueTail - queueHead;
}

/*******************************************************************************
* Function Name: mock_readRxByte()
****************************************************************************//**
* \brief
*  Returns the next element in the queue
*
* \return
*  The next element of the queue
*******************************************************************************/
uint8_t mock_readRxByte(void){
    /* Return the next queue element */
    int queueSize = queueTail - queueHead;
    if ( queueSize > 0){
        /* Return the next queue element */
        uint8_t data = rxQueue[queueHead++];
        /* Reset queue if no elements in it */
        if(queueTail == queueHead){
            mock_clearRxQueue();   
        }
        return data;
    }
    /* No elements ready, return zero */
    return ZERO;
}

/*******************************************************************************
* Function Name: mock_queueRxByte()
****************************************************************************//**
* \brief
*  Place a byte into the queue
*
* \param byte [in]
*   The value to place into the queue
*
* \return
*  The number of elements in the queue
*******************************************************************************/
uint32_t mock_queueRxByte(uint8_t byte){
    /* Place into queue */
    rxQueue[queueTail] = byte;
    /*  Return size of the queue */
    return ++queueTail;
}

/*******************************************************************************
* Function Name: mock_clearRxQueue()
****************************************************************************//**
* \brief
*  Resets the queue size and index
*
* \return
*  None
*******************************************************************************/
void mock_clearRxQueue(void){
    queueHead = ZERO;
    queueTail = ZERO;
}

/*******************************************************************************
* Function Name: mock_queueArray()
****************************************************************************//**
* \brief
*  Enqueue elements from an array
*
* \param arr [in]
*   Array containing values to enqueue
*
* \param len [in]
*   Number of elements to enqueue
*
* \return
*  None
*******************************************************************************/
void mock_queueArray(uint8_t* arr, uint16_t len){

    uint8_t i;
    for(i=ZERO; i< len; i++){
        mock_queueRxByte(arr[i]);   
    }
}

            /* mock tests - should move to their own test space */        
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte());
//            mock_queueRxByte(0x01);
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte());
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte());
//            mock_queueRxByte(0x02);
//            mock_queueRxByte(0x03);
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte());
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte());
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte());
//            uint8_t arr[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
//            mock_queueArray(arr, 6);
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte());
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte());
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte());
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte());
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte());
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte());
//            usbUart_print("Queue size: %d, Value:0x%x\r\n", mock_getRxBytesPending(), mock_readRxByte())

/* [] END OF FILE */
