/***************************************************************************
*                                       MICA
* File: packets.c
* Workspace: micaComponents
* Project Name: libMica
* Version: v1.4
* Author: Craig Cheney
*
* Brief:
*   Binary packet specification for MICA
* 
* Authors:
*   Craig Cheney
*
* Change Log:
*   2018.09.17 CC - Document created
********************************************************************************/
#include "packets.h"
#include "micaCommon.h"
#include <stdlib.h>


/*******************************************************************************
* Function Name: packets_initialize()
****************************************************************************//**
* \brief
*  Initializes the packet structure to default values
*
* \param buffer
*   Pointer to the packet to be initialized. 
*
* \return
*  A number indicating the error code
*  Errors codes                             | Description
*   ------------                            | -----------
*   packets_ERR_SUCCESS            | On Successful init
*******************************************************************************/
uint32_t packets_initialize(packets_BUFFER_FULL_S *packetBuffer) {
    /* Set everything to zero */
    memset(packetBuffer, ZERO, sizeof(packets_BUFFER_FULL_S ));
    /* Reset the state */ 
    packetBuffer->receive.bufferState = packets_BUFFER_RECEIVE_WAIT_FOR_START;
    /* Reset the tx state */
    packetBuffer->send.bufferState = packets_BUFFER_SEND_WAIT;
    return packets_ERR_SUCCESS;
}

/*******************************************************************************
* Function Name: packets_generateBuffers()
****************************************************************************//**
* \brief
*  Allocates memory for a new packet buffer
*
* \param buffer
*   Pointer to the packet to be initialized. 
*
* \return
*  A number indicating the error code
*  Errors codes                             | Description
*   ------------                            | -----------
*   packets_ERR_SUCCESS            | On Successful init
*******************************************************************************/
uint32_t packets_generateBuffers(packets_BUFFER_FULL_S *packetBuffer, uint16_t bufferSize) {
    uint32_t error = packets_ERR_SUCCESS;
    /* Local References */
    packets_BUFFER_PROCESS_S* rxBuffer = &(packetBuffer->receive.processBuffer);
    packets_BUFFER_PROCESS_S* txBuffer = &(packetBuffer->send.processBuffer);
    uint8_t* rxPayload = packetBuffer->receive.packet.payload;
    uint8_t* txPayload = packetBuffer->send.packet.payload;
    
    /* Make sure that buffers have not already been allocated */
    if(rxBuffer->buffer != NULL || txBuffer->buffer != NULL || rxPayload != NULL || txPayload != NULL){
        error |= packets_ERR_STATE;
    }

    if(!error) {
        /* Create receive process buffer */
        uint8* processRxBufferAdr = (uint8 *) calloc(ONE, bufferSize);
        if(processRxBufferAdr == NULL){
            error |= packets_ERR_MEMORY; 
            /* Do not try to allocate again */
            goto packets_clean1;
        }
        packetBuffer->receive.processBuffer.buffer = processRxBufferAdr;
        packetBuffer->receive.processBuffer.bufferLen = bufferSize;

        /* Create Send process buffer */
        uint8* processTxBufferAdr = (uint8 *) calloc(ONE, bufferSize);
        if(processTxBufferAdr == NULL){
            error |= packets_ERR_MEMORY; 
            goto packets_clean2;
        }
        packetBuffer->send.processBuffer.buffer = processTxBufferAdr;
        packetBuffer->send.processBuffer.bufferLen = bufferSize;
        
        /*  Create the rxPayload */
        uint8* rxPayloadAdr = (uint8 *) calloc(ONE, bufferSize);
        if(rxPayloadAdr == NULL){
            error |= packets_ERR_MEMORY; 
            goto packets_clean3;
        }
        packetBuffer->receive.packet.payload = rxPayloadAdr;
        packetBuffer->receive.packet.payloadMax = bufferSize;
        packetBuffer->receive.bufferState = packets_BUFFER_RECEIVE_WAIT_FOR_START;
        
        /*  Create the txPayload */
        uint8* txPayloadAdr = (uint8 *) calloc(ONE, bufferSize);
        if(txPayloadAdr == NULL){
            error |= packets_ERR_MEMORY; 
            goto packets_clean4;
        }
        packetBuffer->send.packet.payload = txPayloadAdr;
        packetBuffer->send.packet.payloadMax = bufferSize;
        packetBuffer->send.bufferState = packets_BUFFER_SEND_WAIT;

        /* Clean up on error */
        if(error) {  
packets_clean4:
            free(txPayloadAdr);      
packets_clean3:
            free(rxPayloadAdr);
packets_clean2:
            free(processTxBufferAdr);
packets_clean1:
            free(processRxBufferAdr);
        }
    }
    /* return error flags */
    return error;
}

/*******************************************************************************
* Function Name: packets_destoryBuffers()
****************************************************************************//**
* \brief
*  Deallocates memory from a packet
*
* \param buffer
*   Pointer to the packet to be initialized. 
*
* \return
*  A number indicating the error code
*  Errors codes                             | Description
*   ------------                            | -----------
*   packets_ERR_SUCCESS            | On Successful init
*******************************************************************************/
uint32_t packets_destoryBuffers(packets_BUFFER_FULL_S *buffer) {
    uint32_t error = packets_ERR_SUCCESS;
    /* Free the Send process buffer is it exists */
    if(buffer->send.processBuffer.buffer != NULL){ 
        free(buffer->send.processBuffer.buffer);   
        buffer->send.processBuffer.buffer = NULL;
        buffer->send.processBuffer.bufferLen = ZERO;
        buffer->send.processBuffer.bufferIndex = ZERO;
        buffer->send.processBuffer.timeCount = ZERO;
    } else {
        error |= packets_ERR_MEMORY;
    }
    /* Free the receive process buffer is it exists */
    if(buffer->receive.processBuffer.buffer != NULL){ 
        free(buffer->receive.processBuffer.buffer);   
        buffer->receive.processBuffer.buffer = NULL;
        buffer->receive.processBuffer.bufferLen = ZERO;
        buffer->receive.processBuffer.bufferIndex = ZERO;
        buffer->receive.processBuffer.timeCount = ZERO;
    } else {
        error |= packets_ERR_MEMORY;
    }
    /* Free the receive payload buffer is it exists */
    if(buffer->receive.packet.payload != NULL) { 
        free(buffer->receive.packet.payload);   
        buffer->receive.packet.payload = NULL;
        buffer->receive.packet.payloadMax = ZERO;
        buffer->receive.packet.payloadLen = ZERO;
    } else {
        error |= packets_ERR_MEMORY;
    }
    /* Free the send payload buffer is it exists */
    if(buffer->send.packet.payload != NULL) { 
        free(buffer->send.packet.payload);   
        buffer->send.packet.payload = NULL;
        buffer->send.packet.payloadMax = ZERO;
        buffer->send.packet.payloadLen = ZERO;
    } else {
        error |= packets_ERR_MEMORY;
    }
    /* Return error */
    return error;
}


/*******************************************************************************
* Function Name: packets_flushRxBuffers()
****************************************************************************//**
* \brief
*  Flush the RX and TX process buffer
*
* \param buffer
*  Pointer to the full buffers
*
* \return
* A error code with the result of packing
*
*******************************************************************************/
void packets_flushRxBuffers(packets_BUFFER_FULL_S* buffer){
    /* Reset RX buffers */
    buffer->receive.processBuffer.bufferIndex = ZERO;
    buffer->receive.bufferState = packets_BUFFER_RECEIVE_WAIT_FOR_START;   
}


/*******************************************************************************
* Function Name: packets_flushTxBuffers()
****************************************************************************//**
* \brief
*  Flush the RX and TX process buffer
*
* \param buffer
*  Pointer to the full buffers
*
* \return
* A error code with the result of packing
*
*******************************************************************************/
void packets_flushTxBuffers(packets_BUFFER_FULL_S* buffer){
     /* Reset TX buffers */
    buffer->send.processBuffer.bufferIndex = ZERO;
    buffer->send.bufferState = packets_BUFFER_SEND_WAIT;
}

/*******************************************************************************
* Function Name: packets_flushBuffers()
****************************************************************************//**
* \brief
*  Flush the RX and TX process buffer
*
* \param buffer
*  Pointer to the full buffers
*
* \return
* A error code with the result of packing
*
*******************************************************************************/
void packets_flushBuffers(packets_BUFFER_FULL_S* buffer){
    packets_flushTxBuffers(buffer);
    packets_flushRxBuffers(buffer);
}

/*******************************************************************************
* Function Name: packets_constructPacket()
****************************************************************************//**
* \brief
*  Pack the data in the TX packet to the TX processBuffer
*
* \param packet
*  Pointer to the packet to pack
*
*
* \return
* A error code with the result of packing
*
*******************************************************************************/
uint32_t packets_constructPacket(packets_BUFFER_FULL_S* buffer) {
    uint32_t error = packets_ERR_SUCCESS;
    /* Create local references */
    packets_BUFFER_PROCESS_S* txBuffer = &(buffer->send.processBuffer);
    packets_BUFFER_STATE_SEND_T* bufferState = &(buffer->send.bufferState);
    packets_PACKET_S* packet = &(buffer->send.packet);
    
    /* *** Validate packet *** */
    /* LEN less than max */
    if (packet->payloadLen > packets_LEN_MAX_PAYLOAD) {
        error |= packets_ERR_LENGTH;
    }
    /* LEN less than process buffer max */
    if ( (packet->payloadLen + packets_LEN_OVERHEAD) > txBuffer->bufferLen) {
        error |= packets_ERR_MEMORY;
    }
    /* Send buffer must not be busy */
    if(*bufferState != packets_BUFFER_SEND_WAIT) {
       error |= packets_ERR_STATE;
    }
    /* Assemble the packet if no error */
    if(!error) {
        /* Reset the buffer count */
        txBuffer->bufferIndex = ZERO;
        /* Header */
        txBuffer->buffer[(txBuffer->bufferIndex)++] = packets_SYM_START;
        txBuffer->buffer[(txBuffer->bufferIndex)++] = packet->cmd;
        /* Payload length MSB */
        txBuffer->buffer[(txBuffer->bufferIndex)++] = (packet->payloadLen >> BITS_ONE_BYTE) & MASK_BYTE_ONE;
        /* Payload length MSB */
        txBuffer->buffer[(txBuffer->bufferIndex)++] = (packet->payloadLen) & MASK_BYTE_ONE;
        /* Payload */
        uint16_t j;
        for(j = ZERO; j < packet->payloadLen; j++) {
            txBuffer->buffer[(txBuffer->bufferIndex)++] = packet->payload[j];
        }
        /* Footer */
        /* Flags Bytes 3-0 */
        txBuffer->buffer[(txBuffer->bufferIndex)++] = (uint8_t) ((packet->flags) >> BITS_ONE_BYTE);
        txBuffer->buffer[(txBuffer->bufferIndex)++] = (uint8_t)  (packet->flags);
        /* Calculate checksum */
        uint16_t checksum = packets_computeChecksum16(txBuffer->buffer, packets_LEN_HEADER + packet->payloadLen + packets_LEN_FLAGS);
        txBuffer->buffer[(txBuffer->bufferIndex)++] = (checksum >> BITS_ONE_BYTE) & MASK_BYTE_ONE;
        txBuffer->buffer[(txBuffer->bufferIndex)++] = (checksum) & MASK_BYTE_ONE;
        txBuffer->buffer[(txBuffer->bufferIndex)++] = packets_SYM_END;
        /* Advance to Ready to send */
        *bufferState =  packets_BUFFER_SEND_READY;
    }
    /* Return error code */
    return error;
}


/*******************************************************************************
* Function Name: packets_sendPacket()
****************************************************************************//**
* \brief
*   Constructs and then sends of the data stored in the TX process buffer 
*
* \param packet
*  Pointer to the packet to send of out the rx buffer
*
* \return
* A error code with the result of packing
*
*******************************************************************************/
uint32_t packets_sendPacket(packets_BUFFER_FULL_S *buffer){
    uint32_t error = packets_ERR_SUCCESS;
    /* Construct the tx packet */
    error |= packets_constructPacket(buffer);
    /* Create local references */
    packets_BUFFER_PROCESS_S* txBuffer = &(buffer->send.processBuffer);
    packets_BUFFER_STATE_SEND_T* bufferState = &(buffer->send.bufferState);
    /* Send buffer must not be busy */
    if(*bufferState != packets_BUFFER_SEND_READY) {
       error |= packets_ERR_STATE;
    }
    /* Ensure valid txFunction */
    if(buffer->comms.txPutArray == NULL){
        error |= packets_ERR_CALLBACK;
    }
    if(!error) {
        /* Write out the function */
        // (txBuffer->buffer, txBuffer->bufferIndex );
        // @TODO: check available buffer size first
        buffer->comms.txPutArray(txBuffer->buffer, txBuffer->bufferIndex );
        /* Move back to the original state */
        *bufferState = packets_BUFFER_SEND_WAIT;
        /* Flush the TX buffer */
        packets_flushTxBuffers(buffer);   

    }
    return error;
}


/*******************************************************************************
* Function Name: packets_processRxByte()
****************************************************************************//**
* \brief
*  Processes the lastest received byt . 
*
* \return
*  A number indicating the error code
*  Errors codes                             | Description
*   ------------                            | -----------
*   packets_ERR_SUCCESS            | On Successful init
*******************************************************************************/
uint32_t packets_processRxByte(packets_BUFFER_FULL_S* buffer, uint8_t byte) {
     uint32_t error = packets_ERR_SUCCESS;
    /* Create local references */
    packets_BUFFER_PROCESS_S* rxBuffer = &(buffer->receive.processBuffer);
    packets_BUFFER_STATE_RECEIVE_T* bufferState = &(buffer->receive.bufferState);
    uint16_t* payloadLen = &(buffer->receive.packet.payloadLen);

    /* Ensure that buffers are valid and there is room */
    if( 
        (rxBuffer->bufferLen == ZERO) ||
        (rxBuffer->buffer == NULL) ||
        (rxBuffer->bufferIndex >= rxBuffer->bufferLen) 
    ) {
        error |= packets_ERR_MEMORY;
    }

    if(!error) {
        /* Act according to the state of the packet buffer */
        switch (buffer->receive.bufferState) {
            /* Waiting for start, ensure that the data byte is valid */
            case packets_BUFFER_RECEIVE_WAIT_FOR_START: {
                if(byte != packets_SYM_START ){
                    error |= packets_ERR_START_SYM;
                    break;
                }
                /* Reset the count */
                rxBuffer->bufferIndex = ZERO;
                /* Store the byte */
                rxBuffer->buffer[(rxBuffer->bufferIndex)++] = byte;
                /* Advance to the next state */
                *bufferState = packets_BUFFER_RECEIVE_HEADER;
                break;
            }
            /* Waiting for the header */
            case packets_BUFFER_RECEIVE_HEADER: {
                /* Store the byte */
                rxBuffer->buffer[(rxBuffer->bufferIndex)++] = byte;
                /* Check to see if more data is required */
                if (rxBuffer->bufferIndex == packets_LEN_HEADER) {
                    /* Store the payload length */
                    *payloadLen = (rxBuffer->buffer[packets_INDEX_LEN_MSB] << BITS_ONE_BYTE) | rxBuffer->buffer[packets_INDEX_LEN_LSB];
                    if (*payloadLen == ZERO) {
                        /* Skip payload collection */
                        *bufferState = packets_BUFFER_RECEIVE_FOOTER;
                    } else {
                        /* Advance to the next state */
                        *bufferState = packets_BUFFER_RECEIVE_PAYLOAD;
                    }
                }
                break;
            }
            /* Receiving Payload info */
            case packets_BUFFER_RECEIVE_PAYLOAD: {
                /* Store the data  */
                rxBuffer->buffer[(rxBuffer->bufferIndex)++] = byte;
                /* See if all data has been captured */
                if (rxBuffer->bufferIndex == (packets_LEN_HEADER +  *payloadLen)){
                    /* Advance to next state */
                    *bufferState = packets_BUFFER_RECEIVE_FOOTER;   
                /* Data out of range */
                } else if (rxBuffer->bufferIndex > (packets_LEN_HEADER +  *payloadLen)){
                    /* Reset count */
                    *bufferState = packets_BUFFER_RECEIVE_WAIT_FOR_START;
                    error |= packets_ERR_LENGTH;
                }
                break;
            }
            /* Wait for the footer */
            case packets_BUFFER_RECEIVE_FOOTER: {
                /* Store the data */
                rxBuffer->buffer[(rxBuffer->bufferIndex)++] = byte;
                /*If all data has been captured */
                uint16_t fullPacketLength = packets_LEN_HEADER + *payloadLen + packets_LEN_FOOTER;
                if (rxBuffer->bufferIndex == fullPacketLength) {
                    /* Packet is complete */   
                    *bufferState = packets_BUFFER_RECEIVE_COMPLETE;
                }
                break;
            }
            /* Packet is complete, but has not been handled, throw error */
            case packets_BUFFER_RECEIVE_COMPLETE: {
                    error |= packets_ERR_STATE;
                break;
            }
        }
    } 
     /* Return the packet state */
     return error;
}

/*******************************************************************************
* Function Name:packets_parsePacket()
****************************************************************************//**
* \brief
*  Parses a data packet from the received data.
*
* \param packet
* Pointer to a packet structure to place the infomation in
*
* \return
* The error code of the result packet parsing
* The possible error codes are:
*
*  Errors codes                         | Description
*   ------------                        | -----------
*   packets_ERR_SUCCESS        | The packet was successfully parsed.
*   packets_ERR_FORMAT         | The packet was in the incorrect format.
*   packets_ERR_MODULE         | An unknown module was requested.
*   packets_ERR_LENGTH         | The packet payload length was outside the allowed value.
*   packets_ERR_CHECKSUM       | The calculated checksum did not match the reported checksum.
*******************************************************************************/
uint32_t packets_parsePacket(packets_BUFFER_FULL_S* buffer) {
    uint32_t error = packets_ERR_SUCCESS;
    /* Create local references */
    packets_BUFFER_PROCESS_S* rxBuffer = &(buffer->receive.processBuffer);
    packets_BUFFER_STATE_RECEIVE_T* bufferState = &(buffer->receive.bufferState);
    packets_PACKET_S* packet = &(buffer->receive.packet);
    
    /* Ensure packet is complete */
    if (*bufferState != packets_BUFFER_RECEIVE_COMPLETE){
        return packets_ERR_INCOMPLETE;    
    }
    /* Ensure start of packet symbol */
    uint8 startByte = rxBuffer->buffer[packets_INDEX_START];
    if (startByte != packets_SYM_START) {
        error |= packets_ERR_START_SYM;
    }

    /* Get the Command - Cannot tell if valid at this point */
    uint8_t cmd =  rxBuffer->buffer[packets_INDEX_CMD];
    packet->cmd = cmd;
    /* Populate the module */
    error |= packets_getModuleFromCmd(cmd, &(packet->moduleId));
    
    /* Get the payload length */
    packet->payloadLen = (rxBuffer->buffer[packets_INDEX_LEN_MSB] << BITS_ONE_BYTE) | rxBuffer->buffer[packets_INDEX_LEN_LSB];
    if (packet->payloadLen > packets_LEN_MAX_PAYLOAD) {
        error |= packets_ERR_LENGTH;  
    }
    
    /* Ensure Payload array is allocated */
    if(packet->payload == NULL){
        error |= packets_ERR_MEMORY;   
    }
    if(!error){
        /* Transfer the payload to the new buffer */
        memmove(packet->payload, &(rxBuffer->buffer[packets_INDEX_PAYLOAD]), packet->payloadLen); 

        /* Start of the footer */
        uint8_t* footerPtr = &(rxBuffer->buffer[packets_LEN_HEADER + packet->payloadLen]);
        /* Get the flags */
        uint8 flags1 = *footerPtr++;
        uint8 flags0 = *footerPtr++;
        packet->flags = (uint32_t) ((flags1 << BITS_ONE_BYTE) | flags0);
        /* Validate checksum */
        uint16_t calculatedChecksum = packets_computeChecksum16(rxBuffer->buffer, packets_LEN_HEADER + packet->payloadLen + packets_LEN_FLAGS);
        uint8 checkSumMsb = *footerPtr++;
        uint8 checkSumLsb = *footerPtr++;
        uint16_t reportedChecksum = (checkSumMsb << BITS_ONE_BYTE) | checkSumLsb;
        if( calculatedChecksum != reportedChecksum) {
            error |= packets_ERR_CHECKSUM;
        }
        /* Check end of packet symbol */
        uint8 endSymbol = *footerPtr;
        if (endSymbol != packets_SYM_END) {
            error|= packets_ERR_END_SYM;
        }
        /* Flush the buffer */
        packets_flushRxBuffers(buffer);
        /* Return the error code */
    }
    return error;
} 

/*******************************************************************************
* Function Name: packets_acknowledgePacket()
****************************************************************************//**
* \brief
*  Respond to a received packet
*
* \param rxPacket [in]
*  Pointer to the received packet
*
* \param validateFn [in]
*  Pointer to the device specific validation function
* 
* \return
*  Returns the error of associated with 
*******************************************************************************/
uint32_t packets_acknowledgePacket(packets_BUFFER_FULL_S* packet, uint32_t (*validateFn)(packets_PACKET_S* rxPacket, packets_PACKET_S* txPacket) ){
    packets_PACKET_S* rxPacket = &(packet->receive.packet);
    packets_PACKET_S* txPacket = &(packet->send.packet);
    /* Set the default settings */
    txPacket->moduleId = rxPacket->moduleId;
    txPacket->cmd = rxPacket->cmd;
    txPacket->flags = packets_FLAG_ACK;
    /* Validate the command */
    uint32_t validateErr = validateFn(rxPacket, txPacket);
    uint32_t responseErr = packets_ERR_SUCCESS;
    /* Command is valid */
    if(validateErr ==packets_ERR_SUCCESS){
        /* Respond if the NO ACK flag is not set */
        if( !(packet->receive.packet.flags & packets_FLAG_NO_ACK)){
            /* Send the response packet */
            responseErr = packets_sendPacket(packet);
        }
    /* An error occured during validation */
    } else {
        responseErr = validateErr;
    }
    return responseErr;
}


/*******************************************************************************
* Function Name: packets_getModuleFromCmd()
****************************************************************************//**
* \brief
*  Returns the module id from the CMD
*
* \param cmd [in]
* The host or response command
*
* \param module [out]
* Pointer on where to place the output
*
* \return
* Error of the operation
*
*******************************************************************************/
uint32_t packets_getModuleFromCmd(uint8_t cmd, uint8_t *module) {
    uint8 id;
    /* Mask the Response flag */
    cmd &= (~packets_RSP_BIT);
    /* Check the memory space */
    if (cmd >= packets_CMD_CONTROL_MIN && cmd <= packets_CMD_CONTROL_MAX) {
        id = packets_ID_MODULE_CONTROL;
    } else if (cmd >= packets_CMD_ACTUATION_MIN && cmd <= packets_CMD_ACTUATION_MAX){
        id = packets_ID_MODULE_ACTUATION;   
    } else if (cmd >= packets_CMD_SENSING_MIN && cmd <= packets_CMD_SENSING_MAX){
        id = packets_ID_MODULE_SENSING;   
    }  else if (cmd >= packets_CMD_ENERGY_MIN && cmd <= packets_CMD_ENERGY_MAX){
        id = packets_ID_MODULE_ENERGY;   
    } else {
        return packets_ERR_UNKNOWN;
    }
    /* Set the module value out */
    *module = id;
    /* Return success */
    return packets_ERR_SUCCESS;
}



/*******************************************************************************
* Function Name: packets_computeChecksum16()
****************************************************************************//**
* \brief
*  Calculates the 16 bit, two's complement, checksum from an array of data
*
* \param data
* Array which contains the data
*
* \param length
* The length of the data to interate over
*
* \return
* The value of the checksum in two's complement form
*
*******************************************************************************/
uint16_t packets_computeChecksum16(uint8_t* data, uint16_t length){
    /* Local variables */
    uint8_t idx;
    uint16_t sum = ZERO;
    /* Iterate over the values */
    for( idx = ZERO; idx < length; idx++ ){
        sum += data[idx];
    }
    /* Return the sum in two's complement */
    return (1 + ~sum);
}

/*******************************************************************************
* Function Name: packets_printPacket()
****************************************************************************//**
* \brief
*  Prints the content of a packet
*
* \param packet [in]
*  Packet to display
* 
* \param fn [in]
*   Print function to use to display results
* 
* \return
*  None
*******************************************************************************/
  void packets_printPacket(packets_PACKET_S* packet, void (*printFn)(char *pszFmt, ...)){
    printFn("\r\nModule Id: %d\r\n", packet->moduleId);
    printFn("Command: %x\r\n", packet->cmd);
    uint16_t len = packet->payloadLen; 
    printFn("payload Len: %d\r\n", len);
    printFn("Payload:[ ", packet->cmd);
    uint16_t i;
    for(i = ZERO; i<len; i++){
        printFn("0x%x ",packet->payload[i]);
    }
    printFn("]\r\n");
    printFn("Flags: 0x%x\r\n\r\n", packet->flags);
}

/* [] END OF FILE */
